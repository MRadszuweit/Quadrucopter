
#include "xbee.h"

static int verbose = NO;
static int API_escaped_mode = YES;
static int input_flag = NO;
static int serial_timeout = TIMEOUT;

void set_timout(int timeout){
	serial_timeout = timeout;
}

void print_error_message(char* mode,const char* func){
	if (func!=NULL) printf("%s error in %s, code %d\n",mode,func,errno);
	else printf("%s error, code %d\n",mode,errno);
}

void delete_cr(char* Buffer){
	char* pos = Buffer;
	pos = strstr(Buffer,"\r");
	if (pos!=NULL) *pos = '\0';
}

void raw_print(char* Buffer,int size){
	int i;
	char Temp[8];
	char ToPrint[512];
	ToPrint[0] = '\0';
	for (i=0;i<size;i++){
		sprintf(Temp,"%d\t",(unsigned char)Buffer[i]);
		strcat(ToPrint,Temp);
	}
	printf("%s\n",ToPrint);
}

void complete_flush(int dev_handle){
	int byte_count = 0;
	tcdrain(dev_handle);
	do{
		if (tcflush(dev_handle,TCIOFLUSH)<0){
			printf("error flushing iostream\n");
			return;
		}
		ioctl(dev_handle,FIONREAD,&byte_count);
	}while(byte_count>0);			
}

int string_to_hex_array(char* String,unsigned char* Hex){
	int i;
	char* S;
	char* dummy;
	char T[3];
	T[2] = '\0';
	if (String[1]=='x' || String[1]=='X') S = &(String[2]); else S = String;
	
	i = 0;
	while((*S)!='\0' && i<BUFFER_SIZE){
		T[0] = *(S++);		
		T[1] = *(S++);
		Hex[i] = (unsigned char)strtol(T,&dummy,16);
		i++;
	}
	return i;
}

int wait_for_bytes(int dev_handle,int n,int timeout){		// timeout in ms
	//int max;
	int byte_count = 0;										// minimum n bytes
	int time = 0;
	
	do{
		ioctl(dev_handle,FIONREAD,&byte_count);
		//max = timeout-time;
		//if (byte_count>0) timeout = max/2;
		//if (byte_count!=0) printf("%d bytes in queue\n",byte_count);
		usleep(1000);
		
		time++;
	}while(byte_count<n && time<timeout);
	if (byte_count<n) return FAILED;else return SUCCESS;	
}

int read_until_CR(int dev_handle,char* Buffer,int timeout){
	char c;
	
	int pos = 0;
	int status = 0;
	do{
		wait_for_bytes(dev_handle,1,timeout);
		status = read(dev_handle,&c,1);	
		if (status==1 && c!='\r'){
			Buffer[pos] = c;
			pos++;
		}		
	}while(status==1 && c!='\r');
	if (status<1) print_error_message("read",__func__);
	Buffer[pos] = '\0';
	return pos;
}

void init_serial_port(char* device_name,int* handle){
	struct termios config;
	
	memset(&config,0,sizeof(config));	
	//Input flags - Turn off input processing
		// convert break to null byte, no CR to NL translation,
		// no NL to CR translation, don't mark parity errors or breaks
		// no input parity check, don't strip high bit off,
		// no XON/XOFF software flow control
	config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
     INLCR | PARMRK | INPCK | ISTRIP | IXON);
                    
	// Output flags - Turn off output processing
		// no CR to NL translation, no NL to CR-NL translation,
		// no NL to CR translation, no column 0 CR suppression,
		// no Ctrl-D suppression, no fill characters, no case mapping,
		// no local output processing
	config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
     ONOCR | OFILL | OLCUC | OPOST);
     
    // Turn off character processing
		// clear current char size mask, no parity checking,
		// no output processing
		// 8 bit input	
	config.c_cflag &= ~(CSIZE | PARENB | CRTSCTS);
	config.c_cflag |= CS8;
	
	// non canonical mode
	config.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	
	// One input byte is enough to return from read()
	// block for at most 10*0.1 sec
	config.c_cc[VMIN]  = 0;
	config.c_cc[VTIME] = 1;
	
	speed_t speed = DEF_BAUD;
	if (cfsetispeed(&config,speed) < 0 || cfsetospeed(&config,speed) < 0){
		printf("could not apply default baud rate -> abort\n");
		exit(0);
	}
	else printf("use baud rate: %d\n",speed);
	
	// open device
	int dev_handle = open(device_name, O_RDWR | O_NOCTTY | O_NDELAY);	
	if (dev_handle==-1){
		printf("could not open device %s: error %d -> abort\n",device_name,errno);	
		exit(0);
	}
	
	if(!isatty(dev_handle)){
		printf("no serial device found at %s -> abort\n",device_name);	
		exit(0);
	}	
	*handle = dev_handle;

	// apply settings (before, wait until all data is transmitted/received)
	if(tcsetattr(dev_handle, TCSAFLUSH, &config) < 0){
		printf("could not apply settings -> abort\n");
		exit(0);
	}
	
	printf("device online\n");	
}

int transparent_send(int dev_handle,char* Buffer){
	int len,status;
	if (tcflush(dev_handle,TCIOFLUSH)<0) printf("error flushing serial io");
	len = strlen(Buffer);
	status = write(dev_handle,Buffer,len);
	tcdrain(dev_handle);
	if (status!=len) print_error_message("write",__func__);
	return (status==len ? SUCCESS : FAILED);
}

int enter_AT_command_mode(int dev_handle){
	int status;
	char Buffer[4];
	Buffer[3] = '\0';
	char* Cmd = "+++";
	if (tcflush(dev_handle,TCIOFLUSH)<0) printf("error flushing serial io");
		
	sleep(1);
	status = write(dev_handle,Cmd,3);
	tcdrain(dev_handle);
	if (status!=3) print_error_message("write",__func__);
	sleep(1);
	wait_for_bytes(dev_handle,3,serial_timeout);
	status = read(dev_handle,Buffer,3);	
	if (status!=3) print_error_message("read",__func__);
	//raw_print(Buffer,3);
	if (strcmp(Buffer,OK)==0) return SUCCESS; else return FAILED;
}

int exit_AT_command_mode(int dev_handle){
	int status;
	char Buffer[4];
	Buffer[3] = '\0';
	char* Cmd = "ATCN\r";
	if (tcflush(dev_handle,TCIOFLUSH)<0) printf("error flushing serial io");
	status = write(dev_handle,Cmd,5);
	tcdrain(dev_handle);
	if (status!=5) print_error_message("write",__func__);
	wait_for_bytes(dev_handle,3,serial_timeout);
	status = read(dev_handle,Buffer,3);	
	if (status!=3) print_error_message("read",__func__);		
	if (strcmp(Buffer,OK)==0) return SUCCESS; else return FAILED;
}

int execute_AT_commands(int dev_handle){
	int status;
	char Buffer[4];
	Buffer[3] = '\0';
	char* Cmd = "ATAC\r";
	if (tcflush(dev_handle,TCIOFLUSH)<0) printf("error flushing serial io");
	status = write(dev_handle,Cmd,5);
	tcdrain(dev_handle);
	if (status!=5) print_error_message("write",__func__);
	wait_for_bytes(dev_handle,3,serial_timeout);
	status = read(dev_handle,Buffer,3);	
	if (status!=3) print_error_message("read",__func__);		
	if (strcmp(Buffer,OK)==0) return SUCCESS; else return FAILED;
}

int permanent_write(int dev_handle){
	int status;
	char Buffer[4];
	Buffer[3] = '\0';
	char* Cmd = "ATWR\r";
	if (tcflush(dev_handle,TCIOFLUSH)<0) printf("error flushing serial io");
	status = write(dev_handle,Cmd,5);
	tcdrain(dev_handle);
	if (status!=5) print_error_message("write",__func__);
	wait_for_bytes(dev_handle,3,serial_timeout);
	status = read(dev_handle,Buffer,3);	
	if (status!=3) print_error_message("read",__func__);		
	if (strcmp(Buffer,OK)==0) return SUCCESS; else return FAILED;
}

int send_AT_command(int dev_handle,char* Cmd,char** Val,int num){
	int i,status;
	char Command[512];
	char Temp[32];
	char Buffer[7];
	sprintf(Command,"AT%s",Cmd);
	int size = 4;
	for (i=0;i<num;i++){
		//sprintf(Temp," %X",Val[i]);
		sprintf(Temp," %s",Val[i]);
		strcat(Command,Temp);
		size += strlen(Temp);
	}
	size++;
	if (size+1>512){
		printf("error: command size exceeds 512 bytes -> ignore\n");
		return FAILED;
	}
	else{		
		if (verbose) printf("transmit cmd: %s ...\n",Command);	
		Command[size-1] = '\r';
		Command[size] = '\0';
		Buffer[3] = '\0';
		Buffer[6] = '\0';			
		
		if (tcflush(dev_handle,TCIOFLUSH)<0) printf("error flushing serial io");
		status = write(dev_handle,Command,size);
		tcdrain(dev_handle);
		if (status!=size) print_error_message("write",__func__);
		wait_for_bytes(dev_handle,3,serial_timeout);
		status = read(dev_handle,Buffer,6);	
		if (status!=size) printf("received: %s\n",Buffer);
		if (status!=3){
			if (strcmp(Buffer,ERROR)==0) printf("%s: xbee error received: %s\n",__func__,Buffer);
			else print_error_message("read",__func__);
			return FAILED;		
		}
		if (strcmp(Buffer,OK)==0) return SUCCESS; else return FAILED;		
	}
}

int receive_AT_parameter(int dev_handle,char* Cmd,char* Val,int* size){
	int status;	
	char Command[512];	
	
	sprintf(Command,"AT%s\r",Cmd);
	if (tcflush(dev_handle,TCIOFLUSH)<0) printf("error flushing serial io");
	status = write(dev_handle,Command,5);
	tcdrain(dev_handle);
	if (status!=5) print_error_message("write",__func__);
		
	status = read_until_CR(dev_handle,Val,serial_timeout);
	*size = status;
	/*else{
		wait_for_bytes(dev_handle,size,serial_timeout);
		status = read(dev_handle,Val,size);	
	}
	if (status<1) print_error_message("read",__func__);	
	if (size<1000) Val[status] = '\0';*/
	
	if (verbose) printf("xbee: received %s=%s\n",Cmd,Val);
	if (status<1) return FAILED; else return SUCCESS;
}

