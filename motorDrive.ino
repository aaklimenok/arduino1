int LED = 13;
int S0 = A0; //RRR
int S1 = A1; //RR
int S2 = A2; //R
int S3 = A3;
int S4 = A4;
int S5 = A5;
int MOTOR_LEFT_IN1 = 9;
int MOTOR_LEFT_IN2 = 10;
int MOTOR_RIGHT_IN1 = 5;
int MOTOR_RIGHT_IN2 = 6;

void motorLeft(float speed){
  if (speed > 0) {
    speed = min(100, speed);
    float pwm = (200 - 60) * speed * 0.01 + 60; //pwm calculate (min 60)
    analogWrite(MOTOR_LEFT_IN1, pwm);
    digitalWrite(MOTOR_LEFT_IN2, LOW);
    ledOn();
  }
  if (speed == 0) {
    digitalWrite(MOTOR_LEFT_IN1, HIGH);
    digitalWrite(MOTOR_LEFT_IN2, HIGH);
  }
}

void motorRight(float speed){
  if (speed > 0) {
    speed = min(100, speed);
    float pwm = (255 - 93) * speed * 0.01 + 93; //pwm calculate (min 93)
    digitalWrite(MOTOR_RIGHT_IN1, LOW);
    analogWrite(MOTOR_RIGHT_IN2, pwm);
  }
  if (speed == 0) {
    digitalWrite(MOTOR_RIGHT_IN1, HIGH);
    digitalWrite(MOTOR_RIGHT_IN2, HIGH);
  }
}

void motorMove(float steering, float speed){
  //Right
  if (steering > 0){
    steering = min(100, steering);
    motorLeft(speed);
    motorRight(speed - speed * steering * 0.01);
  }
  //Left
  else {
    steering = max(-100, steering);
    motorLeft(speed + speed * steering * 0.01);
    motorRight(speed);
  }  
}

void ledOn(){
  digitalWrite(LED, HIGH);
}

void ledOff(){
  digitalWrite(LED, LOW);
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(S0, INPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
  pinMode(MOTOR_LEFT_IN1, OUTPUT);
  pinMode(MOTOR_LEFT_IN2, OUTPUT);
  pinMode(MOTOR_RIGHT_IN1, OUTPUT);
  pinMode(MOTOR_RIGHT_IN2, OUTPUT);
}

void loop() {
  motorMove(-99, 50);
}
