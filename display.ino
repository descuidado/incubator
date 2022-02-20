void view (int data) {
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, dataArray[data]);
	digitalWrite(latchPin, HIGH);
}

void view3(int char1, int char2, int char3, bool t) {
	view(char1);
	digitalWrite(d1, LOW);
	delay(veiwDelayTime);
	digitalWrite(d1, HIGH);
	if (t) {
		char2 = char2 + 10;
	}
	view(char2);
	digitalWrite(d2, LOW);
	delay(veiwDelayTime);
	digitalWrite(d2, HIGH);

	view(char3);
	digitalWrite(d3, LOW);
	delay(veiwDelayTime);
	digitalWrite(d3, HIGH);
}

void viewNumber(unsigned short int number, bool t) {
	unsigned short int dv100 = number / 100;
	unsigned short int dv10 = number % 100;
	dv10 = dv10 / 10;
	unsigned short int dv1 = number % 10;
	view3(dv100, dv10, dv1, t);
}

void mainView(int cha, unsigned short int number) {
	unsigned short int dv10 = number / 10;
	unsigned short int dv1 = number % 10;
	view3(cha, dv10, dv1, false);
}

void mainDisplay() {
	//unsigned long int timeLeftia = (((lastTimeTurned + checkMillis) - millis()) / 60000);
	if (viewTemp) {
		viewNumber(temperature, true);
		if ((millis() - mainDisplayMillis) > 2000) {
			viewTemp = false;
			mainDisplayMillis = millis();
		}
	} else {
		mainView(H, humidity);
		if ((millis() - mainDisplayMillis) > 2000) {
			viewTemp = true;
			mainDisplayMillis = millis();
		}
	}
}
