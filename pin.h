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
#define _lo(x, y, v1, v2) ((x)<(y) ? (v1) : (v2))
#define _ddr(i) _lo((i), 8, DDRD, _lo((i), 14, DDRB, DDRC))
#define _port(i) _lo((i), 8, PORTD, _lo((i), 14, PORTB, PORTC))
#define _pin(i) _lo((i), 8, PIND, _lo((i), 14, PINB, PINC))
#define _number(i) ((i)-_lo((i), 8, 0, _lo((i), 14, 8, 14)))
#define _an_pin(i) (B01000000 | (i))
#define _byte_pos(pos) (B00000001<<(pos))
#define _byte_inv(pos) (B11111111^(B00000001<<(pos)))
//set In - Out
#define setPinIn(i) _ddr((i)) &= _byte_inv(_number((i)))
#define setPinOut(i) _ddr((i)) |= _byte_pos(_number((i)))
//set Hi- Low
#define setPinHi(i) _port((i)) |= _byte_pos(_number((i)))
#define setPinLow(i) _port((i)) &= _byte_inv(_number((i)))
//invert
#define invert(i) _port((i)) ^= _byte_pos(_number((i)))
//read digit pin status
#define readPin(i) ((_pin((i)) & _byte_pos(_number((i))))>>_number((i)))
//read analog pin status
#define readAPin(i) (AnRead(_an_pin(_number((i)))))

#endif //pin_H