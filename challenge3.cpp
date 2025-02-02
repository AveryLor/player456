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
long duration = 0, distance = 0;

int seq = 0;
int time = 0;
int time180 = 6;
int time90 = 3;

typedef enum {
  FORWARD,
  LEFT,
  RIGHT,
  STOP
} movement;

movement state = FORWARD;

void setup() {
  // put your setup code here, to run once:
  // motor control
  pinMode(leftPin1, OUTPUT);
  pinMode(leftPin2, OUTPUT);
  pinMode(rightPin1, OUTPUT);
  pinMode(rightPin2, OUTPUT);

  pinMode(A4, OUTPUT);
  pinMode(A5, INPUT);

  pinMode(color0, OUTPUT);
  pinMode(color1, OUTPUT);
  pinMode(color2, OUTPUT);
  pinMode(color3, OUTPUT);
  pinMode(colorOut, INPUT);

  pinMode(A1, OUTPUT);

  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

  digitalWrite(color0, HIGH);
  digitalWrite(color1, LOW);
  digitalWrite(A1, LOW);

  Serial.begin(9600);

  // speed control
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(10, 110);  //ENA pin
  analogWrite(11, 110);  //ENB pin

  digitalWrite(A4, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(A4, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(A4, LOW);
  duration = pulseIn(A5, HIGH);
  distance = (duration/2) / 29.1;
  Serial.println(distance);


  if (distance > 15 && time == 0) {
    state = FORWARD;
  } else if (time == 0) {
    state = RIGHT;
    time = 3;
  }

  if (time > 0) {
    time--;
  }

  switch (seq) {
    case 0:
      if (red > 170) {
        digitalWrite(A1, HIGH);
        delay(500);
        digitalWrite(A1, LOW);
        seq = 1;
      }
      break;
    case 1:
      if (green > 150 && blue < 150) {
        digitalWrite(A1, HIGH);
        delay(500);
        digitalWrite(A1, LOW);
        seq = 2;
      }
      break;
    case 2:
      if (green > 150 && blue > 150) {
        digitalWrite(A1, HIGH);
        delay(500);
        digitalWrite(A1, LOW);
        seq = 3;
      }
      break;
    case 3:
      if (green > 150 && blue < 150) {
        digitalWrite(A1, HIGH);
        delay(500);
        digitalWrite(A1, LOW);
        seq = 4;
      }
      break;
    case 4:
      if (green > 150 && blue > 150) {
        digitalWrite(A1, HIGH);
        delay(500);
        digitalWrite(A1, LOW);
        seq = 5;
      }
      break;
    case 5:
      stop = 1;
      break;
  }


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



}
