#ifndef pin_H
#define pin_H
//DIGITAL
#define DPIN_0 0
#define DPIN_1 1
#define DPIN_2 2
#define DPIN_3 3
#define DPIN_4 4
#define DPIN_5 5
#define DPIN_6 6
#define DPIN_7 7
#define DPIN_8 8
#define DPIN_9 9
#define DPIN_10 10
#define DPIN_11 11
#define DPIN_12 12
#define DPIN_13 13
//ANALOG
#define APIN_0 14
#define APIN_1 15
#define APIN_2 16
#define APIN_3 17
#define APIN_4 18
#define APIN_5 19
//utils
#define _compare(v1, v2, v3, v4) ((v1)<(v2) ? (v3) : (v4))
#define _pin(i) ((i)-_lo((i), 8, 0, _lo((i), 14, 8, 14)))
//set In - Out
char setPinIn(char pin);
char setPinOut(char pin);
char setPinHi(char pin);
char setPinLow(char pin);


#endif //pin_H