#ifndef F_CPU
#define F_CPU 7372800UL
#endif

// Modelcraft 25 A BEC
#define MOTOR_PRESC CLK13_8	
#define MOTOR_FULL_CYCLE 18432U // 20.0 ms @ F_CPU=7.3728 MHz & prescaler=1/8 
#define MOTOR_STANDBY 0x0400	
#define MOTOR_MIN 0x0425
#define MOTOR_MAX 0x0800
#define ADC_ZERO_HOR 0x01A0
#define ADC_ZERO_VERT 0x0293
#define TURN_MAX 0x0080
#define NICK_MAX 0x000A

#define MULT_NORM 0x00FF

#define TIMEOUT_MS 0x20
#define CMD_BUFFER_SIZE 0x80
#define DEFAULT_XBEE_ADDR 0x00,0x13,0xA2,0x00,0x40,0xC5,0x79,0xB8

#define TWI_BITRATE 0x02

#define NO_SEND 0x00
#define SEND_DATA 0x01

#define MASTER
#define ADC_ADDR 0x02

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "../timer/timer64.h"
#include "../usart/uart64.h"
#include "../BMA220/BMA220.h"
#include "../twi/twi.h"
#include "../math/math.h"
#include "../ADC/adc.h"


/////////////////// global variables //////////////////////////////////

static volatile uint8_t sreg;

volatile int8_t Mean_Buffer_x[10];
volatile int8_t Mean_Buffer_y[10];
volatile uint8_t mean_Buffer_pos;

volatile float32 mean_gx,mean_gy,mean_gz,gain1,gain2;
volatile uint16 gyro_x,gyro_y,gyro_z,geir,roll,nick,motora,motorb,motorc,motord;
volatile uint32 ADC_time;
volatile uint8_t power_cell1,power_cell2,power_cell3;
volatile int16_t rot_x,rot_y,ascend,err_x_prev,err_y_prev,offset_geir,offset_roll,offset_nick,
	Kp,Kd,Ki,Ka,tana256,N_Int,N_Diff,diff_limit,diff_filtered_x,diff_filtered_y,sum_limit,turn,move;
volatile int32_t sum_err_x,sum_err_y,sum_thres,slider,stick_horizontal,stick_vertical;

static volatile uint8_t send_data = NO_SEND;
static volatile uint8_t motors_locked = YES;

/*volatile uint16_t Geir[FILTER_BUFFER_SIZE];
volatile uint16_t Roll[FILTER_BUFFER_SIZE];
volatile uint16_t Nick[FILTER_BUFFER_SIZE];

volatile uint8_t buffer_pos = 0x00;*/

/////////////////// interrupt handlers ////////////////////////////////


ISR(BADISR_vect)
{
	
}

void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

/////////////////////// constants /////////////////////////////////////

//const uint8_t default_xbee_address[] PROGMEM = {0x00,0x13,0xA2,0x00,0x40,0xC5,0x79,0xB8};

const char cmd_loopback[] PROGMEM = "loopback";
const char cmd_bma220[] PROGMEM = "BMA220check";
const char cmd_ADC[] PROGMEM = "ADCcheck";
const char cmd_start[] PROGMEM = "startsend";
const char cmd_stop[] PROGMEM = "stopsend";
const char cmd_motor[] PROGMEM = "motor";
const char cmd_cell[] PROGMEM = "cell";
const char cmd_calibrate[] PROGMEM = "calibrate";
const char cmd_lock[] PROGMEM = "lock";
const char cmd_unlock[] PROGMEM = "unlock";
const char cmd_eeprom[] PROGMEM = "eeprom";
const char cmd_reset[] PROGMEM = "reset";
const char cmd_zero[] PROGMEM = "zero";

///////////////////// eeprom paramters ///////////////////////////////////

uint16_t EEMEM EE_Kp  = 300; 
uint16_t EEMEM EE_Ki  = 0; 
uint16_t EEMEM EE_Kd  = 0; 
uint16_t EEMEM EE_Ka  = 0; 
uint16_t EEMEM EE_sum_thres = 128;
uint16_t EEMEM EE_diff_thres = 60;

