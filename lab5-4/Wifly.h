#include <stdint.h>

#include <USART3.h>
#include <USART2.h>


//Possible types of the ping
#define PING 1

//Possible ids of the ping
#define ID 7


void Wifly_Send_Ping();

void Wifly_Receive_Ping(uint8_t byte);

void Wifly_Print_Last_Received_Message(void);

