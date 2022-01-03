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

    GSM.Initialize(true);


    Serial.println("--------------------------------------------------");
    Serial.print("Manufacturer      : "); if (GSM_AT.Manufacturer == 1) Serial.println("Telit");
    Serial.print("Model             : "); if (GSM_AT.Model == 1) Serial.println("GE910 QUAD");
    Serial.print("Firmware          : "); Serial.println(GSM_AT.Modem_Firmware);
    Serial.print("IMEI Number       : "); Serial.println(GSM_AT.IMEI);
    Serial.print("Serial Number     : "); Serial.println(GSM_AT.Serial_Number);
    Serial.print("ICCID             : "); Serial.println(GSM_AT.ICCID);
    Serial.println("--------------------------------------------------");


    GSM.Connect();

}

void loop() {



    delay(1000);

}