/////////////////////// functions ////////////////////////////////////////

void wdt_init(void)
{
    MCUSR = 0;
    wdt_disable();
}

void soft_reset(void){
    wdt_enable(WDTO_15MS);
    while(1){};          
} 

uint8_t str_cmp(char* str1,const char* str2){
	uint8_t c1,c2;
	//unsigned char Out[2];
	
	uint8_t i = 0x00;	
	do{
		c2 = pgm_read_byte(&(str2[i]));		
		c1 = str1[i];		
		if (c1!=c2){ 			
			//Out[0] = c1;
			//Out[1] = c2;
			//transmit_API_frame_TX1(Out,0x02,API_QUIET,API_DEFAULT,TIMEOUT_MS);				
			if (c2=='\0') return SUCCESS; else return FAILED;
		}		
		i++;
	}while(c1!='\0' && c2!='\0');
	return SUCCESS;
}

void init_ports(){
	
	DDRA = 0x00;		// input
	PORTA = 0xFF;		// pullups on
	
	DDRB = (1<<PB4);	
	PORTB = ~((1<<PB0) | (1<<PB1) | (1<<PB4));	
	
	DDRC = (1<<PC7);		
	PORTC = ~(1<<PC7);		
	
	DDRD = (1<<PD3);		
	PORTD = 0xFF;		
	
	DDRE = (1<<PE2) | (1<<PE3) | (1<<PE4) | (1<<PE5);		
	PORTE = (1<<PE5); 									// no pullup on PE6 ! 
	// (1<<PE0) | (1<<PE1) | 
	
	DDRF = 0x00;		
	PORTF = ~((1<<PF0) | (1<<PF1) | (1<<PF2));	

	DDRG = (1<<PG0) | (1<<PG1);		
	PORTG = ~((1<<PG0) | (1<<PG1));			
}

void init_motor_pwm(){	
	ICR1 = MOTOR_FULL_CYCLE;
	ICR3 = MOTOR_FULL_CYCLE;
	
	timer_init_13(TIMER1,FAST_PWM_MODE_ICR,MOTOR_PRESC);
	timer_init_13(TIMER3,FAST_PWM_MODE_ICR,MOTOR_PRESC);
	
	timer_channel_settings_13(TIMER1,CHANNEL_A,CLEAR_ON_MATCH,MOTOR_STANDBY);
	timer_channel_settings_13(TIMER1,CHANNEL_B,CLEAR_ON_MATCH,MOTOR_STANDBY);
	timer_channel_settings_13(TIMER1,CHANNEL_C,CLEAR_ON_MATCH,MOTOR_STANDBY);
	timer_channel_settings_13(TIMER3,CHANNEL_C,CLEAR_ON_MATCH,MOTOR_STANDBY);
	
	/*DDRE |=(1<<PE5);	
	TCCR3A = 0b00001010;
	TCCR3B = 0b00011010;
	OCR3C = MOTOR_STANDBY;*/
}

void init_xbee_interface(){
	
	init_RXstream1();	
	
	uint8_t Dest[8];
	Dest[0] = 0x00;
	Dest[1] = 0x13;
	Dest[2] = 0xA2;
	Dest[3] = 0x00;
	Dest[4] = 0x40;
	Dest[5] = 0xC5;
	Dest[6] = 0x79;
	Dest[7] = 0xB8;	
	uart_set_dest_addr(Dest);
}

void init_bma220(){	
	bma220_initSPI4();
	
	bma220_set_range(RANGE_2g);
	bma220_set_bandwidth(BW_25Hz);
	bma220_read10bit_mode(BLOCK);			// 10bit prec enabled
	bma220_enable_newdata_int(INT_6);
	
	sreg = SREG;
	cli();
	
	uint16_t r = 50;
	uint16_t q = 10;
	
	uint16_t covariance = sqrt32((uint32_t)q*(q+4*r));
	covariance -= q;
	covariance >>= 1;
	uint16_t h = covariance+r;
	gain1 = init_float32_rational(covariance,h);
	gain2 = init_float32(1);
	sub_float32((float32*)&gain2,(float32*)&gain1);
	
	mean_gx = init_float32(0);
	mean_gy = init_float32(0);
	mean_gz = init_float32(255);	
	
	gyro_x.t16 = 0x0000;
	gyro_y.t16 = 0x0000;
	gyro_z.t16 = 0x00FF;
	
	SREG = sreg;
}

