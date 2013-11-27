#!/bin/bash
gcc main.cpp SSD1306.cpp wire.cpp font.c iocpp.c -lstdc++ -lpthread -o oled_test
echo "wire ok"
gcc GSM_take_sms.cpp GSM_shield.cpp Serial.cpp iocpp.c -lstdc++ -lpthread -o sim900_test
echo "serial ok"
