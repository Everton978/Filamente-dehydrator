#include "../IHM/Lcd/Menu.hpp"

Settings g_settings;

namespace {

// ---------- Ações (troque pelo que sua estufa precisa fazer) ----------
void actStart()   { g_settings.running = true; }
void actStop()    { g_settings.running = false; }
void presetPLA()  { g_settings.tempTarget = 45; g_settings.timeMin = 240; }
void presetPETG() { g_settings.tempTarget = 65; g_settings.timeMin = 240; }
void presetABS()  { g_settings.tempTarget = 80; g_settings.timeMin = 240; }

// ---------- Helpers para declarar itens de forma curta ----------
constexpr MenuItem Sub(const char* label, const MenuPage* target) {

  return MenuItem{label, ItemType::Submenu, target, nullptr, nullptr, 0, 0, 0, nullptr};

}
constexpr MenuItem Act(const char* label, void (*fn)()) {

  return MenuItem{label, ItemType::Action, nullptr, fn, nullptr, 0, 0, 0, nullptr};

}
constexpr MenuItem Val(const char* label, int16_t* v, int16_t mn, int16_t mx, int16_t step, const char* unit) 
{

  return MenuItem{label, ItemType::Value, nullptr, nullptr, v, mn, mx, step, unit};

}
constexpr MenuItem Back(const char* label = "Voltar") {

  return MenuItem{label, ItemType::Back, nullptr, nullptr, nullptr, 0, 0, 0, nullptr};

}

#define COUNT(a) static_cast<uint8_t>(sizeof(a) / sizeof((a)[0]))

// ---------- Páginas: declare as filhas ANTES das mães ----------
const MenuItem kPresetItems[] = {
  Act("PLA",  presetPLA),
  Act("PETG", presetPETG),
  Act("ABS",  presetABS),
  Back(),
};
const MenuPage kPresets{"Presets", kPresetItems, COUNT(kPresetItems)};

const MenuItem kConfigItems[] = {
  Val("Kp", &g_settings.kp, 0, 500, 1, ""),
  Val("Ki", &g_settings.ki, 0, 500, 1, ""),
  Val("Kd", &g_settings.kd, 0, 500, 1, ""),
  Back(),
};
const MenuPage kConfig{"Config", kConfigItems, COUNT(kConfigItems)};

const MenuItem kRootItems[] = {
  Act("Bem-Vindo", actStart),
  Act("Parar",   actStop),
  Val("Temp",  &g_settings.tempTarget, 30, 90, 1, "C"),
  Val("Tempo", &g_settings.timeMin,    10, 720, 10, "min"),
  Sub("Presets", &kPresets),
  Sub("Config",  &kConfig),
};
const MenuPage kRoot{"Estufa", kRootItems, COUNT(kRootItems)};

}  // namespace

// ---------------------------------------------------------------------

Menu::Menu(LiquidCrystal_I2C& lcd) : _lcd(lcd) {
  invalidate();
}

const MenuPage* Menu::root() const { return &kRoot; }

void Menu::invalidate() {
  _cache[0][0] = '\0';
  _cache[1][0] = '\0';
}

void Menu::printLine(uint8_t row, const char* text) {
  if (strncmp(_cache[row], text, 16) == 0) return;  // sem mudança, evita flicker
  _lcd.setCursor(0, row);
  _lcd.print(text);
  strncpy(_cache[row], text, 16);
  _cache[row][16] = '\0';
}

void Menu::draw(const MenuPage* page, uint8_t cursor, bool editing, int16_t editValue) {
  // Janela de 2 linhas: cursor na linha 0 só quando é o primeiro item
  const uint8_t top = (cursor > 0) ? cursor - 1 : 0;

  for (uint8_t row = 0; row < 2; ++row) {
    const uint8_t idx = top + row;
    char line[17];

    if (idx >= page->count) {
      memset(line, ' ', 16);
      line[16] = '\0';
    } else {
      const MenuItem& it = page->items[idx];
      const bool sel = (idx == cursor);
      const char mark = sel ? (editing ? '*' : '>') : ' ';

      char right[12] = "";
      switch (it.type) {
        case ItemType::Value: {
          const int16_t v = (sel && editing) ? editValue : *it.value;
          snprintf(right, sizeof(right), "%d%s", v, it.unit ? it.unit : "");
          break;
        }
        case ItemType::Submenu:
          right[0] = '\x7E';  // seta "→" na ROM A00 do HD44780
          right[1] = '\0';
          break;
        default:
          break;
      }

      const int rl = strlen(right);
      const int w = 15 - rl;
      snprintf(line, sizeof(line), "%c%-*.*s%s", mark, w, w, it.label, right);
    }
    printLine(row, line);
  }
}