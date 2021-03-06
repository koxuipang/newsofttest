/*************************************************************************
	> File Name: pinmap_cb.h
	> Author: jyzhiyu
	> Mail: jyzhiyu@gmail.com 
	> Created Time: 2013年08月10日 星期六 17时19分46秒
 ************************************************************************/
#ifndef PINMAP
#define PINMAP

#include<stdlib.h>

#define _NONE   -1

#define PORTA   0
#define PORTB   1
#define PORTC   2
#define PORTD   3
#define PORTE   4
#define PORTF   5
#define PORTG   6
#define PORTH   7
#define PORTI   8

#define _MAX_PORTA  18
#define _MAX_PORTB  24
#define _MAX_PORTC  25
#define _MAX_PORTD  28
#define _MAX_PORTE  12
#define _MAX_PORTF  6
#define _MAX_PORTG  12
#define _MAX_PORTH  28
#define _MAX_PORTI  22

#define _PORT_INDEX(PORT,INDEX)    (PORT*32 + INDEX)

//#ifdef PINMAP_CB

#ifdef 		CUBIEBOARD
#define _GPIO_MAX       96
int _GPIO_MAP[_GPIO_MAX + 1] = {
    _PORT_INDEX(PORTH,21),                          //Pin 0, LED 2
    _PORT_INDEX(PORTD,0),   _NONE,                  //pin 1,2 at U14 
    _PORT_INDEX(PORTD,2),   _PORT_INDEX(PORTD,1),   //pin 3,4 at U14
    _PORT_INDEX(PORTD,4),   _PORT_INDEX(PORTD,3),   //pin 5,6
    _PORT_INDEX(PORTD,6),   _PORT_INDEX(PORTD,5),   //pin 7,8
    _NONE,                  _PORT_INDEX(PORTD,7),   //pin 9,10
    _PORT_INDEX(PORTD,9),   _PORT_INDEX(PORTD,8),   //pin 11,12
    _PORT_INDEX(PORTD,11),  _PORT_INDEX(PORTD,10),  //pin 13,14
    _PORT_INDEX(PORTD,13),  _PORT_INDEX(PORTD,12),  //pin 15,16
    _PORT_INDEX(PORTD,15),  _PORT_INDEX(PORTD,14),  //pin 17,18
    _PORT_INDEX(PORTD,16),  _NONE,                  //pin 19,20
    _PORT_INDEX(PORTD,18),  _PORT_INDEX(PORTD,17),  //pin 21,22
    _PORT_INDEX(PORTD,20),  _PORT_INDEX(PORTD,19),  //pin 23,24
    _PORT_INDEX(PORTD,22),  _PORT_INDEX(PORTD,21),  //pin 25,26
    _PORT_INDEX(PORTD,27),  _PORT_INDEX(PORTD,23),  //pin 27,28
    _PORT_INDEX(PORTD,24),  _PORT_INDEX(PORTD,26),  //pin 29,30
    _PORT_INDEX(PORTB,2),   _PORT_INDEX(PORTD,25),  //pin 31,32
    _NONE,                  _NONE,                  //pin 33,34
    _NONE,                  _NONE,                  //pin 35,36
    _PORT_INDEX(PORTH,7),   _NONE,                  //pin 37,38
    _PORT_INDEX(PORTB,11),  _PORT_INDEX(PORTB,10),  //pin 39,40
    _NONE,                  _NONE,                  //pin 41,42
    _NONE,                  _NONE,                  //pin 43,44
    _PORT_INDEX(PORTI,10),  _PORT_INDEX(PORTI,12),  //pin 45,46
    _PORT_INDEX(PORTI,11),  _PORT_INDEX(PORTI,13),  //pin 47,48
    
    _NONE,                  _PORT_INDEX(PORTH,15),  //pin 1,2 at U15
    _NONE,                  _PORT_INDEX(PORTH,14),  //pin 3,4
    _PORT_INDEX(PORTG,0),   _PORT_INDEX(PORTB,18),  //pin 5,6
    _PORT_INDEX(PORTB,19),  _PORT_INDEX(PORTG,3),   //pin 7,8
    _PORT_INDEX(PORTG,2),   _PORT_INDEX(PORTG,1),   //pin 9,10
    _PORT_INDEX(PORTG,4),   _PORT_INDEX(PORTG,5),   //pin 11,12
    _PORT_INDEX(PORTG,6),   _PORT_INDEX(PORTG,7),   //pin 13,14
    _PORT_INDEX(PORTG,8),   _PORT_INDEX(PORTG,9),   //pin 15,16
    _PORT_INDEX(PORTG,10),  _PORT_INDEX(PORTG,11),  //pin 17,18
    _NONE,                  _NONE,                  //pin 19,20
    _NONE,                  _PORT_INDEX(PORTI,4),   //pin 21,22
    _NONE,                  _PORT_INDEX(PORTI,5),   //pin 23,24
    _NONE,                  _PORT_INDEX(PORTI,6),   //pin 25,26
    _NONE,                  _PORT_INDEX(PORTI,7),   //pin 27,28
    _NONE,                  _PORT_INDEX(PORTI,8),   //pin 29,30
    _NONE,                  _PORT_INDEX(PORTI,9),   //pin 31,32
    _PORT_INDEX(PORTH,27),  _PORT_INDEX(PORTE,4),   //pin 33,34
    _PORT_INDEX(PORTH,26),  _PORT_INDEX(PORTE,5),   //pin 35,36
    _NONE,                  _PORT_INDEX(PORTE,6),   //pin 37,38
    _NONE,                  _PORT_INDEX(PORTE,7),   //pin 39,40
    _NONE,                  _PORT_INDEX(PORTE,8),   //pin 41,42
    _NONE,                  _PORT_INDEX(PORTE,9),   //pin 43,44
    _NONE,                  _PORT_INDEX(PORTE,10),  //pin 45,46
    _NONE,                  _PORT_INDEX(PORTE,11)   //pin 47,48
   
};
#endif

