#include <SPI.h>
#include <EEPROM.h>  
#include <avr/io.h>
#include <avr/interrupt.h>
#include <scpi/scpi.h>

scpi_command_t scpi_commands[] = {
	{ .pattern = "*IDN?", .callback = SCPI_CoreIdnQ,},
	{ .pattern = "*RST", .callback = SCPI_CoreRst,},
	{ .pattern = "MEASure:VOLTage:DC?", .callback = DMM_MeasureVoltageDcQ,},
	SCPI_CMD_LIST_END
};
scpi_interface_t scpi_interface = {
	.write = myWrite,
	.error = NULL,
	.reset = NULL,
	.srq = NULL,
};
size_t myWrite(scpi_context_t * context, const char * data, size_t len) {
    (void) context;
    return Serial.write(data,len);
}
scpi_t scpi_context;
#define SCPI_INPUT_BUFFER_LENGTH 100
char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
#define SCPI_ERROR_QUEUE_SIZE 5 
int16_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];

void setup() {
    Serial.begin(115200); // baud rate 
    SCPI_Init(&scpi_context,
    scpi_commands,
    &scpi_interface,
    scpi_units_def,
    "LPI", "LTC2662", "0", "00",
    scpi_input_buffer, SCPI_INPUT_BUFFER_LENGTH,
    scpi_error_queue_data, SCPI_ERROR_QUEUE_SIZE);
}
char smbuffer[16];
void loop() {
    SCPI_Input(&scpi_context, 
                smbuffer, 
                Serial.readBytes(smbuffer,sizeof(smbuffer)));

}
