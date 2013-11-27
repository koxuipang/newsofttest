#include<stdio.h>
#include<unistd.h>
#include <itead.h>

#define pin 0

void main(void)
{
	int i;
	
/*	for(i=0;i<255;i++)
	{
		printf("the value is %d\n",i);
		analogWrite(pin,i);
		delay(50);
	}*/
	char temp = 0;

	SPIBegin(SPI0);
	SetDataMode(SPI0,SPI_MODE0);
	SetBitOrder(SPI0,LSBFIRST);
	SetClockDivider(SPI0,1000000);
	temp = SPITransfer(0xaa);
	printf("Read 0x%x \n",temp);

}

