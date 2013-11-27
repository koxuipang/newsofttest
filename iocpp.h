#ifndef _IOCPP_
#define _IOCPP_

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/types.h>

#include <regmap.h>
#include <pinmap.h>

typedef struct{
	unsigned char D0;
	unsigned char D1;
	unsigned char D2;
	unsigned char D3;
	unsigned char D4;
	unsigned char D5;
	unsigned char D6;
	unsigned char D7;
}STRUCT_8BITS_BUS;

typedef struct{
	unsigned char D0;
	unsigned char D1;
	unsigned char D2;
	unsigned char D3;
	unsigned char D4;
	unsigned char D5;
	unsigned char D6;
	unsigned char D7;
	unsigned char D8;
	unsigned char D9;
	unsigned char D10;
	unsigned char D11;
	unsigned char D12;
	unsigned char D13;
	unsigned char D14;
	unsigned char D15;
}STRUCT_16BITS_BUS;


#define INPUT 			0
#define OUTPUT 			1
#define	PULLUP		 	2
#define	PULLDOWN		3
#define	CHECK		 	4

#define UART0 			10
#define UART2 			11
#define UART3 			12
#define UART4			13
#define UART7 			14

#define SPI0			0
#define SPI1			1

#define _A10_GPIO_BASE   	(0x01C20800)
#define _A10_PWM_BASE    	(0x01C20e00)
#define _GPIO_BASE			(0x01C20000)
#define _GPIO_PWM			(0x01c20000)

#define _LOW				0 	
#define _HIGH				1
#define LOW					0 	
#define HIGH				1
#define fd_nu 				1
#define speed_nu 			2
#define SPI0 				0
#define SPI1				1

#define	_MAX_PINS			144

#define I2C_BLOCK_MAX     	32
#define I2C_READ          	1
#define I2C_WRITE         	0
#define I2C_BYTE_DATA     	2
#define I2C_WORD_DATA	  	3
#define I2C1              	1
#define I2C2              	2

#define _MMAP_SIZE			4096
#define _MMAP_MASK			(_MMAP_SIZE - 1)
#define __BLOCK_SIZE 			512

#define SPI_MODE0 			0x00
#define SPI_MODE1 			0x01
#define SPI_MODE2 			0x02
#define SPI_MODE3 			0x03

#define SPI_CLOCK_DIV1   	0x00
#define SPI_CLOCK_DIV2   	0x01
#define SPI_CLOCK_DIV4   	0x02
#define SPI_CLOCK_DIV8   	0x03
#define SPI_CLOCK_DIV16  	0x04
#define SPI_CLOCK_DIV32  	0x05
#define SPI_CLOCK_DIV64  	0x06
#define SPI_CLOCK_DIV128 	0x07

#define LSBFIRST 0
#define MSBFIRST 1
/*******************************i2c**************************************/
#define I2C_FUNC_I2C					0x00000001
#define I2C_FUNC_10BIT_ADDR				0x00000002
#define I2C_FUNC_PROTOCOL_MANGLING		0x00000004 
#define I2C_FUNC_SMBUS_PEC				0x00000008
#define I2C_FUNC_SMBUS_BLOCK_PROC_CALL	0x00008000 
#define I2C_FUNC_SMBUS_QUICK			0x00010000 
#define I2C_FUNC_SMBUS_READ_BYTE		0x00020000 
#define I2C_FUNC_SMBUS_WRITE_BYTE		0x00040000 
#define I2C_FUNC_SMBUS_READ_BYTE_DATA	0x00080000 
#define I2C_FUNC_SMBUS_WRITE_BYTE_DATA	0x00100000 
#define I2C_FUNC_SMBUS_READ_WORD_DATA	0x00200000 
#define I2C_FUNC_SMBUS_WRITE_WORD_DATA	0x00400000 
#define I2C_FUNC_SMBUS_PROC_CALL		0x00800000 
#define I2C_FUNC_SMBUS_READ_BLOCK_DATA	0x01000000 
#define I2C_FUNC_SMBUS_WRITE_BLOCK_DATA 0x02000000 
#define I2C_FUNC_SMBUS_READ_I2C_BLOCK	0x04000000
#define I2C_FUNC_SMBUS_WRITE_I2C_BLOCK	0x08000000

#define I2C_FUNC_SMBUS_BYTE (I2C_FUNC_SMBUS_READ_BYTE | \
                             I2C_FUNC_SMBUS_WRITE_BYTE)
#define I2C_FUNC_SMBUS_BYTE_DATA (I2C_FUNC_SMBUS_READ_BYTE_DATA | \
                                  I2C_FUNC_SMBUS_WRITE_BYTE_DATA)
#define I2C_FUNC_SMBUS_WORD_DATA (I2C_FUNC_SMBUS_READ_WORD_DATA | \
                                  I2C_FUNC_SMBUS_WRITE_WORD_DATA)
#define I2C_FUNC_SMBUS_BLOCK_DATA (I2C_FUNC_SMBUS_READ_BLOCK_DATA | \
                                   I2C_FUNC_SMBUS_WRITE_BLOCK_DATA)
#define I2C_FUNC_SMBUS_I2C_BLOCK (I2C_FUNC_SMBUS_READ_I2C_BLOCK | \
                                  I2C_FUNC_SMBUS_WRITE_I2C_BLOCK)

#define I2C_FUNC_SMBUS_HWPEC_CALC	I2C_FUNC_SMBUS_PEC

#define I2C_SMBUS_BLOCK_MAX		32		
#define I2C_SMBUS_I2C_BLOCK_MAX	32	

#define I2C_SMBUS_READ			1
#define I2C_SMBUS_WRITE			0
#define I2C_SMBUS_QUICK		    0
#define I2C_SMBUS_BYTE		    1
#define I2C_SMBUS_BYTE_DATA	    2 
#define I2C_SMBUS_WORD_DATA	    3
#define I2C_SMBUS_PROC_CALL	    4
#define I2C_SMBUS_BLOCK_DATA	    5
#define I2C_SMBUS_I2C_BLOCK_BROKEN  6
#define I2C_SMBUS_BLOCK_PROC_CALL   7		
#define I2C_SMBUS_I2C_BLOCK_DATA    8

#define I2C_RETRIES	0x0701	
#define I2C_TIMEOUT	0x0702	
#define I2C_SLAVE	0x0703	
#define I2C_SLAVE_FORCE	0x0706	
#define I2C_TENBIT	0x0704		

#define I2C_FUNCS	0x0705	
#define I2C_RDWR	0x0707	
#define I2C_PEC		0x0708	

#define I2C_SMBUS	0x0720

#endif

