"""
FIXME: Description for TSIC

"""
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor
from esphome.const import (
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    CONF_PIN,
    CONF_DC_PIN,
    CONF_EXTERNAL_VCC,
)

cg.add_library(
    name="TsicSensor",
    repository="https://github.com/FlorianMickler/TsicSensor",
    version=None,
)

CODEOWNERS = [""]

tsic_ns = cg.esphome_ns.namespace("tsic")
TSICComponent = tsic_ns.class_("TSICComponent", cg.PollingComponent, sensor.Sensor)

tt = cg.global_ns.namespace("TsicType").enum("TsicType")
TSIC_TYPES = {
    "TSIC_206": tt.TSIC_206,
    "TSIC_306": tt.TSIC_306,
    "TSIC_316": tt.TSIC_316,
    "TSIC_506": tt.TSIC_506,
    "TSIC_516": tt.TSIC_516,
    "TSIC_716": tt.TSIC_716,
}
validate_tsic_type = cv.enum(TSIC_TYPES, upper=True)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        TSICComponent,
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(
        {
            cv.Required(CONF_PIN): pins.internal_gpio_input_pin_schema,
            cv.Optional(CONF_DC_PIN, default=CONF_EXTERNAL_VCC): cv.Any(
                cv.one_of(CONF_EXTERNAL_VCC),
                pins.internal_gpio_output_pin_schema,
            ),
            cv.Required("type"): cv.templatable(validate_tsic_type),
        }
    )
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_data_pin(pin))
    if config[CONF_DC_PIN] == CONF_EXTERNAL_VCC:
        print("------------------- in cg stage: CONF_EXTERNAL_VCC-----")
        cg.add(var.set_external_vcc(1))
    else:
        dc_pin = await cg.gpio_pin_expression(config[CONF_DC_PIN])
        cg.add(var.set_external_vcc(0))
        cg.add(var.set_dc_pin(dc_pin))
    cg.add(var.set_tsic_type(config["type"]))
