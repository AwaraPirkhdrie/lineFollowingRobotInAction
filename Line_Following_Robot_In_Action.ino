/****************************************************************
Line_Following_Robot_In_Action.ino
Awara
*****************************************************************/
// sensor values
int LDRONE; 
int LDRTWO; 
int LDRTHREE; 
// calibration offsets
int leftOff = 0;
int rightOff = 0;
int center = 0;
// pins for motor speed and direction
int speedOne = 6;
int speedTwo = 7;
int directionOne = 8;
int directionTwo = 9;
// starting speed and rotation offset
int startSpeed = 70;
int rotate = 30;
// sensor threshold
int threshhold = 5;
int left = startSpeed;
int right = startSpeed;
int buttonPush = 2;
int motorsOff = true;
int light = 13;

// Sensor calibration routine
void calibrate(){
    for (int i = 0; i < 10; i++){ // run this 10 times to obtain average
        digitalWrite(light, HIGH); 
        delay(100);
        LDRONE = analogRead(0);
        LDRTWO = analogRead(1);
        LDRTHREE = analogRead(2);
        leftOff = leftOff + LDRONE;
        center = center + LDRTWO;
        rightOff = rightOff + LDRTHREE; 
        delay(100);
        digitalWrite(light, LOW);
        delay(100);
    }
    // obtain average for each sensor
    leftOff = leftOff / 10;
    rightOff = rightOff / 10;
    center = center / 10;
    // calculate offsets for left and right sensors
    leftOff = center - leftOff;
    rightOff = center - rightOff;
}

void setup(){
    pinMode(buttonPush, INPUT_PULLUP);
    pinMode(light, OUTPUT);
    pinMode(speedOne, OUTPUT);
    pinMode(speedTwo, OUTPUT);
    pinMode(directionOne, OUTPUT);
    pinMode(directionTwo, OUTPUT);
    // calibrate the sensors
    calibrate();
    delay(3000);
    digitalWrite(light, HIGH);
    delay(100);
    // set motor direction to forward
    digitalWrite(directionOne, HIGH);
    digitalWrite(directionTwo, HIGH);
    // set speed of both motors
    analogWrite(speedOne, left);
    analogWrite(speedTwo, right);
}

void loop(){
    if (!digitalRead(buttonPush)){
        // button is pressed. Toggle motors on or off.
        motorsOff = !motorsOff;
        if (motorsOff){
            analogWrite(speedOne, 0);
            analogWrite(speedTwo, 0);
        }
        delay(500);
    }
    // make both motors same speed
    left = startSpeed;
    right = startSpeed;
    // read the sensors and add the offsets
    LDRONE = analogRead(0) + leftOff;
    LDRTWO = analogRead(1);
    LDRTHREE = analogRead(2) + rightOff;
    // if LDRONE is greater than the centre sensor + threshold turn right
    if (LDRONE > (LDRTWO + threshhold)){
        left = startSpeed + rotate;
        right = startSpeed - rotate;
    }
    // if LDR3 is greater than the centre sensor + threshold turn left
    if (LDRTHREE > (LDRTWO + threshhold)){
        left = startSpeed - rotate;
        right = startSpeed + rotate;
    }
    // send the speed values to the motors
    if (!motorsOff){
        analogWrite(speedOne, left);
        analogWrite(speedTwo, right);
    }
}
