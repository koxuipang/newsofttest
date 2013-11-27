#ifndef _TwoWire_h
#define _TwoWire_h

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/types.h>

#include <iocpp.h>

/*
extern "C" int			pinMode(int pin, int mode);
extern "C" int			digitalWrite(int pin,int value);
extern "C" int			digitalRead(int pin);
extern "C" void			analogWrite(int pin, int value);
extern "C" void         delay(unsigned int howLong) ;
extern "C" void         delayMicroseconds(unsigned int howLong) ;
extern "C" unsigned int millis(void) ;
extern "C" unsigned int micros(void) ;
*/

extern "C" 	__u16 WireBegin(__u16 device);
extern "C"	__u16 WireBeginTransmission(__u16 device,__u8 addr);
extern "C"	__u16 WireWrite(__u16 device, __u8 data);
extern "C"	__u16 WireEndTransmission(__u16 device);
extern "C"	__u16 WireRequestFrom(__u16 device, __u8 addr, __u16 quantity);
extern "C"	__u16 WireAvailable(__u16 device);
extern "C"	__u8  WireRead(__u16 device);
extern "C"	__u8  WireEnd(__u16 device);

class TwoWireFirst{
	private:

	public:
		void begin(void);
		void begin(uint8_t address);
		void begin(int address);
		void beginTransmission(uint8_t address);
		void beginTransmission(int address);
		uint8_t endTransmission(void);
		uint8_t endTransmission(uint8_t sendStop);
		uint8_t requestFrom(uint8_t address, uint8_t quantity);
		uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);
		uint8_t requestFrom(int address, int quantity);
		uint8_t requestFrom(int address, int quantity, int sendStop);
		virtual size_t write(uint8_t data);
		virtual size_t write(const uint8_t *data, size_t quantity);
		virtual int available(void);
		virtual int read(void);

		
	    inline size_t write(unsigned long n) { return write((uint8_t)n); }
	    inline size_t write(long n) { return write((uint8_t)n); }
	    inline size_t write(unsigned int n) { return write((uint8_t)n); }
	    inline size_t write(int n) { return write((uint8_t)n); }
};

class TwoWireSecond{
	private:

	public:
		void begin(void);
		void begin(uint8_t address);
		void begin(int address);
		void beginTransmission(uint8_t address);
		void beginTransmission(int address);
		uint8_t endTransmission(void);
		uint8_t endTransmission(uint8_t sendStop);
		uint8_t requestFrom(uint8_t address, uint8_t quantity);
		uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);
		uint8_t requestFrom(int address, int quantity);
		uint8_t requestFrom(int address, int quantity, int sendStop);
		virtual size_t write(uint8_t data);
		virtual size_t write(const uint8_t *data, size_t quantity);
		virtual int available(void);
		virtual int read(void);

	    inline size_t write(unsigned long n) { return write((uint8_t)n); }
	    inline size_t write(long n) { return write((uint8_t)n); }
	    inline size_t write(unsigned int n) { return write((uint8_t)n); }
	    inline size_t write(int n) { return write((uint8_t)n); }
		
};

#endif