int channel_scan(int dev_handle,int scan_time){			// scan time in ms
														// range: 0-15000
	int len,status;	
	char Command[512];	
	
	printf("xbee: scanning channels in %f sec ...\n",(double)scan_time/1000);
	sprintf(Command,"ATED %X\r",scan_time);
	len = strlen(Command);
	//printf("command: %s\n, len=%d",Command,len);
	if (tcflush(dev_handle,TCIOFLUSH)<0) printf("error flushing serial io");
	status = write(dev_handle,Command,len);
	tcdrain(dev_handle);
	if (status!=len) print_error_message("write",__func__);
		
	int size = 128;
	char Buffer[128];		
	wait_for_bytes(dev_handle,size,serial_timeout);
	status = read(dev_handle,Buffer,size);	
	if (status<1) print_error_message("read",__func__);
	
	Buffer[status] = '\0';
	printf("scan result: %s\n",Buffer);
	if (status<1) return FAILED; else return SUCCESS;
	
	/*int size;	
	char Cmd[3];
	char Buffer[128];	
	
	printf("scanning channels ...\n");
	size = 128;
	sprintf(Cmd,"ED");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) return FAILED;		
	delete_cr(Buffer);
	printf("scan result: %s\n",Buffer);*/	
}

int get_xbee_device_info(int dev_handle){
	int status,p;
	int size[1];
	char Buffer[16];
	char Cmd[3];
	
	status = SUCCESS;
	
	*size = 8;
	printf("reading device info ... \n\n");
	printf("addresssing info:\n");	
	printf("own address:\n");
	sprintf(Cmd,"SH");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;
	printf("\tIEEE 64bit address High: %s\n",Buffer);
	sprintf(Cmd,"SL");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;
	printf("\tIEEE 64bit address Low: %s\n",Buffer);
	printf("destination address:\n");
	sprintf(Cmd,"DH");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;
	printf("\tIEEE 64bit address High: %s\n",Buffer);
	sprintf(Cmd,"DL");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;
	printf("\tIEEE 64bit address Low: %s\n",Buffer);
	
	sprintf(Cmd,"ID");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;
	printf("\tnetwork ID: %s\n",Buffer);
	sprintf(Cmd,"CI");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;
	printf("\tcluster ID: %s\n",Buffer);	
	sprintf(Cmd,"NI");
	*size = 21;
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;
	delete_cr(Buffer);
	printf("\tnode identifier: (%s)\n",Buffer);
	
	*size = 6;	
	sprintf(Cmd,"PL");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;		
	delete_cr(Buffer);
	if (strcmp(Buffer,"ERROR")==0) printf("\ttransmit power: ERROR\n");
	else{
		p = atoi(Buffer);
		switch(p){
			case PL_10dB: printf("\ttransmit power: %s\n",STRINGIZE(PL_10dB));break;
			case PL_12dB: printf("\ttransmit power: %s\n",STRINGIZE(PL_12dB));break;
			case PL_14dB: printf("\ttransmit power: %s\n",STRINGIZE(PL_14dB));break;
			case PL_16dB: printf("\ttransmit power: %s\n",STRINGIZE(PL_16dB));break;
			case PL_18dB: printf("\ttransmit power: %s\n",STRINGIZE(PL_18dB));break;
		}		
	}
	
	*size = 6;	
	sprintf(Cmd,"CH");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;		
	delete_cr(Buffer);
	printf("\tuse channel %s\n",Buffer);
	
	*size = 6;
	printf("interfacing info:\n");
	sprintf(Cmd,"BD");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;		
	if (strlen(Buffer)>3) printf("\tBAUD rate: %d\n",(int)strtol(Buffer,NULL,16));
	else{		
		int b = atoi(Buffer);
		switch(b){
			case BAUD_1200: p = 1200;break;
			case BAUD_2400: p = 2400;break;
			case BAUD_4800: p = 4800;break;
			case BAUD_9600: p = 9600;break;
			case BAUD_19200: p = 19200;break;
			case BAUD_38400: p = 38400;break;
			case BAUD_57600: p = 57600;break;
			case BAUD_115200: p = 115200;break;	
			default: p = 0;		
		}
		printf("\tBAUD rate: %d\n",p);
	}
	
	sprintf(Cmd,"RO");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;		
	delete_cr(Buffer);
	printf("\tpacketization timeout: %sx(char times)\n",Buffer);
	
	sprintf(Cmd,"FT");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;		
	delete_cr(Buffer);
	printf("\tflow control threshold: %s\n",Buffer);
	
	*size = 6;
	sprintf(Cmd,"AP");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;		
	printf("\tAPI mode: %s\n",Buffer);
	
	sprintf(Cmd,"AO");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;		
	printf("\tAPI output format: %s\n",Buffer);
	
	printf("firmware info:\n");
	*size = 8;
	sprintf(Cmd,"VR");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;		
	printf("\tfirmware version: %s\n",Buffer);
	
	sprintf(Cmd,"HV");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;		
	printf("\thardware version: %s\n",Buffer);
	
	sprintf(Cmd,"DD");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;		
	printf("\tdevice type ID: %s\n",Buffer);
	
	*size = 6;
	printf("security:\n");
	sprintf(Cmd,"EE");
	if (receive_AT_parameter(dev_handle,Cmd,Buffer,size)==FAILED) status = FAILED;		
	if (strcmp(Buffer,ERROR)==0) printf("\tencryption: ERROR\n");
	else{
		p = atoi(Buffer);
		if (p) printf("\tencryption: YES\n"); else printf("\tencryption: NO\n");
	}
	
	*size = 256;
	char B[256];
	sprintf(Cmd,"FN");
	if (receive_AT_parameter(dev_handle,Cmd,B,size)==FAILED) status = FAILED;	
	printf("\tneighbours: %s\n",B);
	
	printf("\n");
	return status;
}

int replace_CR(char* string,char r){
	char c;
	int i = 0;
	int res = 0;
	do{
		c = string[i];
		if (c=='\r'){
			string[i] = r;
			res++;
		}
		i++;
	}while(c!='\0');	
	return res;
}

void print_hex_string(unsigned char* String,int size){
	int i;
	for (i=0;i<size;i++) printf("0x%.2X\t",(unsigned char)String[i]);
	printf("\n");
}

unsigned char compute_check_sum(unsigned char* Buffer,int start,int end){
	union four_byte{
		uint32_t i;
		uint8_t c[4];
	} sum;
	int i;
	
	sum.i = 0;
	for (i=start;i<=end;i++) sum.i += Buffer[i];
	unsigned char check_sum = sum.c[0];
	check_sum = 0xFF-check_sum;
	return check_sum;
}

int test_check_sum(unsigned char* Buffer,int start,int end,unsigned char check_sum){
	union four_byte{
		uint32_t i;
		uint8_t c[4];
	} sum;
	int i;
	
	sum.i = 0;
	for (i=start;i<=end;i++) sum.i += Buffer[i];
	check_sum += sum.c[0];
	if (verbose) printf("sum: 0x%.2X\n",check_sum);
	if (check_sum==0xFF) return SUCCESS; else return FAILED;
}

void API_handle_special_char(unsigned char* Buffer,int* size){
	unsigned char c;
	int j,no_delim;
	
	if (API_escaped_mode==YES){
		int i = (*size)-1;
		while(i>=0){
			c = Buffer[i];
			if (i>0 && c==API_DELIM) no_delim = 1; else no_delim = 0;
			if (c==API_ESCAPE || c==API_XON || c==API_XOFF || no_delim){				
				for (j=(*size)-1;j>i;j--) Buffer[j+1] = Buffer[j];			
				Buffer[i+1] = c ^ API_XOR_MASK;
				Buffer[i] = API_ESCAPE;
				(*size)++;
			}
			i--;
		}
	}
}

void API_recover_special_char(unsigned char* Buffer,int* size){	
	int j;
	
	if (API_escaped_mode==YES){
		int i = 0;
		while(i<(*size)){		
			if (Buffer[i]==API_ESCAPE){				
				for (j=i;j<(*size)-1;j++) Buffer[j] = Buffer[j+1];
				Buffer[i] ^= API_XOR_MASK;
				(*size)--;
			}
			i++;
		}
	}
}

int API_count_escape(unsigned char* Buffer,int size){
	int i;
	
	int counter = 0;	
	if (API_escaped_mode==YES){
		for (i=0;i<size;i++) if (Buffer[i]==API_ESCAPE) counter++;	
	}
	return counter;
}

int API_receive_frame(int dev_handle,unsigned char* API_frame,int* size,
  unsigned char* message_type){
	const int max =10;
	
	union four_byte{
		uint32_t i;
		uint8_t c[4];
	} len;
	
	int i,n,status,xor;
	unsigned char c;
	unsigned char Buffer[512];
	int msg_compl = NO;
	int counter = 0;
	int pos = 0;
	
	n = 0;
	while(!msg_compl && counter<max){
		wait_for_bytes(dev_handle,1,serial_timeout);
		status = read(dev_handle,&c,1);
		if (status==1 && c==API_DELIM){			
			wait_for_bytes(dev_handle,4,serial_timeout);
			status = read(dev_handle,Buffer,4);
			if (status!=4) return FAILED;
			n = status;
			API_recover_special_char(Buffer,&n);
			len.i = 0;
			len.c[1] = Buffer[0];
			len.c[0] = Buffer[1];
			pos = 0;
			for (i=2;i<n;i++){
				API_frame[pos] = Buffer[i];
				if (verbose) printf("%d read: 0x%.2X\n",pos,Buffer[i]);
				pos++;
			}
			if (verbose) printf("length: %d\n",len.i);
			xor = NO;
			if (API_escaped_mode==YES){
				while(pos<len.i+1){
					wait_for_bytes(dev_handle,1,serial_timeout);
					status = read(dev_handle,&c,1);
					if (status!=1) return FAILED;
					if (c==API_ESCAPE) xor = YES;
					else{
						API_frame[pos] = (xor==YES) ? c^API_XOR_MASK : c;	
						if (verbose) printf("%d read xor: %d val: 0x%.2X\n",pos,xor,API_frame[pos]);
						xor = NO;			
						pos++;
						if (pos>=BUFFER_SIZE){
							printf("Buffer overlflow ! pos: %d length %d\n",pos,(int)len.i);
							fflush(stdout);						
						}
					}
				}
			}
			else{
				wait_for_bytes(dev_handle,len.i+1-pos,serial_timeout);
				if (read(dev_handle,Buffer,len.i+1-pos)!=len.i+1-pos) return FAILED;
				memcpy(&(API_frame[pos]),Buffer,(len.i+1-pos)*sizeof(char));
			}
			*message_type = API_frame[0];
			//*ack = API_frame[1];
			*size= len.i-1;
			if (test_check_sum(API_frame,0,len.i-1,API_frame[len.i])==SUCCESS) msg_compl = YES;
			else return FAILED;					
			for (i=1;i<len.i;i++) API_frame[i-1] = API_frame[i];					
		}
		else counter++;		
	}
	
	if (msg_compl==YES) return SUCCESS; else return FAILED;
}