void transmit_error_message(){
	char Out[5];
	Out[0] = 'E';
	Out[1] = 'R';
	Out[2] = 'R';
	Out[3] = 'O';
	Out[4] = 'R';
	transmit_API_frame_TX1(Out,0x05,API_QUIET,API_DEFAULT,TIMEOUT_MS);
}

uint8_t test_bma220(){
	
	char Out[8];	
	Out[0] = 'B';
	Out[1] = 'M';
	Out[2] = 'A';
	Out[3] = '2';
	Out[4] = '2';
	Out[5] = '0';

	uint8_t chip_ID = bma220_getID();
	
	if (chip_ID==BMA220_ID) Out[6] = 0x01; else Out[6] = 0x00;
	
	// read bandwidth
	sreg = SREG;
	cli();
	uint8_t reg = bma220_read8(0x14);	
	Out[7] = reg;	
	SREG = sreg;
	
	transmit_API_frame_TX1(Out,0x08,API_QUIET,API_DEFAULT,TIMEOUT_MS);
	if (chip_ID==BMA220_ID) return SUCCESS; else return FAILED;
}

uint8_t test_ADC(){	
	uint8_t Out[16];
	
	uint8_t size = 0x00;
	uint8_t status = twi_master_receive(ADC_ADDR,Out,&size,0x10); 
	if (status==SUCCESS) transmit_API_frame_TX1(Out,size,API_QUIET,API_DEFAULT,TIMEOUT_MS);	
	else transmit_error_message();
}

void get_gyro_data(){
	int16_t acc[3];
	bma220_get_acc_data(acc,INT6);
	//gyro_x.t16 = acc[0];
	//gyro_y.t16 = acc[1];
	//gyro_z.t16 = acc[2];	
	
	sreg = SREG;
	cli();
	
	volatile float32 X = init_float32(acc[0]);
	mult_float32(&X,&gain1);
	mult_float32(&mean_gx,&gain2);
	add_float32(&mean_gx,&X);
	
	volatile float32 Y = init_float32(acc[1]);
	mult_float32(&Y,&gain1);
	mult_float32(&mean_gy,&gain2);
	add_float32(&mean_gy,&Y);
	
	volatile float32 Z = init_float32(acc[2]);
	mult_float32(&Z,&gain1);
	mult_float32(&mean_gz,&gain2);
	add_float32(&mean_gz,&Z);
	
	gyro_x.t16 = float32_to_int16(mean_gx);
	gyro_y.t16 = float32_to_int16(mean_gy);
	gyro_z.t16 = float32_to_int16(mean_gz);
	
	SREG = sreg;
}

void get_ADC_data(){
	uint8_t Received[16];
	//uint16 a,b,c;
	//uint32 d;
	
	uint8_t size = 0x00;
	uint8_t status = twi_master_receive(ADC_ADDR,Received,&size,0x10); 
	if (status==SUCCESS && size==0x0E){
		geir.t8[1] = Received[1];
		geir.t8[0] = Received[2];
		roll.t8[1] = Received[4];
		roll.t8[0] = Received[5];
		nick.t8[1] = Received[7];
		nick.t8[0] = Received[8];
		
		ADC_time.t8[3] = Received[10];
		ADC_time.t8[2] = Received[11];
		ADC_time.t8[1] = Received[12];
		ADC_time.t8[0] = Received[13];
	
		sreg = SREG;
		cli();
		geir.t16 -= 0x01FA;//offset_geir;
		roll.t16 -= 0x01FA;//offset_roll;
		nick.t16 -= 0x01FA;//offset_nick;
		
		SREG = sreg;
	}
}

init_ADC(){
	ADC_set_reference(ADC_REF_INTERNAL_Vcc);
	ADC_settings(ADC_SINGLE_SAMPLE_MODE,ADC_LEFT_ALIGNED,ADC_PRESC_128);	
}

