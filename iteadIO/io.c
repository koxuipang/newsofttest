/*******************************************************************************
*>author	:koxuipang
*>date	:2013/8/27
*>description	: base function
********************************************************************************/

#define ITEADPLUS

#include <iocpp.h>
#include <pinmap.h>

static int g_i2c_fd_offset_array[2][3]={0};

static int marks[_MAX_PINS];
static int range[_MAX_PINS];

static volatile uint32_t *_REG_TIMER;
static volatile uint32_t *_REG_GPIO;
static volatile uint32_t *_REG_CLK;
static volatile uint32_t *_REG_PWM;
static volatile uint32_t *_gpio;
static volatile uint32_t *_pwm;

static uint64_t epochMilli, epochMicro ;
static int 	g_pin_mode[144] = {0};
static int 	g_soft_pwm_pin=0;
int 		g_fd_array[6]={};
static int 	g_SPI_data_array[3][2]={};
static int 	g_pthread_nu=-1;
static int 	g_pthread_nu_s=0;

union i2c_data
{
        uint8_t  byte;
        uint16_t word;
        uint8_t  block[I2C_BLOCK_MAX + 2];
};
struct i2c_ioctl_data
{
        char read_write;
        uint8_t command;
        int size;
        union i2c_data *data;
};
void 	*pwm_thread(void *argk);
int 	set_high_pri (int pri);
/*******************************************************************************
*>author	:koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	寄存器级别操作函数
********************************************************************************/

uint32_t *get_memory_addr(uint32_t phy_addr)
{
	uint32_t *_phy_to_memory;
	uint32_t fd;
	
	fd = open ("/dev/mem", O_RDWR | O_SYNC);
	if(fd == -1)
	{
		printf("get_memory is failed\n");
	}
	_phy_to_memory = (uint32_t *)mmap(0, __BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, phy_addr);
	return _phy_to_memory;
}
uint32_t Readreg32(uint32_t _addr)
{
	static volatile uint32_t *_addr_target;
	int _fd = 0;
	uint32_t _value = 0;


	_fd = open("/dev/mem", O_RDWR|O_NDELAY);
	if(-1 == _fd)
	{
		printf("Open /dev/mem failed!\n");
		return -1;
	}
	
	uint32_t _mmap_base = (_addr & ~_MMAP_MASK);
	uint32_t _mmap_seek = ((_addr - _mmap_base)>>2);

	_addr_target = (uint32_t *)mmap(NULL, _MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, _mmap_base);

	if(0 == _addr_target)
	{
		close(_fd);
		printf("mmap failed:fd(%d),addr(0x%x),size(%d)\n", _fd, _addr, _MMAP_SIZE);
		return -1;
	}
	else
	{
		_value =*(_addr_target + _mmap_seek);
        #ifdef CORE_DEBUG
            printf("Readreg32: Addr = 0x%x, Value = 0x%x", (_addr_target + _mmap_seek), _value);
        #endif
		close(_fd);
		munmap((uint32_t *)_addr_target, _MMAP_SIZE);
		return (_value);
	}
}

uint32_t Writereg32(uint32_t _addr, uint32_t _value)

{
	static volatile uint32_t *_addr_target;
	int _fd = 0;


	_fd = open("/dev/mem", O_RDWR|O_NDELAY);
	
	if(-1 == _fd)
	{
		printf("Open /dev/mem failed!\n");
		return -1;
	}
	
	uint32_t _mmap_base = (_addr & ~_MMAP_MASK);
	uint32_t _mmap_seek = ((_addr - _mmap_base)>>2);

	_addr_target = (uint32_t *)mmap(NULL, _MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, _mmap_base);

	if(0 == _addr_target)
	{
		close(_fd);
		printf("mmap failed:fd(%d),addr(0x%x),size(%d)\n", _fd, _addr, _MMAP_SIZE);
		return -1;
	}
	else
	{
		*(_addr_target + _mmap_seek) = _value;
        #ifdef CORE_DEBUG
            printf("Writereg32: Addr = 0x%x, Value = 0x%x", (_addr_target + _mmap_seek), *(_addr_target + _mmap_seek));
        #endif
		close(_fd);
		munmap((uint32_t *)_addr_target, _MMAP_SIZE);
		return (_value);
	}
}

