#include "tsic.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace tsic {

static const char *const TAG = "tsic";

void TSICComponent::setup() {
	ESP_LOGCONFIG(TAG, "Setting up TSIC...");
	byte data_pin = this->data_pin_->get_pin();
	byte dc_pin = TsicExternalVcc;
	if (!this->external_vcc_)
		dc_pin = this->dc_pin_->get_pin();

	//_sensor = TsicSensor::create(12, TsicExternalVcc, TsicType::TSIC_306);
	_sensor = TsicSensor::create(data_pin, dc_pin, this->type_);
}

void TSICComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "TSIC:");
  //LOG_I2C_DEVICE(this);
  LOG_PIN("  Data Pin:", this->data_pin_);
  ESP_LOGCONFIG(TAG, "  TSIC_TYPE: %d", this->type_);
  if (!this->external_vcc_) {
	  LOG_PIN("  DC Pin:", this->dc_pin_);
  } else {
	  ESP_LOGCONFIG(TAG, "  DC Pin: external_vcc");
  }

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
