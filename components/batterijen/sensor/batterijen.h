#pragma once
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace batterijen {

class BatterijenSensor : public Component, public uart::UARTDevice {
 public:
  void set_pack_id(uint8_t id) { pack_id_ = id; }
  void set_cell_sensor(size_t index, sensor::Sensor *sens) {
    if (index >= cells_.size()) cells_.resize(index + 1, nullptr);
    cells_[index] = sens;
  }
  void set_soc_sensor(sensor::Sensor *sens) { soc_sensor_ = sens; }

  void loop() override {
    while (available() >= 10) {
      if (peek() == 0x55) {
        uint8_t header[4];
        read_array(header, 4);
        if (header[0] == 0x55 && header[1] == 0xAA &&
            header[2] == 0xEB && header[3] == 0x90) {
          uint8_t pid = read();
          read(); // reserved
          if (pid == pack_id_) {
            ESP_LOGI("batterijen", "Frame for pack_id %u", pid);

            // Cellen uitlezen
            for (size_t i = 0; i < cells_.size(); i++) {
              uint8_t low = read();
              uint8_t high = read();
              uint16_t mv = low | (high << 8);
              if (cells_[i] && mv > 0 && mv < 5000) {
                float volts = mv / 1000.0f;
                cells_[i]->publish_state(volts);
                ESP_LOGD("batterijen", "Cell %u: %.3f V", (unsigned)i+1, volts);
              }
            }

            // Pack voltage (2 bytes) & current (2 bytes)
            read(); read();
            read(); read();

            // SOC uitlezen (1 byte)
            uint8_t soc_val = read();
            if (soc_sensor_ && soc_val <= 100) {
              soc_sensor_->publish_state(soc_val);
              ESP_LOGD("batterijen", "SOC: %u %%", soc_val);
            }
          }
        }
      } else {
        read(); // skip 1 byte tot header match
      }
    }
  }

 protected:
  uint8_t pack_id_{1};
  std::vector<sensor::Sensor *> cells_;
  sensor::Sensor *soc_sensor_{nullptr};
};

}  // namespace batterijen
}  // namespace esphome
