#include <SPI.h>
#include <EEPROM.h>  
#include <avr/io.h>
#include <avr/interrupt.h>
#include "scpi/scpi.h"
#include <avr/pgmspace.h>

size_t myWrite(scpi_t * context, const char * data, size_t len) {
    (void) context;
    return Serial.write(data,len);
}
#define SCPI_INPUT_BUFFER_LENGTH 100
char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
#define SCPI_ERROR_QUEUE_SIZE 5 
int16_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];
// const scpi_command_t scpi_commands[] PROGMEM = 
scpi_command_t scpi_commands[] =  {
	{ .pattern = "*IDN?", .callback = SCPI_CoreIdnQ,},
	{ .pattern = "*RST", .callback = SCPI_CoreRst,},
	//{ .pattern = "MEASure:VOLTage:DC?", .callback = DMM_MeasureVoltageDcQ,},
	SCPI_CMD_LIST_END
};
scpi_interface_t scpi_interface = {0};
scpi_reg_val_t scpi_regs ={0};
scpi_t scpi_context = {0};
const char* idns[4] = {"LPI", "LTC2662", "0", "00"};

void setup() {
    scpi_context.cmdlist = scpi_commands;
    scpi_context.param_list = {0};
    scpi_context.interface = &scpi_interface;
    scpi_context.output_count = 0;
    scpi_context.input_count = 0;
    scpi_context.cmd_error = false;
    scpi_context.error_queue = NULL;
    scpi_context.registers = &scpi_regs;
    scpi_context.units = scpi_units_def;
    scpi_context.user_context = NULL;
    scpi_context.buffer.length  =SCPI_INPUT_BUFFER_LENGTH;
    scpi_context.buffer.data  =scpi_input_buffer;
    scpi_context.buffer.position  =0;
    scpi_interface.write = myWrite;
    scpi_context.idn[0] = idns[0];
    scpi_context.idn[1] = idns[1];
    scpi_context.idn[2] = idns[2];
    scpi_context.idn[3] = idns[3];
    Serial.begin(115200); // baud rate 
    SCPI_Init(&scpi_context);
}
char smbuffer[16];
void loop() {
    SCPI_Input(&scpi_context, 
                smbuffer, 
                Serial.readBytes(smbuffer,sizeof(smbuffer)));

}
