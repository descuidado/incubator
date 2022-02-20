#include <EEPROM.h>
#include <Wire.h>
#include <Button.h>
#include "DFRobot_SHT20.h"

DFRobot_SHT20 sht20;

unsigned short int A = 20;
unsigned short int C = 21;
unsigned short int E = 22;
unsigned short int F = 23;
unsigned short int H = 24;
unsigned short int L = 25;
unsigned short int P = 26;
unsigned short int U = 27;
unsigned short int r = 28;

unsigned short int humidityPin = 4; //relay to turn on vaporizer it will increase humidity
unsigned short int elementPin = 5; //relay to control the elemnt to heat the enviroment so tempreture increase
unsigned short int timerPin = 6; // relay to turn on the motor that turns eggs
unsigned short int fanPin = 7; //relay to turn on the fan it will reduce the tempreture and humidity
unsigned short int d1 = 9; //first digit of 7 segment
unsigned short int d2 = 10; //second digit of 7 segment
unsigned short int d3 = 13; //third digit of 7 segment
Button upbtn(14);			//A0 //Button to add value or upward in menu //one side should be GND and other side should connect to micro
Button downbtn(15);		//A1 //button to reduce value or downward in menu //one side should be GND and other side should connect to micro
Button setbtn(16);		//A2 //button to set values and go to menu //one side should be GND and other side should connect to micro
//Button startTurn(2);
//Button stopTurn(3);
unsigned short int startTurn = 2;
unsigned short int stopTurn = 3;
unsigned short int buzzer = A3;		//A3
int latchPin = 8;		//Pin connected to ST_CP of 74HC595
int clockPin = 12;	//Pin connected to SH_CP of 74HC595
int dataPin = 11;		//Pin connected to DS of 74HC595

//these values will be set from eeprom
float humidityHigh = 70; //maximum humidity
float humidityIdeal = 55;
float humidityLow = 40; //minimum humidity
short int tempretureHigh = 379; //maximum tempreture
short int tempretureIdeal = 377;
short int tempretureLow = 375; //minimum tempreture
short int timerHour = 2; //time to wait for auto turn the eggs (hour)
short int timerMinutes = 50; //time to wait for auto turn the eggs (minute)
float humidity = 0;
float temperature = 0;

bool humidityHighBool = false;
bool humidityLowBool = false;
bool tempretureHighBool = false;
bool tempretureLowBool = false;
bool viewTemp = false;
unsigned long int mainDisplayMillis = 0;
unsigned long int sensorCHeckMillis = 0;
unsigned long int checkMillis = 0;

unsigned long int millisControl = 0; //sht20 not related to us
unsigned long int timeToTurnMillis = 20000; // Two minutes to turn the eggs motor
short int displayTimeWait = 3000; // time wait to display first run setup title
short int veiwDelayTime = 5; //delay to extend 74hc595 life span

unsigned long int lastTimeTurned = 0; //last time eggs turned to calculate what is next time
unsigned short int mod = 0; //control setiings display
bool shouldTurn = false;
bool loopControl = true;
bool firstStopPress = true;
//byte data;
//int dataArray[] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x1f, 0x70, 0x7f, 0x7b, 0x77, 0x4e, 0x4f, 0x47, 0x37, 0xe, 0x67, 0x3e};
short int dataArray[] = {	0b01111110, 0b00110000, 0b01101101,
                            0b01111001, 0b00110011, 0b01011011,
                            0b00011111, 0b01110000, 0b01111111,
                            0b01111011, 0b11111110, 0b10110000,
                            0b11101101, 0b11111001, 0b10110011,
                            0b11011011, 0b10011111, 0b11110000,
                            0b11111111, 0b11111011, 0b01110111,
                            0b01001110, 0b01001111, 0b01000111,
                            0b00110111, 0b00001110, 0b01100111,
                            0b00111110, 0b00000101,
                        }; //array to on and off leds based on 74hc595 common kathod 7segment
float humidityArray[] = { 55, 55, 55, 55, 55, 55};
short int tempretureArray[] = { 377, 377, 377, 377, 377, 377};
short int place = 0;
void setup() {

	pinMode(humidityPin, OUTPUT);
	pinMode(elementPin, OUTPUT);
	pinMode(timerPin, OUTPUT);
	pinMode(fanPin, OUTPUT);
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	pinMode(d1, OUTPUT);
	pinMode(d2, OUTPUT);
	pinMode(d3, OUTPUT);

	upbtn.begin();
	downbtn.begin();
	setbtn.begin();
	//startTurn.begin();
	pinMode(startTurn, INPUT_PULLUP);
	//stopTurn.begin();
	pinMode(stopTurn, INPUT_PULLUP);

	sht20.initSHT20();	// Init SHT20 Sensor
	delay(100);
	sht20.checkSHT20();
	Serial.begin(9600);
	delay(3000);
	millisControl = millis();	// Check SHT20 Sensor

	if (EEPROM.read(0) != 1) {
		esave();
	} else {
		eread();
	}
}

void loop() {
	//get values
	if ((millis() - sensorCHeckMillis) > 2000) {
		humidityArray[place] = sht20.readHumidity();                  // Read Humidity
		humidity = 0;
		for (short int i = 0; i < 6; i++) {
			humidity = humidity + humidityArray[i];
		}
		humidity = humidity / 6;

		tempretureArray[place] = int(sht20.readTemperature() * 10);             // Read Temperature
		temperature = 0;
		for (short int i = 0; i < 6; i++) {
			temperature = temperature + tempretureArray[i];
		}
		temperature = temperature / 6;
		sensorCHeckMillis = millis();
		place++;
		if (place >= 6) {
			place = 0;
		}
	}

	if (temperature > 2000 || humidity > 100 ) {
		tone(buzzer, 5000);
		while (true) {
			view3(E, r, r, false);
		}
	}
	
	//settings menu
	if ( setbtn.pressed() ) {
		settings();
	}

	if (digitalRead(stopTurn) == LOW ) {
		shouldTurn = false;
		if (firstStopPress) {
			lastTimeTurned = millis();
			firstStopPress = false;
		}
	}
	if (digitalRead(startTurn) == LOW) {
		shouldTurn = true;
		firstStopPress = true;
	}
	if (millis() < 1555200000) {
		unsigned long int hourMillis = timerHour * 3600000;
		unsigned long int minutesMillis = timerMinutes * 60000;
		checkMillis = minutesMillis + hourMillis;
		if ((millis() - lastTimeTurned) > checkMillis) {
			shouldTurn = true;
		}
	}

	if (shouldTurn) {
		digitalWrite(timerPin, HIGH);
	} else {
		digitalWrite(timerPin, LOW);
	}

	onOff();


	mainDisplay();
}