/*******************************************************************************
*>author :koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	设置管脚模式 模式: INPUT、OUTPUT、PWM_OUTPUT、PULLUP、PULLDOWN、PULLOFF、CHECK
********************************************************************************/
int pinMode(int _pin, int _mode)
{
	int fd,_pin_save_phy;

	_pin_save_phy = _pin;

	if(g_soft_pwm_pin == _pin)
		g_soft_pwm_pin = -1;
	
	_pin = _GPIO_MAP[_pin];

	if (-1 == _pin)
	{
		printf("ERR: This _pin is not availbale!\n");
		return -1;
	}

	_gpio 	= get_memory_addr(_GPIO_BASE);
	_gpio 	+= 0x200;
	
	_pwm 	= get_memory_addr(_GPIO_PWM);
	_pwm 	+= 0x380;

	switch(_mode)
	{
		case INPUT:
		{
			int _Bank 	= _pin >> 5;
			int _Index 	= _pin - (_Bank << 5);
			int _Offset = ((_Index - ((_Index >> 3) << 3)) << 2);
			uint32_t _Phyaddr 	= _A10_GPIO_BASE + (_Bank * 36) + ((_Index >> 3) << 2);
			uint32_t _RegValue 	= 0;

			_RegValue = Readreg32(_Phyaddr);
	
			_RegValue &= ~(7<<_Offset);
			Writereg32(_Phyaddr, _RegValue);
			_RegValue = Readreg32(_Phyaddr);

			//g_pin_mode[_pin_save_phy] = INPUT;
			break;
		}
		case OUTPUT:
		{
			int _Bank 	= _pin >> 5;
			int _Index 	= _pin - (_Bank << 5);
			int _Offset = ((_Index - ((_Index >> 3) << 3)) << 2);
			uint32_t _Phyaddr 	= _A10_GPIO_BASE + (_Bank * 36) + ((_Index >> 3) << 2);
			uint32_t _RegValue 	= 0;

			_RegValue = Readreg32(_Phyaddr);
			
			_RegValue &= ~(7 << _Offset);
			_RegValue |= (1 << _Offset);
			Writereg32(_Phyaddr,_RegValue);
			_RegValue = Readreg32(_Phyaddr);

			//g_pin_mode[_pin_save_phy] = OUTPUT;
			break;
		} 
		case PULLUP:
		{

			int pud 	= 1;
			int bank 	= _pin >> 5;
			int index 	= _pin - (bank << 5);
			int sub 	= index >> 4;
			int sub_index = index - 16*sub;
			uint32_t phyaddr = _A10_GPIO_BASE + (bank * 36) + 0x1c + sub; 
			uint32_t _RegValue 	= 0;
			
			pud &= 3 ;
			
			_RegValue = Readreg32(phyaddr);
			_RegValue &= ~(3 << (sub_index << 1));
			_RegValue |= (pud << (sub_index << 1));
			
			Writereg32(phyaddr,_RegValue);
			_RegValue = Readreg32(phyaddr);

			//g_pin_mode[_pin_save_phy] = PULLUP;
			break;
			
		}
		case PULLDOWN:
		{
			int pud 	= 2;
			int bank 	= _pin >> 5;
			int index 	= _pin - (bank << 5);
			int sub 	= index >> 4;
			int sub_index = index - 16*sub;
			uint32_t phyaddr = _A10_GPIO_BASE + (bank * 36) + 0x1c + sub; 
			uint32_t _RegValue 	= 0;
			
			pud &= 3 ;
			
			_RegValue = Readreg32(phyaddr);
			_RegValue &= ~(3 << (sub_index << 1));
			_RegValue |= (pud << (sub_index << 1));
			
			Writereg32(phyaddr,_RegValue);
			_RegValue = Readreg32(phyaddr);

			//g_pin_mode[_pin_save_phy] = PULLDOWN;
			break;
		}
		case CHECK:
		{
			int _Bank 	= _pin >> 5;
			int _Index 	= _pin - (_Bank << 5);
			int _Offset = ((_Index - ((_Index >> 3) << 3)) << 2);
			uint32_t _Phyaddr 	= _A10_GPIO_BASE + (_Bank * 36) + ((_Index >> 3) << 2);
			uint32_t _RegValue 	= 0;

			_RegValue = Readreg32(_Phyaddr);
			_RegValue &= (7<<_Offset);
			_RegValue >>= _Offset;
			if(_RegValue == 1)
				return OUTPUT;
			else if(_RegValue == 0)
			{
				int pud 	= 1;
				int bank 	= _pin >> 5;
				int index 	= _pin - (bank << 5);
				int sub 	= index >> 4;
				int sub_index = index - 16*sub;
				uint32_t phyaddr = _A10_GPIO_BASE + (bank * 36) + 0x1c + sub; 
				uint32_t _RegValue1 	= 0;
			
				pud &= 3 ;
			
				_RegValue1 = Readreg32(phyaddr);
				_RegValue1 &= (3 << (sub_index << 1));
				_RegValue1 =	_RegValue1 >> (sub_index << 1);
				if(_RegValue1 == 0x00)
					return INPUT;
				else if(_RegValue1 == 0x01)
					return PULLUP;
				else if(_RegValue1 == 0x02)
					return PULLDOWN;
				else
					return -1;				
				
			} 

			return -1;
		} 
		default:
			break;
	}
}
/*******************************************************************************
*>author : koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 微妙实现
********************************************************************************/
void delayMicrosecondsHard (unsigned int howLong)
{
	struct timeval tNow, tLong, tEnd ;

	gettimeofday (&tNow, NULL) ;
	tLong.tv_sec  = howLong / 1000000 ;
	tLong.tv_usec = howLong % 1000000 ;
	timeradd (&tNow, &tLong, &tEnd) ;

	while (timercmp (&tNow, &tEnd, <))
	gettimeofday (&tNow, NULL) ;
}
/*******************************************************************************
*>author : koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 延时函数
********************************************************************************/
void delayMicroseconds (unsigned int howLong)
{
	struct timespec sleeper ;

	if (howLong ==   0)
		return ;
	else if (howLong  < 100)
		delayMicrosecondsHard (howLong) ;
	else
	{
		sleeper.tv_sec  = 0;
		sleeper.tv_nsec = (long)(howLong * 1000);
		nanosleep (&sleeper, NULL);
	}
}
/*******************************************************************************
*>author : koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 pwm  线程执行函数
********************************************************************************/

