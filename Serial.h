#ifndef _IteadSerial
#define _IteadSerial

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <iocpp.h>

typedef unsigned int size_t;

extern "C" int	SerialBegin(int part_data, int baud);
extern "C" int	SerialEnd(int fd);
extern "C" int	SerialFlush(int fd);
extern "C" int	SerialWrite(int fd, unsigned char c);
extern "C" int	SerialPrint(int fd, const char *s);
extern "C" int	SerialPrintln(int fd, const char *s);
extern "C" int	SerialAvailable(int fd);
extern "C" int	SerialRead(int fd);

class SoftwareSerial{
	private:
		
	public:
		void begin(char device,unsigned long baud);
		void end(char device);
		virtual int available(char device);
		virtual int peek(char device);
		virtual int read(char device);
		virtual void flush(char device);   
		virtual size_t write(char device,uint8_t c);
		size_t write(char device,const char *s); 
		
		inline size_t write(char device,unsigned long n){ return write(device,(uint8_t)n); }	 
		inline size_t write(char device,long n){ return write(device,(uint8_t)n); }	 
		inline size_t write(char device,unsigned int n){ return write(device,(uint8_t)n); }	 
		inline size_t write(char device,int n){ return write(device,(uint8_t)n); }
		 
		void print(char device,const char *s); 
		void println(char device,const char *s); 
};
#endif

