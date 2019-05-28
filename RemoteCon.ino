#include <LiquidCrystal.h>

const byte sw_pin = 7;

byte vx = 0;
byte vy = 0;
byte arrV[5];
const byte HEADER = 0xAA;
const byte ENDER = 0xAB;
const byte MSGID_XY = 0xF1;
const byte MSGID_SEN = 0xFA;

byte MSG_STATE = 0;
byte MSG_ID = -1;

byte payload[7];
byte payload_size = 0;

long lastTimeStamp = 0;
long currTimeStamp = 0;

byte trasmissionStatus = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// The setup() function runs once each time the micro-controller starts

void buzz() {
	for (int i = 0; i < 2; i++) {
		tone(8, 220, 500);
		delay(100);
		noTone(8);
	}
}

void setup()
{
	pinMode(sw_pin, INPUT_PULLUP);
	lcd.begin(16, 2);
	Serial.begin(9600);
	lcd.print("=Captain Marvel=");
	lcd.setCursor(0, 1);
	lcd.print("!Ready To Go!");
	buzz();
}

void parseMsg(char c) {
	byte b = byte(c);
	switch (MSG_STATE) {
	case 0x00:
		if (b == 0xAA) {
			MSG_STATE = 1;
			MSG_ID = -1;
			payload_size = 0;
		}
		break;
	case 0x01:
		MSG_STATE = 2;
		MSG_ID = b;
		break;
	case 0x02:
		if (b == 0xAB) {
			MSG_STATE = 0;
		}
		else {
			payload[payload_size++] = b;
		}
		break;
	}
}

void sendXYMsg() {
	// left y ->0 , right y -> 100
	// up x -> 100, down x -> 0
	vx = (analogRead(A0) - 500)*0.1 + 50;
	vy = (analogRead(A1) - 500)*0.1 + 50;
	arrV[0] = HEADER;
	arrV[1] = MSGID_XY;
	arrV[2] = vx;
	arrV[3] = vy;
	arrV[4] = ENDER;
	Serial.flush();
	Serial.write(arrV, 5);
}

byte dst = 0;
byte tmpD = 0;
byte tmpR = 0;
byte sod = 0;
void updateScreen() {

	if ( trasmissionStatus == 0 ) {
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("!! SIGNAL LOST !!");
		//buzz();
	} else {
		//Serial.println("###");

		if (MSG_STATE == 0 && MSG_ID != -1) {
			//Serial.println(MSG_ID);

			if (MSG_ID == MSGID_SEN) {
				dst = payload[0];
				tmpD = payload[1];
				tmpR = payload[2];
				sod = payload[3];
			}

			// update display
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Dist: ");
			lcd.print(dst);
			lcd.print(" cm");
			lcd.setCursor(0, 1);
			lcd.print("Temp: ");
			lcd.print(tmpD);
			lcd.print(".");
			lcd.print(tmpR);

			if (sod == 1) {
				lcd.print(" #-_-#");
			}
			else {
				lcd.print("      ");
			}
		}

	}
}

// Add the main program code into the continuous loop() function
void loop()
{
	// send JoyStick XY value to serial
	sendXYMsg();
	delay(300);

	currTimeStamp = millis();

	if (currTimeStamp - lastTimeStamp > 3000) {
		trasmissionStatus = 0;
	}
	else {
		trasmissionStatus = 1;
	}

	updateScreen();
}

void serialEvent() {
	// read message from serial
	while (Serial.available() > 0) {
		lastTimeStamp = millis();
		parseMsg(Serial.read());
	}
	updateScreen();
	
}
