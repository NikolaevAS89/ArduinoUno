#include "ShiftRegistr.h"

ShiftRegistr::ShiftRegistr(int latchPin, int clockPin, int dataPin, int size){
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	_size = size;
  _state = new byte[size];
  _last_state = (byte*)malloc(size);
  for(int i=0;i<size;i++){
    _state[i]=B00000000;
    _last_state[i]=B00000000;
  }
}

ShiftRegistr::~ShiftRegistr(){
  delete _state;
  delete _last_state;
}

void ShiftRegistr::setPinsToHigh(int module){
  if(module<_size){
    _state[module] = B11111111;
  }
}

void ShiftRegistr::setPinsToLow(int module){
  if(module<_size){
    _state[module] = B00000000;
  }
}

void ShiftRegistr::setPinToHigh(int module, int pin){
  if(module<_size){
    _state[module] = (B00000001<<pin) | _state[module] ;
  }
}

void ShiftRegistr::setPinToLow(int module, int pin){
  if(module<_size){
    _state[module] = _state[module] ^ (B00000001<<pin);
  }
}

void ShiftRegistr::rollback(){
  for(int i=0;i<_size;i++){
    _state[i] = _last_state[i];
  }  
}

void ShiftRegistr::commit(){
  boolean isCheange = false;
  for(int i=0;i<_size;i++){
    if(_state[i]!=_last_state[i]){
      _last_state[i] = _state[i];
      isCheange = true;
    }
  }
  if(isCheange){
    boolean d = true;
    /*
    // устанавливаем синхронизацию "защелки" на LOW
    digitalWrite(latchPin, LOW);
    // передаем последовательно на dataPin
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay); 
 
    //"защелкиваем" регистр, тем самым устанавливая значения на выходах
    digitalWrite(latchPin, HIGH);
    */
  }
}
