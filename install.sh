#!/bin/bash
cd iteadIO
make install
cd ..
cp GSM_shield.h SSD1306.h digitalIO.h Serial.h wire.h iocpp.h pinmap.h regmap.h iteadcpp.h /usr/include 
cp GSM_shield.h SSD1306.h digitalIO.h Serial.h wire.h iocpp.h pinmap.h regmap.h iteadcpp.h /usr/local/include 
gcc -shared -fpic Serial.cpp wire.cpp iocpp.c -o libiteadcpp.so.1.0.0 -lpthread
gcc -shared -fpic SSD1306.cpp wire.cpp iocpp.c font.c -o libiteadoled.so -lpthread
gcc -shared -fpic GSM_shield.cpp Serial.cpp iocpp.c -o libiteadsim900.so -lpthread
mv libiteadcpp.so.1.0.0 libiteadoled.so libiteadsim900.so /usr/lib
cd /usr/lib
ln -sf libiteadcpp.so.1.0.0 libiteadcpp.so
cd -
cp iteadcompilecpp /bin
cp iteadcompilecpp /usr/bin
cp iteadcompilecpp /usr/local/bin/