void API_insert_params(unsigned char* Buffer,int* pos,char** Val,int num){
	union four_byte{
		uint32_t i;
		uint8_t c[4];
	} val;
	int i,j,fin;
	
	//pos = 7;
	for (i=0;i<num;i++){
		val.i = (uint32_t)atoi(Val[i]);
		fin = 0;
		for (j=3;j>=0;j--) if (val.c[j]!=0){
			fin = j;
			break;
		}
		for (j=0;j<=fin;j++){
			Buffer[*pos] = val.c[fin-j];
			(*pos)++;
		}		
	}
}

/*int API_receive_frame(int dev_handle,char* API_frame,int* size,
 unsigned char* message_type,unsigned char* ack){
	const int max_try = 100;
	
	union four_byte{
		uint32_t i;
		uint8_t c[4];
	} len,sum;
	int i,esc,b_size;
	unsigned char c;
	unsigned char Buffer[512];
		
	int counter = 0;
	int status = 0;
	int message_complete = 0;
	unsigned char check_sum = 0x00;
	len.i = 0;
	do{
		wait_for_bytes(dev_handle,1,serial_timeout);
		status = read(dev_handle,&c,1);	
		if (status==1 && c==API_DELIM){
			b_size = 2;
			if (wait_for_bytes(dev_handle,b_size,serial_timeout)==FAILED) break;
			status = read(dev_handle,Buffer,b_size);	
			if (status==b_size){
				esc = API_count_escape(Buffer,b_size);
				if (esc>0){
					if (wait_for_bytes(dev_handle,esc,serial_timeout)==FAILED) break;
					status = read(dev_handle,&(Buffer[b_size]),esc);	
					API_recover_special_char(Buffer,&b_size);				
				}
				len.c[1] = Buffer[0];
				len.c[0] = Buffer[1];
				if (verbose) printf("length: %d\n",len.i);
			}
			if (wait_for_bytes(dev_handle,len.i,serial_timeout)==FAILED) break;
			status = read(dev_handle,Buffer,len.i+1);	
			if (status==len.i+1){				
				check_sum = Buffer[len.i];
				Buffer[len.i] = '\0';
				if (verbose) printf("checksum: 0x%.2X\n",check_sum);				
				sum.i = 0;
				for (i=0;i<len.i;i++) sum.i += Buffer[i];
				check_sum += sum.c[0];
				if (check_sum!=0xFF){	
					fflush(stdout);				
					printf("checksum error -> skip message:\n");
					print_hex_string(Buffer,len.i);											
					break;
				}				
				*size= len.i-2;
				*message_type = Buffer[0];
				*ack = Buffer[1];
				memcpy(API_frame,&(Buffer[2]),(*size)*sizeof(char));
				message_complete = 1;
			}							
		}	
		else if (status<1) counter++;					
	}while(!message_complete && counter<max_try);
	
	if (message_complete) return SUCCESS; else return FAILED;
}*/

unsigned char API_parse_AT_command_response(unsigned char* Response_frame,int size,
char* Cmd,unsigned char** Val){
	 int i;
	 
	*Val = NULL;
	if (size<4){
		printf("wrong frame type: size=%d-> skip\n",size);
		return API_ERROR;		
	}
	Cmd[0] = Response_frame[1];
	Cmd[1] = Response_frame[2];
	Cmd[2] = '\0';
	char status = Response_frame[3];
	if (size>4){
		*Val = realloc(*Val,(size-4)*sizeof(unsigned char)); 
		memcpy(*Val,&(Response_frame[4]),(size-4)*sizeof(unsigned char));		
	}
	if (verbose){
		printf("received API frame:\n");
		printf("command: %s\n",Cmd);
		switch(status){
			case API_OK: printf("status: OK\n");break;
			case API_ERROR: printf("status: error\n");break;
			case API_INVALID_COMMAND: printf("status: invalid command\n");break;
			case API_INVALID_PARAMETER: printf("status: invalid parameter\n");break;				
		}
		printf("parameter: ");
		if ((*Val)!=NULL){
			printf("0x");
			for (i=0;i<size-4;i++) printf("%.2X",(unsigned char)(*Val)[i]);
			printf("\n");
		}
		else printf("none\n");
	}		
	return status;
}

unsigned char API_parse_AT_remote_command_response(unsigned char* Response_frame,int size,
char* Cmd,unsigned char* Addr_64,unsigned char** Val){
	 int i;
	 
	*Val = NULL;
	if (size<14){
		printf("wrong frame type: size=%d-> skip\n",size);
		return API_ERROR;		
	}
	for (i=0;i<8;i++) Addr_64[i] = Response_frame[i+1];
	// 9,10 reserved
	Cmd[0] = Response_frame[11];
	Cmd[1] = Response_frame[12];
	Cmd[2] = '\0';
	char status = Response_frame[13];
	if (size>14){
		*Val = realloc(*Val,(size-14)*sizeof(unsigned char)); 
		memcpy(*Val,&(Response_frame[14]),(size-14)*sizeof(unsigned char));		
	}
	if (verbose){
		printf("received API frame:\n");
		printf("command: %s\n",Cmd);
		switch(status){
			case API_OK: printf("status: OK\n");break;
			case API_ERROR: printf("status: error\n");break;
			case API_INVALID_COMMAND: printf("status: invalid command\n");break;
			case API_INVALID_PARAMETER: printf("status: invalid parameter\n");break;				
		}
		printf("parameter: ");
		if ((*Val)!=NULL){
			printf("0x");
			for (i=0;i<size-14;i++) printf("%.2X",(unsigned char)(*Val)[i]);
			printf("\n");
		}
		else printf("none\n");
	}		
	return status;
}

int API_send_AT_command(int dev_handle,char* Cmd,char** Val,int num,unsigned char ack){
	
	union four_byte{
		uint32_t i;
		uint8_t c[4];
	} len;
	
	int i,pos,status;
	unsigned char API_frame[512];	
	
	API_frame[0] = API_DELIM;
	API_frame[3] = API_AT_COMMAND;
	if (ack==API_RESPONSE) API_frame[4] = 0x01; else API_frame[4] = 0x00;
	API_frame[5] = Cmd[0];
	API_frame[6] = Cmd[1];
	
	// insert parameters
	pos = 7;
	API_insert_params(API_frame,&pos,Val,num);
	
	// compute length
	len.i = (uint32_t)pos-3;
	API_frame[1] = (unsigned char)len.c[1];
	API_frame[2] = (unsigned char)len.c[0];
	
	// compute checksum	
	API_frame[pos] = compute_check_sum(API_frame,3,pos-1);
	pos++;
	
	// handle special chars
	if (API_escaped_mode) API_handle_special_char(API_frame,&pos);
	
	if (verbose){
		printf("send API command type 0x08:\n");
		for (i=0;i<pos;i++) printf("0x%.2X\t",API_frame[i]);
		printf("\n");
		for (i=0;i<pos;i++) printf("%d\t",i);
		printf("\n");
	}
	
	// send
	if (tcflush(dev_handle,TCIOFLUSH)<0) printf("error flushing serial io");	
	status = write(dev_handle,API_frame,pos);
	tcdrain(dev_handle);
	if (status!=pos){
		print_error_message("write",__func__);	
		return FAILED;
	}
	else return SUCCESS;
}

int API_send_AT_remote_command(int dev_handle,unsigned char* Dest_addr, char* Cmd,
 unsigned char* Val,int size,unsigned char ack,unsigned char apply_ch){
	
	union four_byte{
		uint32_t i;
		uint8_t c[4];
	} len;
	
	int i,pos,status;
	unsigned char API_frame[512];	
	
	API_frame[0] = API_DELIM;
	API_frame[3] = API_REMOTE_AT_COMMAND;
	API_frame[4] = (ack==API_RESPONSE) ? 0x01 : 0x00;	
	for (i=0;i<8;i++) API_frame[5+i] = Dest_addr[i];
	API_frame[13] = 0xFF;
	API_frame[14] = 0xFE;
	API_frame[15] = (apply_ch==API_APPLY_CHANGES) ? 0x02 : 0x00;	
	API_frame[16] = Cmd[0];
	API_frame[17] = Cmd[1];
	
	pos = 18;
	for (i=0;i<size;i++){
		API_frame[i+18] = Val[i];	
		pos++;
	}
	
	len.i = (uint32_t)pos-3;
	API_frame[1] = (unsigned char)len.c[1];
	API_frame[2] = (unsigned char)len.c[0];
	
	API_frame[pos] = compute_check_sum(API_frame,3,pos-1);
	pos++;
	
	if (API_escaped_mode) API_handle_special_char(API_frame,&pos);
	
	if (verbose){
		printf("send API command type 0x17:\n");
		for (i=0;i<pos;i++) printf("0x%.2X\t",API_frame[i]);
		printf("\n");
		for (i=0;i<pos;i++) printf("%d\t",i);
		printf("\n");
	}
		
	tcdrain(dev_handle);		
	if (tcflush(dev_handle,TCIOFLUSH)<0) printf("error flushing serial io");	
	status = write(dev_handle,API_frame,pos);
	
	if (status!=pos){
		print_error_message("write",__func__);	
		return FAILED;
	}
	else return SUCCESS;
}

