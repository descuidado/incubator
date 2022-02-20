void esave() {
	//set Humidity High
	EEPROM.write(1, humidityHigh);
	EEPROM.write(2, humidityIdeal);
	EEPROM.write(3, humidityLow);
	EEPROM.write(4, tempretureHigh);
	EEPROM.write(5, tempretureIdeal);
	EEPROM.write(6, tempretureLow);
	EEPROM.write(7, timerHour);
	EEPROM.write(8, timerMinutes);

	EEPROM.write(0, 1);
}

void settings() {
	mod = 1;
	while (loopControl) {
		if (mod == 1) {
			view3(H, U, H, false);
			if ( setbtn.pressed() ) {
				humidityHigh = setNumber(0, 100, humidityHigh, false);
				EEPROM.write(1, humidityHigh);
				loopControl = false;
			} else if ( upbtn.pressed() ) {
				mod = 8;
			} else if ( downbtn.pressed() ) {
				mod = 2;
			}
		} else if (mod == 2) {
			view3(H, U, 1, false);
			if ( setbtn.pressed() ) {
				humidityIdeal = setNumber(humidityLow, humidityHigh, humidityIdeal, false);
				EEPROM.write(2, humidityIdeal);
				loopControl = false;
			} else if ( upbtn.pressed() ) {
				mod = 1;
			} else if ( downbtn.pressed() ) {
				mod = 3;
			}
		}	else if (mod == 3) {
			view3(H, U, L, false);
			if ( setbtn.pressed() ) {
				humidityLow = setNumber(0, humidityHigh, humidityLow, false);
				EEPROM.write(3, humidityLow);
				loopControl = false;
			} else if ( upbtn.pressed() ) {
				mod = 2;
			} else if ( downbtn.pressed() ) {
				mod = 4;
			}
		} else if (mod == 4) {
			view3(F, E, H, false);
			if ( setbtn.pressed() ) {
				tempretureHigh = setNumber(0, 1400, tempretureHigh, true);
				EEPROM.write(4, tempretureHigh);
				loopControl = false;
			} else if ( upbtn.pressed() ) {
				mod = 3;
			} else if ( downbtn.pressed() ) {
				mod = 5;
			}
		} else if (mod == 5) {
			view3(F, E, 1, false);
			if ( setbtn.pressed() ) {
				tempretureIdeal = setNumber(tempretureLow, tempretureHigh, tempretureIdeal, true);
				EEPROM.write(5, tempretureIdeal);
				loopControl = false;
			} else if ( upbtn.pressed() ) {
				mod = 4;
			} else if ( downbtn.pressed() ) {
				mod = 6;
			}
		} else if (mod == 6) {
			view3(F, E, L, false);
			if ( setbtn.pressed() ) {
				tempretureLow = setNumber(0, tempretureHigh, tempretureLow, true);
				EEPROM.write(6, tempretureLow);
				loopControl = false;
			} else if ( upbtn.pressed() ) {
				mod = 5;
			} else if ( downbtn.pressed() ) {
				mod = 7;
			}
		} else if (mod == 7) {
			view3(0, E, H, false);
			if ( setbtn.pressed() ) {
				timerHour = setNumber(0, 23, timerHour, false);
				EEPROM.write(7, timerHour);
				loopControl = false;
			} else if ( upbtn.pressed() ) {
				mod = 6;
			} else if ( downbtn.pressed() ) {
				mod = 8;
			}
		} else if (mod == 8) {
			view3(0, E, L, false);
			if ( setbtn.pressed() ) {
				timerMinutes = setNumber(0, 59, timerMinutes, false);
				EEPROM.write(8, timerMinutes);
				loopControl = false;
			} else if ( upbtn.pressed() ) {
				mod = 7;
			} else if ( downbtn.pressed() ) {
				mod = 1;
			}
		}
	}
	loopControl = true;
	mod = 0;
}

void eread() {
	humidityHigh = EEPROM.read(1);
	humidityIdeal = EEPROM.read(2);
	humidityLow = EEPROM.read(3);
	tempretureHigh = EEPROM.read(4);
	tempretureIdeal = EEPROM.read(5);
	tempretureLow = EEPROM.read(6);
	timerHour = EEPROM.read(7);
	timerMinutes = EEPROM.read(8);
}

short int setNumber(short int minimum, short int maximum, short int startNumber, bool t) {
	while (loopControl) {
		viewNumber(startNumber, t);
		if ( upbtn.pressed() ) {
			startNumber++;
			if (startNumber >= maximum) {
				startNumber = minimum;
			}
		} else if ( downbtn.pressed() ) {
			startNumber--;
			if (startNumber <= minimum ) {
				startNumber = maximum;
			}
		} else if ( setbtn.pressed() ) {
			loopControl = false;
		}
	}
	loopControl = true;
	return startNumber;
}

void onOff() {
	if (humidity > humidityHigh) {
		digitalWrite(fanPin, HIGH);
		humidityHighBool = true;
	}
	if (humidityHighBool && humidity < humidityIdeal) {
		digitalWrite(fanPin, LOW);
		humidityHighBool = false;
	}

	if (temperature > tempretureHigh) {
		digitalWrite(fanPin, HIGH);
		tempretureHighBool = true;
	}
	if (tempretureHighBool && temperature < tempretureIdeal) {
		digitalWrite(fanPin, LOW);
		tempretureHighBool = false;
	}

	if (humidity < humidityLow) {
		digitalWrite(humidityPin, HIGH);
		humidityLowBool = true;
	}
	if (humidityLowBool && humidity > humidityIdeal) {
		digitalWrite(humidityPin, LOW);
		humidityLowBool = false;
	}

	if (temperature < tempretureLow) {
		digitalWrite(elementPin, HIGH);
		tempretureLowBool = true;
	}
	if (tempretureLowBool && temperature > tempretureIdeal) {
		digitalWrite(elementPin, LOW);
		tempretureLowBool = false;
	}
}