void get_power_cell_voltage(){
	sreg = SREG;
	cli();
	ADC_enable();	
	ADC_set_channel(ADC_CH0);
	power_cell1 = ADC_single_conversion_low_prec();
	ADC_set_channel(ADC_CH1);
	power_cell2 = ADC_single_conversion_low_prec();
	ADC_set_channel(ADC_CH2);
	power_cell3 = ADC_single_conversion_low_prec();
	
	ADC_disable();
	SREG = sreg;
}

void send_power_cell_info(){
	uint8_t Out[7];
	
	get_power_cell_voltage();
	
	Out[0] = 'C';
	Out[1] = 'E';
	Out[2] = 'L';
	Out[3] = 'L';
	
	Out[4] = power_cell1;
	Out[5] = power_cell2;
	Out[6] = power_cell3;
	
	transmit_API_frame_TX1(Out,0x07,API_QUIET,API_DEFAULT,TIMEOUT_MS);
}

void send_acc_data(){
	char Out[20];

	Out[0] = 'G';
	Out[1] = 'Y';
	Out[2] = 'R';
	Out[3] = 'O';
	
	volatile uint16 temp;
	

	/*Out[4] = gyro_x.t8[1];
	Out[5] = gyro_x.t8[0];
	Out[6] = gyro_y.t8[1];
	Out[7] = gyro_y.t8[0];
	Out[8] = gyro_z.t8[1];
	Out[9] = gyro_z.t8[0];*/
	
	temp.t16 = ascend;
	Out[4] = temp.t8[1];
	Out[5] = temp.t8[0];
	
	temp.t16 = turn;
	Out[6] = temp.t8[1];
	Out[7] = temp.t8[0];
	
	temp.t16 = move;
	Out[8] = temp.t8[1];
	Out[9] = temp.t8[0];
	
	Out[10] = motora.t8[1];
	Out[11] = motora.t8[0];
	Out[12] = motorc.t8[1];
	Out[13] = motorc.t8[0];	
	Out[14] = gyro_x.t8[1];	
	Out[15] = gyro_x.t8[0];	
	
	Out[16] = ADC_time.t8[3];
	Out[17] = ADC_time.t8[2];
	Out[18] = ADC_time.t8[1];
	Out[19] = ADC_time.t8[0];

	transmit_API_frame_TX1(Out,0x14,API_QUIET,API_DEFAULT,TIMEOUT_MS);
}

void send_OK(uint8_t timeout){
	uint8_t Out[3];
	Out[0] = 'O';
	Out[1] = 'K';
	transmit_API_frame_TX1(Out,0x02,API_QUIET,API_DEFAULT,timeout);			
}

void set_all_motors(uint16_t speed){
	sreg = SREG;
	cli();
	
	timer_channel_cmpval_13(TIMER1,CHANNEL_A,speed);
	timer_channel_cmpval_13(TIMER1,CHANNEL_B,speed);
	timer_channel_cmpval_13(TIMER1,CHANNEL_C,speed);
	timer_channel_cmpval_13(TIMER3,CHANNEL_C,speed);
	
	SREG = sreg;
}

uint8_t calibrate_gyro(uint8_t unlock){
	uint8_t counter = 0;
	do{
		_delay_ms(1);
		counter++;
	}while(bma220_new_data_present()==NO && counter<0xFE);
	get_gyro_data();
	rot_y = (int16_t)gyro_x.t16;
	rot_x = (int16_t)gyro_y.t16;	
	
	sum_err_x = 0;
	sum_err_y = 0;
		
	if (unlock==YES && counter<0xFE){
		motors_locked = NO;
		return SUCCESS; 
	}
	else return FAILED;
}

void calibrate_acc(){
	get_ADC_data();
	offset_geir = geir.t16;
	offset_roll = roll.t16;
	offset_nick = nick.t16;
}

