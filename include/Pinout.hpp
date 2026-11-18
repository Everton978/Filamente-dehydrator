#pragma once
#include <cstdint>

namespace Pinout{


// Barramento I2C 
constexpr uint8_t I2C_SDA = 21;
constexpr uint8_t I2C_SCL = 22;

//Encoder
constexpr uint8_t  SCLKA = 34;
constexpr uint8_t DtB = 35;
constexpr uint8_t SwitchBtn =36;


//Sensores LM35
constexpr uint8_t Temp1 = 32;
constexpr uint8_t Temp2 = 33;

constexpr uint8_t Umi_Data = 26;

//Circuito_ZERO_CROOZZ

constexpr uint8_t ZeroCross = 24;

//Saida_Controle

constexpr uint8_t OutControl = 25;

}