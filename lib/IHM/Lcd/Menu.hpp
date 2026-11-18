#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

enum class ItemType : uint8_t {
  Submenu,  // abre outra página
  Action,   // chama uma função
  Value,    // edita um número (int16_t)
  Back      // volta uma página
};

struct MenuPage;

struct MenuItem {
  const char* label;
  ItemType type;
  const MenuPage* target;   // Submenu
  void (*action)();         // Action
  int16_t* value;           // Value
  int16_t min, max, step;   // Value
  const char* unit;         // Value (ex.: "C", "min")
};

struct MenuPage {
  const char* title;
  const MenuItem* items;
  uint8_t count;
};

// Parâmetros editáveis pelo menu
struct Settings {
  int16_t tempTarget = 55;   // °C
  int16_t timeMin = 240;     // minutos
  int16_t kp = 10;
  int16_t ki = 1;
  int16_t kd = 0;
  bool running = false;
};
extern Settings g_settings;

// Define o que aparece no LCD (estrutura dos menus + desenho das linhas)
class Menu {
public:
  explicit Menu(LiquidCrystal_I2C& lcd);

  const MenuPage* root() const;

  // Desenha a janela de 2 linhas da página, com o cursor no item `cursor`.
  void draw(const MenuPage* page, uint8_t cursor, bool editing, int16_t editValue);

  // Força redesenho completo na próxima chamada de draw()
  void invalidate();

private:
  void printLine(uint8_t row, const char* text);

  LiquidCrystal_I2C& _lcd;
  char _cache[2][17];
};


/*
// lib/IHM/Lcd/Menu.hpp
#pragma once
#include <stdint.h>
#include <LiquidCrystal_I2C.h>
#include "MenuNav.hpp"

class MenuLcd {
public:
    static constexpr uint8_t kCols = 16;   // 20 para 20x4
    static constexpr uint8_t kRows = 2;    // 4 para 20x4

    explicit MenuLcd(LiquidCrystal_I2C& lcd) : lcd_(lcd) {}
    void invalidar();                          // após lcd.clear()/reinit
    void desenhar(const MenuNav& nav);
    void escreverLinha(uint8_t linha, const char* texto);   // reaproveitada pela tela de status

private:
    void formatarItem(char* out, const MenuItem& it, bool sel, bool edit, int32_t valorEd) const;
    LiquidCrystal_I2C& lcd_;
    char cache_[kRows][kCols + 1] = {};
};
*/