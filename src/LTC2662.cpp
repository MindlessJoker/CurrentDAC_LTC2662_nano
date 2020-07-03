#include <SPI.h>
#include <EEPROM.h>  
#include <avr/io.h>
#include <avr/interrupt.h>

void handleFaultReg(byte fr){
    if (fr){
        Serial.print("Fault Register = ");
        Serial.println(fr,BIN);
        }
}

byte fault_reg, comm_and_addr,val1, val0; // bytes to read from spi
byte old_comm_and_addr; // bytes which have been sent to SPI before

void writeSPI32(byte command, byte address, unsigned int value) {
    
  
}


class Channel{
    byte channel;
    unsigned long range; // im uamps
    byte sign; // 0 - "+" polarity, 1 - "-" polarity
    float current;
    byte on;
    byte pol_pin;
    bool pol_changed = false;
    float mux_current;
public:
    Channel(byte a_channel=0,unsigned long a_range=300,byte a_sign=0,float a_current=0,byte a_on=1, byte a_pol_pin=8){
        channel = a_channel;
        range = a_range;
        sign = a_sign;
        current = a_current;
        on = a_on;
        pol_pin = a_pol_pin;
    }
    
    void setRange(unsigned long new_range){
        range = new_range;
        if (current*1000 > range){
        current = float(range)/1000;
        }
    }
    
    void setCurrent(float new_current){
        //Serial.print("New current ");
        //Serial.println(new_current);
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
        if (abs(new_current)*1000 > range){
            current = float(range)/1000;
        }
        else{
            current = abs(new_current);
        }
        on = 1;
        }
    }
    
    bool isOn(){
        return on;
    } 
    
    byte getRangeCode(){
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
    
    float getCurrent(){
        return current;
    }
    
    unsigned int getCurrentCode(){
        return (unsigned int)((current*1000/range)*65535);
    }
    
    byte getPolPin(){
        return pol_pin;
    }
    
    void writeRange(unsigned long new_range){
        //Serial.println(String("Channel range set" + String(channel) + String(range)));
        setRange(new_range);
        writeSPI32(B0110,channel,getRangeCode());
        writeCurrent(current);
        }
    
    void writeCurrent(float new_current){
        //Serial.print("current ");Serial.println(current);
        setCurrent(new_current);
        if ((on==0) || (current==0)){
            writeSPI32(B0100,channel,0); // power down channel
            digitalWrite(pol_pin,sign);
                pol_changed = false;
            return;
        }
        else{
            if (pol_changed){
                writeSPI32(B0110,channel,getRangeCode());
                digitalWrite(pol_pin,sign);
                pol_changed = false;
            }
            writeSPI32(B0,channel,getCurrentCode()); // write current to register
        }
        update(); // update channel's output
    }
    
    void update(){
        writeSPI32(B1,channel,0);
    }
    
    String checkCurrent(int value){
        mux_current = 1.1*(1.0*range/1000) * (5.0*value/1023 ) / 1.25;
        //Serial.print("Mux current ");
        //Serial.println(mux_current);
        if (abs(mux_current - current)/(current+1) < 0.2){
//            return String("Ok Ch" + String(channel) + ", Is=" + String(current) + ", Ir=" + String(mux_current));
            return String(" " + String(channel) + "," + String(current,1) + "," + String(mux_current,0)) + ",";
        }
        else{
//            return String("Bd Ch" + String(channel) + ", Is=" + String(current) + ", Ir=" + String(mux_current));
            return String(" " + String(channel) + "," + String(current,1) + "," + String(mux_current,0)) + ",";
        }
        
    }
    
    String str(){
        return String("range=" + String(range) + "uA, sing=" + String(sign) + 
                      ",current=" + String(current) + "mA, on=" + String(on) +
                      ",pol_pin=" + String(pol_pin));
    }
    
};