int API_transmit_data(int dev_handle,unsigned char* Dest_addr,char* Data,int size,
 unsigned char ack,unsigned char opt){
	union four_byte{
		uint32_t i;
		uint8_t c[4];
	} len;
	
	int i,frame_size,status;
	unsigned char API_frame[512];
	
	// create frame
	API_frame[0] = API_DELIM;
	len.i = size+11;
	API_frame[1] = len.c[1];
	API_frame[2] = len.c[0];
	API_frame[3] = API_TRANSMIT_REQUEST_64BIT;
	API_frame[4] = (ack==API_RESPONSE) ? 0x01 : 0x00;
	for (i=0;i<8;i++) API_frame[5+i] = Dest_addr[i];
	API_frame[13] = opt;
	for (i=0;i<size;i++) API_frame[14+i] = Data[i];
	
	// compute checksum
	unsigned char cs = compute_check_sum(API_frame,3,size+13);
	API_frame[size+14] = cs;
	frame_size = size+15;
	
	// handle special chars
	if (API_escaped_mode) API_handle_special_char(API_frame,&frame_size);
	
	if (verbose){
		printf("send API command type 0x00 (size=%d):\n",size);
		for (i=0;i<frame_size;i++) printf("0x%.2X\t",API_frame[i]);
		printf("\n");
		for (i=0;i<frame_size;i++) printf("%d\t",i);
		printf("\n");
	}
	
	// send
	if (tcflush(dev_handle,TCIOFLUSH)<0) printf("error flushing serial io");
	status = write(dev_handle,API_frame,frame_size);
	tcdrain(dev_handle);
	if (status!=frame_size){
		print_error_message("write",__func__);	
		return FAILED;
	}
	else return SUCCESS;
}

int transmit_string(int dev_handle,char** Param_values,char* Data){
	int ack,opt;
	unsigned char message;			
	char* dummy;	
	unsigned char API_frame[BUFFER_SIZE];	
		
	if (strcmp(Param_values[0],"yes")==0) ack = YES; else ack = NO;
	opt = strtol(Param_values[1],&dummy,16);
	if (opt>0xFF){
		printf("transmit: invalid option %s\n",Param_values[1]);
		return FAILED;
	}
	unsigned char Addr[8]; //{0x00,0x13,0xA2,0x00,0x40,0xDD,0x0A,0xDD};
	if (string_to_hex_array(Param_values[4],Addr)!=8){
		printf("transmit: invalid address %s\n",Param_values[4]);
		return FAILED;
	}
	int size = (int)strlen(Data);
	if (size==0){
		printf("transmit: no data to transmit\n");
		return FAILED;
	}
		
	API_transmit_data(dev_handle,Addr,Data,size,ack,(unsigned char)opt);
		
	if (ack==YES){
		printf("wait for response ...\n");
		fflush(stdout);
		if (API_receive_frame(dev_handle,API_frame,&size,&message)==SUCCESS){				
			printf("received: message type: 0x%.2X\t size: %d\n",message,size);						
		}
		else{
			printf("no response\n");
			return FAILED;
		}
	}
	return SUCCESS;
}

int transmit_string_fix(int dev_handle,char** Param_values,char* Data,int size){
	int ack,opt;
	unsigned char message;			
	char* dummy;	
	unsigned char API_frame[BUFFER_SIZE];	
		
	if (strcmp(Param_values[0],"yes")==0) ack = YES; else ack = NO;
	opt = strtol(Param_values[1],&dummy,16);
	if (opt>0xFF){
		printf("transmit: invalid option %s\n",Param_values[1]);
		return FAILED;
	}
	unsigned char Addr[8]; //{0x00,0x13,0xA2,0x00,0x40,0xDD,0x0A,0xDD};
	if (string_to_hex_array(Param_values[4],Addr)!=8){
		printf("transmit: invalid address %s\n",Param_values[4]);
		return FAILED;
	}
	
	API_transmit_data(dev_handle,Addr,Data,size,ack,(unsigned char)opt);
		
	if (ack==YES){
		printf("wait for response ...\n");
		fflush(stdout);
		if (API_receive_frame(dev_handle,API_frame,&size,&message)==SUCCESS){				
			printf("received: message type: 0x%.2X\t size: %d\n",message,size);						
		}
		else{
			printf("no response\n");
			return FAILED;
		}
	}
	return SUCCESS;
}

int enter_AT(int dev_handle){
	printf("entering AT command mode ...\n");
	int status = enter_AT_command_mode(dev_handle);
	if(status==FAILED) printf("xbee: failed to enter AT command mode\n");
	return status;
}

int exit_AT(int dev_handle){
	printf("exiting AT command mode ...\n");
	int status = exit_AT_command_mode(dev_handle);
	if(status==FAILED) printf("xbee: failed to exit AT command mode\n");
	return status;
}

void set_port(int argc,char** argv,char* device_name){
	int i;
	char Arg[512];
	char* part;
	
	sprintf(device_name,"/dev/ttyUSB0");		
	for (i=1;i<argc;i++){
		sprintf(Arg,"%s",argv[i]);
		part = strtok(Arg,"=");	
		if (strcmp(part,"-port")==0){
			part = strtok(NULL,"=");			
			sprintf(device_name,"/dev/%s",part);
		}
	}
	printf("use port %s\n",device_name);
}

void assign_params(int argc,char** argv,char** Names,char** Values,int num){
	int i,j;
	char Arg[512];
	char* part;
	
	for (i=1;i<argc;i++){
		sprintf(Arg,"%s",argv[i]);
		part = strtok(Arg,"=");			
		for (j=0;j<num;j++) if (strcmp(part,Names[j])==0){
			part = strtok(NULL,"=");
			if (part!=NULL){
				strcpy(Values[j],part);
				break;
			}
		}
	}	
}

char* Value_to_hex_string(unsigned char* Value,int size){
	int i;
	char Sub[BUFFER_SIZE];
	
	char* Result = (char*)malloc(BUFFER_SIZE*sizeof(char));
	sprintf(Result,"0x");
	for(i=0;i<size;i++){
		sprintf(Sub,"%.2X",Value[i]);
		strcat(Result,Sub);
	}	
	return Result;
}

int API_query_parameter(int dev_handle,char* Cmd,unsigned char** Value,int* vsize){
	unsigned char message,status;
	unsigned char API_frame[BUFFER_SIZE];
	char Cmd_back[BUFFER_SIZE];
	int size = 0;
		
	if (API_send_AT_command(dev_handle,Cmd,NULL,0,API_RESPONSE)==FAILED){
		if (verbose) printf("API error sending AT command\n");
		return FAILED;
	}
	if (API_receive_frame(dev_handle,API_frame,&size,&message)==FAILED){
		if (verbose) printf("API error: no response from device\n");
		return FAILED;
	}	
	
	if (message!=API_AT_COMMAND_RESPONSE){
		if (verbose) printf("API error: wrong response from device\n");
		return FAILED;
	} 		
	status = API_parse_AT_command_response(API_frame,size,Cmd_back,Value);
	if (status!=API_OK){
		if (verbose) printf("API transmission error: code %.2X 1st:0x%.2X\n",status,API_frame[0]);
		return FAILED;
	}
	if (strcmp(Cmd,Cmd_back)!=0){
		if (verbose) printf("API error: corrupted message\n");
		return FAILED;
	}
	*vsize = size-4;
		
	return SUCCESS;
}

int API_read_device_info(int dev_handle){	
	char* Out1;
	char* Out2;
	char Command[BUFFER_SIZE];
	
	unsigned char* Value = NULL;
	int size = 0;
	int res = SUCCESS;
	printf("host device info:\n");
	
	sprintf(Command,"HV");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	Out1 = Value_to_hex_string(Value,size);
	printf("\thardware Version: %s\n",Out1);	
	free(Out1);
	
	sprintf(Command,"VR");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	Out1 = Value_to_hex_string(Value,size);
	printf("\tfirmware Version: %s\n",Out1);
	free(Out1);
	
	sprintf(Command,"BD");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;	
	switch(Value[size-1]){
		case BAUD_1200: printf("\tbaud rate: 1200");break;
		case BAUD_2400: printf("\tbaud rate: 2400");break;
		case BAUD_4800: printf("\tbaud rate: 4800");break;
		case BAUD_9600: printf("\tbaud rate: 9600");break;
		case BAUD_19200: printf("\tbaud rate: 19200");break;
		case BAUD_38400: printf("\tbaud rate: 38400");break;
		case BAUD_57600: printf("\tbaud rate: 57600");break;
		case BAUD_115200: printf("\tbaud rate: 115200");break;
		default: 
			Out1 = Value_to_hex_string(Value,size);
			printf("\tunknown baud rate: %s",Out1);
			free(Out1);
	}	
	
	sprintf(Command,"NB");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	switch(*Value){
		case NO_PARITY: printf(", no parity\n");break;
		case EVEN_PARITY: printf(", even parity\n");break;
		case ODD_PARITY: printf(", odd parity\n");break;
		case FORCED_H_PARITY: printf(", forced high parity\n");break;
		case FORCED_L_PARITY: printf(", forced low parity\n");break;
		default: 
			Out1 = Value_to_hex_string(Value,size);
			printf(", unknwon parity: %s\n",Out1);
			free(Out1);
	}
	
	sprintf(Command,"AP");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	switch(Value[size-1]){
		case API_OFF: printf("\tinterfacing protocol: AT\n");break; 
		case API_ON: printf("\tinterfacing protocol: API, non escaped\n");break; 
		case API_ESC: printf("\tinterfacing protocol: API, escaped\n");break; 
		default:
			Out1 = Value_to_hex_string(Value,size);
			printf(",\tunknwon protocol: %s\n",Out1);
			free(Out1);
	}
	
	sprintf(Command,"SH");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	Out1 = Value_to_hex_string(Value,size);
	sprintf(Command,"SL");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	Out2 = Value_to_hex_string(Value,size);
	printf("\thost address: High=%s Low=%s\n",Out1,Out2);
	free(Out1);
	free(Out2);
	
	sprintf(Command,"ID");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	Out1 = Value_to_hex_string(Value,size);
	printf("\tPAN ID: %s\n",Out1);
	free(Out1);
	
	sprintf(Command,"CH");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	Out1 = Value_to_hex_string(Value,size);
	printf("\tchannel: %s\n",Out1);
	free(Out1);
	
	sprintf(Command,"DH");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	Out1 = Value_to_hex_string(Value,size);
	sprintf(Command,"DL");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	Out2 = Value_to_hex_string(Value,size);
	printf("\tstandard remote address: High=%s Low=%s\n",Out1,Out2);
	free(Out1);
	free(Out2);
	
	sprintf(Command,"NI");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;	
	Value[size] = '\0';
	printf("\tnode identifier: %s\n",(char*)Value);	
	
	sprintf(Command,"PL");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	switch(Value[size-1]){
		case PL_10dB: printf("\tpower level: 10dB\n");break;
		case PL_12dB: printf("\tpower level: 12dB\n");break;
		case PL_14dB: printf("\tpower level: 14dB\n");break;
		case PL_16dB: printf("\tpower level: 16dB\n");break;
		case PL_18dB: printf("\tpower level: 18dB\n");break;
		default:
			Out1 = Value_to_hex_string(Value,size);
			printf("\tunknown power level: %s\n",Out1);
			free(Out1);
	}
	
	sprintf(Command,"CA");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	Out1 = Value_to_hex_string(Value,size);
	printf("\tclear channel assessment threshold: -%sdB\n",Out1);
	free(Out1);
	
	printf("\n\n");
	if (Value!=NULL) free(Value);
	return res;
}

