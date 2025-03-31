//black(900)>white(600);
//      in1   in2   in3   in4
//frw   pwm   0     pwm   0
//bck   0/pwm 1     0/pwm 1
//break 1     1     1     1
//MOTOR1 - left; MOTOR2 - right
//TOP_VIEW  A5 A4 A3 A2 A1 A0
//left motor 60<=PWM
//left motor <=PWM
int LED = 13;
int MOTOR1_IN1 = 3;
int MOTOR1_IN2 = 5;
int MOTOR2_IN1 = 6;
int MOTOR2_IN2 = 11;
int SENS_D1 = A0;
int SENS_D2 = A1;
int SENS_D3 = A2;
int SENS_D4 = A3;
int SENS_D5 = A4;
int SENS_D6 = A5;
int SENS_D7 = A6;
int SENS_D8 = A7;
int valD1 = 0;
int valD2 = 0;
int valD3 = 0;
int valD4 = 0;
int valD5 = 0;
int valD6 = 0;
int valD7 = 0;
int valD8 = 0;
float kP = 0.2;

void motorLeft(float speed){
  if (speed > 0) { //min PWM 30
    int pwm = 30 + (255 - 30) * speed / 100.0;
    //Serial.print("motorLeft() = "); Serial.print(pwm); Serial.print("; ");
    analogWrite(MOTOR1_IN1, pwm);
    digitalWrite(MOTOR1_IN2, LOW);
  }
  if (speed == 0) {
    //Serial.print("motorLeft() = STOP");
    digitalWrite(MOTOR1_IN1, HIGH);
    digitalWrite(MOTOR1_IN2, HIGH);
  }
}

void motorRight(float speed){
  if (speed > 0) { //min PWM 47
    int pwm = 47 + (255 - 47) * speed / 100.0;
    //Serial.print("motorRight() = "); Serial.print(pwm); Serial.print("; ");
    analogWrite(MOTOR2_IN1, pwm);
    digitalWrite(MOTOR2_IN2, LOW);
  }
  if (speed == 0) {
    //Serial.print("motorRight() = STOP");
    digitalWrite(MOTOR2_IN1, HIGH);
    digitalWrite(MOTOR2_IN2, HIGH);
  }
}
//Steering -100 <= val <= 100
void motorSteering(float val, float speed){
  if (val > 100) {val = 100;}
  if (val < -100) {val = -100;}
  float speed_left = speed + speed * val / 100.0;
  float speed_right = speed - speed * val / 100.0;
  motorLeft(speed_left);
  motorRight(speed_right);
}

void motorBrake(){
  digitalWrite(MOTOR1_IN1, HIGH);
  digitalWrite(MOTOR1_IN2, HIGH);
  digitalWrite(MOTOR2_IN1, HIGH);
  digitalWrite(MOTOR2_IN2, HIGH);
}

void log_Sensor(){
  Serial.print("SENS_D1: "); Serial.print(analogRead(SENS_D1)); Serial.print("; ");
  Serial.print("SENS_D2: "); Serial.print(analogRead(SENS_D2)); Serial.print("; ");
  Serial.print("SENS_D3: "); Serial.print(analogRead(SENS_D3)); Serial.print("; ");
  Serial.print("SENS_D4: "); Serial.print(analogRead(SENS_D4)); Serial.print("; ");
  Serial.print("SENS_D5: "); Serial.print(analogRead(SENS_D5)); Serial.print("; ");
  Serial.print("SENS_D6: "); Serial.print(analogRead(SENS_D6)); Serial.print("; ");
  Serial.print("SENS_D7: "); Serial.print(analogRead(SENS_D7)); Serial.print("; ");
  Serial.print("SENS_D8: "); Serial.print(analogRead(SENS_D8)); Serial.print("; ");
  Serial.println();
}

void Led_on(){
  digitalWrite(LED, HIGH);
}
void Led_off(){
  digitalWrite(LED, LOW);
}

void Blink(int time){
  Led_on();
  delay(time);
  Led_off();
}


void setup() {
  Serial.begin(9600);
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
  pinMode(SENS_D1, INPUT);
  pinMode(SENS_D2, INPUT);
  pinMode(SENS_D3, INPUT);
  pinMode(SENS_D4, INPUT);
  pinMode(SENS_D5, INPUT);
  pinMode(SENS_D6, INPUT);
  pinMode(SENS_D7, INPUT);
  pinMode(SENS_D8, INPUT);
  pinMode(LED, OUTPUT);
  delay(3000);
}

void loop() {
  float kp = 0.5;
  valD1 = analogRead(SENS_D1); valD2 = analogRead(SENS_D2); valD3 = analogRead(SENS_D3); valD4 = analogRead(SENS_D4); valD5 = analogRead(SENS_D5); valD6 = analogRead(SENS_D6); valD7 = analogRead(SENS_D7); valD8 = analogRead(SENS_D8); 
  int sumr = valD1 + valD2 + valD3 + valD4;
  int suml = valD5 + valD6 + valD7 + valD8;
  int err = sumr - suml;
  err = max(min(err, 500), -500) / 5.0;
  motorSteering(err * kp, 10); delay(10);
  //Serial.println(err * kp); delay(150);
  
}