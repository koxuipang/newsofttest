#include <iocpp.h>
#include <SSD1306.h>

struct TwoWireFirst Wire;
extern const unsigned char oled_font5x8[][16];
unsigned char m_col,m_row;

SSD1306::SSD1306(int8_t SCLK, int8_t DC, int8_t RST, int8_t CS) 
{
  cs = CS;
  rst = RST;
  dc = DC;
  sclk = SCLK;
}
SSD1306::SSD1306(int8_t reset) 
{
  sclk = dc = cs = -1;
  rst = reset;
}


void SSD1306::begin(void) 
{
	rst=11;
	uint8_t vccstate=SSD1306_SWITCHCAPVCC;
	uint8_t i2caddr=0x3c;
	
	_i2caddr = i2caddr;
	Wire.begin(); 
	
	pinMode(rst, OUTPUT);
	digitalWrite(rst, HIGH);
	delay(10);
	digitalWrite(rst, LOW);
	delay(100);
	digitalWrite(rst, HIGH);
   	delay(100);
	
    ssd1306_command(SSD1306_DISPLAYOFF);                   
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);           
    ssd1306_command(0x80);                                 
    ssd1306_command(SSD1306_SETMULTIPLEX);                 
    ssd1306_command(0x3F);
    ssd1306_command(SSD1306_SETDISPLAYOFFSET);             
    ssd1306_command(0x0);                                  
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);           
    ssd1306_command(SSD1306_CHARGEPUMP);                   
    if (vccstate == SSD1306_EXTERNALVCC){ 
		ssd1306_command(0x10); 
		}else{ 
		ssd1306_command(0x14); }
    ssd1306_command(SSD1306_MEMORYMODE);                   
    ssd1306_command(0x00);                                 
    ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);
    ssd1306_command(SSD1306_SETCOMPINS);                   
    ssd1306_command(0x12);
    ssd1306_command(SSD1306_SETCONTRAST);                  
    if (vccstate == SSD1306_EXTERNALVCC){ 
			ssd1306_command(0x9F); 
		}else{ 
			ssd1306_command(0xCF); 
		}
    ssd1306_command(SSD1306_SETPRECHARGE);                 
    if (vccstate == SSD1306_EXTERNALVCC){ 
			ssd1306_command(0x22); 
		}else{ 
			ssd1306_command(0xF1); 
		}
    ssd1306_command(SSD1306_SETVCOMDETECT);                
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);          
    ssd1306_command(SSD1306_NORMALDISPLAY);                
  	ssd1306_command(SSD1306_DISPLAYON);
    delay(50);

    ssd1306_command(SSD1306_SETLOWCOLUMN | 0x0);  
    ssd1306_command(SSD1306_SETHIGHCOLUMN | 0x0); 
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0); 

    for (byte i = 0; i < SSD1306_LCDHEIGHT / 8; i++) {
        ssd1306_command(0xB0 + i);	
        ssd1306_command(0);			
        ssd1306_command(0x10);		

        for(byte j = 0; j < 8; j++){
            Wire.beginTransmission(_i2caddr);
            Wire.write(0x40);
            for (byte k = 0; k < SSD1306_LCDWIDTH / 8; k++) {
                Wire.write(0);
            }
            Wire.endTransmission();
        }
    }
}
void SSD1306::invertDisplay(uint8_t i) 
{
	if (i) {
		ssd1306_command(SSD1306_INVERTDISPLAY);
	} else {
		ssd1306_command(SSD1306_NORMALDISPLAY);
	}
}

void SSD1306::ssd1306_command(uint8_t c) 
{

    uint8_t control = 0x00;   
    Wire.beginTransmission(_i2caddr);
    Wire.write(control);
    Wire.write(c);
    Wire.endTransmission();
}