int API_read_device_IO_info(int dev_handle){
	const int IO_NUM = 9;
		
	int i;
	char* Out1;	
	char Command[BUFFER_SIZE];	
	const char* Func1[IO_NUM];
	const char* Func2[IO_NUM];
	const char* IO_NAMES[IO_NUM];
	
	IO_NAMES[0] = "pin 20";
	IO_NAMES[1] = "pin 19";
	IO_NAMES[2] = "pin 18";
	IO_NAMES[3] = "pin 17";
	IO_NAMES[4] = "pin 11";
	IO_NAMES[5] = "pin 15";
	IO_NAMES[6] = "pin 16";
	IO_NAMES[7] = "pin 12";
	IO_NAMES[8] = "pin 9";
	//IO_NAMES[9] = "pin 13";
	
	Func1[0] = "commissioning button";
	Func1[1] = "n/a";
	Func1[2] = "n/a";
	Func1[3] = "n/a";
	Func1[4] = "n/a";
	Func1[5] = "associated indicator";
	Func1[6] = "RTS flow control";
	Func1[7] = "CTS flow control";
	Func1[8] = "sleep request";
	//Func1[9] = "ON/output";
	
	Func2[0] = "ADC";
	Func2[1] = "ADC";
	Func2[2] = "ADC";
	Func2[3] = "ADC";
	Func2[4] = "ADC";
	Func2[5] = "ADC";
	Func2[6] = "n/a";
	Func2[7] = "n/a";
	Func2[8] = "n/a";
	//Func2[9] = "n/a";
	
	unsigned char* Value = NULL;
	int size = 0;
	int res = SUCCESS;
	printf("host device i/o pins:\n");
	
	sprintf(Command,"PR");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	Out1 = Value_to_hex_string(Value,size);
	printf("\tpull-up resistor mask: %s\n",Out1);
	free(Out1);
	
	sprintf(Command,"IU");
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	if (Value[size-1]==NO) printf("\tuart i/o output disabled\n");
	else if (Value[size-1]==YES) printf("\tuart i/o output enabled\n");
	else{
		Out1 = Value_to_hex_string(Value,size);
		printf("\tuart i/o output: unknown state (%s)\n",Out1);
		free(Out1);
	}
	
	for (i=0;i<IO_NUM;i++){
		sprintf(Command,"D%d",i);		
		if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
		switch(Value[size-1]){
			case IO_DISABLED: printf("\t%s (%s): disabled\n",Command,IO_NAMES[i]);break;
			case 0x01: printf("\t%s (%s): %s\n",Command,IO_NAMES[i],Func1[i]);break;
			case 0x02: printf("\t%s (%s): %s\n",Command,IO_NAMES[i],Func2[i]);break;
			case IO_DIG_INPUT: printf("\t%s (%s): digital in\n",Command,IO_NAMES[i]);break;
			case IO_LOW: printf("\t%s (%s): digital out@low\n",Command,IO_NAMES[i]);break;
			case IO_HIGH: printf("\t%s (%s): digital out@high\n",Command,IO_NAMES[i]);break;
			default:
				Out1 = Value_to_hex_string(Value,size);
				printf("\tunknown state at %s: %s\n",Command,Out1);
				free(Out1);
		}		
	}	
	
	sprintf(Command,"P0");		
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	switch(Value[size-1]){
		case IO_DISABLED: printf("\tP0 (pin 6): disabled\n");break;
		case IO_RSSI: printf("\tP0 (pin 6): RSSI output\n");break;
		case IO_PWM: printf("\tP0 (pin 6): PWM0 out\n");break;
		default: 	
			Out1 = Value_to_hex_string(Value,size);
			printf("\tunknown state at P0: %s\n",Out1);
			free(Out1);	
	}
	
	sprintf(Command,"P1");		
	if (API_query_parameter(dev_handle,Command,&Value,&size)==FAILED) res = FAILED;
	switch(Value[size-1]){
		case IO_DISABLED: printf("\tP1 (pin 7): disabled\n");break;
		case 0x01: printf("\tP1 (pin 7): n/a\n");break;
		case IO_PWM: printf("\tP1 (pin 7): PWM0 out\n");break;
		default: 	
			Out1 = Value_to_hex_string(Value,size);
			printf("\tunknown state at P1: %s\n",Out1);
			free(Out1);			
	}
	
	printf("\n\n");
	if (Value!=NULL) free(Value);
	return res;
}


int API_query_remote_parameter(int dev_handle,unsigned char* Addr_64,char* Cmd,
 unsigned char** Value,int* vsize){
	unsigned char message,status;
	unsigned char API_frame[BUFFER_SIZE];
	unsigned char Sender_addr[8];
	char Cmd_back[BUFFER_SIZE];
	int size = 0;	
		
	if (API_send_AT_remote_command(dev_handle,Addr_64,Cmd,NULL,0,API_RESPONSE,
	 API_NO_CHANGE)==FAILED){
		 if (verbose) printf("API error sending remote AT command\n");
		 return FAILED;		
	}
			
	if (API_receive_frame(dev_handle,API_frame,&size,&message)==FAILED){
		if (verbose) printf("API error: no response from device\n");
		return FAILED;
	}	
	
	if (message!=API_AT_REMOTE_COMMAND_RESPONSE){
		if (verbose) printf("API error: wrong response from device\n");
		return FAILED;
	} 		
	status = API_parse_AT_remote_command_response(API_frame,size,Cmd_back,Sender_addr,Value);
	if (status!=API_OK){
		if (verbose) printf("API transmission error: code %.2X 1st:0x%.2X\n",status,API_frame[0]);
		return FAILED;
	}
	if (strcmp(Cmd,Cmd_back)!=0){
		if (verbose) printf("API error: corrupted message\n");
		return FAILED;
	}
	*vsize = size-14;
	
	return SUCCESS;
}

int API_loopback_test(int dev_handle,unsigned char* Addr_64){
	
	unsigned char rssi_host,rssi_remote,message;
	char Data[BUFFER_SIZE];
	unsigned char API_frame[BUFFER_SIZE];
	unsigned char Received[BUFFER_SIZE];
	
	char* Options[5];
	Options[0] = "no";
	Options[1] = "0x00";
	Options[2] =  "";
	Options[3] = "ttyUSB0";
	Options[4] = "0013A20040DD0ADD";
	
	complete_flush(dev_handle);
	
	sprintf(Data,"loopback");
	if (transmit_string(dev_handle,Options,Data)==FAILED){		
		return FAILED;	
	}
	
	int size = 0;
	if (API_receive_frame(dev_handle,API_frame,&size,&message)==FAILED){
		printf("loopback test: no response from device\n");
		return FAILED;
	}
	
	if (message!=API_RECEIVE_TRANSMISSION){
		printf("loopback test: wrong message type received\n");
		return FAILED;
	} 		
	
	if (size<10){
		printf("loopback test: broken frame\n");
		return FAILED;
	}
		
	int len = size-10;
	rssi_host = API_frame[8];
	rssi_remote = API_frame[size-1];
	memcpy(Received,&(API_frame[10]),(len-1)*sizeof(unsigned char));
	Received[len-1] = '\0';
	if (strcmp((char*)Received,"rssi=")!=0){
		printf("loopback test: wrong response\n");
		return FAILED;
	}
	
	printf("loopback test: OK\n");
	printf("signal strength at host: 0x%X\n",rssi_host);
	printf("signal strength at remote: 0x%X\n",rssi_remote);
	printf("remote MCU online...\n\n");
	
	return SUCCESS;	
}

int API_remote_xbee_test(int dev_handle,unsigned char* Addr_64){
	char Cmd[BUFFER_SIZE];
	char* Out1;
	char* Out2;
	
	unsigned char* Value = NULL;
	int res = SUCCESS;
	int size = 0;
	
	complete_flush(dev_handle);
	
	//verbose = 1;
	sprintf(Cmd,"SH");	
	if (API_query_remote_parameter(dev_handle,Addr_64,Cmd,&Value,&size)==FAILED){
		//verbose = 0;
		return FAILED;	
	}	
	Out1 = Value_to_hex_string(Value,size);
	sprintf(Cmd,"SL");
	if (API_query_remote_parameter(dev_handle,Addr_64,Cmd,&Value,&size)==FAILED){
		//verbose = 0;
		return FAILED;	
	}
	Out2 = Value_to_hex_string(Value,size);
	printf("remote test xbee: OK\n");
	printf("serial number: High=%s Low=%s\n",Out1,Out2);
	printf("remote xbee online ...\n");
	free(Out1);
	free(Out2);
	
	printf("\n");
	if (Value!=NULL) free(Value);
	//verbose = 0;
	return res;	
}

