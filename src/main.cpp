/*
-Por motivos de elucidação da construção do codigo a interrupção do zero-Cross será a primeira função declarada após as configuração da placa
-A arquitetura das classes tiveram como primazia realizar a forma fisica do projeto, pensando em isolar  cada conexão do hardware e criar uma camada de visualização e controle termico.
*/


#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Pinout.hpp"
#include "PID.hpp"
#include "../IHM/Input/Enconder.hpp"
#include "../IHM/Lcd/Menu.hpp"
#include "../IHM/DisplayManager.hpp"
//Variaveis

volatile unsigned long tempoZeroCross = 0;
volatile bool zeroCrossDetected = false;


//Namespaces

using namespace Pinout;
using namespace EnconderBtn;
using namespace ManagerMenu;
//Objetos
LiquidCrystal_I2C lcd(0x27, 16, 2);

Enconder Enc (SCLKA,DtB, SwitchBtn);

Menu menu(lcd);
DisplayManager ui(Enc, menu);


//Funcoes de interrupcao

void CrossZero(){

  tempoZeroCross = micros();

  zeroCrossDetected = true;

}



//Prototipos de funcoes
void configBoard();



// Funcoes do FrameWork

void setup() {
  Wire.begin(Pinout::I2C_SDA, Pinout::I2C_SCL);
  configBoard();
 
}

void loop() {
  // put your main code here, to run repeatedly:
    static bool rodandoAntes = false;

  if (g_settings.running && !rodandoAntes) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bem-vindo");
    delay(1500);           // aqui pode, é só uma vez ao iniciar
    lcd.clear();
    menu.invalidate();     // força o menu a redesenhar
  }
  rodandoAntes = g_settings.running;

  ui.update();
}

 

 
//Declaracoes de funcoes

void configBoard() {

  Serial.begin(115200);

  //Initi Pinout.hpp
  pinMode(ZeroCross,INPUT);
  pinMode(OutControl,OUTPUT);


  //I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  //LCD 16x2
  lcd.init();
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print(" Estufa Filamento");
  lcd.setCursor(0, 1);
  lcd.print("Inicializando...");
  delay(1000);
  lcd.clear();
  Enc.begin();
  ui.begin();

}

/*const int PIN_ZC   = 2;   // saída do detector de cruzamento por zero (INT0)
const int PIN_GATE = 8;   // vai ao optoacoplador (MOC3021) do gate

volatile unsigned long tempoZero = 0;  // instante do último cruzamento (µs)
volatile bool esperandoDisparo = false;
unsigned long atrasoUs = 4000;         // atraso atual em µs

// Chamada automaticamente a cada cruzamento por zero
void cruzouZero() {
  digitalWrite(PIN_GATE, LOW);   // desliga o gate no zero
  tempoZero = micros();          // guarda o instante
  esperandoDisparo = true;       // avisa o loop para disparar
}

// Ajusta o disparo em milissegundos após o cruzamento por zero
void setAtrasoMs(float ms) {
  if (ms < 0.2) ms = 0.2;        // não disparar colado no zero
  if (ms > 8.0) ms = 8.0;        // 60 Hz: semiciclo = 8,33 ms
  atrasoUs = ms * 1000;
}

void setup() {
  pinMode(PIN_GATE, OUTPUT);
  digitalWrite(PIN_GATE, LOW);
  pinMode(PIN_ZC, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_ZC), cruzouZero, RISING);

  setAtrasoMs(4.0);              // ~90° (meia potência aprox.)
}

void loop() {
  if (esperandoDisparo) {
    noInterrupts();
    unsigned long t0 = tempoZero;
    interrupts();

    if (micros() - t0 >= atrasoUs) {
      digitalWrite(PIN_GATE, HIGH);  // gate fica ligado até o próximo zero
      esperandoDisparo = false;
    }
  }
}*/