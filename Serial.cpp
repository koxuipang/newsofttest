#include <Serial.h>

void SoftwareSerial::begin(char device,unsigned long baud) 
{
	SerialBegin(device,baud);
}
void SoftwareSerial::end(char device)
{
	SerialEnd(device);
}
int SoftwareSerial::available(char device)
{
	return SerialAvailable(device);
}
int SoftwareSerial::peek(char device)
{

}
int SoftwareSerial::read(char device)
{
	return SerialRead(device);
}
void SoftwareSerial::flush(char device)    
{
	SerialFlush(device);
}
size_t SoftwareSerial::write(char device,uint8_t c)   
{
	return SerialWrite(device,c);
}
size_t SoftwareSerial::write(char device,const char *s)   
{
	SerialPrint(device,s);
}
void SoftwareSerial::print(char device,const char *s) 
{
	SerialPrint(device,s);
}
void SoftwareSerial::println(char device,const char *s) 
{
	SerialPrintln(device,s);
}
