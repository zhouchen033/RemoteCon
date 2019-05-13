// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       RemoteCon.ino
    Created:	2019/5/13 9:02:12
    Author:     Raymond-PC\Raymond
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//
byte vx = 0;
byte vy = 0;
byte arrV[5];
const byte HEADER = 0xAA;
const byte ENDER = 0xAB;
const byte MSGID = 0xF1;

// The setup() function runs once each time the micro-controller starts
void setup()
{
	Serial.begin(9600);

}

// Add the main program code into the continuous loop() function
void loop()
{
	// left y ->0 , right y -> 100
	// up x -> 100, down x -> 0
	vx = (analogRead(A0) - 500)*0.1 + 50;
	vy = (analogRead(A1) - 500)*0.1 + 50;
	arrV[0] = HEADER;
	arrV[1] = MSGID;
	arrV[2] = vx;
	arrV[3] = vy;
	arrV[4] = ENDER;
	Serial.write(arrV, 5);
	//Serial.print("X:");
	//Serial.print(vx, DEC);
	//Serial.print(" | Y:");
	//Serial.println(vy, DEC);


	delay(300);
}
