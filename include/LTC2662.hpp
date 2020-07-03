#include <SPI.h>
#include <EEPROM.h>  
#include <avr/io.h>
#include <avr/interrupt.h>


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
    Channel(byte a_channel=0,unsigned long a_range=300,byte a_sign=0,float a_current=0,byte a_on=1, byte a_pol_pin=8);
    void setRange(unsigned long new_range);
    
    void setCurrent(float new_current);
    
    bool isOn();
    byte getRangeCode();
    
    float getCurrent();
    
    unsigned int getCurrentCode();
    
    byte getPolPin();
    
    void writeRange(unsigned long new_range);
    
    void writeCurrent(float new_current);
    
    void update();
    
    String checkCurrent(int value);
    
    String str();
};