void *pwm_thread(void *argk)
{
	int _mark, _space ;

	set_high_pri(50);

	for (;;)
	{
		_mark  = marks[g_soft_pwm_pin];
		_space = range[g_soft_pwm_pin]-_mark;

		if (_mark != 0)
		digitalWrite(g_soft_pwm_pin, _HIGH);
		delayMicroseconds (_mark * 20);

		if (_space != 0)
		digitalWrite(g_soft_pwm_pin, _LOW);
		delayMicroseconds(_space * 20);
		if(g_soft_pwm_pin == -1)
			break;
		if(g_pthread_nu_s==1){
			g_pthread_nu_s=0;
			break;
		}
	}
}
/*******************************************************************************
*>author : koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 调度配置函数
********************************************************************************/
int set_high_pri (int pri)
{
	struct sched_param sched ;

	memset (&sched, 0, sizeof(sched)) ;

	if (pri > sched_get_priority_max (SCHED_RR))
		pri = sched_get_priority_max (SCHED_RR) ;

	sched.sched_priority = pri ;
	return sched_setscheduler (0, SCHED_RR, &sched) ;
}
/*******************************************************************************
*>author : koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 Attempt to set a high priority schedulling for the running program
********************************************************************************/
void analogWrite(int pin, int value)
{
	pin &= 255;
	
	if(g_pthread_nu == -1)
	{
		pinMode(pin,OUTPUT);
		digitalWrite(pin, _LOW);
		g_soft_pwm_pin = pin;
		marks[pin] = 0;
		range[pin] = 255;
		g_pthread_nu=pin;
		pthread_t soft_pwm_struct;
		pthread_create(&soft_pwm_struct, NULL, pwm_thread, NULL);
	}	
	else if(g_pthread_nu == pin){
		g_pthread_nu == pin;
	}else{
		g_pthread_nu == -1;
		g_pthread_nu_s = 1;

		pinMode(pin,OUTPUT);
		digitalWrite(pin, _LOW);
		g_soft_pwm_pin = pin;
		marks[pin] = 0;
		range[pin] = 255;
		g_pthread_nu=pin;
		pthread_t soft_pwm_struct;
		pthread_create(&soft_pwm_struct, NULL, pwm_thread, NULL);
	}
	
	if (value < 0)
		value = 0;
	else if (value > range [pin])
		value = range [pin];

	marks [pin] = value;
}

/*******************************************************************************
*>author : koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 写数据到指定端口
********************************************************************************/
int digitalWrite(int _pin,int _value)
{

	_pin = _GPIO_MAP[_pin];

	uint32_t regval = 0;
	int bank = _pin >> 5;
	int index = _pin - (bank << 5);
	uint32_t _offset = (((bank * 0x24) + 0x10) >> 2); 		

	if(-1 == _pin)
	{
		printf("invalid _pin,please check it over.\n");
		return -1;
	}

	if((*(_gpio + _offset) & (1 << (index & 31))) != _value)
	{
		if(_value == _LOW)
		{
			*(_gpio + _offset) = *(_gpio + _offset) & ~(1 << (index & 31));  
		} else {
			*(_gpio + _offset) = *(_gpio + _offset) | (1 << (index & 31));
		}

		return 0;
	}
	
}

/*******************************************************************************
*>author : koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 从指定端口读取数据
********************************************************************************/
int digitalRead(int _pin)
{
	_pin = _GPIO_MAP[_pin];

	uint32_t regval = 0;
	int bank 	= _pin >> 5;
	int index 	= _pin - (bank << 5);
	uint32_t _offset = (((bank * 0x24) + 0x10) >> 2); 

	if ((*(_gpio + _offset) & (1 << (index & 31))) != 0)
		return _HIGH;
	else
		return _LOW;	
}

/*******************************************************************************
*>author : koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 延时函数
********************************************************************************/
void delay (unsigned int howLong)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(howLong / 1000) ;
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}

/*******************************************************************************
*>author : koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 读取系统运行时间 
********************************************************************************/
unsigned int millis(void)
{
	struct timeval tv;
	uint64_t now;

	gettimeofday(&tv, NULL) ;
	now  = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);

	return (uint32_t)(now - epochMilli);
}

/*******************************************************************************
*>author : koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 读取系统运行时间
********************************************************************************/
unsigned int micros(void)
{
	struct timeval tv;
	uint64_t now;

	gettimeofday (&tv, NULL);
	now  = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec;

	return (uint32_t)(now - epochMicro);
}

/*******************************************************************************
*>author : koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 设置8位总线
********************************************************************************/
int Set8BitsBUS(STRUCT_8BITS_BUS *bus,char d0,char d1,char d2,char d3,char d4,char d5,char d6,char d7,char mode)
{
	int statue;

	bus->D0 = d0;
	bus->D1 = d1;
	bus->D2 = d2;
	bus->D3 = d3;
	bus->D4 = d4;
	bus->D5 = d5;
	bus->D6 = d6;
	bus->D7 = d7;
	
	pinMode(d0,mode);
	pinMode(d1,mode);
	pinMode(d2,mode);
	pinMode(d3,mode);
	pinMode(d4,mode);
	pinMode(d5,mode);
	pinMode(d6,mode);
	statue = pinMode(d7,mode);

	if(statue < 0)
	{
		return -1;
	} else {
		return 0;
	}

}

/*******************************************************************************
*>author 	: koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 8位数据总线写数据
********************************************************************************/
int digitalWrite8(STRUCT_8BITS_BUS *bus,char data)
{
	int statue = 0;

	(data & 0x01)?digitalWrite(bus->D0,1):digitalWrite(bus->D0,0);
	(data & 0x02)?digitalWrite(bus->D1,1):digitalWrite(bus->D1,0);
	(data & 0x04)?digitalWrite(bus->D2,1):digitalWrite(bus->D2,0);
	(data & 0x08)?digitalWrite(bus->D3,1):digitalWrite(bus->D3,0);
	(data & 0x10)?digitalWrite(bus->D4,1):digitalWrite(bus->D4,0);
	(data & 0x20)?digitalWrite(bus->D5,1):digitalWrite(bus->D5,0);
	(data & 0x40)?digitalWrite(bus->D6,1):digitalWrite(bus->D6,0);
	(data & 0x80)?(statue=digitalWrite(bus->D7,1)):(statue=digitalWrite(bus->D7,0));

	return statue;
	
}

