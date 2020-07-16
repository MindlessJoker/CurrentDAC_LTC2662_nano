#include "scpi/scpi.h"
#include "DAC_control.hpp"
#include <SPI.h>
#include <EEPROM.h>  
#include <avr/io.h>
#include <avr/interrupt.h>


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
#define SCPI_RES_ERR SCPI_RES_OK

int mux_pin = A0;

scpi_result_t LTC2662_query_property(scpi_t * context) 
{
    dac_control_t* dac_control = (dac_control_t*) context->user_context;
    int32_t channel_no;
    LTC2662_Channel* cur_channel;
    SCPI_CommandNumbers(context,&channel_no,1);
    if (channel_no>=CHANNEL_COUNT)
        return SCPI_RES_ERR;
    cur_channel = &(dac_control->channels[channel_no]);
    switch(SCPI_CmdTag(context))
    {
        case TAG_REG_ENABLE:
            SCPI_ResultInt(context,cur_channel->isOn()); 
            break;
        case TAG_REG_VALUE:
            SCPI_ResultDouble(context,cur_channel->getCurrent()); 
            break;
        case TAG_REG_MODE:
            SCPI_ResultInt(context,dac_control->modes[channel_no]);  //TODO
            break;
        case TAG_REG_RANGE:
            SCPI_ResultDouble(context,cur_channel->getRange());
            break;
        default:
            return SCPI_RES_ERR;
            break; // should through error here
    };
    return SCPI_RES_OK;
}
scpi_result_t LTC2662_set_property(scpi_t * context) {
    dac_control_t* dac_control = (dac_control_t*) context->user_context;
    int32_t channel_no;
    double d;
    int32_t i;
    LTC2662_Channel* cur_channel;
    SCPI_CommandNumbers(context,&channel_no,1);
    
    if (channel_no>=CHANNEL_COUNT)
        return SCPI_RES_ERR;
    cur_channel = &(dac_control->channels[channel_no]);
    switch(SCPI_CmdTag(context))
    {
        case TAG_REG_RANGE:
        case TAG_REG_MODE:
        case TAG_REG_ENABLE:
            if (SCPI_ParamInt(context,&i,true))
            {
                switch(SCPI_CmdTag(context))
                {
                    case TAG_REG_RANGE:
                        cur_channel->writeRange(i);
                        break;
                    case TAG_REG_MODE:
                        if((i<0)||(i>1))
                            return SCPI_RES_ERR;
                        dac_control->modes[channel_no] = i;
                        if (i == MODE_SWEEP)
                            cur_channel->writeCurrent(dac_control->sweeps[channel_no].get_value());
                        break;
                    case TAG_REG_ENABLE:
                        break;
                }
            }
            else
                return SCPI_RES_ERR;
            break;
        case TAG_REG_VALUE:
            SCPI_ParamDouble(context,&d,1); 
            cur_channel->writeCurrent(d);
            break;
        default:
            return SCPI_RES_ERR;
    };
    return SCPI_RES_OK;
    
    
}
scpi_result_t MUX_query(scpi_t * context) {
    dac_control_t* dac_control = (dac_control_t*) context->user_context;
    int32_t mux_value,mux_channel;
    SCPI_CommandNumbers(context,&mux_channel,1);
    dac_control->channels[0].writeMux(mux_channel);
    delay(1);
    mux_value = analogRead(mux_pin);
    SCPI_ResultInt(context,mux_value);
    return SCPI_RES_OK;   
}


