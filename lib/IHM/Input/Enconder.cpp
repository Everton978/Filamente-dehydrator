#include "Enconder.hpp"

namespace EnconderBtn {

// Tabela de transições de quadratura: índice = (estado_antigo << 2) | estado_novo
static const int8_t kTable[16] = {
   0, -1,  1,  0,
   1,  0,  0, -1,
  -1,  0,  0,  1,
   0,  1, -1,  0
};

Enconder::Enconder(uint8_t pinA, uint8_t pinB, uint8_t pinBtn)
  : _a(pinA), _b(pinB), _btn(pinBtn) {}

void Enconder::begin() {
  pinMode(_a, INPUT_PULLUP);
  pinMode(_b, INPUT_PULLUP);
  pinMode(_btn, INPUT_PULLUP);

  _state = (digitalRead(_a) << 1) | digitalRead(_b);
}

EnconderEvent Enconder::read() {
  // --- rotação (polling) ---
  const uint8_t s = (digitalRead(_a) << 1) | digitalRead(_b);
  if (s != (_state & 0x03)) {
    _state = ((_state << 2) | s) & 0x0F;
    _acc += kTable[_state];

    if (_acc >= 4)       { _backlog++; _acc = 0; }
    else if (_acc <= -4) { _backlog--; _acc = 0; }
  }

  if (_backlog > 0) { _backlog--; return EnconderEvent::Right; }
  if (_backlog < 0) { _backlog++; return EnconderEvent::Left; }

  // --- botão (ativo em LOW) ---
  const bool raw = (digitalRead(_btn) == LOW);
  const uint32_t now = millis();

  if (raw != _lastRaw) {
    _lastRaw = raw;
    _tChange = now;
  }

  if ((now - _tChange) >= kDebounceMs && raw != _stable) {
    _stable = raw;
    if (_stable) {
      _tPress = now;
      _longSent = false;
    } else if (!_longSent) {
      return EnconderEvent::Click;
    }
  }

  if (_stable && !_longSent && (now - _tPress) >= kLongPressMs) {
    _longSent = true;
    return EnconderEvent::LongPress;
  }

  return EnconderEvent::None;
}

}  // namespace EnconderBtn