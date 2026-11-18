#pragma once
#include <Arduino.h>

namespace EnconderBtn {

enum class EnconderEvent : uint8_t {
  None,
  Left,
  Right,
  Click,      // clique curto (disparado ao soltar o botão)
  LongPress   // clique longo (disparado enquanto segura)
};

// Leitura do rotary encoder por polling (sem ISR e sem registradores).
// read() precisa ser chamado com frequência: evite delay() no loop().
class Enconder {
public:
  Enconder(uint8_t pinA, uint8_t pinB, uint8_t pinBtn);

  void begin();
  EnconderEvent read();   // devolve um evento por chamada

private:
  static constexpr uint16_t kDebounceMs = 30;
  static constexpr uint16_t kLongPressMs = 800;

  uint8_t _a, _b, _btn;

  // Rotação
  uint8_t _state = 0;
  int8_t _acc = 0;
  int8_t _backlog = 0;

  // Botão
  bool _lastRaw = false;
  bool _stable = false;
  bool _longSent = false;
  uint32_t _tChange = 0;
  uint32_t _tPress = 0;
};

}  // namespace EnconderBtn