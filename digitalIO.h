#include <iocpp.h>
extern "C"	int		pinMode(int pin, int mode);
extern "C"	int		digitalWrite(int pin,int value);
extern "C"	int		digitalRead(int pin);
extern "C"	void		analogWrite(int pin, int value);
extern "C"	void		delay(unsigned int howLong);
extern "C"	void		delayMicroseconds(unsigned int howLong);
extern "C"	unsigned int	millis(void);
extern "C"	unsigned int	micros(void);

extern "C" int Set8BitsBUS(STRUCT_8BITS_BUS *bus,char d0,char d1,char d2,char d3,char d4,char d5,char d6,char d7,char mode);
extern "C" int digitalWrite8(STRUCT_8BITS_BUS *bus,char data);
extern "C" unsigned char digitalRead8(STRUCT_8BITS_BUS *bus);
extern "C" int Set16BitsBUS(STRUCT_16BITS_BUS *bus,char d0,char d1,char d2,char d3, char d4,char d5,char d6,char d7,char d8,char d9,char d10,char d11,char d12,char d13,char d14,char d15,char mode);
extern "C" int digitalWrite16(STRUCT_16BITS_BUS *bus,short int data);
extern "C" unsigned short int digitalRead16(STRUCT_16BITS_BUS *bus);

