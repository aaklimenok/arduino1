//black(900)>white(600);
//      in1   in2   in3   in4
//frw   pwm   0     pwm   0
//bck   0/pwm 1     0/pwm 1
//break 1     1     1     1
//MOTOR1 - left; MOTOR2 - right
//TOP_VIEW  A5 A4 A3 A2 A1 A0
//left motor 23<=PWM
//left motor <=PWM
int LED = 13;
int MOTOR1_IN = 10; //in4
int MOTOR1_PWM = 9; //in3
int MOTOR2_IN = 6; //in2
int MOTOR2_PWM = 5; //in1
int SENS_0 = A0;
int SENS_1 = A1;
int SENS_2 = A2;
int SENS_3 = A3;
int SENS_4 = A4;
int SENS_5 = A5;
float kP = 0.2;
int SENS_0_MAX = 0;
int SENS_1_MAX = 0;
int SENS_2_MAX = 0;
int SENS_3_MAX = 0;
int SENS_4_MAX = 0;
int SENS_5_MAX = 0;
int SENS_0_MIN = 0;
int SENS_1_MIN = 0;
int SENS_2_MIN = 0;
int SENS_3_MIN = 0;
int SENS_4_MIN = 0;
int SENS_5_MIN = 0;

void motorLeft(int speed){
  if (speed >= 0) {
    analogWrite(MOTOR1_PWM, 127);
    delay(5);
    analogWrite(MOTOR1_PWM, int(255*abs(speed)/100.0));
    digitalWrite(MOTOR1_IN, 0);
  } else {
    analogWrite(MOTOR1_PWM, 127);
    delay(5);
    analogWrite(MOTOR1_PWM, 255 - int(255*abs(speed)/100.0));
    digitalWrite(MOTOR1_IN, 1);
  }
}

void motorRight(int speed){
  if (speed > 0) {
    analogWrite(MOTOR2_PWM, 127);
    delay(5);
    analogWrite(MOTOR2_PWM, 255 - int(255*abs(speed)/100.0));
    digitalWrite(MOTOR2_IN, 1);
  } else {
    analogWrite(MOTOR2_PWM, 127);
    delay(5);
    analogWrite(MOTOR2_PWM, int(255*abs(speed)/100.0));
    digitalWrite(MOTOR2_IN, 0);
  }
}
//Steering -100 <= val <= 100
void Steering(float val, int speed){
Serial.println(speed-speed*2*val/100.0);
  if (val > 100) {val = 100;}
  if (val < -100) {val = -100;}
  if (val > 0) {
    motorLeft(speed);
    motorRight(speed-speed*2*val/100.0);
  } else {
    motorLeft(speed+speed*2*val/100.0);
    motorRight(speed);
  }
}

void stop_move(){
  digitalWrite(MOTOR1_PWM, 1);
  digitalWrite(MOTOR1_IN, 1);
  digitalWrite(MOTOR2_PWM, 1);
  digitalWrite(MOTOR2_IN, 1);
}

void log(){
  //Serial.print("SENS_0: ");
  Serial.print(analogRead(SENS_0));
  Serial.print("; ");
  //Serial.print("SENS_1: ");
  Serial.print(analogRead(SENS_1));
  Serial.print("; ");
  //Serial.print("SENS_2: ");
  Serial.print(analogRead(SENS_2));
  Serial.print("; ");
 // Serial.print("SENS_3: ");
  Serial.print(analogRead(SENS_3));
  Serial.print("; ");
  //Serial.print("SENS_4: ");
  Serial.print(analogRead(SENS_4));
  Serial.print("; ");
  //Serial.print("SENS_5: ");
  Serial.print(analogRead(SENS_5));
  Serial.println("; ");
}

void relay(float val){
  if (val > 0) {
    motorRight(40);
    motorLeft(20);
  } else {
    motorRight(40);
    motorLeft(20);
  }
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

float Calibrate(int val, int max, int min) {
  return (val-min)/float(max-min)*100;
}

void Mesuare(){
  Blink(1000);
  delay(100);
  Blink(100);
  delay(100);
  Blink(100);
  delay(100);
  Blink(100);
  SENS_0_MAX = analogRead(SENS_0);
  SENS_1_MAX = analogRead(SENS_1);
  SENS_2_MAX = analogRead(SENS_2);
  SENS_3_MAX = analogRead(SENS_3);
  SENS_4_MAX = analogRead(SENS_4);
  SENS_5_MAX = analogRead(SENS_5);
  delay(1000);
  Blink(1000);
  delay(100);
  Blink(100);
  delay(100);
  Blink(100);
  delay(100);
  Blink(100);
  SENS_0_MIN = analogRead(SENS_0);
  SENS_1_MIN = analogRead(SENS_1);
  SENS_2_MIN = analogRead(SENS_2);
  SENS_3_MIN = analogRead(SENS_3);
  SENS_4_MIN = analogRead(SENS_4);
  SENS_5_MIN = analogRead(SENS_5);
}

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR1_IN, OUTPUT);
  pinMode(MOTOR1_PWM, OUTPUT);
  pinMode(MOTOR2_IN, OUTPUT);
  pinMode(MOTOR2_PWM, OUTPUT);
  pinMode(SENS_0, INPUT);
  pinMode(SENS_1, INPUT);
  pinMode(SENS_2, INPUT);
  pinMode(SENS_3, INPUT);
  pinMode(SENS_4, INPUT);
  pinMode(SENS_5, INPUT);
  pinMode(LED, OUTPUT);
  Mesuare();
  delay(1000);
  //motorLeft(7);
  //motorRight(7);
  //delay(3000);
  stop_move();
}

float err() {
  float error = 0;
  error -= Calibrate(analogRead(SENS_5), SENS_5_MAX, SENS_5_MIN)*1.2;
  error -= Calibrate(analogRead(SENS_4), SENS_4_MAX, SENS_4_MIN)*1.1;
  error -= Calibrate(analogRead(SENS_3), SENS_3_MAX, SENS_3_MIN)*1;
  error += Calibrate(analogRead(SENS_2), SENS_2_MAX, SENS_2_MIN)*1;
  error += Calibrate(analogRead(SENS_1), SENS_1_MAX, SENS_1_MIN)*1.1;
  error += Calibrate(analogRead(SENS_0), SENS_0_MAX, SENS_0_MIN)*1.2;
  return error;
}

void loop() {
  Steering(err()*0.2-28, 20);
  delay(20);
}