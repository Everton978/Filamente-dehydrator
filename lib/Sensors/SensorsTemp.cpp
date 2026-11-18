#include "SensorsTemp.hpp"

using namespace Pinout;

LM35::LM35((uint8_t _pino, float _RefV, uint16_t _ResolucAdc){ 
    pino = _pino;
    Refv = _RefV;
    ResolucAdc = _ResolucAdc;
        
}

void LM35::begin(){

    pinMode(_pino;INPUT);
}

float LM35::float LerCelsius(uint8_t Amostras){

    int SomaLeituras=0;
    for(int i =0; i < Amostras; i++){
        SomaLeituras += analogRead(_pino); 
        milis(2);
    }

}
