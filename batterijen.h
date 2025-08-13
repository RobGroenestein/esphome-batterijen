#pragma once
#include "esphome/core/component.h"

namespace esphome {
namespace batterijen {

class Batterijen : public Component {
 public:
  void setup() override { ESP_LOGI("batterijen", "Setup complete"); }
  void loop() override {}
};

}  // namespace batterijen
}  // namespace esphome