void SSD1306::startscrollright(uint8_t start, uint8_t stop)
{
	ssd1306_command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
	ssd1306_command(0X00);
	ssd1306_command(start);
	ssd1306_command(0X00);
	ssd1306_command(stop);
	ssd1306_command(0X01);
	ssd1306_command(0XFF);
	ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306::startscrollleft(uint8_t start, uint8_t stop)
{
	ssd1306_command(SSD1306_LEFT_HORIZONTAL_SCROLL);
	ssd1306_command(0X00);
	ssd1306_command(start);
	ssd1306_command(0X00);
	ssd1306_command(stop);
	ssd1306_command(0X01);
	ssd1306_command(0XFF);
	ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306::startscrolldiagright(uint8_t start, uint8_t stop)
{
	ssd1306_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
	ssd1306_command(0X00);
	ssd1306_command(SSD1306_LCDHEIGHT);
	ssd1306_command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
	ssd1306_command(0X00);
	ssd1306_command(start);
	ssd1306_command(0X00);
	ssd1306_command(stop);
	ssd1306_command(0X01);
	ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306::startscrolldiagleft(uint8_t start, uint8_t stop)
{
	ssd1306_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
	ssd1306_command(0X00);
	ssd1306_command(SSD1306_LCDHEIGHT);
	ssd1306_command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
	ssd1306_command(0X00);
	ssd1306_command(start);
	ssd1306_command(0X00);
	ssd1306_command(stop);
	ssd1306_command(0X01);
	ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306::stopscroll(void)
{
	ssd1306_command(SSD1306_DEACTIVATE_SCROLL);
}

void SSD1306::ssd1306_data(uint8_t c) 
{
    uint8_t control = 0x40;  
    Wire.beginTransmission(_i2caddr);
    Wire.write(control);
    Wire.write(c);
    Wire.endTransmission();
}

void SSD1306::fill(unsigned char dat)
{
    unsigned char i,j;

    ssd1306_command(0x00);
    ssd1306_command(0x10);
    ssd1306_command(0xB0);

	for (byte i=0; i<(SSD1306_LCDHEIGHT/8); i++)
    {
        ssd1306_command(0xB0 + i);	
        ssd1306_command(0);			
        ssd1306_command(0x10);		

        for(byte j = 0; j < 8; j++){
            Wire.beginTransmission(_i2caddr);
            Wire.write(0x40);
            for (byte k = 0; k < 16; k++) {
                Wire.write(dat);
            }
            Wire.endTransmission();
        }
    }
}
void SSD1306::setCursor(unsigned char column, unsigned char line)
{	
    m_col = column;
    m_row = line;
	ssd1306_command(0xB0 + m_row);
	ssd1306_command(m_col & 0xf);	
    ssd1306_command(0x10 | (m_col >> 4));
}

void SSD1306::write(uint8_t c)
{
	int fd;
	unsigned char block[32];
	unsigned char i,j=0;
	
	if (c > 0x20 && c < 0x7f) {
	    c -= 0x21;
	    ssd1306_command(0xB0 + m_row);			
	    ssd1306_command(m_col & 0xf);			
	    ssd1306_command(0x10 | (m_col >> 4));	
	    Wire.beginTransmission(_i2caddr);
	    Wire.write(0x40);
	    for (byte i = 0; i <= 14; i += 2) {
	        Wire.write(oled_font5x8[c][i]);
	    }
	    Wire.endTransmission();

	    ssd1306_command(0xB0 + m_row + 1);
	    ssd1306_command(m_col & 0xf);
	    ssd1306_command(0x10 | (m_col >> 4));

	    Wire.beginTransmission(_i2caddr);
	    Wire.write(0x40);
	    for (byte i = 1; i <= 15; i += 2) {
	        Wire.write(oled_font5x8[c][i]);
	    }
	    Wire.endTransmission();
	} else {
	    ssd1306_command(0xB0 + m_row);
	    ssd1306_command(m_col & 0xf);
	    ssd1306_command(0x10 | (m_col >> 4));

	    Wire.beginTransmission(_i2caddr);
	    Wire.write(0x40);
	    for (byte i = 0; i < 8; i ++) {
	        Wire.write(0);
	    }
	    Wire.endTransmission();

	    ssd1306_command(0xB0 + m_row + 1);		
	    ssd1306_command(m_col & 0xf);			
	    ssd1306_command(0x10 | (m_col >> 4));	

	    Wire.beginTransmission(_i2caddr);
	    Wire.write(0x40);
	    for (byte i = 0; i < 8; i ++) {
	        Wire.write(0);
	    }
	    Wire.endTransmission();
	}
    m_col += 9;
}
void SSD1306::write_str(const char * str)
{
	int i=0;
	while(str[i]!='\0'){
		write(str[i]);
		i++;
	}		
}
void SSD1306::clear(byte x, byte y, byte width, byte height)
{
    ssd1306_command(SSD1306_SETLOWCOLUMN | 0x0); 
    ssd1306_command(SSD1306_SETHIGHCOLUMN | 0x0);
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0); 
    height >>= 3;
    width >>= 3;
    y >>= 3;
    for (byte i = 0; i < height; i++) {
        ssd1306_command(0xB0 + i + y);		
        ssd1306_command(x & 0xf);			
        ssd1306_command(0x10 | (x >> 4));	
        for(byte j = 0; j < 8; j++){
            Wire.beginTransmission(_i2caddr);
            Wire.write(0x40);
            for (byte k = 0; k < width; k++) {
                Wire.write(0);
            }
            Wire.endTransmission();
        }
    }
}
extern "C"{
	SSD1306 obj;
	void begin(void){
		obj.begin();
	}
	void invertDisplay(uint8_t i){
		obj.invertDisplay(i);
	}
	void startscrollright(uint8_t start, uint8_t stop){
		obj.startscrollright(start,stop);
	}
	void startscrollleft(uint8_t start, uint8_t stop){
		obj.startscrollleft(start,stop);
	}
	void startscrolldiagright(uint8_t start, uint8_t stop){
		obj.startscrolldiagright(start,stop);
	}
	void startscrolldiagleft(uint8_t start, uint8_t stop){
		obj.startscrolldiagleft(start,stop);
	}
	void stopscroll(void){
		obj.stopscroll();
	}
	void fill(unsigned char dat){
		obj.fill(dat);
	}
	void setCursor(unsigned char column, unsigned char line){
		obj.setCursor(column,line);
	}
	void write_oled(uint8_t c){
		obj.write(c);
	}
	void write_str(const char * str){
		obj.write_str(str);
	}
	void clear(byte x, byte y, byte width, byte height){
		obj.clear(x,y,width,height);
	}
	

}

