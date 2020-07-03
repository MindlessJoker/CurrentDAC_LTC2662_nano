
#ifndef LTC2662_H
#define LTC2662_H

class LTC2662_Channel{
    uint8_t channel;
    unsigned long range; // im uamps
    uint8_t sign; // 0 - "+" polarity, 1 - "-" polarity
    float current;
    uint8_t on;
    uint8_t pol_pin;
    bool pol_changed = false;
    float mux_current;
    uint8_t slaveSelectPin;
public:
    LTC2662_Channel(uint8_t a_channel=0,unsigned long a_range=300,uint8_t a_sign=0,float a_current=0,uint8_t a_on=1, uint8_t a_pol_pin=8,uint8_t slaveSelectPin=10)
    {
        channel = a_channel;
        range = a_range;
        sign = a_sign;
        current = a_current;
        on = a_on;
        pol_pin = a_pol_pin;
        slaveSelectPin = slaveSelectPin;
    }
    void writeSPI32(uint8_t command, uint8_t address, unsigned int value);
    void setRange(unsigned long new_range);
    long getRange(){return range;};
    void setCurrent(float new_current);
    bool isOn();
    uint8_t getRangeCode();
    float getCurrent();
    unsigned int getCurrentCode();
    uint8_t getPolPin();
    void writeRange(unsigned long new_range);
    void writeCurrent(float new_current);
    void update();
};

#endif