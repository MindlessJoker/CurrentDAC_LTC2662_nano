#include <SPI.h>
#include <EEPROM.h>  
#include <avr/io.h>
#include <avr/interrupt.h>
#include "LTC2662.hpp"

extern void writeSPI32(uint8_t command, uint8_t address, unsigned int value) ;

uint8_t LTC2662_Channel::global_toggle_register = 0;

void LTC2662_Channel::setRange(unsigned long new_range){
    range = new_range;
    if (current*1000 > range){
        current = float(range)/1000;
    }
}
void LTC2662_Channel::writeMux(int channel)
{
    writeSPI32(LTC2662_CMD_MUX,0,channel&(B11111));
}
void LTC2662_Channel::prepareCurrentChange(float new_current){
    uint8_t new_sign =new_current<0 ? 1: 0;
    if (new_current==0){
        sign = 0;
        current = 0;
        on=0;
    }
    else{
        if ( new_sign != sign){
            pol_changed=true;
            sign = new_sign;
        }
        if (abs(new_current)*1000 > range)
        {
            current = float(range)/1000;
            if(sign)
                current = -current;
        }
        else
            current = new_current;
        on = 1;
    }
}
    
bool LTC2662_Channel::isOn(){
    return on;
} 
    
uint8_t LTC2662_Channel::getRangeCode(){
    switch (range){
        case 3125:
            return B1;
        case 6250:
            return B10;
        case 12500:
            return B11;
        case 25000:
            return B100;
        case 50000:
            return B101;
        case 100000:
            return B110;
        case 200000:
            return B111;
        case 300000:
            return B1111;
        default:
            return B110;
            
    }
}
    
float LTC2662_Channel::getCurrent(){
    return current;
}
    
    
uint8_t LTC2662_Channel::getPolPin(){
    return pol_pin;
}
    
void LTC2662_Channel::writeRange(unsigned long new_range){
    //Serial.println(String("Channel range set" + String(channel) + String(range)));
    setRange(new_range);
    writeSPI32(LTC2662_CMD_RANGE,channel,getRangeCode());
    setCurrent(current);
    }

void LTC2662_Channel::setCurrent(float new_current){
    //Serial.print("current ");Serial.println(current);
    prepareCurrentChange(new_current);
    if ((on==0) || (current==0)){
        writeSPI32(LTC2662_CMD_POWER_DOWN,channel,0); // power down channel
        digitalWrite(pol_pin,sign);
            pol_changed = false;
        return;
    }
    else{
        if (pol_changed){
            writeSPI32(LTC2662_CMD_RANGE,channel,0); // high-z
            delay(pol_changed_delay0);
            digitalWrite(pol_pin,sign);
            delay(pol_changed_delay1);
            writeSPI32(LTC2662_CMD_RANGE,channel,getRangeCode());
            pol_changed = false;
        }
        writeCurrentRegisterAB(abs(current), false); // write current to register
        update(); // update channel's output
    }
}
    
void LTC2662_Channel::update(){
    writeSPI32(LTC2662_CMD_UPDATE,channel,0);
}

void LTC2662_Channel::setCurrentB(float new_current)
{
    if (abs(new_current)*1000 > range)
        currentB = float(range)/1000;
    else
        currentB = abs(new_current);
    writeCurrentRegisterAB(currentB,true);
}
void LTC2662_Channel::setToggle(bool toggle)
{
    uint8_t not_channel_mask = ~(1<<channel);
    uint8_t new_toggle = ((toggle? 1 : 0)<<channel);
    writeToggleRegister( (not_channel_mask & getToggleRegister()) | new_toggle );
}

void LTC2662_Channel::writeToggleRegister(uint8_t toggle_reg)
{
    toggle_reg = toggle_reg & 0x1F;
    *(p_toggle_register) = toggle_reg;
    writeSPI32(LTC2662_CMD_TOGGLE_SEL,0 ,(unsigned int) toggle_reg);
}

void LTC2662_Channel::writeCurrentRegisterAB(float current_, bool is_B)
{
    is_B = is_B ? 1 : 0;
    bool toggle = getToggle();
    bool need_toggle = (toggle ^ is_B);

    
    uint16_t current_int = floatToCurrent(current_);
    if (need_toggle)
        setToggle(is_B);
    //Write current
    Serial.println(current_int);
    writeSPI32(LTC2662_CMD_WRITE,channel,current_int);
    if (need_toggle)
        setToggle(toggle);
}