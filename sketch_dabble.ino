#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

#include <DabbleESP32.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#define LED_BUILTIN 2
#include <driver/ledc.h>

// Movement
const byte motorPin_fwd[] = {16 , 5};   // IN Pins
const byte motorPin_bkwrd[] = { 17, 18}; // IN  pins
const uint8_t movementDelay = 250; // Determines a minimum delay upto which robot will move for a single input


// Arm

Adafruit_PWMServoDriver servoDriver = Adafruit_PWMServoDriver(0x40);

#define servoMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define servoMAX  600 // This is the 'maximum' pulse length count (out of 4096)

uint8_t min_val[] = { 90 , 0 , 0 , 0 , 0 , 0 };
uint8_t max_val[] = { 180 , 180 , 180 , 180 , 180 , 180 };
uint8_t servoDelay[] = { 50 , 50 , 50 , 50 , 50 , 50 }; // customize sensitivity for each servo
uint8_t value[] = { 0 , 0 , 0 , 0 , 0 , 0 };
byte servo = 0;

// if true , in  movement mode
bool mode = false;
bool debug = false;

void setup() {
  for (int i = 0; i < 2; i++) {
    pinMode(motorPin_fwd[i], OUTPUT);
    pinMode(motorPin_bkwrd[i], OUTPUT);
  }
  pinMode(LED_BUILTIN, OUTPUT);

  servoDriver.begin();
  servoDriver.setPWMFreq(60);

  processMovement(-1);

  Serial.begin(115200);           // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin("ESPController");  //set bluetooth name of your device
}

void loop() {
  int movement = -1;
  Dabble.processInput();

  if (GamePad.isUpPressed()) {
    
    if (mode) {
      movement = 0;
    } else {
      Serial.println("Selected servo 1");
      servo = 0;
      blink(1 , 125);
    }
  }
  
  if (GamePad.isRightPressed()) {
    if (mode) {
      movement = 3;
    } else {
    Serial.println("Selected servo 2");
    servo = 1;
    blink(2 , 125);
    }
  }

  if (GamePad.isDownPressed()) {
    if (mode) {
      movement = 1;
    } else {
    Serial.println("Selected servo 3");
    servo = 2;
    blink(3 , 125);
    }
  }

  if (GamePad.isLeftPressed()) {
    if (mode) {
      movement = 2;
    } else {
    Serial.println("Selected servo 4");
    servo = 3;
    blink(4 , 125);
    }
  }

  
  if (GamePad.isSquarePressed())
  {
    Serial.println("Selected servo 5");
    servo = 4;
    blink(5 , 125);
  }

  if (GamePad.isCirclePressed())
  {
    Serial.println("Selected servo 6");
    servo = 5;
    blink(6 , 125);
  }

  if (GamePad.isCrossPressed()) {
    blink(1 , 125);
    moveServo(false);
  }

  if (GamePad.isTrianglePressed()) {
    blink(1 , 125);
    moveServo(true);
  }

  if (GamePad.isSelectPressed()) {
    mode = !mode;
    blink(1 , 200);
    if(mode) {
      blink(10 , 25);
      Serial.println("Movement mode");
    } else {
      Serial.println("Robotic arm controlling mode");
    }
    
  }

  if(GamePad.isStartPressed()) {
    if(mode) {
    blink(10 , 25);
    processMovement(-1);
    }
  }

  processMovement(movement);

  if(debug && mode) {
      Serial.println(String(digitalRead(motorPin_fwd[0])) + " , " + String(motorPin_bkwrd[0]) + " , " + String(motorPin_fwd[1]) + " , " + String(motorPin_bkwrd[1]));
      delay(700);
  }
}

void moveServo(bool up) {

  if((value[servo] == max_val[servo]) && up) {
    if(debug) {
      Serial.println("Servo " + String(servo + 1) + " reached its max value.");
    }
    return; 
  }
  if((value[servo] == min_val[servo]) && (!up)) {
    if(debug) {
      Serial.println("Servo " + String(servo + 1) + " reached its min value.");
    }
    return;
  }
  if(up) {++value[servo];} else {--value[servo];}

  servoDriver.setPWM(servo, 0, map(value[servo], 0, 180, servoMIN, servoMAX));
  delay(servoDelay[servo]);
  if(debug) {
  Serial.println("Angle of Servo " + String(servo + 1) + " :" + String(value[servo]));
  }
}

void blink(uint8_t n , uint16_t d) {
  for (uint8_t i = 1; i <= n; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(d);
    digitalWrite(LED_BUILTIN, LOW);
    delay(d);
  }
}

// Movement part

// @dirn : 0 -> front ; 1 -> back; 2 -> left; 3 -> right
void processMovement(byte dirn) {
  switch(dirn) {

    case 0 :  if(debug) {Serial.println("Move forward");}
              moveMotor(0 , 1);
              moveMotor(1 , 1);
              break;

    case 1 :  if(debug) {Serial.println("Move backward");}
              moveMotor(0 , 2);
              moveMotor(1 , 2);
              break;

    case 2 :  if(debug) {Serial.println("Move left");}
              moveMotor(0 , 1);
              moveMotor(1 , 2);
              break;
              
    case 3 :  if(debug) {Serial.println("Move right");}
              moveMotor(0 , 2);
              moveMotor(1 , 1);
              break;
              
    default : if(debug){Serial.println("STOP");}
              moveMotor(0, 0);
              moveMotor(1, 0);
              break;
  }
}


void moveMotor(byte motor , byte direction) {
  switch(direction) {
    case 1 :  digitalWrite(motorPin_fwd[motor], HIGH);
              digitalWrite(motorPin_bkwrd[motor], LOW);
              break;

    case 0 :  digitalWrite(motorPin_fwd[motor],LOW);
              digitalWrite(motorPin_bkwrd[motor], LOW);
              break;

    case 2 :  digitalWrite(motorPin_fwd[motor], LOW);
              digitalWrite(motorPin_bkwrd[motor], HIGH);
              break;

    default : digitalWrite(motorPin_fwd[motor],LOW);
              digitalWrite(motorPin_bkwrd[motor], LOW);
              break;
  }
}