void write_parameters(uint8_t* Buffer){
	uint16 val;
	sreg = SREG;
	cli();
	
	val.t8[1] = Buffer[0];
	val.t8[0] = Buffer[1];
	eeprom_write_word((uint16_t*)&EE_Kp,val.t16);
	val.t8[1] = Buffer[2];
	val.t8[0] = Buffer[3];
	eeprom_write_word((uint16_t*)&EE_Ki,val.t16);
	val.t8[1] = Buffer[4];
	val.t8[0] = Buffer[5];
	eeprom_write_word((uint16_t*)&EE_Kd,val.t16);
	val.t8[1] = Buffer[6];
	val.t8[0] = Buffer[7];
	eeprom_write_word((uint16_t*)&EE_Ka,val.t16);
	val.t8[1] = Buffer[8];
	val.t8[0] = Buffer[9];
	eeprom_write_word((uint16_t*)&EE_sum_thres,val.t16);
	val.t8[1] = Buffer[10];
	val.t8[0] = Buffer[11];
	eeprom_write_word((uint16_t*)&EE_diff_thres,val.t16);
					
	SREG = sreg;	
}

void init_flight_control(){
	uint8_t i;
	
	sreg = SREG;
	cli();		
	
	//N_Int = 50;					// integral rise time in sample times
	//N_Diff = 3;					// differential rise time in sample times
	
	//geometry
	rot_x = -19;
	rot_y = 33;
	tana256 = -74;					// tan(alpha)=b/2/(a-b/2) relative to 256
	
	// initial values
	offset_geir = 0x0200;
	offset_roll = 0x0200;
	offset_nick = 0x0200;
	
	sum_err_x = 0;
	sum_err_y = 0;
	ascend = MOTOR_STANDBY;
	slider = MOTOR_STANDBY;
	turn = 0x0000;
	move = 0x0000;
	
	for (i=0;i<10;i++){
		Mean_Buffer_x[i] = 0x00;
		Mean_Buffer_y[i] = 0x00;
	}
	mean_Buffer_pos = 0x00;	

	// PID paramters, all realtive to 2^8
	// real Kp = Kd/tau = Kp*25
	
	Kp = eeprom_read_word(&EE_Kp);									// Proportianal weight
	Ki = eeprom_read_word(&EE_Ki);									// Integral weight
	Kd = eeprom_read_word(&EE_Kd);									// Differential weight
	Ka = eeprom_read_word(&EE_Ka);									// Ascalleration weight
	
	// limiters 
	sum_limit = eeprom_read_word(&EE_sum_thres);
	diff_limit = eeprom_read_word(&EE_diff_thres);
	
	sum_thres = sum_limit;									// Ki relative to 2^16
	sum_thres *= 65536;										// Kp,Kd relative to 2^8
	sum_thres /= (uint32_t)Ki;
	
	ascend = MOTOR_STANDBY;
	
	SREG = sreg;	
}

