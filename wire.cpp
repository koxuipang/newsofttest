#include <wire.h>

void TwoWireFirst::begin(void)
{
	WireBegin(1);
}
void TwoWireFirst::begin(uint8_t address)
{
	WireBegin(1);
	WireBeginTransmission(1,address);
}
void TwoWireFirst::begin(int address)
{
	begin((uint8_t)address);
}
void TwoWireFirst::beginTransmission(uint8_t address)
{
	WireBeginTransmission(1,address);
}
void TwoWireFirst::beginTransmission(int address)
{
	beginTransmission((uint8_t)address);
}

uint8_t TwoWireFirst::endTransmission()
{
	return WireEndTransmission(1);
}
uint8_t TwoWireFirst::endTransmission(uint8_t sendStop)
{
	uint8_t i;
	i=sendStop;
	return WireEndTransmission(1);
}

uint8_t TwoWireFirst::requestFrom(uint8_t address, uint8_t quantity)
{
	return WireRequestFrom(1,address,quantity);
}
uint8_t TwoWireFirst::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
	uint8_t i;
	i=sendStop;
	return WireRequestFrom(1,address,quantity);
}
uint8_t TwoWireFirst::requestFrom(int address, int quantity)
{
	return requestFrom((uint8_t)address,(uint8_t)quantity);
}
uint8_t TwoWireFirst::requestFrom(int address, int quantity, int sendStop)
{
	return requestFrom((uint8_t)address,(uint8_t)quantity,(uint8_t)sendStop);
}

size_t TwoWireFirst::write(uint8_t data)
{
	return WireWrite(1,data);
}
size_t TwoWireFirst::write(const uint8_t *data, size_t quantity)
{
	for(size_t i = 0; i < quantity; ++i){
		write(data[i]);    
	}
	return quantity;
}
int TwoWireFirst::available(void)
{
	return WireAvailable(1);
}
int TwoWireFirst::read(void)
{
	return WireRead(1);
}

void TwoWireSecond::begin(void)
{
	WireBegin(2);
}
void TwoWireSecond::begin(uint8_t address)
{
	WireBegin(2);
	WireBeginTransmission(2,address);
}
void TwoWireSecond::begin(int address)
{
	begin((uint8_t)address);
}
void TwoWireSecond::beginTransmission(uint8_t address)
{
	WireBeginTransmission(2,address);
}
void TwoWireSecond::beginTransmission(int address)
{
	beginTransmission((uint8_t)address);
}

uint8_t TwoWireSecond::endTransmission()
{
	return WireEndTransmission(2);
}
uint8_t TwoWireSecond::endTransmission(uint8_t sendStop)
{
	uint8_t i;
	i=sendStop;
	return WireEndTransmission(2);
}

uint8_t TwoWireSecond::requestFrom(uint8_t address, uint8_t quantity)
{
	return WireRequestFrom(2,address,quantity);
}
uint8_t TwoWireSecond::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
	uint8_t i;
	i=sendStop;
	return WireRequestFrom(2,address,quantity);
}
uint8_t TwoWireSecond::requestFrom(int address, int quantity)
{
	return requestFrom((uint8_t)address,(uint8_t)quantity);
}
uint8_t TwoWireSecond::requestFrom(int address, int quantity, int sendStop)
{
	return requestFrom((uint8_t)address,(uint8_t)quantity,(uint8_t)sendStop);
}

size_t TwoWireSecond::write(uint8_t data)
{
	return WireWrite(2,data);
}
size_t TwoWireSecond::write(const uint8_t *data, size_t quantity)
{
	for(size_t i = 0; i < quantity; ++i){
		write(data[i]);    
	}
	return quantity;
}
int TwoWireSecond::available(void)
{
	return WireAvailable(2);
}
int TwoWireSecond::read(void)
{
	return WireRead(2);
}