int API_remote_gyro_test(int dev_handle,unsigned char* Addr_64){
	const unsigned char mask1 = 0x07;
	const unsigned char mask2 = 0x18;
	
	unsigned char check,message,info;
	char Data[BUFFER_SIZE];
	unsigned char API_frame[BUFFER_SIZE];
	unsigned char Received[BUFFER_SIZE];
	
	char* Options[5];
	Options[0] = "no";
	Options[1] = "0x00";
	Options[2] =  "";
	Options[3] = "ttyUSB0";
	Options[4] = "0013A20040DD0ADD";	
	
	complete_flush(dev_handle);
	
	sprintf(Data,"BMA220check");		
	if (transmit_string(dev_handle,Options,Data)==FAILED) return FAILED;	
	
	int size = 0;
	if (API_receive_frame(dev_handle,API_frame,&size,&message)==FAILED){
		printf("BMA220 test: no response from device\n");
		goto GYRO_FAILED;
	}
	
	if (message!=API_RECEIVE_TRANSMISSION){
		printf("BMA220 test: wrong message type received\n");
		goto GYRO_FAILED;
	} 		
	
	if (size<10){
		printf("BMA220 test: broken frame\n");
		goto GYRO_FAILED;
	}
	
	int len = size-10;
	check = API_frame[size-2];
	info = API_frame[size-1];
	memcpy(Received,&(API_frame[10]),(len-2)*sizeof(unsigned char));
	Received[len-2] = '\0';
	if (strcmp((char*)Received,"BMA220")!=0){
		printf("BMA220 test: wrong message string %s received\n",(char*)Received);
		goto GYRO_FAILED;
	}
	
	if (check==SUCCESS){
		printf("gyro sensor test: OK\n");
		
		unsigned char BW = info & mask1;
		unsigned char RA = info & mask2;
		RA >>= 3;
		
		printf("range: 0x%.2X\n",RA);
		printf("bandwidth: 0x%.2X\n",BW);
		printf("BMA220 online ...\n\n");
		return SUCCESS;
	}
	else{
	GYRO_FAILED:		
		return FAILED;		
	}
}

int API_remote_ADC_test(int dev_handle,unsigned char* Addr_64){
	
	union four_byte{
		uint32_t i;
		uint8_t c[4];
	} geir,roll,nick;	
	
	unsigned char message;
	char Data[BUFFER_SIZE];
	unsigned char API_frame[BUFFER_SIZE];
	unsigned char Received[BUFFER_SIZE];
	
	char* Options[5];
	Options[0] = "no";
	Options[1] = "0x00";
	Options[2] =  "";
	Options[3] = "ttyUSB0";
	Options[4] = "0013A20040DD0ADD";	
	
	complete_flush(dev_handle);
	
	sprintf(Data,"ADCcheck");		
	if (transmit_string(dev_handle,Options,Data)==FAILED) return FAILED;	
	
	int size = 0;
	if (API_receive_frame(dev_handle,API_frame,&size,&message)==FAILED){
		if (verbose) printf("ADC test: no response from device\n");
		return FAILED;
	}
	
	if (message!=API_RECEIVE_TRANSMISSION){
		if (verbose) printf("ADC test: wrong message type received\n");
		return FAILED;
	} 		
	
	if (size<10){
		if (verbose) printf("ADC test: broken frame\n");
		return FAILED;
	}
	
	int len = size-10;
	memcpy(Received,&(API_frame[10]),len*sizeof(unsigned char));
	Received[len] = '\0';
		
	if (Received[0]=='G' && Received[3]=='R' && Received[6]=='N'){
		geir.c[3] = 0;
		geir.c[2] = 0;
		geir.c[1] = (uint8_t)Received[1];
		geir.c[0] = (uint8_t)Received[2];
		roll.c[3] = 0;
		roll.c[2] = 0;
		roll.c[1] = (uint8_t)Received[4];
		roll.c[0] = (uint8_t)Received[5];
		nick.c[3] = 0;
		nick.c[2] = 0;
		nick.c[1] = (uint8_t)Received[7];
		nick.c[0] = (uint8_t)Received[8];
		
		printf("acceleration sensor test: OK\n");
		printf("geir: 0x%.4X\troll: 0x%.4X\tnick: 0x%.4X\n",geir.i,roll.i,nick.i);	
		printf("secondary MCU online ...\n\n");
		return SUCCESS;
	}
	else if (strcmp((char*)Received,"ERROR")!=0)// printf("accelaration sensor test: FAILED\n\n");
	printf("accelaration sensor test: unknown error (received: %s)\n\n",Received);
	
	return FAILED;
}

int test_xbee_online(int dev_handle){
	complete_flush(dev_handle);
	if (enter_AT(dev_handle)==FAILED) return FAILED;
	
	if (get_xbee_device_info(dev_handle)==SUCCESS) printf("xbee online\n device info:\n");		
	else return FAILED;
	
	exit_AT(dev_handle);
	return SUCCESS;
}

int API_remote_power_cell_test(int dev_handle,unsigned char* Addr_64,int quiet){
	
	int pos;
	unsigned char message;
	char Data[BUFFER_SIZE];
	unsigned char API_frame[BUFFER_SIZE];
	unsigned char Received[BUFFER_SIZE];
	unsigned char* Sub;
	
	char* Options[5];
	Options[0] = "no";
	Options[1] = "0x00";
	Options[2] =  "";
	Options[3] = "ttyUSB0";
	Options[4] = "0013A20040DD0ADD";	
	
	complete_flush(dev_handle);
	
	sprintf(Data,"cell");		
	if (transmit_string(dev_handle,Options,Data)==FAILED) return FAILED;	
	
	int size = 0;
	if (API_receive_frame(dev_handle,API_frame,&size,&message)==FAILED){
		if (verbose) printf("power cell test: no response from device\n");
		return FAILED;
	}
	
	if (message!=API_RECEIVE_TRANSMISSION){
		if (verbose) printf("power cell test: wrong message type received\n");
		return FAILED;
	} 		
	
	if (size<10){
		if (verbose) printf("power cell test: broken frame\n");
		return FAILED;
	}
	
	int len = size-10;
	memcpy(Received,&(API_frame[10]),len*sizeof(unsigned char));
	Received[len] = '\0';
	
	if ((Sub=(unsigned char*)strstr((char*)Received,"CELL"))!=NULL && len>=7){
		pos = 4;								
		double V1 = (double)Sub[pos++];
		V1 *= 0.0627;
		double V2 = (double)Sub[pos++];
		V2 *= 0.0629;
		double V3 = (double)Sub[pos++];
		V3 *= 0.0412;
		if (quiet==NO){
			printf("\nbattery power: total %.2fV\n",V1);
			printf("cell 1: %.2fV\n",V1-V2);
			printf("cell 2: %.2fV\n",V2-V3);
			printf("cell 3: %.2fV\n\n",V3);			
		}
		else{
			if (V1<9.5) printf("WARNING: low battery power at %fV !\n",V1);
		}
		return SUCCESS;
	}
	else return FAILED;
}

void filter(){
	const int n = 10000;
	
	int i;
	double data,t;
	int16_t rec;
	float32 X;
	
	float32 alpha = init_float32(1);
	float32 h = init_float32(50);
	div_float32(&alpha,&h);
	
	FILE* file = fopen("/home/radszuweit/Daten/filter","w");
	
	float32 x = init_float32(0);
	for (i=1;i<n;i++){
		t = (double)i/n*10*M_PI;
		data = 255.*(sin(t)+sin(2.*t+0.2)/3.)/2.;
		data += (double)20.*rand()/RAND_MAX;		
		X = init_float32((int16_t)data);
		//memcpy(&x0,&x,sizeof(float32));
				
		sub_float32(&X,&x);
		mult_float32(&X,&alpha);
		add_float32(&x,&X);
		rec = float32_to_int16(x);
		
		fprintf(file,"%f\t%d\t%d\n",t,(int)data,rec);
	}
	
	fclose(file);
	exit(0);
}

int API_send_string_with_ack(int dev_handle,char* String,char* Ack_response){
	unsigned char message;	
	unsigned char API_frame[BUFFER_SIZE];
	unsigned char Received[BUFFER_SIZE];
	
	char* Options[5];
	Options[0] = "no";
	Options[1] = "0x00";
	Options[2] =  "";
	Options[3] = "ttyUSB0";
	Options[4] = "0013A20040DD0ADD";	
	
	complete_flush(dev_handle);
		
	if (transmit_string(dev_handle,Options,String)==FAILED) return FAILED;	
	
	if (Ack_response==NULL) return SUCCESS;
	
	int size = 0;
	if (API_receive_frame(dev_handle,API_frame,&size,&message)==FAILED){
		printf("send %s: no response from device\n",String);
		return FAILED;
	}
	
	if (message!=API_RECEIVE_TRANSMISSION){
		printf("%s: wrong message type received\n",String);
		return FAILED;
	} 		
	
	if (size<10){
		printf("%s: broken frame\n",String);
		return FAILED;
	}
		
	int len = size-10;
	memcpy(Received,&(API_frame[10]),len*sizeof(unsigned char));
	Received[len] = (char)'\0';
	if (strcmp((char*)Received,Ack_response)!=0){
		printf("send %s: FAILED\n",String);
		printf("received: length: %d content: ",len);
		int i;
		for (i=0;i<len;i++) printf("%x:",Received[i]);
		printf("\n");
		fflush(stdout);
		return FAILED;
	}
	else{
		printf("send %s: OK\n",String);
		fflush(stdout);
		return SUCCESS;	
	}
}

int API_send_fix_string_with_ack(int dev_handle,char* String,int length,char* Ack_response){
	unsigned char message;	
	unsigned char API_frame[BUFFER_SIZE];
	unsigned char Received[BUFFER_SIZE];
	
	char* Options[5];
	Options[0] = "no";
	Options[1] = "0x00";
	Options[2] =  "";
	Options[3] = "ttyUSB0";
	Options[4] = "0013A20040DD0ADD";	
	
	complete_flush(dev_handle);
		
	if (transmit_string_fix(dev_handle,Options,String,length)==FAILED) return FAILED;	
	
	if (Ack_response==NULL) return SUCCESS;
	
	int size = 0;
	if (API_receive_frame(dev_handle,API_frame,&size,&message)==FAILED){
		printf("send %s: no response from device\n",String);
		return FAILED;
	}
	
	if (message!=API_RECEIVE_TRANSMISSION){
		printf("%s: wrong message type received\n",String);
		return FAILED;
	} 		
	
	if (size<10){
		printf("%s: broken frame\n",String);
		return FAILED;
	}
		
	int len = size-10;
	memcpy(Received,&(API_frame[10]),len*sizeof(unsigned char));
	Received[len] = (char)'\0';
	if (strcmp((char*)Received,Ack_response)!=0){
		printf("send %s: FAILED\n",String);
		printf("received: length: %d content: ",len);
		int i;
		for (i=0;i<len;i++) printf("%x:",Received[i]);
		printf("\n");
		fflush(stdout);
		return FAILED;
	}
	else{
		printf("send %s: OK\n",String);
		fflush(stdout);
		return SUCCESS;	
	}
}

