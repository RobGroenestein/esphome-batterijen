import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]

batterijen_ns = cg.esphome_ns.namespace("batterijen")
BatterijenSensor = batterijen_ns.class_("BatterijenSensor", cg.Component, uart.UARTDevice)

CONF_PACK_ID = "pack_id"
CONF_CELLS = "cells"

CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(): cv.declare_id(BatterijenSensor),
        cv.Required(CONF_PACK_ID): cv.int_,
        cv.Required(CONF_CELLS): cv.ensure_list(sensor.sensor_schema()),
    })
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    cg.add(var.set_pack_id(config[CONF_PACK_ID]))

    for i, sens_conf in enumerate(config[CONF_CELLS]):
        sens = await sensor.new_sensor(sens_conf)
        cg.add(var.set_cell_sensor(i, sens))
