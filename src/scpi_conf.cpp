#include "scpi/scpi.h"


#define TAG_REG_ENABLE 1
#define TAG_REG_VALUE 2
#define TAG_REG_MODE 3
#define TAG_REG_RANGE 4
#define TAG_SWEEP_START 5
#define TAG_SWEEP_STEP 6
#define TAG_SWEEP_IDX 7
#define TAG_SWEEP_COUNT 8
#define TAG_SWEEP_PSC 9
#define TAG_SWEEP_PSC_VAL 10

scpi_result_t LTC2662_query_property(scpi_t * context) {return SCPI_RES_OK;}
scpi_result_t LTC2662_set_property(scpi_t * context) {return SCPI_RES_OK;}

scpi_result_t SWEEP_query_property(scpi_t * context) {return SCPI_RES_OK;}
scpi_result_t SWEEP_set_property(scpi_t * context) {return SCPI_RES_OK;}

extern const scpi_command_t scpi_commands[] =  {
	{ .pattern = "*IDN?", .callback = SCPI_CoreIdnQ,},
	{ .pattern = "*RST", .callback = SCPI_CoreRst,},
    //LTC functions
    { .pattern = "ENable#",  .callback = LTC2662_set_property,  .tag=TAG_REG_ENABLE},
    { .pattern = "ENable#?", .callback = LTC2662_query_property,.tag=TAG_REG_ENABLE},
    { .pattern = "VALue#",   .callback = LTC2662_set_property,  .tag=TAG_REG_VALUE},
    { .pattern = "VALue#?",  .callback = LTC2662_query_property,.tag=TAG_REG_VALUE},
    { .pattern = "RANGE#",   .callback = LTC2662_set_property,  .tag=TAG_REG_RANGE},
    { .pattern = "RANGE#?",  .callback = LTC2662_query_property,.tag=TAG_REG_RANGE},
    { .pattern = "MODE#",    .callback = LTC2662_set_property,  .tag=TAG_REG_MODE},
    { .pattern = "MODE#?",   .callback = LTC2662_query_property,.tag=TAG_REG_MODE},
    { .pattern = "SWEEP#:START",    .callback = SWEEP_set_property,     .tag=TAG_SWEEP_START},
    { .pattern = "SWEEP#:START?",   .callback = SWEEP_query_property,   .tag=TAG_SWEEP_START},
    { .pattern = "SWEEP#:STEP",     .callback = SWEEP_set_property,     .tag=TAG_SWEEP_STEP},
    { .pattern = "SWEEP#:STEP?",    .callback = SWEEP_query_property,   .tag=TAG_SWEEP_STEP},
    { .pattern = "SWEEP#:COUNT",    .callback = SWEEP_set_property,     .tag=TAG_SWEEP_COUNT},
    { .pattern = "SWEEP#:COUNT?",   .callback = SWEEP_query_property,   .tag=TAG_SWEEP_COUNT},
    { .pattern = "SWEEP#:IDX",      .callback = SWEEP_set_property,     .tag=TAG_SWEEP_IDX},
    { .pattern = "SWEEP#:IDX?",     .callback = SWEEP_query_property,   .tag=TAG_SWEEP_IDX},
    { .pattern = "SWEEP#:PSC",      .callback = SWEEP_set_property,     .tag=TAG_SWEEP_PSC},
    { .pattern = "SWEEP#:PSC?",     .callback = SWEEP_query_property,   .tag=TAG_SWEEP_PSC},
    { .pattern = "SWEEP#:PSC_VAL",  .callback = SWEEP_set_property,     .tag=TAG_SWEEP_PSC_VAL},
    { .pattern = "SWEEP#:PSC_VAL?", .callback = SWEEP_query_property,   .tag=TAG_SWEEP_PSC_VAL},
    //{ .pattern = "LIST", .callback = LTC2662_Range,},

	//{ .pattern = "MEASure:VOLTage:DC?", .callback = DMM_MeasureVoltageDcQ,},
	SCPI_CMD_LIST_END
};