int API_send_start(int dev_handle){
	const int max_try = 10;
	int counter = 0,status;
	
	//set_timout(50);
	do{
		if (counter>0 && verbose) printf("retry ...\n");
		fflush(stdout);
		status = API_send_string_with_ack(dev_handle,"startsend","OK");
		counter++;
	}while(status==FAILED && counter<max_try);
	//set_timout(TIMEOUT);
	
	return status;
}

int API_send_stop(int dev_handle){
	const int max_try = 10;
	int counter = 0,status;
	
	do{
		if (counter>0 && verbose) printf("retry ...\n");
		fflush(stdout);
		status = API_send_string_with_ack(dev_handle,"stopsend","OK");
		counter++;
	}while(status==FAILED && counter<max_try);
	
	return status;
}

void send_parameter_data(int dev_handle,char* Filename){
	char Buffer[BUFFER_SIZE];
	char Out[BUFFER_SIZE];
	unsigned char API_frame[BUFFER_SIZE];
	unsigned char Received[BUFFER_SIZE];
	unsigned char message;
	char* Part;
	int16 val;	
	
	char* Options[5];
	Options[0] = "no";
	Options[1] = "0x00";
	Options[2] =  "";
	Options[3] = "ttyUSB0";
	Options[4] = "0013A20040DD0ADD";	
		
	sprintf(Out,"eeprom");
	FILE* file = fopen(Filename,"r");
	if (file!=NULL){	
		int pos = strlen(Out);
		while(fgets(Buffer,BUFFER_SIZE,file)!=NULL){
			Part = strtok(Buffer,"\t\n");
			//printf("set %s: ",Part);
			Part = strtok(NULL,"\t\n");
			if (Part!=NULL){				
				val.t16 = (int16_t)atoi(Part);
				//printf("%d\n",val.t16);
				Out[pos] = (char)val.t8[1];
				pos++;
				Out[pos] = (char)val.t8[0];
				pos++;
			}
		}		
		Out[pos] = '\0';	
		fclose(file);
		fflush(stdout);	
		
		complete_flush(dev_handle);
		if (transmit_string_fix(dev_handle,Options,Out,pos)==FAILED){
			printf("error transmitting string ->skip\n");
		}	
	
		int size = 0;
		if (API_receive_frame(dev_handle,API_frame,&size,&message)==FAILED){
			printf("receive parameters: no response from device\n");
			return;
		}
	
		if (message!=API_RECEIVE_TRANSMISSION){
			if (verbose) printf("receive parameters: wrong message type received\n");
		} 		
	
		if (size<10){
			if (verbose) printf("receive parameters: broken frame\n");
		}
	
		int len = size-10;
		memcpy(Received,&(API_frame[10]),len*sizeof(unsigned char));
		Received[len] = '\0';
			
		unsigned char* Sub;
		if ((Sub=(unsigned char*)strstr((char*)Received,"PA"))!=NULL && len>=10){
			printf("\nreceived parameters:\n");
			pos = 2;		
			val.t8[1] = Sub[pos++];
			val.t8[0] = Sub[pos++];
			printf("Kp: %d\n",val.t16);
			val.t8[1] = Sub[pos++];
			val.t8[0] = Sub[pos++];
			printf("Ki: %d\n",val.t16);
			val.t8[1] = Sub[pos++];
			val.t8[0] = Sub[pos++];
			printf("Kd: %d\n",val.t16);
			val.t8[1] = Sub[pos++];
			val.t8[0] = Sub[pos++];
			printf("Ka: %d\n",val.t16);
			printf("\n");
		}
		else printf("no parameters received from remote device\n");			
	}
	else printf("could not open paramter file %s -> skip\n",Filename);	
}

void handle_input(int dev_handle,char** Options){
	char Buffer[BUFFER_SIZE];
	char Tra[BUFFER_SIZE];
	printf("\n");
	API_send_stop(dev_handle);
	if (fgets(Buffer,BUFFER_SIZE,stdin)!=NULL){
		if (strcmp(Buffer,"exit\n")==0){
			if (dev_handle>0) close(dev_handle);
			printf("\nclose device %d\n",dev_handle);
			printf("exit program\n");		
			exit(0);		
		}
		else if (strcmp(Buffer,"sd\n")==0){
			sprintf(Tra,"lock");
			API_send_string_with_ack(dev_handle,Tra,"OK");	
		}
		else if (strcmp(Buffer,"eeprom\n")==0){
			char* Name = "/home/radszuweit/AVR/Programme/qcopter/parameters";
			send_parameter_data(dev_handle,Name);
		}
		else if (strcmp(Buffer,"reset\n")==0){
			sprintf(Tra,"reset");
			API_send_string_with_ack(dev_handle,Tra,"OK");	
		}
		else if (strcmp(Buffer,"zero\n")==0){
			sprintf(Tra,"zero");
			API_send_string_with_ack(dev_handle,Tra,"OK");	
		}
		else if (strcmp(Buffer,"start\n")==0){
			sprintf(Tra,"calibrate");
			API_send_string_with_ack(dev_handle,Tra,"OK");	
		}
		else{
			char Local[BUFFER_SIZE];
			strcpy(Local,Buffer);
			char* Part = strtok(Local," \n");
			if (strcmp(Part,"transmit")==0){						
				Part = strtok(NULL," \n");
				if (Part!=NULL){											
					API_send_string_with_ack(dev_handle,Part,"OK");									
				}
			}					
		}				
	}
	input_flag = NO;
	API_send_start(dev_handle);		
}

void sig_handler(int signal){
	if (signal==SIGINT) input_flag = YES;
}

