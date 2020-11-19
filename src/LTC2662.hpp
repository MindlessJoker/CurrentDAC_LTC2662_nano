
#ifndef LTC2662_H
#define LTC2662_H

#include <stdint.h>
using namespace std;

#define  LTC2662_CMD_WRITE                0x00  //!< Write to input register n
#define  LTC2662_CMD_UPDATE               0x10  //!< Update (power up) DAC register n
#define  LTC2662_CMD_WRITE_UPDATE_ALL     0x20  //!< Write to input register n, update (power-up) all
#define  LTC2662_CMD_WRITE_UPDATE         0x30  //!< Write to input register n, update (power-up) 
#define  LTC2662_CMD_POWER_DOWN           0x40  //!< Power down n
#define  LTC2662_CMD_POWER_DOWN_ALL       0x50  //!< Power down chip (all DAC's, MUX and reference)

#define  LTC2662_CMD_RANGE                0x60  //!< Write span to dac n
#define  LTC2662_CMD_CONFIG               0x70  //!< Configure reference / toggle
#define  LTC2662_CMD_WRITE_ALL            0x80  //!< Write to all input registers
#define  LTC2662_CMD_UPDATE_ALL           0x90  //!< Update all DACs
#define  LTC2662_CMD_WRITE_ALL_UPDATE_ALL 0xA0  //!< Write to all input reg, update all DACs
#define  LTC2662_CMD_MUX                  0xB0  //!< Select MUX channel (controlled by 5 LSbs in data word)
#define  LTC2662_CMD_TOGGLE_SEL           0xC0  //!< Select which DACs can be toggled (via toggle pin or global toggle bit)
#define  LTC2662_CMD_GLOBAL_TOGGLE        0xD0  //!< Software toggle control via global toggle bit
#define  LTC2662_CMD_RANGE_ALL            0xE0  //!< Set span for all DACs
#define  LTC2662_CMD_NO_OPERATION         0xF0  //!< No operation

#define  LTC2662_RANGE_HI_Z                0x0000
#define  LTC2662_RANGE_3_125MA             0x0001
#define  LTC2662_RANGE_6_25MA              0x0002
#define  LTC2662_RANGE_12_5MA              0x0003
#define  LTC2662_RANGE_25MA                0x0004
#define  LTC2662_RANGE_50MA                0x0005
#define  LTC2662_RANGE_100MA               0x0006
#define  LTC2662_RANGE_200MA               0x0007
#define  LTC2662_RANGE_SW_TO_NEG_V         0x0008
#define  LTC2662_RANGE_300MA               0x0009

#define  LTC2662_MUX_HIZ            0x0000
#define  LTC2662_MUX_CURRENT0       0x0001
#define  LTC2662_MUX_CURRENT1       0x0002
#define  LTC2662_MUX_CURRENT2       0x0003
#define  LTC2662_MUX_CURRENT3       0x0004
#define  LTC2662_MUX_CURRENT4       0x0005
#define  LTC2662_MUX_VCC            0x0006
#define  LTC2662_MUX_VREF           0x0008
#define  LTC2662_MUX_VREFLO         0x0009
#define  LTC2662_MUX_TEMPERATURE    0x000A
#define  LTC2662_MUX_VDD0           0x0010
#define  LTC2662_MUX_VDD1           0x0011
#define  LTC2662_MUX_VDD2           0x0012
#define  LTC2662_MUX_VDD3           0x0013
#define  LTC2662_MUX_VDD4           0x0014
#define  LTC2662_MUX_VP             0x0015
#define  LTC2662_MUX_VM             0x0016
#define  LTC2662_MUX_GND            0x0017
#define  LTC2662_MUX_VOLTAGE0      (0x0018+0)
#define  LTC2662_MUX_VOLTAGE1      (0x0018+1)
#define  LTC2662_MUX_VOLTAGE2      (0x0018+2)
#define  LTC2662_MUX_VOLTAGE3      (0x0018+3)
#define  LTC2662_MUX_VOLTAGE4      (0x0018+4)




const float LTC2662_MIN_OUTPUT[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
const float LTC2662_MAX_OUTPUT[10] = {0.0, 3.125, 6.25, 12.5, 25.0, 50.0, 100.0, 200.0, 0, 300.0};

#define  LTC2662_REF_DISABLE                 0x0001  //! Disable internal reference to save power when using an ext. ref.
#define  LTC2662_THERMAL_SHUTDOWN            0x0002  //! Disable thermal shutdown (NOT RECOMMENDED)
#define  LTC2662_POWERLIMIT_PROTECTION       0x0003  //! Disable power limit protection (NOT RECOMMENDED)
#define  LTC2662_OPEN_CIRCUIT_DETECT_DISABLE 0x0004  //! Disable open circuit detection


class LTC2662_Channel{
    static uint8_t global_toggle_register;
    uint8_t channel;
    unsigned long range; // im uamps
    uint8_t sign; // 0 - "+" polarity, 1 - "-" polarity
    float current;
    float currentB; //current value for B register
    uint8_t on;
    uint8_t pol_pin;
    bool pol_changed = false;
    uint32_t pol_changed_delay0 = 1; // swich current off
    uint32_t pol_changed_delay1 = 5; // relay off
    float mux_current;
    uint8_t slaveSelectPin;
    uint8_t* p_toggle_register;
public:
    LTC2662_Channel(uint8_t a_channel=0,       unsigned long a_range=300,
                    uint8_t a_sign=0,          float         a_current=0,
                    uint8_t a_on=1,            uint8_t       a_pol_pin=8,
                    uint8_t slaveSelectPin=10, uint8_t *a_p_toggle_register = NULL)
    {
        channel = a_channel;
        range = a_range;
        sign = a_sign;
        current = a_current;
        on = a_on;
        pol_pin = a_pol_pin;
        slaveSelectPin = slaveSelectPin;
        currentB = 0;
        if (a_p_toggle_register!=NULL)
            p_toggle_register = a_p_toggle_register;
        else
            p_toggle_register = &global_toggle_register;
    }
    unsigned int floatToCurrent(float current_){return (unsigned int)(abs(current_*1000/range)*65535);}
    // void writeSPI32(uint8_t command, uint8_t address, unsigned int value);
    void setRange(unsigned long new_range);
    long getRange(){return range;};
    void setCurrent(float new_current);
    float getCurrent();
    
    void setCurrentB(float new_current);
    float getCurrentB() {return currentB;}

    void setToggle(bool toggle);
    bool getToggle() {return (((1<<channel) & getToggleRegister())>>channel);}
    void writeMux(int channel);
    bool isOn();
    uint8_t getRangeCode();

    uint8_t getPolPin();
    void writeRange(unsigned long new_range);
    
    void update();
private:
    uint8_t getToggleRegister(){
        return *p_toggle_register;}
    void writeToggleRegister(uint8_t toggle_reg);
    void prepareCurrentChange(float new_current);
    void writeCurrentRegisterAB(float current, bool is_B=false);
};

#endif