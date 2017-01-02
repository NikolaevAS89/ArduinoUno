#ifndef ShiftRegistr_H
#define ShiftRegistr_H

#include "CyberLib.h"

class ShiftRegistr {
  private :
    int _latchPin; //Пин подключен к ST_CP входу 74HC595
    int _clockPin; //Пин подключен к SH_CP входу 74HC595
    int _dataPin; //Пин подключен к DS входу 74HC595
    int _size; //количество подсоединенных поледовательной модулей
    byte *_state; //ссылка на массив данных
    byte *_last_state; //ссылка на массив данных
  public :
  	ShiftRegistr(int latchPin = 8, int clockPin = 12, int dataPin = 11, int size = 1);
   ~ShiftRegistr();

    void setPinsToHigh(int module);
    void setPinsToLow(int module);
    
    void setPinToHigh(int module, int pin);
    void setPinToLow(int module, int pin);
    
    void rollback();
    void commit();
};

#endif
