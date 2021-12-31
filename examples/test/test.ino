// Define Ovoo Libraries
#include "Telit_xE910.h"

void setup() {

	// ********** PORT J **********
	DDRJ	&= 0b11111011;	PORTJ	|= 0b00000100;	// PJ2 - Input / Pull Down [GSM Ring]
	DDRJ	&= 0b11110111;	PORTJ	&= 0b11110111;	// PJ3 - Input / Pull Down [GSM Power Monitor]
	DDRJ	|= 0b00010000;	PORTJ	|= 0b00010000;	// PJ4 - Output / Pull Down [GSM Communication Enable]
	DDRJ	|= 0b00100000;	PORTJ	&= 0b11011111;	// PJ5 - Output / Pull Down [GSM Shut Down]
	DDRJ	|= 0b01000000;	PORTJ	&= 0b10111111;	// PJ6 - Output / Pull Down [GSM On Off]
	DDRJ	|= 0b10000000;	PORTJ	&= 0b01111111;	// PJ7 - Output / Pull Down [NC]


    Serial.begin(115200);

    GSM.Power_ON(false, false, true);
    GSM.Initialize(true);

}

void loop() {

    //Serial.println(GSM_AT.IMEI);
    Serial.println(GSM_AT.Serial_Number);
    //Serial.println(GSM_AT.ICCID);
    //Serial.println(GSM_AT.Manufacturer);
    //Serial.println(GSM_AT.Model);
    //Serial.println(GSM_AT.Modem_Firmware);

    delay(1000);

}