scpi_result_t SWEEP_query_property(scpi_t * context) {
    dac_control_t* dac_control = (dac_control_t*) context->user_context;
    int32_t channel_no;
    Sweep* cur_sweep;
    SCPI_CommandNumbers(context,&channel_no,1);
    if (channel_no>=CHANNEL_COUNT)
        return SCPI_RES_ERR;
    cur_sweep = &(dac_control->sweeps[channel_no]);
    switch(SCPI_CmdTag(context))
    {
        case TAG_SWEEP_START:
            SCPI_ResultDouble(context,cur_sweep->get_start()); 
            break;
        case TAG_SWEEP_STEP:
            SCPI_ResultDouble(context,cur_sweep->get_step()); 
            break;
        case TAG_SWEEP_IDX:
            SCPI_ResultInt(context,cur_sweep->get_step_idx()); 
            break;
        case TAG_SWEEP_COUNT:
            SCPI_ResultInt(context,cur_sweep->get_step_count()); 
            break;
        case TAG_SWEEP_PSC:
            SCPI_ResultInt(context,cur_sweep->get_prescale_counter()); 
            break;
        case TAG_SWEEP_PSC_VAL :
            SCPI_ResultInt(context,cur_sweep->get_prescale()); 
            break;
        default:
            return SCPI_RES_ERR;
    };
    return SCPI_RES_OK;
}
scpi_result_t SWEEP_set_property(scpi_t * context) {
    dac_control_t* dac_control = (dac_control_t*) context->user_context;
    int32_t channel_no;
    double d;
    int32_t i;
    uint32_t ui;
    float old_val,new_val;
    uint32_t tag = SCPI_CmdTag(context);
    Sweep* cur_sweep;
    SCPI_CommandNumbers(context,&channel_no,1);
    if (channel_no>=CHANNEL_COUNT)
        return SCPI_RES_ERR;
    cur_sweep = &(dac_control->sweeps[channel_no]);
    old_val = cur_sweep->get_value();

    switch(tag)
    {
        case TAG_SWEEP_START:
        case TAG_SWEEP_STEP:
            if (SCPI_ParamDouble(context,&d,true))
            {
                if (tag==TAG_SWEEP_START)
                    cur_sweep->set_start(d);
                else //TAG_SWEEP_STEP
                    cur_sweep->set_step(d);
            }
            else
            {
                return SCPI_RES_ERR;
            }
            break;
        case TAG_SWEEP_IDX:
        case TAG_SWEEP_COUNT:
        case TAG_SWEEP_PSC:
            if (SCPI_ParamInt(context,&i,true))
            {
                ui = i<0? 0 : (uint32_t) i;
                switch (tag)
                {
                case TAG_SWEEP_IDX:
                    cur_sweep->set_step_idx(ui);
                    break;
                case TAG_SWEEP_COUNT:
                    cur_sweep->set_step_count(ui);
                    break;
                case TAG_SWEEP_PSC:
                    cur_sweep->set_prescale(ui);
                    break;
                default:
                    break;
                }
            }
            else
                return SCPI_RES_ERR;
            break;
        default:
            return SCPI_RES_ERR;
    };
    new_val = cur_sweep->get_value();
    if ((old_val!=new_val) && (dac_control->modes[channel_no]==MODE_SWEEP))
    {
        dac_control->channels[channel_no].writeCurrent(new_val);
    }
    return SCPI_RES_OK;
}

extern const scpi_command_t scpi_commands[] =  {
	{ .pattern = "*IDN?", .callback = SCPI_CoreIdnQ,},
	{ .pattern = "*RST", .callback = SCPI_CoreRst,},
    //LTC functions
    { .pattern = "STATus#",  .callback = LTC2662_set_property,  .tag=TAG_REG_ENABLE},
    { .pattern = "STATus#?", .callback = LTC2662_query_property,.tag=TAG_REG_ENABLE},
    { .pattern = "CURrent#",   .callback = LTC2662_set_property,  .tag=TAG_REG_VALUE},
    { .pattern = "CURrent#?",  .callback = LTC2662_query_property,.tag=TAG_REG_VALUE},
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
    //{ .pattern = "SWEEP#:VALUE?", .callback = SWEEP_query_property,   .tag=TAG_SWEEP_PSC_VAL},
    { .pattern = "MUXread#?",.callback = MUX_query},
    //{ .pattern = "LIST", .callback = LTC2662_Range,},

	//{ .pattern = "MEASure:VOLTage:DC?", .callback = DMM_MeasureVoltageDcQ,},
	SCPI_CMD_LIST_END
};