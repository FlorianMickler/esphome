#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
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

  float get_setup_priority() const override;
};

}  // namespace tsic
}  // namespace esphome
