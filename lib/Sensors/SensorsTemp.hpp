#pragma once

#include <Arduino.h>
#include "Pinout.hpp"

class LM35{

    public:

    // Construtor: define o pino, tensão de referência e resolução
    LM35(uint8_t _pino, float _RefV, uint16_t _ResolucAdc);
    void begin();
    float LerCelsius(uint8_t Amostras);
    float lerFahrenheit(uint8_t Amostras);
    float lerKelvin(uint8_t Amostras);


    private:

    uint8_t pino;
    float RefV;
    uint16_t ResolucAdc;
}