void PID_flight_control(uint8_t set_motors){
	
	uint8_t i;
	volatile uint32_t sum;
		
	sreg = SREG;
	cli();
	
	// filter ascend
	ascend = 14UL*ascend+2UL*slider;
	ascend /= 16UL;
	
	// filter turn
	turn = 14UL*turn+2UL*stick_horizontal;
	turn /= 16UL;	
	
	// filter move
	move = 14UL*move+2UL*stick_vertical;
	move /= 16UL;	
		
	int16_t err_x = gyro_x.t16-rot_y-move;
	int16_t err_y = gyro_y.t16-rot_x;	
	
	int16_t err_x_diff = err_x-err_x_prev;	
	if (mean_Buffer_pos<10) Mean_Buffer_x[mean_Buffer_pos] = err_x_diff;
	err_x_diff = 0;
	for (i=0;i<10;i++) err_x_diff += (int16_t)Mean_Buffer_x[i];
	if (err_x_diff>diff_limit) err_x_diff = diff_limit;
	if (err_x_diff<-diff_limit) err_x_diff = -diff_limit;
	
	int16_t err_y_diff = err_y-err_y_prev;
	if (mean_Buffer_pos<10) Mean_Buffer_y[mean_Buffer_pos] = err_y_diff;
	err_y_diff = 0;
	for (i=0;i<10;i++) err_y_diff += (int16_t)Mean_Buffer_y[i];	
	if (err_y_diff>diff_limit) err_y_diff = diff_limit;
	if (err_y_diff<-diff_limit) err_y_diff = -diff_limit;
	
	mean_Buffer_pos++;
	if (mean_Buffer_pos==10) mean_Buffer_pos = 0;
		
	sum_err_x += err_x;
	if (sum_err_x>sum_thres) sum_err_x = sum_thres;
	if (sum_err_x<-sum_thres) sum_err_x = -sum_thres;
	
	sum_err_y += err_y;
	if (sum_err_y>sum_thres) sum_err_y = sum_thres;
	if (sum_err_y<-sum_thres) sum_err_y = -sum_thres;	
	
	//if (PING & (1<<PG1)) PORTG &= ~(1<<PG1); else PORTG |= (1<<PG1);
	
	sum = sum_err_x/256;
	volatile int32_t mx = (int32_t)Kp*err_x+Ki*sum+Kd*err_x_diff;
	
	sum = sum_err_y/256;
	volatile int32_t my = (int32_t)Kp*err_y+Ki*sum+Kd*err_y_diff;
	
	volatile int32_t M1 = (int32_t)(256*mx-tana256*my)/65536;				// 2^16=256*256
	volatile int32_t M2 = (int32_t)(tana256*mx+256*my)/65536;
	
	volatile int16_t m1 = (int16_t)M1;
	volatile int16_t m2 = (int16_t)M2;
	volatile int16_t m3 = -m1;
	volatile int16_t m4 = -m2;
		
	m1 += ascend+turn;		// a
	m2 += ascend-turn;		// b
	m3 += ascend+turn;		// c
	m4 += ascend-turn;		// d	
		
	/*if (m1<MOTOR_MIN){
		m3 += (int16_t)m1-MOTOR_MIN;
		m1 = MOTOR_MIN;		
	}
	else if (m3<MOTOR_MIN){		
		m1 += (int16_t)m3-MOTOR_MIN;
		m3 = MOTOR_MIN;
	}
	if (m2<MOTOR_MIN){
		m4 += (int16_t)m2-MOTOR_MIN;
		m2 = MOTOR_MIN;
	}
	else if (m4<MOTOR_MIN){
		m2 += (int16_t)m4-MOTOR_MIN;
		m4 = MOTOR_MIN;
	}*/
	
	if (set_motors==NO){
		if ((m1-MOTOR_MIN)>0x0064) motors_locked = YES;
		if ((m2-MOTOR_MIN)>0x0064) motors_locked = YES;
		if ((m3-MOTOR_MIN)>0x0064) motors_locked = YES;
		if ((m4-MOTOR_MIN)>0x0064) motors_locked = YES;		
	}
	else{
		uint16_t tot_err = err_x*err_x+err_y*err_y;
		if (tot_err>0x4000){ //0x2710)
			set_all_motors(MOTOR_STANDBY);
			ascend = MOTOR_STANDBY;
			motors_locked = YES;
		}
		else if (motors_locked==NO){				
			timer_channel_cmpval_13(TIMER1,CHANNEL_A,m1);
			timer_channel_cmpval_13(TIMER1,CHANNEL_B,m2);
			timer_channel_cmpval_13(TIMER1,CHANNEL_C,m3);
			timer_channel_cmpval_13(TIMER3,CHANNEL_C,m4);		
		}
		
		motora.t16 = (uint16_t)m1;
		motorb.t16 = (uint16_t)m2;
		motorc.t16 = (uint16_t)m3;
		motord.t16 = (uint16_t)m4;
	}
				
	err_x_prev = err_x;
	err_y_prev = err_y;
	
	SREG = sreg;
}

