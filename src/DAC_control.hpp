#ifndef DAC_CONTROL_H
#define DAC_CONTROL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "LTC2662.hpp"
#include "Sweep.hpp"
#define CHANNEL_COUNT 5
#define MODE_NORMAL 0
#define MODE_SWEEP  1
struct dac_control_struct_t_ {
    LTC2662_Channel channels[CHANNEL_COUNT];
    Sweep sweeps[CHANNEL_COUNT];
    uint32_t modes[CHANNEL_COUNT];
}  ;
typedef struct dac_control_struct_t_ dac_control_t;

#ifdef  __cplusplus
}
#endif

#endif