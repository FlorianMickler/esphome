#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"
#include <TsicSensor.h>

namespace esphome {
namespace tsic {

class TSICComponent : public PollingComponent, public sensor::Sensor {
 public:
  TsicSensor* _sensor;
  /// Setup (reset) the sensor and check connection.
  void setup() override;
  void dump_config() override;
  /// Update the sensor values (temperature)
  void update() override;

  void set_data_pin(InternalGPIOPin* pin) { this->data_pin_ = pin; };
  void set_dc_pin(InternalGPIOPin* pin) { this->dc_pin_ = pin; };
  void set_external_vcc(int val) { this->external_vcc_ = (val > 0); };
  void set_tsic_type(TsicType type) { this->type_ = type; };

  float get_setup_priority() const override;
 protected:
	InternalGPIOPin* data_pin_;
	InternalGPIOPin* dc_pin_;
	TsicType type_;
	bool external_vcc_;
};

}  // namespace tsic
}  // namespace esphome