/*******************************************************************************
*>author 	: koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 8位数据总线读取数据
********************************************************************************/
unsigned char digitalRead8(STRUCT_8BITS_BUS *bus)
{
	unsigned char data=0x00; 

	digitalRead(bus->D0)?(data |= 0x01):(data |= 0x00);
	digitalRead(bus->D1)?(data |= 0x02):(data |= 0x00);
	digitalRead(bus->D2)?(data |= 0x04):(data |= 0x00);
	digitalRead(bus->D3)?(data |= 0x08):(data |= 0x00);
	digitalRead(bus->D4)?(data |= 0x10):(data |= 0x00);
	digitalRead(bus->D5)?(data |= 0x20):(data |= 0x00);
	digitalRead(bus->D6)?(data |= 0x40):(data |= 0x00);
	digitalRead(bus->D7)?(data |= 0x80):(data |= 0x00);

	return data;
}

/*******************************************************************************
*>author 	: koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 设置16位总线
********************************************************************************/
int Set16BitsBUS(STRUCT_16BITS_BUS *bus,char d0,char d1,char d2,char d3,char d4,char d5,char d6,char d7,char d8,char d9,char d10,char d11,char d12,char d13,char d14,char d15,char mode)
{
	int statue;

	bus->D0 = d0;
	bus->D1 = d1;
	bus->D2 = d2;
	bus->D3 = d3;
	bus->D4 = d4;
	bus->D5 = d5;
	bus->D6 = d6;
	bus->D7 = d7;
	bus->D8 = d8;
	bus->D9 = d9;
	bus->D10 = d10;
	bus->D11 = d11;
	bus->D12 = d12;
	bus->D13 = d13;
	bus->D14 = d14;
	bus->D15 = d15;

	pinMode(d0,mode);
	pinMode(d1,mode);
	pinMode(d2,mode);
	pinMode(d3,mode);
	pinMode(d4,mode);
	pinMode(d5,mode);
	pinMode(d6,mode);
	pinMode(d7,mode);
	pinMode(d8,mode);
	pinMode(d9,mode);
	pinMode(d10,mode);
	pinMode(d11,mode);
	pinMode(d12,mode);
	pinMode(d13,mode);
	pinMode(d14,mode);
	statue = pinMode(d15,mode);
	
	if(statue < 0)
	{
		return -1;
	} else {
		return 0;
	}
}

/*******************************************************************************
*>author 	: koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 16位总线写数据
********************************************************************************/
int digitalWrite16(STRUCT_16BITS_BUS *bus,short int data)
{
	int statue = 0;

	(data & 0x0001)?digitalWrite(bus->D0,1):digitalWrite(bus->D0,0);
	(data & 0x0002)?digitalWrite(bus->D1,1):digitalWrite(bus->D1,0);
	(data & 0x0004)?digitalWrite(bus->D2,1):digitalWrite(bus->D2,0);
	(data & 0x0008)?digitalWrite(bus->D3,1):digitalWrite(bus->D3,0);
	(data & 0x0010)?digitalWrite(bus->D4,1):digitalWrite(bus->D4,0);
	(data & 0x0020)?digitalWrite(bus->D5,1):digitalWrite(bus->D5,0);
	(data & 0x0040)?digitalWrite(bus->D6,1):digitalWrite(bus->D6,0);
	(data & 0x0080)?digitalWrite(bus->D7,1):digitalWrite(bus->D7,0);
	(data & 0x0100)?digitalWrite(bus->D8,1):digitalWrite(bus->D8,0);
	(data & 0x0200)?digitalWrite(bus->D9,1):digitalWrite(bus->D9,0);
	(data & 0x0400)?digitalWrite(bus->D10,1):digitalWrite(bus->D10,0);
	(data & 0x0800)?digitalWrite(bus->D11,1):digitalWrite(bus->D11,0);
	(data & 0x1000)?digitalWrite(bus->D12,1):digitalWrite(bus->D12,0);
	(data & 0x2000)?digitalWrite(bus->D13,1):digitalWrite(bus->D13,0);
	(data & 0x4000)?digitalWrite(bus->D14,1):digitalWrite(bus->D14,0);
	(data & 0x8000)?(statue=digitalWrite(bus->D15,1)):(statue=digitalWrite(bus->D15,0));

	return statue;
}

