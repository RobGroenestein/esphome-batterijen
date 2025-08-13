#pragma once
#include "esphome/core/component.h"
#include "esphome/core/log.h"  // <-- deze toevoegen

namespace esphome {
namespace batterijen {

class Batterijen : public Component {
 public:
  void setup() override { ESP_LOGI("batterijen", "Setup complete"); }
  void loop() override {}
};

}  // namespace batterijen
}  // namespace esphome
