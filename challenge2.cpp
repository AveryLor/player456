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

// Ultrasonic

long duration = 0, distance = 0;

int redMin = 67; 
int redMax = 900; 
int greenMin = 102; 
int greenMax = 1300; 
int blueMin = 95;
int blueMax = 1000;

int redPW, greenPW, bluePW = 0;
int red, blue, green;

int time180 = 6;
int time90 = 3;
int stop = 0;

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
  // put your setup code here, to run once:

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

  
  pinMode(A4, OUTPUT);
  pinMode(A5, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(A4, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(A4, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(A4, LOW);
  duration = pulseIn(A5, HIGH);
  distance = (duration/2) / 29.1;
  Serial.println(distance);



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
/*
	Serial.print("Red = ");
	Serial.print(red);
	Serial.print(" - Green = ");
	Serial.print(green);
	Serial.print(" - Blue = ");
	Serial.println(blue);
*/


  
  if (green < 200 && blue < 200 && red < 200 && stop == 1){
      state = STOP;
  }
  else if (distance > 15 && time == 0){
    state = FORWARD;
    stop = 1;
  }
  else {
    if (red > 200){
      time = time180;
      state = RIGHT;
    }
    else if (blue > 200 && green > 200){
      time = time90;
      state = LEFT;
    }
    else if (green > 200){
      time = time90;
      state = RIGHT;
    }
  }


  if (time > 0) {
    time--;
  }
 
  //Controlling speed (0 = off and 255 = max speed):     
  analogWrite(10, 110); //ENA pin
  analogWrite(11, 110); //ENB pin
  
  switch (state) {
    case FORWARD: // forward
      digitalWrite(leftPin1, HIGH);
      digitalWrite(leftPin2, LOW);
      digitalWrite(rightPin1, HIGH);
      digitalWrite(rightPin2, LOW);
      break;
    case LEFT: // left
      digitalWrite(leftPin1, LOW);
      digitalWrite(leftPin2, HIGH);
      digitalWrite(rightPin1, HIGH);
      digitalWrite(rightPin2, LOW);
      break;
    case RIGHT: // right
      digitalWrite(leftPin1, HIGH);
      digitalWrite(leftPin2, LOW);
      digitalWrite(rightPin1, LOW);
      digitalWrite(rightPin2, HIGH);
      break;
    case STOP: // stop
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