/*******************************************************************************
*>author 	:  	koxuipang
*>date	:		2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 读取16位总线数据
********************************************************************************/
unsigned short int digitalRead16(STRUCT_16BITS_BUS *bus)
{
	unsigned short int data=0x0000; 

	digitalRead(bus->D0)?(data |= 0x0001):(data |= 0x0000);
	digitalRead(bus->D1)?(data |= 0x0002):(data |= 0x0000);
	digitalRead(bus->D2)?(data |= 0x0004):(data |= 0x0000);
	digitalRead(bus->D3)?(data |= 0x0008):(data |= 0x0000);
	digitalRead(bus->D4)?(data |= 0x0010):(data |= 0x0000);
	digitalRead(bus->D5)?(data |= 0x0020):(data |= 0x0000);
	digitalRead(bus->D6)?(data |= 0x0040):(data |= 0x0000);
	digitalRead(bus->D7)?(data |= 0x0080):(data |= 0x0000);
	digitalRead(bus->D8)?(data |= 0x0100):(data |= 0x0000);
	digitalRead(bus->D9)?(data |= 0x0200):(data |= 0x0000);
	digitalRead(bus->D10)?(data |= 0x0400):(data |= 0x0000);
	digitalRead(bus->D11)?(data |= 0x0800):(data |= 0x0000);
	digitalRead(bus->D12)?(data |= 0x1000):(data |= 0x0000);
	digitalRead(bus->D13)?(data |= 0x2000):(data |= 0x0000);
	digitalRead(bus->D14)?(data |= 0x4000):(data |= 0x0000);
	digitalRead(bus->D15)?(data |= 0x8000):(data |= 0x0000);

	return data;

}

/*******************************************************************************
*>author 	:koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 串口启动函数
********************************************************************************/

