#pragma once
#include <Arduino.h>
#include "Input/Enconder.hpp"
#include "Lcd/Menu.hpp"

namespace ManagerMenu {

// Regra de navegação: traduz os eventos do encoder em movimento/edição no Menu.
//  - Girar:        move o cursor (ou altera o valor, se estiver editando)
//  - Clique:       entra em submenu / executa ação / inicia e confirma edição / volta (item Back)
//  - Clique longo: cancela a edição ou volta uma página
class DisplayManager {
public:
  DisplayManager(EnconderBtn::Enconder& enconder, Menu& menu);

  void begin();
  void update();   // chamar a cada loop()

private:
  static constexpr uint8_t kMaxDepth = 4;

  void handle(EnconderBtn::EnconderEvent ev);
  void goBack();

  EnconderBtn::Enconder& _enc;
  Menu& _menu;

  const MenuPage* _stack[kMaxDepth];
  uint8_t _cursor[kMaxDepth];
  uint8_t _depth = 0;

  bool _editing = false;
  int16_t _editValue = 0;
  bool _dirty = true;
};

}  // namespace ManagerMenu