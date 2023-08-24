#include "tsic.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace tsic {

static const char *const TAG = "tsic";

void TSICComponent::setup() { 
	ESP_LOGCONFIG(TAG, "Setting up TSIC...");
	_sensor = TsicSensor::create(12, TsicExternalVcc, TsicType::TSIC_306);
}

void TSICComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "TSIC:");
  //LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with TSIC failed!");
  }
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Temperature", this);
}

void TSICComponent::update() {
  float temperature = -1000.;
  if (_sensor) {
	  temperature = _sensor->getTempCelsius();
  }

  ESP_LOGD(TAG, "Got Temperature=%.1f°C", temperature);

  this->publish_state(temperature);
  this->status_clear_warning();
}

float TSICComponent::get_setup_priority() const { return setup_priority::DATA; }

}  // namespace tsic
}  // namespace esphome
