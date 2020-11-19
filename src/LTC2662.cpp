#include <SPI.h>
#include <EEPROM.h>  
#include <avr/io.h>
#include <avr/interrupt.h>
#include "LTC2662.hpp"

extern void writeSPI32(uint8_t command, uint8_t address, unsigned int value) ;

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
    if (new_current==0){
        sign = 0;
        current = 0;
        on=0;
    }
    else{
        if ((new_current>0)==sign){
            pol_changed=true;
            if (sign==0){
                sign =1;
            }
            else{
                sign =0;
            }
        }
        if (abs(new_current)*1000 > range)
            current = float(range)/1000;
        else
            current = abs(new_current);
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
    
unsigned int LTC2662_Channel::getCurrentCode(){
    return (unsigned int)((current*1000/range)*65535);
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
        else
        {
            writeSPI32(LTC2662_CMD_WRITE,channel,getCurrentCode()); // write current to register
        }
    }
    update(); // update channel's output
}
    
void LTC2662_Channel::update(){
    writeSPI32(LTC2662_CMD_UPDATE,channel,0);
}

void LTC2662_Channel::setCurrentB(float new_current)
{
    init8_t toggle = getToggle();
    setToggle(1);
    //Write current
    
    setToggle(toggle);
}
void LTC2662_Channel::setToggle()
{

}
uint8_t LTC2662_Channel::getToggleRegister();
void LTC2662_Channel::setToggleRegister(uint8_t toggle_reg);