int SerialBegin (int part_data, int baud)
{
  struct termios _options;
  speed_t _myBaud;
  int     _status,_fd;
  char *device;

	switch(part_data)
	{
		case UART0:
			
		break;
		case UART2:
			device = "/dev/ttyS1";
		break;
		case UART3:
			device = "/dev/ttyS2";
		break;
		case UART4:
			device = "/dev/ttyS3";
		break;
		case UART7:
			device = "/dev/ttyS4";
		break;
		default:
		break;
	}

	switch (baud)
	{
		case   9600:	_myBaud =   B9600 ; break ;
		case  19200:	_myBaud =  B19200 ; break ;
		case  38400:	_myBaud =  B38400 ; break ;
		case  57600:	_myBaud =  B57600 ; break ;
		case 115200:	_myBaud = B115200 ; break ;
		default:
		return -2;
	}

  if ((_fd = open (device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
    return -1 ;

  fcntl (_fd, F_SETFL, O_RDWR) ;

	switch(part_data)
	{
		case UART0:
			g_fd_array[0] = _fd;
		break;
		case UART2:
			g_fd_array[1] = _fd;
		break;
		case UART3:
			g_fd_array[2] = _fd;
		break;
		case UART4:
			g_fd_array[3] = _fd;
		break;
		case UART7:
			g_fd_array[4] = _fd;
		break;
		default:
		break;
	}

	tcgetattr (_fd, &_options) ;

    cfmakeraw   (&_options) ;
    cfsetispeed (&_options, _myBaud) ;
    cfsetospeed (&_options, _myBaud) ;

    _options.c_cflag |= (CLOCAL | CREAD) ;
    _options.c_cflag &= ~PARENB ;
    _options.c_cflag &= ~CSTOPB ;
    _options.c_cflag &= ~CSIZE ;
    _options.c_cflag |= CS8 ;
    _options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG) ;
    _options.c_oflag &= ~OPOST ;

    _options.c_cc [VMIN]  =   0 ;
    _options.c_cc [VTIME] = 100 ;

	tcsetattr (_fd, TCSANOW | TCSAFLUSH, &_options) ;

	ioctl (_fd,TIOCMGET,&_status);

	_status |= TIOCM_DTR ;
	_status |= TIOCM_RTS ;

	ioctl (_fd, TIOCMSET, &_status);
	usleep (10000) ;	

	return _fd;
}

/*******************************************************************************
*>author 	: koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 清空缓存函数
********************************************************************************/

int SerialFlush (int fd)
{
	char _statue;
	int  _sfd;

	switch(fd)
	{
		case UART0:
			_sfd = g_fd_array[0];
		break;
		case UART2:
			_sfd = g_fd_array[1];
		break;
		case UART3:
			_sfd = g_fd_array[2];
		break;
		case UART4:
			_sfd = g_fd_array[3];
		break;
		case UART7:
			_sfd = g_fd_array[4];
		break;
		default:
		break;
	}

 	_statue = tcflush(_sfd, TCIOFLUSH);

	if(_statue < 0)
	{
		return -1;
	} else {
		return 0;
	}
}

/*******************************************************************************
*>author 	:koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 串口结束函数
********************************************************************************/

int SerialEnd (int fd)
{
	int _statue,_sfd;
	
	switch(fd)
	{
		case UART0:
			_sfd = g_fd_array[0];
		break;
		case UART2:
			_sfd = g_fd_array[1];
		break;
		case UART3:
			_sfd = g_fd_array[2];
		break;
		case UART4:
			_sfd = g_fd_array[3];
		break;
		case UART7:
			_sfd = g_fd_array[4];
		break;
		default:
		break;
	}

  	_statue = close(_sfd) ;
	
	if(_statue < 0)
	{
		return -1;
	} else {
		return 0;
	}
}

/*******************************************************************************
*>author 	:koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 向某一特定串口写8位数据
********************************************************************************/

int SerialWrite (int fd, unsigned char c)
{
	int _statue,_sfd;
	
	switch(fd)
	{
		case UART0:
			_sfd = g_fd_array[0];
		break;
		case UART2:
			_sfd = g_fd_array[1];
		break;
		case UART3:
			_sfd = g_fd_array[2];
		break;
		case UART4:
			_sfd = g_fd_array[3];
		break;
		case UART7:
			_sfd = g_fd_array[4];
		break;
		default:
		break;
	}
	
  	_statue = write (_sfd, &c, 1);
	
	if(_statue < 0)
	{
		return -1;
	} else {
		return 0;
	}
}
/*******************************************************************************
*>author 	:koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 通过某一串口输出字符串
********************************************************************************/

int SerialPrint (int fd, const char *s)
{
	int _statue,_sfd;
	
	switch(fd)
	{
		case UART0:
			_sfd = g_fd_array[0];
		break;
		case UART2:
			_sfd = g_fd_array[1];
		break;
		case UART3:
			_sfd = g_fd_array[2];
		break;
		case UART4:
			_sfd = g_fd_array[3];
		break;
		case UART7:
			_sfd = g_fd_array[4];
		break;
		default:
		break;
	}
  	_statue = write (_sfd, s, strlen (s));
	
	if(_statue < 0)
	{
		return -1;
	} else {
		return 0;
	}
}
/*******************************************************************************
*>author 	:koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 通过某一串口输出字符串(带有换行功能)
********************************************************************************/
int SerialPrintln (int fd, const char *s)
{
	int _statue,_sfd;

	switch(fd)
	{
		case UART0:
			_sfd = g_fd_array[0];
		break;
		case UART2:
			_sfd = g_fd_array[1];
		break;
		case UART3:
			_sfd = g_fd_array[2];
		break;
		case UART4:
			_sfd = g_fd_array[3];
		break;
		case UART7:
			_sfd = g_fd_array[4];
		break;
		default:
		break;
	}
	
  	write (_sfd, s, strlen (s));
	write (_sfd,"\r\n",strlen("\r\n"));
	
	if(_statue < 0)
	{
		return -1;
	} else {
		return 0;
	}
}

/*******************************************************************************
*>author 	:koxuipang
*>date	:2013/8/27
*>return	:	 
*>parameter 	:
*>description	: 	 查询是否接收到数据
********************************************************************************/

int SerialAvailable (int fd)
{
  	int result,_sfd;

	switch(fd)
	{
		case UART0:
			_sfd = g_fd_array[0];
		break;
		case UART2:
			_sfd = g_fd_array[1];
		break;
		case UART3:
			_sfd = g_fd_array[2];
		break;
		case UART4:
			_sfd = g_fd_array[3];
		break;
		case UART7:
			_sfd = g_fd_array[4];
		break;
		default:
		break;
	}
	
  	if (ioctl (_sfd, FIONREAD, &result) == -1)
    	return -1;

	return result;
}

/*******************************************************************************
*>author	:	koxuipang
*>date	:	2013/8/27
*>return	:	 
*>parameter 	:
*>descrption	:	read data from  certain port
********************************************************************************/

int SerialRead (int fd)
{
  	uint8_t x;
	int _sfd;

	switch(fd)
	{
		case UART0:
			_sfd = g_fd_array[0];
		break;
		case UART2:
			_sfd = g_fd_array[1];
		break;
		case UART3:
			_sfd = g_fd_array[2];
		break;
		case UART4:
			_sfd = g_fd_array[3];
		break;
		case UART7:
			_sfd = g_fd_array[4];
		break;
		default:
		break;
	}

  	if (read (_sfd, &x, 1) != 1)
    	return -1 ;

  	return ((int)x)&0xFF;
}

/*******************************************************************************
********************************************************************************
*>							bash level i2c driver
********************************************************************************
********************************************************************************/

struct i2c_msg {
	__u16 addr;	
	unsigned short flags;		
#define I2C_M_TEN			0x10	
#define I2C_M_RD			0x01
#define I2C_M_NOSTART		0x4000
#define I2C_M_REV_DIR_ADDR	0x2000
#define I2C_M_IGNORE_NAK	0x1000
#define I2C_M_NO_RD_ACK		0x0800
	short len;		
	char *buf;		
};
union i2c_smbus_data {
	__u8 byte;
	__u16 word;
	__u8 block[I2C_SMBUS_BLOCK_MAX + 2]; 
};

struct i2c_smbus_ioctl_data {
	char read_write;
	__u8 command;
	int size;
	union i2c_smbus_data *data;
};
struct i2c_rdwr_ioctl_data {
	struct i2c_msg *msgs;	
	int nmsgs;		
};

static inline __s32 cci2c_access(int file, char read_write, __u8 command,int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;
	return ioctl(file,I2C_SMBUS,&args);
}

__s32 cci2c_write_quick(int file, __u8 value)
{
	return cci2c_access(file,value,0,I2C_SMBUS_QUICK,NULL);
}
	
__s32 cci2c_read_byte(int file)
{
	union i2c_smbus_data data;
	if (cci2c_access(file,I2C_SMBUS_READ,0,I2C_SMBUS_BYTE,&data))
		return -1;
	else
		return 0x0FF & data.byte;
}

__s32 cci2c_write_byte(int file, __u8 value)
{
	return cci2c_access(file,I2C_SMBUS_WRITE,value,I2C_SMBUS_BYTE,NULL);
}

__s32 cci2c_read_byte_data(int file, __u8 command)
{
	union i2c_smbus_data data;
	if (cci2c_access(file,I2C_SMBUS_READ,command,I2C_SMBUS_BYTE_DATA,&data))
		return -1;
	else
		return 0x0FF & data.byte;
}
__s32 cci2c_write_byte_data(int file, __u8 command,__u8 value)
{
	union i2c_smbus_data data;
	data.byte = value;
	return cci2c_access(file,I2C_SMBUS_WRITE,command,I2C_SMBUS_BYTE_DATA, &data);
}
/*******************************************************************************
*>author	:	 koxuipang
*>date  	:	 2013/11/13  
*>return	:	 fail:-1 ok:0
*>parameter 	:	file:device fd
				command:device offset
*>description	:	read a word data 
********************************************************************************/
__s32 cci2c_read_word_data(int file, __u8 command)
{
	union i2c_smbus_data data;
	if (cci2c_access(file,I2C_SMBUS_READ,command,I2C_SMBUS_WORD_DATA,&data))
		return -1;
	else
		return 0x0FFFF & data.word;
}
 /*******************************************************************************
 *>author	 :	  koxuipang
 *>date 	 :	  2013/11/13  
 *>return	 :	  fail:-1 ok:0
 *>parameter	 :	 file:device fd
				 command:device offset
				 value:need write word data
 *>description	 :	 write a word data 
 ********************************************************************************/
 __s32 cci2c_write_word_data(int file, __u8 command,__u16 value)
{
	union i2c_smbus_data data;
	data.word = value;
	return cci2c_access(file,I2C_SMBUS_WRITE,command,I2C_SMBUS_WORD_DATA, &data);
}
 
__s32 cci2c_process_call(int file, __u8 command, __u16 value)
{
	union i2c_smbus_data data;
	data.word = value;
	if (cci2c_access(file,I2C_SMBUS_WRITE,command,I2C_SMBUS_PROC_CALL,&data))
		return -1;
	else
		return 0x0FFFF & data.word;
}
/*******************************************************************************
*>author	  :   koxuipang
*>date  :	  2013/11/13  
*>return	  :   fail:-1 ok:0
*>parameter :	 file:device fd
				 command:device offset
				 length:the length of read data
				 *values:read data buffer
*>description	  :  read string data 
 ********************************************************************************/

 __s32 cci2c_read_block_data(int file, __u8 command, __u8 *values)
{
	union i2c_smbus_data data;
	int i;
	if (cci2c_access(file,I2C_SMBUS_READ,command,I2C_SMBUS_BLOCK_DATA,&data))
		return -1;
	else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];
	}
}
/*******************************************************************************
*>author	  :   koxuipang
*>date  :	  2013/11/13  
*>return	  :   fail:-1 ok:0
*>parameter :	file:device fd
			 	command:device offset
			 	length:the length of write data
			 	*values:write data buffer
*>description	  :  	write string data 
********************************************************************************/