uint8_t process_RX1_input(uint8_t timeout,uint8_t mode){ 
	
	uint8_t size,start,msg_size,type,count,status;
	uint8_t Buffer[CMD_BUFFER_SIZE];
	uint8_t Out[CMD_BUFFER_SIZE];
	
	do{
		size = 0x00;
		if(read_API_Buffer1(Buffer,&size,CMD_BUFFER_SIZE,timeout)==SUCCESS){		
			type = parse_API(Buffer,size,&start,&msg_size,&status);
			if (type==API_RECEIVE_TRANSMISSION){								
				if (str_cmp(&(Buffer[start]),cmd_loopback)==SUCCESS){
					Out[0] = 'r';
					Out[1] = 's';
					Out[2] = 's';
					Out[3] = 'i';
					Out[4] = '=';
					Out[5] = get_signal_strength();
					transmit_API_frame_TX1(Out,0x06,API_QUIET,API_DEFAULT,timeout);
				}
				else if (str_cmp(&(Buffer[start]),cmd_motor)==SUCCESS){//(Buffer[start]=='p' && msg_size==0x03){
					volatile uint8_t timer,channel;
					volatile uint16_t x;
					//if (PORTB & (1<<PB4)) PORTB &= ~(1<<PB4); else PORTB |= (1<<PB4);																													
				
					switch(Buffer[start+5]){
						case 'a':
							timer = TIMER1;
							channel = CHANNEL_A;
							break;
						case 'b':
							timer = TIMER1;
							channel = CHANNEL_B;
							break;
						case 'c':
							timer = TIMER1;
							channel = CHANNEL_C;
							break;
						case 'd':
							timer = TIMER3;
							channel = CHANNEL_C;
							break;		
						case 'x':
							channel = CHANNEL_ALL;
							break;																
					}
					
					x = char_to_hex16(&(Buffer[start+6]));	
					if (channel==CHANNEL_ALL){
						sreg = SREG;
						cli();
						uint16_t speed = x;											
						speed += MOTOR_MIN;												
						if (speed>0x0700) speed = 0x0700;		
						set_all_motors(speed);												
						SREG = sreg;
					}
					else{					
						timer_channel_cmpval_13(timer,channel,x);
					}
					
					send_OK(timeout);	
					
					/*Out[0] = timer;
					Out[1] = channel;
					Out[2] = x.t8[1];
					Out[3] = x.t8[0];
					transmit_API_frame_TX1(Out,0x04,API_QUIET,API_DEFAULT,timeout);	*/
				}
				else if(str_cmp(&(Buffer[start]),cmd_bma220)==SUCCESS) test_bma220();				
				else if(str_cmp(&(Buffer[start]),cmd_ADC)==SUCCESS) test_ADC();	
				else if(str_cmp(&(Buffer[start]),cmd_cell)==SUCCESS) send_power_cell_info();
				else if(str_cmp(&(Buffer[start]),cmd_start)==SUCCESS){										
					send_OK(timeout);	
					send_data = SEND_DATA;	
				}
				else if(str_cmp(&(Buffer[start]),cmd_stop)==SUCCESS){											
					send_data = NO_SEND;
					send_OK(timeout);		
				}
				else if(str_cmp(&(Buffer[start]),cmd_calibrate)==SUCCESS){						
					if (calibrate_gyro(YES)==SUCCESS){
						send_OK(timeout);
						PID_flight_control(NO);
					}
					calibrate_acc();				
				}
				else if(str_cmp(&(Buffer[start]),cmd_lock)==SUCCESS){	
					motors_locked = YES;
					set_all_motors(MOTOR_STANDBY);
					send_OK(timeout);	
				}
				else if(str_cmp(&(Buffer[start]),cmd_unlock)==SUCCESS){	
					motors_locked = NO;
					PID_flight_control(NO);
					send_OK(timeout);	
				}
				else if(str_cmp(&(Buffer[start]),cmd_eeprom)==SUCCESS){	
					volatile uint16 val;
					write_parameters(&(Buffer[start+6]));					
					init_flight_control();	
					Out[0] = 'P';
					Out[1] = 'A';
					val.t16 = Kp;
					Out[2] = val.t8[1];
					Out[3] = val.t8[0];
					val.t16 = Ki;
					Out[4] = val.t8[1];
					Out[5] = val.t8[0];
					val.t16 = Kd;
					Out[6] = val.t8[1];
					Out[7] = val.t8[0];
					val.t16 = Ka;
					Out[8] = val.t8[1];
					Out[9] = val.t8[0];
					transmit_API_frame_TX1(Out,0x0A,API_QUIET,API_DEFAULT,TIMEOUT_MS);		
				}
				else if(str_cmp(&(Buffer[start]),cmd_zero)==SUCCESS){	
					write_parameters(&(Buffer[start+6]));
					send_OK(timeout);
					calibrate_gyro(NO);
				}
				else if(str_cmp(&(Buffer[start]),cmd_reset)==SUCCESS){	
					motors_locked = YES;
					send_OK(timeout);
					soft_reset();
				}
				else{
					transmit_API_frame_TX1(&(Buffer[start]),msg_size,API_QUIET,API_DEFAULT,timeout);	
				}							
			}
			else{
				Out[0] = 's';
				Out[1] = 'a';
				Out[2] = 'm';
				Out[3] = 'p';
				//Out[4] = 'l';
				//Out[5] = 'e';
				
				
				if (Buffer[start+12]==0x0E){

					volatile int32_t x;
					uint16 speed,rotz,roty;					
					sreg = SREG;
					cli();
					
					// emergency shutdown (watch DIO4)
					uint8_t dig = Buffer[start+15];
					if (!(dig & (1<<4))){
						motors_locked = YES;
						set_all_motors(MOTOR_STANDBY);							
					}
					
					// ascend speed
					speed.t8[1] = Buffer[start+20];					
					speed.t8[0] = Buffer[start+21];
					x = (int32_t)MOTOR_MAX-MOTOR_MIN;
					x *= (int32_t)speed.t16-ADC_IO_ANALOG_MIN;
					x /= ADC_IO_ANALOG_DIFF;
					x += MOTOR_MIN;
					slider = x;										
					
					// turn speed
					rotz.t8[1] = Buffer[start+16];			
					rotz.t8[0] = Buffer[start+17];	
					
					x = (int32_t)TURN_MAX;
					x *= (int32_t)rotz.t16-ADC_ZERO_HOR;
					x /= ADC_ZERO_HOR;
					stick_horizontal = x;				
					
					// nick angle
					roty.t8[1] = Buffer[start+18];			
					roty.t8[0] = Buffer[start+19];	
					
					x = (int32_t)NICK_MAX;
					x *= (int32_t)roty.t16-ADC_ZERO_VERT;
					x /= ADC_ZERO_VERT;
					stick_vertical = x;																		
																	
					//transmit_API_frame_TX1(&(Buffer[start]),msg_size,API_QUIET,API_DEFAULT,timeout);						
					SREG = sreg;																						
				}
			}
		}
		else{
			uint8_t e = get_uart64_errno();
			Out[0] = 'e';
			Out[1] = 'r';
			Out[2] = 'r';
			Out[3] = e;
			
			if (e>0x01) transmit_API_frame_TX1(Buffer,size,API_QUIET,API_DEFAULT,timeout);	
			//transmit_API_frame_TX1(Out,0x04,API_QUIET,API_DEFAULT,TIMEOUT_MS);
		}
		count = count_RXBuffer1();
	}while(count>mode);				
}

int main(){
	init_ports();	
	
	//timer0_init(CTC_MODE,TOGGLE_ON_MATCH,0xFF,CLK0_1024);
	
	init_ADC();
	
	init_motor_pwm();
	
	enable_uart(UART1,FRAME_8BIT,ONE_STOP_BIT,NO_PARITY,UART_SINGLE_SPEED);
	
	init_xbee_interface();
	
	twi_init_master(TWI_BITRATE,TWI_PRESC_1);
	
	init_bma220();	

	init_flight_control();
	
	sei();
	
	//go_to_sleep();
	
	//uint8_t size = 0x00;
	//uint8_t Out[10];
	//uint8_t Buffer[10];
	volatile uint8_t i = 0;
	
	// Main loop
	while(1){					
		_delay_ms(20);
							
		//if ((i & 0x04)==0) 
		process_RX1_input(TIMEOUT_MS,API_READ_ALL);					
		
		if (bma220_new_data_present()==YES){
			get_gyro_data();				
			get_ADC_data();		
			PID_flight_control(YES);	
			if (send_data==SEND_DATA && ((i & 0x07)==0)) send_acc_data();			
		}
		
		i++;	
	}
}
