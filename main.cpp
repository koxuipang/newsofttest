#include <iteadcpp.h>
/*
gcc main.cpp SSD1306.cpp wire.cpp i2cset.c i2c.c iocpp.c -lstdc++ -o test -lpthread
*/
int main(void)
{
	struct SSD1306 test;
	int i=0;

	test.begin(SSD1306_SWITCHCAPVCC,0x3c);
	test.clear(0,0,127,7);	
	delay(1000);
	test.fill(0x00);
	test.setCursor(20,3);
	test.write_str("hello pengge\n");
	delay(1000);
	delay(1000);
	test.startscrollright(0,7);
	delay(1000);
	test.startscrollleft(0,7);
	delay(1000);
	test.startscrolldiagright(0,7);

	return 1;
}