int main(int argc,char *argv[]){
		
	//filter();
		
	const int param_number = 5;
	
	int i;
	
	int dev_handle = 0;	
	
	char* FileName = "/Home/Daten/remote.out";
	FILE* file = fopen(FileName,"w");
	
	if (file==NULL){
		printf("could not open file %s ->abort\n",FileName);
		exit(0);
	}
	fclose(file);
	
	clock_t start = clock(), diff;

	char** Param_names = (char**)malloc(param_number*sizeof(char*));
	char** Param_values = (char**)malloc(param_number*sizeof(char*));
	for (i=0;i<param_number;i++){
		 Param_names[i] = (char*)malloc(BUFFER_SIZE*sizeof(char));
		 Param_values[i] = (char*)malloc(BUFFER_SIZE*sizeof(char));
	}

	char device_name[BUFFER_SIZE];
	char Param[BUFFER_SIZE];
	char* Part;
	
	char** Params = (char**)malloc(sizeof(char*));
	Params[0] = (char*)malloc(16*sizeof(char));
	char Command[3] = "\0";		
	
	// parameter names
	sprintf(Param_names[0],"-acknowledge");
	sprintf(Param_names[1],"-trasmit_opt");
	sprintf(Param_names[2],"-data");
	sprintf(Param_names[3],"-port");
	sprintf(Param_names[4],"-dest_addr");
	
	// default values	
	sprintf(Param_values[0],"no");
	sprintf(Param_values[1],"0x00");
	Param_values[2][0] = '\0';
	sprintf(Param_values[3],"ttyUSB0");
	sprintf(Param_values[4],"0013A20040DD0ADD");
	
	//unsigned char Addr[8] = {0x00,0x13,0xA2,0x00,0x40,0xDD,0x0A,0xDD};	
	
	signal(SIGINT,&sig_handler);
	
	if (argc>1) set_port(argc,argv,device_name);
	assign_params(argc,argv,(char**)Param_names,(char**)Param_values,param_number);
	init_serial_port(device_name,&dev_handle);
	
	sprintf(Param,"%s",argv[1]);
	Part = strtok(Param,"=");
	if (strcmp(Part,"reset")==0){
		if (enter_AT(dev_handle)==FAILED) goto FINALIZE;
		printf("reset device ... \n");
		sprintf(Command,"RE");	
		if (send_AT_command(dev_handle,Command,Params,0)==SUCCESS) printf("xbee: reset to factory defaults\n");	
		exit_AT(dev_handle);
	}
	
	else if (strcmp(Param,"info")==0){
		if (enter_AT(dev_handle)==FAILED) goto FINALIZE;
		if (get_xbee_device_info(dev_handle)==SUCCESS) printf("xbee: device info\n");		
		exit_AT(dev_handle);
	}
	
	else if (strcmp(Param,"configure")==0){
		char* Dest_H = "13A200";
		char* Dest_L = "40DD0ADD";
			
		if (enter_AT(dev_handle)==FAILED) goto FINALIZE;
			
		sprintf(Command,"DH");
		if (send_AT_command(dev_handle,Command,&Dest_H,1)==SUCCESS) printf("xbee: command %s accepted\n",Command);
	
		sprintf(Command,"DL");
		if (send_AT_command(dev_handle,Command,&Dest_L,1)==SUCCESS) printf("xbee: command %s accepted\n",Command);	
		
		printf("\nset xbee's i/o configuration:\n");	
			
		// remote
		sprintf(Params[0],"%d",IO_ADC);	
		sprintf(Command,"D0");
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: IO0 -> ADC\n");		
		sprintf(Command,"D1");	
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: IO1 -> ADC\n");		
		sprintf(Command,"D2");	
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: IO2 -> ADC\n");
		sprintf(Command,"P0");	
		sprintf(Params[0],"%d",IO_RSSI);
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: IO10 -> RSSI\n");
				
		sprintf(Command,"CA");
		sprintf(Params[0],"%X",60);
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: command %s accepted\n",Command);
	
		sprintf(Command,"CH");
		sprintf(Params[0],"0C");
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: command %s accepted\n",Command);	
	
		sprintf(Command,"FT");
		sprintf(Params[0],"EE");
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: command %s accepted\n",Command);			
	
		sprintf(Command,"AP");
		sprintf(Params[0],"%X",API_OFF);
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: entering API mode ...\n");
	
		sprintf(Command,"PL");
		sprintf(Params[0],"%X",PL_10dB);
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: command %s accepted\n",Command);
	
		sprintf(Command,"CI");
		sprintf(Params[0],"44");	
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: command %s accepted\n",Command);			
	
		sprintf(Command,"NI");
		sprintf(Params[0],"NODE_1");
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: command %s accepted\n",Command);	
			
		sprintf(Command,"BD");
		sprintf(Params[0],"05");	
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: command %s accepted\n",Command);			
	
		if (execute_AT_commands(dev_handle)==SUCCESS) printf("xbee: apply changes\n");
			
		sprintf(Command,"WR");	
		if (send_AT_command(dev_handle,Command,Params,0)==SUCCESS) printf("xbee: wrote settings to flash\n");
		usleep(100000);
		exit_AT(dev_handle);
	}
	
	else if (strcmp(Param,"remote")==0){
		if (enter_AT(dev_handle)==FAILED) goto FINALIZE;
		printf("\nxbee IO info:\n");	
		int cr = 0;
		int size = 0;
		char IO_info[1024];
		sprintf(Command,"IS");
		int k = 0;
		while(k<1000){
			int j;
			for (j=0;j<cr+1;j++) printf("\033[A");
			receive_AT_parameter(dev_handle,Command,IO_info,&size);  // Achtung: mehrere CRs nicht berücksichtigt !
			size += read_until_CR(dev_handle,&IO_info[size],serial_timeout);
			size += read_until_CR(dev_handle,&IO_info[size],serial_timeout);
			cr = replace_CR(IO_info,'\n');			
			printf("%s\n",IO_info);
			usleep(100000);
			k++;
		}
		exit_AT(dev_handle);
	}
	
	else if (strcmp(Param,"atcmd")==0){
		if (enter_AT(dev_handle)==FAILED) goto FINALIZE;
		sprintf(Command,"%s",argv[2]);
		int len = argc-2;
		if (len>0) sprintf(Params[0],"%s",argv[3]);				
		if (send_AT_command(dev_handle,Command,Params,len)==SUCCESS) printf("xbee: command %s accepted\n",Command);				
			
		sprintf(Command,"WR");	
		if (send_AT_command(dev_handle,Command,Params,0)==SUCCESS) printf("xbee: wrote settings to flash\n");
		usleep(100000);
		exit_AT(dev_handle);
	}
	
	else if (strcmp(Param,"api")==0){	
		verbose = 1;
			
		unsigned char* Parameter = NULL;
		unsigned char API_frame[512];
		char AT_cmd[3];
		unsigned char message = 0;
		//unsigned char ack = 0;
		int size = 0;
					
		sprintf(Command,"AP");	
		if (API_escaped_mode==YES) sprintf(Params[0],"%X",API_ESC); else sprintf(Params[0],"%X",API_ON);
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: set API command mode ...\n");	
		if (execute_AT_commands(dev_handle)==SUCCESS) printf("xbee: done\n");
		if (exit_AT_command_mode(dev_handle)==SUCCESS) printf("xbee: exit AT command mode\n");
			
		sprintf(Command,"DL");		
		sprintf(Params[0],"%d",0x0013A200);
		API_send_AT_command(dev_handle,Command,Params,0,API_RESPONSE);
			
		printf("wait for response ...\n");
		fflush(stdout);
		if (API_receive_frame(dev_handle,API_frame,&size,&message)==SUCCESS){				
			if (message!=API_AT_COMMAND_RESPONSE) printf("Warning: response message 0x%X of wrong type\n",message);
			API_parse_AT_command_response(API_frame,size,AT_cmd,&Parameter);
		};
			
		unsigned char Addr[8] = {0x00,0x13,0xA2,0x00,0x40,0xDD,0x0A,0xDD};
		/*char Data[512];
		sprintf(Data,"Hallihallo!");
		size = (int)strlen(Data)-1;
		API_transmit_data(dev_handle,Addr,Data,size,YES,0x00);*/
			
		sprintf(Command,"M1");
		unsigned char Value[2] = {0x00,0xFF};
		API_send_AT_remote_command(dev_handle,Addr,Command,Value,2,API_RESPONSE,YES);
			
		printf("wait for response ...\n");
		fflush(stdout);
		if (API_receive_frame(dev_handle,API_frame,&size,&message)==SUCCESS){				
			printf("received: message type: 0x%.2X\t size: %d\n",message,size);					
		};
			
		close(dev_handle);	
		exit(0);
		/*usleep(100000);
		int byte_count = 0;
		ioctl(dev_handle,FIONREAD,&byte_count);
		printf("%d bytes received\n",byte_count);*/
	}
	
	else if (strcmp(Param,"listen")==0){		
		int rssi,broadcast,len,pos,counter;
		int16 gx,gy,gz,geir,roll,nick;
		int32 avr_time;
		unsigned char API_frame[BUFFER_SIZE];
		unsigned char Buffer[BUFFER_SIZE];
		unsigned char Send_address[8];
		
		int size = 0;
		int max_retry = 10;
		unsigned char message = 0;
		unsigned char Remote_address[8] = {0x00,0x13,0xA2,0x00,0x40,0xDD,0x0A,0xDD};	
						
		/*sprintf(Command,"AP");	
		if (API_escaped_mode==YES) sprintf(Params[0],"%X",API_ESC); else sprintf(Params[0],"%X",API_ON);
		if (send_AT_command(dev_handle,Command,Params,1)==SUCCESS) printf("xbee: set API command mode ...\n");	
		if (execute_AT_commands(dev_handle)==SUCCESS) printf("xbee: done\n");
		if (exit_AT_command_mode(dev_handle)==SUCCESS) printf("xbee: exit AT command mode\n");						*/		
		
		API_read_device_info(dev_handle);
		
		API_read_device_IO_info(dev_handle);	
		
		API_send_stop(dev_handle);
		
		counter = 0;
		while(API_remote_xbee_test(dev_handle,Remote_address)==FAILED && counter<max_retry){counter++;}					
		if (counter==max_retry) printf("remote xbee test: failed\n");
		
		counter = 0;
		while(API_loopback_test(dev_handle,NULL)==FAILED && counter<max_retry){counter++;}					
		if (counter==max_retry) printf("loopback test: failed\n");
		
		counter = 0;		
		while(API_remote_gyro_test(dev_handle,NULL)==FAILED && counter<max_retry){counter++;}					
		if (counter==max_retry) printf("gyro sensor test: failed\n");		
		
		counter = 0;
		while(API_remote_ADC_test(dev_handle,NULL)==FAILED && counter<max_retry){counter++;}					
		if (counter==max_retry) printf("accelaration sensor test: FAILED\n");
		
		counter = 0;
		while(API_remote_power_cell_test(dev_handle,NULL,NO)==FAILED && counter<max_retry){counter++;}					
		if (counter==max_retry) printf("power cell test: FAILED\n");		
		
		API_send_start(dev_handle);
				
		fflush(stdout);
		printf("\nserial port is listening ...\n");
		while(1){			
			if (wait_for_bytes(dev_handle,1,serial_timeout)==SUCCESS){
				if (verbose) printf("received sth.\n");
				if(API_receive_frame(dev_handle,API_frame,&size,&message)==SUCCESS){		
					//printf("received: message type: 0x%.2X\t size: %d\n",message,size);	
					switch(message){
						case API_RECEIVE_TRANSMISSION: 
							if (size<10){
								printf("broken frame -> skip\n");
								break;
							}
							len = size-10;
							memcpy(Buffer,&(API_frame[10]),len*sizeof(char));
							Buffer[len] = '\0';
							unsigned char* Sub;
							//verbose = 1;
							if ((Sub=(unsigned char*)strstr((char*)Buffer,"GYRO"))!=NULL && len>=20){
								pos = 4;
								gx.t8[1] = Sub[pos++];
								gx.t8[0] = Sub[pos++];
								gy.t8[1] = Sub[pos++];
								gy.t8[0] = Sub[pos++];
								gz.t8[1] = Sub[pos++];
								gz.t8[0] = Sub[pos++];
								
								geir.t8[1] = Sub[pos++];
								geir.t8[0] = Sub[pos++];
								roll.t8[1] = Sub[pos++];
								roll.t8[0] = Sub[pos++];
								nick.t8[1] = Sub[pos++];
								nick.t8[0] = Sub[pos++];
								
								avr_time.t8[3] = Sub[pos++];
								avr_time.t8[2] = Sub[pos++];
								avr_time.t8[1] = Sub[pos++];
								avr_time.t8[0] = Sub[pos++];
								
								double t = (double)avr_time.t32;
								t *= (double)1024/8000000;		
								t *= (double)61/78;											
								file = fopen(FileName,"a");
								fprintf(file,"%f\t%d\t%d\t%d\t%d\t%d\t%d\n",t,gx.t16,gy.t16,gz.t16,geir.t16,roll.t16,nick.t16);
								fclose(file);
							}
							else if ((Sub=(unsigned char*)strstr((char*)Buffer,"CELL"))!=NULL && len>=7){
								pos = 4;								
								double V1 = (double)Sub[pos++];
								V1 *= 0.0627;
								double V2 = (double)Sub[pos++];
								V2 *= 0.0629;
								double V3 = (double)Sub[pos++];
								V3 *= 0.0412;
								printf("\nbattery power: total %.2fV\n",V1);
								printf("cell 1: %.2fV\n",V1-V2);
								printf("cell 2: %.2fV\n",V2-V3);
								printf("cell 3: %.2fV\n\n",V3);													
							}								
							else if (verbose){
								printf("from:\t");
								for (i=0;i<8;i++){
									Send_address[i] = API_frame[i];
									printf("%.2X",Send_address[i]);
								}
								printf("\n");												
								rssi = API_frame[8];
								printf("signal strength: %d\n",rssi);
								broadcast = API_frame[9];
								printf("broadcast mode: %d\n",broadcast);
								printf("data package:\n");// %d\n\n",Buffer);
								for (i=0;i<len;i++) printf(":%.2X",Buffer[i]);								
								printf("\n\n");							
							}
							//verbose = 0;
							break;
						default:
							printf("unhandled message:\n");
							printf("%s\n",API_frame);
					}
				}
			}
			if (input_flag==YES) handle_input(dev_handle,Param_values);		
			diff = clock()-start;
			double sec = (double)diff/100000.;
			//usleep(1000000);
			//printf("diff: %d\n",(long)diff);
			if (sec>10.0){
				counter = 0;
				while(API_remote_power_cell_test(dev_handle,NULL,NO)==FAILED && counter<max_retry){counter++;}					
				if (counter==max_retry) printf("power cell test: FAILED\n");
				start = clock();	
			}
		}
	}
	
	else if (strcmp(Param,"transmit")==0){			
		if(transmit_string(dev_handle,Param_values,Param_values[2])==FAILED){
			printf("failed to transmit data %s\n",Param_values[2]);
		}
	}
	
FINALIZE:	
	close(dev_handle);		
	return 0;
}
