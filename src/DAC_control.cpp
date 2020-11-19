#include <SPI.h>
#include <EEPROM.h>  
#include <avr/io.h>
#include <avr/interrupt.h>
#include "scpi/scpi.h"
#include <avr/pgmspace.h>
#include "DAC_control.hpp"
#include "LTC2662.hpp"

unsigned long ms_timer = 0;
unsigned long next_ms_time = 0;
unsigned long mux_last_check_time = 0;
const int slaveSelectPin = 10;
bool timeout = false;
bool check_mux = false;
void startTimer(){
    cli();// отключить глобальные прерывания    
    TCCR1A = 0; // установить регистры в 0
    TCCR1B = 0;
    TCCR1B |= (1 << WGM12); // включение в CTC режим
    TCCR1B |= (1 << CS10);
    OCR1A = 16000; // установка таймера на 1 мс
    TIMSK1 |= (1 << OCIE1A);  // включение прерываний по совпадению
    sei();
    ms_timer = 0;
//    Serial.println("Timer started");
}
void stopTimer(){
    TCCR1A = 0;
    TCCR1B = 0; // stop timer
}
ISR(TIMER1_COMPA_vect){
    ms_timer++;
    mux_last_check_time++;
//    if (ms_timer%100 == 0){
//      Serial.println(ms_timer);
//    }
    if (ms_timer == next_ms_time){
//        Serial.print("IN MSTIMER ");
//        Serial.println(ms_timer);
        timeout = true;
    }
    if (mux_last_check_time > 2000){
        check_mux = true;
    }
}

//#define TAG_REG_ENABLE 0
void writeSPI32(uint8_t command, uint8_t address, unsigned int value) {
    digitalWrite(slaveSelectPin, LOW);
    SPI.transfer(0); //fault_reg
    SPI.transfer((command&(0xF0)) | (address&(0x0F))); //comm_and_addr
    SPI.transfer(value>>8); //val1
    SPI.transfer(value); //val0
    digitalWrite(slaveSelectPin, HIGH);
}

extern const scpi_command_t scpi_commands[];
size_t myWrite(scpi_t * context, const char * data, size_t len) {
    (void) context;
    return Serial.write(data,len);
}
#define SCPI_INPUT_BUFFER_LENGTH 100
char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
#define SCPI_ERROR_QUEUE_SIZE 5 
int16_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];
// const scpi_command_t scpi_commands[] PROGMEM = 

scpi_interface_t scpi_interface = {0};
scpi_t scpi_context = {0};
const char* idns[4] = {"LPI", "CurrentDAC_LTC2662_nano", "0", "00"};




dac_control_t dac_control = {
    .channels = {
        LTC2662_Channel(0,100000,0,0,0,8-0,slaveSelectPin),
        LTC2662_Channel(1,100000,0,0,0,8-1,slaveSelectPin),
        LTC2662_Channel(2,100000,0,0,0,8-2,slaveSelectPin),
        LTC2662_Channel(3,100000,0,0,0,8-3,slaveSelectPin),
        LTC2662_Channel(4,100000,0,0,0,8-4,slaveSelectPin)
    },
    .sweeps = {
        Sweep(),
        Sweep(),
        Sweep(),
        Sweep(),
        Sweep()
    },
    .modes = {0,0,0,0,0}
};
void trigger0_handler(){
    for(uint32_t i =0; i<CHANNEL_COUNT;++i)
    {
        if(dac_control.modes[i]==MODE_SWEEP)
        {
            if (dac_control.sweeps[i].sweep_step())
                dac_control.channels[i].setCurrent(dac_control.sweeps[i].get_value());
        }
    }
}


void setup() {
    scpi_context.cmdlist = scpi_commands;
    scpi_context.param_list = {0};
    scpi_context.interface = &scpi_interface;
    scpi_context.output_count = 0;
    scpi_context.input_count = 0;
    scpi_context.cmd_error = false;
    scpi_context.error_queue = NULL;
    scpi_context.registers = NULL;
    scpi_context.units = scpi_units_def;
    scpi_context.user_context = (void*) &dac_control;
    scpi_context.buffer.length  =SCPI_INPUT_BUFFER_LENGTH;
    scpi_context.buffer.data  =scpi_input_buffer;
    scpi_context.buffer.position  = 0;
    scpi_context.idn[0] = idns[0];
    scpi_context.idn[1] = idns[1];
    scpi_context.idn[2] = idns[2];
    scpi_context.idn[3] = idns[3];
    scpi_interface.write = myWrite;

    Serial.begin(115200); // baud rate 
    SCPI_Init(&scpi_context);
    startTimer();
    pinMode (slaveSelectPin, OUTPUT);
    SPI.begin(); // initialize SPI:
    attachInterrupt(0,trigger0_handler,RISING);//connect trigger on pin 2
    for (byte i=0; i<CHANNEL_COUNT; ++i)
    {
        pinMode(8-i,OUTPUT);
        dac_control.channels[i].writeRange(100000);
        dac_control.channels[i].setCurrent(0);
    }
}
char smbuffer[16];
int read_num = 0;
int time_to_process;
void loop() {
    read_num = Serial.readBytes(smbuffer,sizeof(smbuffer));
    if (read_num)
    {
        SCPI_Input(&scpi_context, 
                    smbuffer, 
                    read_num);
        time_to_process = ms_timer-time_to_process;
    }

}

