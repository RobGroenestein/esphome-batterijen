import esphome.codegen as cg
import esphome.config_validation as cv

batterijen_ns = cg.esphome_ns.namespace("batterijen")
Batterijen = batterijen_ns.class_("Batterijen", cg.Component)

CONFIG_SCHEMA = cv.Schema({})

async def to_code(config):
    cg.add_global(cg.RawExpression("// batterijen component loaded"))