__s32 cci2c_write_block_data(int file, __u8 command, __u8 length, const __u8 *values)
{
	union i2c_smbus_data data;
	int i;
	if (length > 32)
		length = 32;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;
	return cci2c_access(file,I2C_SMBUS_WRITE,command,I2C_SMBUS_BLOCK_DATA, &data);
}
/*******************************************************************************
*>author	 :	 koxuipang
*>date  :	 2013/11/13  
*>return	 :	 fail:-1 ok:0
*>parameter :	file:device fd
				command:device offset
				length:the length of read data
				*values:read data buffer
*>description	 :	read string data 
********************************************************************************/

__s32 cci2c_read_i2c_block_data(int file, __u8 command,__u8 length, __u8 *values)
{
	union i2c_smbus_data data;
	int i;

	if (length > 32)
		length = 32;
	data.block[0] = length;
	if (cci2c_access(file,I2C_SMBUS_READ,command,length == 32 ? I2C_SMBUS_I2C_BLOCK_BROKEN : I2C_SMBUS_I2C_BLOCK_DATA,&data))
		return -1;
	else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];
	}
}
/*******************************************************************************
*>author	 :	 koxuipang
*>date  :	 2013/11/13  
*>return	 :	 fail:-1 ok:0
*>parameter :	file:device fd
				command:device offset
				length:the length of write data
				*values:write data buffer
*>description	 :	write string data 
********************************************************************************/
__s32 cci2c_write_i2c_block_data(int file, __u8 command, __u8 length, const __u8 *values)
{
	union i2c_smbus_data data;
	int i;
	if (length > 32)
		length = 32;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;
	return cci2c_access(file,I2C_SMBUS_WRITE,command,I2C_SMBUS_I2C_BLOCK_BROKEN, &data);
}
__s32 cci2c_block_process_call(int file, __u8 command, __u8 length, __u8 *values)
{
	union i2c_smbus_data data;
	int i;
	if (length > 32)
		length = 32;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;
	if (cci2c_access(file,I2C_SMBUS_WRITE,command,I2C_SMBUS_BLOCK_PROC_CALL,&data))
		return -1;
	else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];
	}
}
/*******************************************************************************
*>author	  :   koxuipang
*>date  :	  2013/11/13  
*>return	  :   fail:-1 ok:0
*>parameter :	file:device fd
*>description	  :   	open device
********************************************************************************/

int cci2c_open_dev(char* filename)
{
    int file ;
    file = open(filename, O_RDWR);
    return file;
}
/*******************************************************************************
*>author	 :	 koxuipang
*>date  :	 2013/11/13  
*>return	 :	 fail:-1 ok:0
*>parameter :	 file:device fd
*>description	 :	 close device
********************************************************************************/
int cci2c_close_dev(int file)
{
 	return close(file);
}
/*******************************************************************************
*>author	:	koxuipang
*>date	:	2013/11/13	
*>return 	:	error number
*>parameter	:	file:device fd
				address:slave address
				force:0,1
*>description	:	set slave device address
********************************************************************************/
int cci2c_set_slave_addr(int file, int address, int force)
{
	if (ioctl(file, force ? I2C_SLAVE_FORCE : I2C_SLAVE, address) < 0) {
		fprintf(stderr,
			"Error: Could not set address to 0x%02x: %s\n",
			address, strerror(errno));
		return -errno;
	}

	return 0;
}
/******************************************************************
*******************************************************************
*******************************************************************
					high level I2C driver
*******************************************************************
*******************************************************************
*******************************************************************/
typedef struct _i2c_data_
{
		int fd;
		unsigned char tx_block[32]; 	  
		int tx_len; 	  
		int tx_index;
		unsigned char tx_offset_addr;

		unsigned char rx_block[32];
		int rx_len;
		int rx_index;
		unsigned char rx_offset_addr;

		int size;
		int used;
}i2c_data;

