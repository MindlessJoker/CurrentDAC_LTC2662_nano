#ifndef DAC_CONTROL_H
#define DAC_CONTROL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "LTC2662.hpp"
#define CHANNEL_COUNT 5
struct dac_control_struct_t_ {
    LTC2662_Channel channels[CHANNEL_COUNT];
}  ;
typedef struct dac_control_struct_t_ dac_control_t;

#ifdef  __cplusplus
}
#endif

#endif