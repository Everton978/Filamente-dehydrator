#include "DisplayManager.hpp"

namespace ManagerMenu {

using EnconderBtn::EnconderEvent;

DisplayManager::DisplayManager(EnconderBtn::Enconder& enconder, Menu& menu)
  : _enc(enconder), _menu(menu) {}

void DisplayManager::begin() {
  _depth = 0;
  _stack[0] = _menu.root();
  _cursor[0] = 0;
  _editing = false;
  _dirty = true;
  _menu.invalidate();
}

void DisplayManager::update() {
  const EnconderEvent ev = _enc.read();
  if (ev != EnconderEvent::None) {
    handle(ev);
    _dirty = true;
  }

  if (_dirty) {
    _menu.draw(_stack[_depth], _cursor[_depth], _editing, _editValue);
    _dirty = false;
  }
}

void DisplayManager::goBack() {
  if (_depth > 0) {
    _depth--;
  }
}

void DisplayManager::handle(EnconderEvent ev) {
  const MenuPage* page = _stack[_depth];
  uint8_t& cur = _cursor[_depth];
  const MenuItem& item = page->items[cur];

  switch (ev) {
    case EnconderEvent::Left:
    case EnconderEvent::Right: {
      const int8_t dir = (ev == EnconderEvent::Right) ? 1 : -1;
      if (_editing) {
        int32_t v = static_cast<int32_t>(_editValue) + dir * item.step;
        if (v < item.min) v = item.min;
        if (v > item.max) v = item.max;
        _editValue = static_cast<int16_t>(v);
      } else {
        cur = (cur + page->count + dir) % page->count;  // navegação circular
      }
      break;
    }

    case EnconderEvent::Click: {
      switch (item.type) {
        case ItemType::Submenu:
          if (_depth + 1 < kMaxDepth) {
            _depth++;
            _stack[_depth] = item.target;
            _cursor[_depth] = 0;
          }
          break;

        case ItemType::Action:
          if (item.action) item.action();
          break;

        case ItemType::Value:
          if (!_editing) {
            _editing = true;
            _editValue = *item.value;
          } else {
            *item.value = _editValue;  // confirma
            _editing = false;
          }
          break;

        case ItemType::Back:
          goBack();
          break;
      }
      break;
    }

    case EnconderEvent::LongPress:
      if (_editing) {
        _editing = false;  // cancela edição sem salvar
      } else {
        goBack();
      }
      break;

    default:
      break;
  }
}

}  // namespace ManagerMenu