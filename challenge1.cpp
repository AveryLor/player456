
#include <Servo.h>

Servo clawServo;  // create servo object to control a servo

// motor pins
int leftPin1 = 2;
int leftPin2 = 3;
int rightPin1 = 4;
int rightPin2 = 5;

// color pins
int color0 = 6;
int color1 = 7;
int color2 = 8;
int color3 = 9;
int EN = 13;
int colorOut = 12;

int redMin = 67;
int redMax = 900;
int greenMin = 102;
int greenMax = 1300;
int blueMin = 95;
int blueMax = 1000;

int redPW, greenPW, bluePW = 0;
int red, blue, green;

typedef enum {
  FORWARD,
  LEFT,
  RIGHT,
  STOP
} movement;

movement state = FORWARD;
int seq = 0;
int time = 0;
int rotTime = 0;
int prevColor = 0;
int currColor = 0;
int numRings = 0;

void setup() {
  // motor control
  pinMode(leftPin1, OUTPUT);
  pinMode(leftPin2, OUTPUT);
  pinMode(rightPin1, OUTPUT);
  pinMode(rightPin2, OUTPUT);

  pinMode(color0, OUTPUT);
  pinMode(color1, OUTPUT);
  pinMode(color2, OUTPUT);
  pinMode(color3, OUTPUT);
  pinMode(colorOut, INPUT);

  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

  digitalWrite(color0, HIGH);
  digitalWrite(color1, LOW);

  Serial.begin(9600);

  // speed control
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  clawServo.attach(A0);  // attaches the servo on pin A0 to the servo object
}

void loop() {
  // put your main code here, to run repeatedly:

  // Color Sensor
  redPW = getRed();
  red = map(redPW, redMin, redMax, 255, 0);
  delay(200);

  greenPW = getGreen();
  green = map(greenPW, greenMin, greenMax, 255, 0);
  delay(200);

  bluePW = getBlue();
  blue = map(bluePW, blueMin, blueMax, 255, 0);
  delay(200);

  // R = 1, G = 2, B = 3

  switch (seq) {
    // On start, prevColor == 0, so move forward until not detecting black
    case 0:
      if (red < 200 && (green < 200 && blue < 200)) {  // if black keep moving forward
        state = FORWARD;
      } else {  // if not black, stop and store color
        state = STOP;
        seq = 1;
        delay(200);
        // track color
        if (red > 200) {
          prevColor = 1;
        } else if (blue > 200 && green > 200) {
          prevColor = 3;
        } else if (green > 200) {
          prevColor = 2;
        }
      }
      break;
    case 1:
      // turn left until hit black
      if (red > 200 || (green > 200 || blue > 200)) {
        state = LEFT;
        Serial.println("1");
      } else {
        state = STOP;
        seq = 2;
        delay(200);
      }
      break;
    case 2:
      time++;
      state = RIGHT;
      Serial.println(time);
      if (red > 200 || (green > 200 || blue > 200)) {
        seq = 3;
      }
      break;
    case 3:
      time++;
      state = RIGHT;
      Serial.println(time);
      if (red < 200 && (green < 200 && blue < 200)) {
        seq = 4;
        state = STOP;
        rotTime = (int)time / 2;
        Serial.println("rotate time is: " + rotTime);
      }
      break;
    case 4:
      time--;
      state = LEFT;
      Serial.println(time);
      if (time <= rotTime) {
        seq = 5;
        state = STOP;
        delay(200);
      }
      break;
    case 5:
      state = FORWARD;
      if (red > 200) {
        currColor = 1;
      } else if (blue > 200 && green > 200) {
        currColor = 3;
      } else if (green > 200) {
        currColor = 2;
      }

      if (currColor != prevColor) {
        prevColor = currColor;
        numRings++;
        Serial.println(numRings);
        if (numRings == 4) {
          seq = 6;
          state = STOP;
          delay(100);
        }
      }
      break;
    case 6:
      state = FORWARD; // need to go forwards (or backwards) a certain distance**********
      for (int pos = 70; pos >= 0; pos--) {  // goes from 0 degrees to 90 degrees
        clawServo.write(pos);
        delay(10);
      }
      seq = 7;
      break;
    case 7:
      state = STOP;
      break;
  } 

  // test rgb values
  Serial.print("Red = ");
  Serial.print(red);
  Serial.print(" - Green = ");
  Serial.print(green);
  Serial.print(" - Blue = ");
  Serial.println(blue);

  // Motor Control

  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(10, 100);  //ENA pin
  analogWrite(11, 100);  //ENB pin

  switch (state) {
    case FORWARD:  // forward
      digitalWrite(leftPin1, HIGH);
      digitalWrite(leftPin2, LOW);
      digitalWrite(rightPin1, HIGH);
      digitalWrite(rightPin2, LOW);
      break;
    case LEFT:  // left
      digitalWrite(leftPin1, LOW);
      digitalWrite(leftPin2, HIGH);
      digitalWrite(rightPin1, HIGH);
      digitalWrite(rightPin2, LOW);
      break;
    case RIGHT:  // right
      digitalWrite(leftPin1, HIGH);
      digitalWrite(leftPin2, LOW);
      digitalWrite(rightPin1, LOW);
      digitalWrite(rightPin2, HIGH);
      break;
    case STOP:  // stop
      digitalWrite(leftPin1, LOW);
      digitalWrite(leftPin2, LOW);
      digitalWrite(rightPin1, LOW);
      digitalWrite(rightPin2, LOW);
      break;
  }
}

// get pw for RED
int getRed() {
  digitalWrite(color2, LOW);
  digitalWrite(color3, LOW);
  return pulseIn(colorOut, LOW);
}

// get pw for GREEN
int getGreen() {
  digitalWrite(color2, HIGH);
  digitalWrite(color3, HIGH);
  return pulseIn(colorOut, LOW);
}

// get pw for BLUE
int getBlue() {
  digitalWrite(color2, LOW);
  digitalWrite(color3, HIGH);
  return pulseIn(colorOut, LOW);
}