static i2c_data g_i2c_data_array[2] = {0};
	
#define  I2C1	1
#define  I2C2	2
/*******************************************************************************
*>author	:	koxuipang
*>date	:	2013/11/13	
*>return 	:	devoce fd
*>parameter	:	device:I2C1,I2C2
*>description	: 	start i2c function
********************************************************************************/
__u16 WireBegin(__u16 device)
{
	int fd;

	switch(device)
	{
		case I2C1:
			fd=cci2c_open_dev("/dev/i2c-1");
			g_i2c_data_array[I2C1].fd=fd;
			break;
		case I2C2:
			fd=cci2c_open_dev("/dev/i2c-2");
			g_i2c_data_array[I2C2].fd=fd;
			break;
		default:break;
	}

	if(fd<0)
	{
		return -1;
	}
	g_i2c_data_array[device].used=1;
	return 0;
}
/*******************************************************************************
*>author	:	koxuipang
*>date	:	2013/11/13
*>return	:	fail:-1 ok:0
*>parameter	:	device:I2C1,I2C2 
				addr:slave addr
*>description	: 	start i2c function and set the slave device addr
********************************************************************************/

__u16 WireBeginTransmission(__u16 device,__u8 addr)
{	
	if(cci2c_set_slave_addr(g_i2c_data_array[device].fd,addr,0)< 0 )
		return -1;
	return 0;
}

/*******************************************************************************
*>author	:	koxuipang
*>date	:	2013/11/13
*>return	:	fail:-1 ok:0
*>parameter	:	device:I2C1,I2C2	
				data:write data from i2c bus	
*>description	: 	write data to i2c devce
********************************************************************************/

__u16 WireWrite(__u16 device, __u8 data)
{
	if (g_i2c_data_array[device].tx_len > 32)
		return -1;
	g_i2c_data_array[device].tx_block[g_i2c_data_array[device].tx_len] = data;
	g_i2c_data_array[device].tx_len++;
	return 0;
}
/*******************************************************************************
*>author	:	koxuipang
*>date	:	2013/11/13
*>return :	fail:-1 ok:0
*>parameter	:	device:I2C1,I2C2	
*>description	: 	stop i2c transmission
********************************************************************************/

__u16 WireEndTransmission(__u16 device)
{
	int size;
	int res;
	int i = 0 ;
	size = g_i2c_data_array[device].tx_len;
	unsigned char addr = g_i2c_data_array[device].tx_block[0];
	unsigned char tmp_block[32];
	for(i=0;i<32;i++)
		tmp_block[i]=g_i2c_data_array[device].tx_block[i+1];
	res = cci2c_write_i2c_block_data(g_i2c_data_array[device].fd, addr, size-1, tmp_block);
	memset(g_i2c_data_array[device].tx_block,0 ,32);
	g_i2c_data_array[device].tx_len=0;
	return res;
}
/*******************************************************************************
*>author	:	koxuipang
*>date	:	2013/11/13
*>return	:	fail:-1 ok:0
*>parameter	:	device:I2C1,I2C2 
				addr:slave addr 
				quantity:need request data quantity	
*>description	: 	request some quantity data form i2c slave device and set slave addr.
********************************************************************************/

__u16 WireRequestFrom(__u16 device, __u8 addr, __u16 quantity)
{
	int res;
	if(g_i2c_data_array[device].fd == 0 && quantity > 32 )
		return -1;
	memset(g_i2c_data_array[device].rx_block,0,32);
	g_i2c_data_array[device].rx_len = quantity;
	g_i2c_data_array[device].rx_offset_addr = addr;

	res = cci2c_read_i2c_block_data(g_i2c_data_array[device].fd,addr,quantity,g_i2c_data_array[device].rx_block);
	return res;
}
/*******************************************************************************
*>author	:	koxuipang
*>date	:	2013/11/13
*>return	:	available data
*>parameter	:	device:I2C1,I2C2
*>description	: 	check how many data can be recieved from i2c device
********************************************************************************/

__u16 WireAvailable(__u16 device)
{
	return g_i2c_data_array[device].rx_len - g_i2c_data_array[device].rx_index;
}
/*******************************************************************************
*>author	:	koxuipang
*>date	:	2013/11/13
*>return	:	unsigned char data
*>parameter	:	device:I2C1,I2C2
*>description	: 	start to read data form i2c device
********************************************************************************/

__u8  WireRead(__u16 device)
{
	int value = -1;
	if(g_i2c_data_array[device].rx_index < g_i2c_data_array[device].rx_len)
		value = g_i2c_data_array[device].rx_block[g_i2c_data_array[device].rx_index];
	g_i2c_data_array[device].rx_index++;
	return value;
}
/*******************************************************************************
*>author	:	koxuipang
*>date	:	2013/11/13
*>return	:	fail:-1 ok:0
*>parameter	:	devcie:I2C1,I2C2	
*>description	: 	stop i2c device 
********************************************************************************/

__u8 WireEnd(__u16 device)
{
	int res = cci2c_close_dev(g_i2c_data_array[device].fd);
	return res;
}

	







