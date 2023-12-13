#include <I2C_16Bit.h>
#include <Wire.h>
#include <Servo.h>

#define I2C_ADDR 0x40
#define DATA_REGISTER_ADDR 0x5E

void setup(){
	servo_9.attach(9, 500, 2500); // Definindo o servo
	Serial.begin(9600);
	I2C_16Bit_begin();
}

void loop(){
	uint16_t Dado = I2C_16Bit_readFromModule(I2C_ADDR, DATA_REGISTER_ADDR);
	Serial.print("Resposta: ");
	Serial.println(Dado);
}