#ifdef ITEADPLUS
#define _GPIO_MAX       144
int _GPIO_MAP[_GPIO_MAX + 1] = {
    _NONE,                          				//Dummy pin	
    _NONE,   				_NONE,                  //PIN1	    	PIN2 
    _NONE,   				_NONE,   				//PIN3	    	PIN4
    _NONE,   				_NONE,   				//PIN5	    	PIN6
    _NONE,   				_NONE,   				//PIN7	    	PIN8
    _PORT_INDEX(PORTD,0),  	_PORT_INDEX(PORTD,1), 	//PIN9	    	PIN10
    _PORT_INDEX(PORTD,2),  	_PORT_INDEX(PORTD,3),   //PIN11	    	PIN12
    _PORT_INDEX(PORTD,4),  	_PORT_INDEX(PORTD,5),  	//PIN13	    	PIN14
    _PORT_INDEX(PORTD,6),  	_PORT_INDEX(PORTD,7),	//PIN15	    	PIN16
    _PORT_INDEX(PORTD,8),  	_PORT_INDEX(PORTD,9), 	//PIN17	    	PIN18
    _PORT_INDEX(PORTD,10),  _PORT_INDEX(PORTD,11), 	//PIN19	    	PIN20
    _PORT_INDEX(PORTD,12),  _PORT_INDEX(PORTD,13), 	//PIN21	    	PIN22
    _PORT_INDEX(PORTD,14),  _PORT_INDEX(PORTD,15), 	//PIN23	    	PIN24
    _PORT_INDEX(PORTD,16),  _PORT_INDEX(PORTD,17), 	//PIN25	    	PIN26
    _PORT_INDEX(PORTD,18),  _PORT_INDEX(PORTD,19), 	//PIN27	    	PIN28
    _PORT_INDEX(PORTD,20),	_PORT_INDEX(PORTD,21), 	//PIN29	    	PIN30
    _PORT_INDEX(PORTD,22),	_PORT_INDEX(PORTD,23), 	//PIN31	    	PIN32
    _PORT_INDEX(PORTD,24),	_PORT_INDEX(PORTD,25), 	//PIN33	   	PIN34
	_PORT_INDEX(PORTD,26), 	_PORT_INDEX(PORTD,27), 	//PIN35	    	PIN36
	_PORT_INDEX(PORTH,0),	_NONE,			//PIN37	    	PIN38
	_PORT_INDEX(PORTH,2),	_PORT_INDEX(PORTH,3),	//PIN39	    	PIN40	
	_PORT_INDEX(PORTH,4),	_PORT_INDEX(PORTH,5),	//PIN41	    	PIN42	
	_PORT_INDEX(PORTH,6),	_PORT_INDEX(PORTH,7),	//PIN43	    	PIN44
	_PORT_INDEX(PORTH,8),	_PORT_INDEX(PORTH,9),	//PIN45	    	PIN46
	_PORT_INDEX(PORTH,10),	_PORT_INDEX(PORTH,11),	//PIN47	   	PIN48
	_PORT_INDEX(PORTH,12),	_PORT_INDEX(PORTH,13),	//PIN49	    	PIN50
	_PORT_INDEX(PORTH,14),	_PORT_INDEX(PORTH,15),	//PIN51	    	PIN52
	_PORT_INDEX(PORTH,16),	_PORT_INDEX(PORTH,17),	//PIN53	    	PIN54
	_PORT_INDEX(PORTH,18),	_PORT_INDEX(PORTH,19),	//PIN55	    	PIN56
	_PORT_INDEX(PORTH,20),	_PORT_INDEX(PORTH,21),	//PIN57	    	PIN58
	_PORT_INDEX(PORTH,22),	_PORT_INDEX(PORTH,23),	//PIN59	    	PIN60
	_PORT_INDEX(PORTH,24),	_PORT_INDEX(PORTH,25), 	//PIN61	    	PIN62
	_PORT_INDEX(PORTH,26),	_PORT_INDEX(PORTH,27), 	//PIN63	    	PIN64
	_NONE,   				_NONE, 					//PIN65	    	PIN66
	_PORT_INDEX(PORTB,18),	_PORT_INDEX(PORTB,19),	//PIN67	   	PIN68
	_PORT_INDEX(PORTB,20),	_PORT_INDEX(PORTB,21),	//PIN69	    	PIN70
	_NONE,   				_NONE,					//PIN71	    	PIN72


	_NONE,   				_NONE,					//PIN73	    	PIN74
	_PORT_INDEX(PORTI,19),	_PORT_INDEX(PORTI,18),	//PIN75	    	PIN76
	_PORT_INDEX(PORTI,17),	_PORT_INDEX(PORTI,16),	//PIN77	    	PIN78
	_PORT_INDEX(PORTG,0),  	_PORT_INDEX(PORTG,1), 	//PIN79	    	PIN80
    _PORT_INDEX(PORTG,2),  	_PORT_INDEX(PORTG,3),  	//PIN81	   	PIN82
    _PORT_INDEX(PORTG,4),  	_PORT_INDEX(PORTG,5),  	//PIN83	   	PIN84
    _PORT_INDEX(PORTG,6),  	_PORT_INDEX(PORTG,7),	//PIN85	    	PIN86
    _PORT_INDEX(PORTG,8),  	_PORT_INDEX(PORTG,9), 	//PIN87	    	PIN88
    _PORT_INDEX(PORTG,10),  _PORT_INDEX(PORTG,11), 	//PIN89	    	PIN90
    _PORT_INDEX(PORTE,0),  	_PORT_INDEX(PORTE,1), 	//PIN91	   	PIN92
    _PORT_INDEX(PORTE,2),  	_PORT_INDEX(PORTE,3),  	//PIN93	   	PIN94
    _PORT_INDEX(PORTE,4),  	_PORT_INDEX(PORTE,5),  	//PIN95	    	PIN96
    _PORT_INDEX(PORTE,6),  	_PORT_INDEX(PORTE,7),	//PIN97	    	PIN98
    _PORT_INDEX(PORTE,8),  	_PORT_INDEX(PORTE,9), 	//PIN99	    	PIN100
    _PORT_INDEX(PORTE,10),  _PORT_INDEX(PORTE,11), 	//PIN101		PIN102
	_PORT_INDEX(PORTI,0),  	_PORT_INDEX(PORTI,1), 	//PIN103		PIN104
    _PORT_INDEX(PORTI,2),	_NONE,					//PIN105		PIN106
    _PORT_INDEX(PORTI,3),  	_PORT_INDEX(PORTB,2), 	//PIN107		PIN108
    _PORT_INDEX(PORTI,10),	_PORT_INDEX(PORTI,11),	//PIN109		PIN110
	_PORT_INDEX(PORTI,12),	_PORT_INDEX(PORTI,13),	//PIN111		PIN112
	_PORT_INDEX(PORTI,4),	_PORT_INDEX(PORTI,5),	//PIN113		PIN114
	_PORT_INDEX(PORTI,6),	_PORT_INDEX(PORTI,7),	//PIN115		PIN116
	_PORT_INDEX(PORTI,8),	_PORT_INDEX(PORTI,9),	//PIN117		PIN118
	_PORT_INDEX(PORTI,20),	_PORT_INDEX(PORTI,21),	//PIN119		PIN120
	_PORT_INDEX(PORTB,3),   _PORT_INDEX(PORTB,4),  	//PIN121		PIN122
	_PORT_INDEX(PORTB,5),  	_PORT_INDEX(PORTB,6), 	//PIN123		PIN124
	_PORT_INDEX(PORTB,7),	_PORT_INDEX(PORTB,8),  	//PIN125		PIN126
	_PORT_INDEX(PORTB,9), 	_PORT_INDEX(PORTB,10),	//PIN127		PIN128
	_PORT_INDEX(PORTB,11), 	_PORT_INDEX(PORTB,12),	//PIN129		PIN130
	_PORT_INDEX(PORTB,13), 	_NONE,					//PIN131		PIN132
	_PORT_INDEX(PORTB,14),	_PORT_INDEX(PORTB,15),	//PIN133		PIN134
	_PORT_INDEX(PORTB,16),	_PORT_INDEX(PORTB,17),	//PIN135		PIN136
	_PORT_INDEX(PORTI,14),	_PORT_INDEX(PORTI,15),	//PIN137		PIN138
	_NONE,   				_NONE,                 	//PIN139		PIN140
    _NONE,   				_NONE,   				//PIN141		PIN142
    _NONE,   				_NONE   				//PIN143		PIN144
};

#endif
#endif
