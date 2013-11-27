#include <stdio.h>
#include <iteadcpp.h>

GSM gsm;

char position;
char phone_num[20]; // array for the phone number string
char sms_text[200]; // array for the SMS text string

int main(void)
{
	printf("system startup\n"); 
	gsm.TurnOn(9600);              	//module power on
	gsm.InitParam(PARAM_SET_1);	//configure the module  
	gsm.Echo(1); 

	gsm.Call("159xxxxxxxx");
	printf("begin send message\n");
	gsm.SendSMS("159xxxxxxxx","hello pengge\n");
}
