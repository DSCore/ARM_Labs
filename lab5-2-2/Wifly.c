#include <stdint.h>

#include <USART3.h>
#include <USART2.h>
#include <Wifly.h>

static Ping_t message;
static Ping_t response;
static int rcvd;

void Wifly_Send_Ping(){
	int i=0;
	void *ptr=0;
	uint8_t *byte;


	message.type=PING;
	message.id=ID;
	response.type=0;
	response.id=0;
	rcvd=0;

	ptr=&message;

	for (i=0;i<8;i++) {
		byte=(uint8_t *)ptr;
		USART3_putchar(*byte);
		ptr=ptr+sizeof(uint8_t);
	}
}

void Wifly_Receive_Ping(uint8_t byte){
	int scale;
	int i;

	if (rcvd%4==3){scale=2^(8*3);}
	else if (rcvd%4==2){scale=2^(8*2);}
	else if (rcvd%4==1){scale=2^(8*1);}
	else if (rcvd%4==0){scale=2^(8*0);}

	if (rcvd<4){
		response.type=response.type+byte*scale;
	}
	else if (rcvd<8) {
		response.id=response.id+byte*scale;
	}
	rcvd++;
}

void Wifly_Print_Last_Received_Message(void){

	if(message.type == 1){
		USART2_putstr("Type: PING\n\rID: \0");
		printHex(message.id);
	}

	else{
		USART2_putstr("Type: \0");
		printHex(message.type);
		USART2_putstr("\n\rID: \0");
		printHex(message.id);
		USART2_putstr(" \n\r\0");
	}
}
