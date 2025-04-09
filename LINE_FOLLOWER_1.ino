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
int MOTOR1_IN1 = 5;//3;
int MOTOR1_IN2 = 6;//5;
int MOTOR2_IN1 = 9;//6;
int MOTOR2_IN2 = 10;//11;
int SENS_D1 = A5;//Left
int SENS_D2 = A4;
int SENS_D3 = A3;
int SENS_D4 = A2;
int SENS_D5 = A1;
int SENS_D6 = A0;//Right
int valD1 = 0;
int valD2 = 0;
int valD3 = 0;
int valD4 = 0;
int valD5 = 0;
int valD6 = 0;
int lastErr = 0;
int sum = 0;
int diff = 0;
int errLF = 0;
int flag = 0;

void motorRight(float speed){
  if (speed > 0) {
    digitalWrite(MOTOR1_IN1, LOW);
    analogWrite(MOTOR1_IN2, speed);
  }
  if (speed == 0) {
    digitalWrite(MOTOR1_IN1, HIGH);
    digitalWrite(MOTOR1_IN2, HIGH);
  }
}

void motorLeft(float speed){
  if (speed > 0) {
    analogWrite(MOTOR2_IN1, speed);
    digitalWrite(MOTOR2_IN2, LOW);
  }
  if (speed == 0) {
    digitalWrite(MOTOR2_IN1, HIGH);
    digitalWrite(MOTOR2_IN2, HIGH);
  }
}
//Steering -100 <= val <= 100
void motorSteering(float val, float speed) {
  float speed_right = 0;
  float speed_left = 0;
  val = constrain(val, -100, 100);
  if (val < 0) {
    speed_right = speed;
    speed_left = speed + speed * val / 100.0;
  } else {
    speed_right = speed - speed * val / 100.0; 
    speed_left = speed;
  }
  motorLeft(speed_left);
  motorRight(speed_right);
}

void motorBrake(){
  motorLeft(0);
  motorRight(0);
}

void logSensor(){
  Serial.print("SENS_D1: "); Serial.print(analogRead(SENS_D1)); Serial.print("; ");
  Serial.print("SENS_D2: "); Serial.print(analogRead(SENS_D2)); Serial.print("; ");
  Serial.print("SENS_D3: "); Serial.print(analogRead(SENS_D3)); Serial.print("; ");
  Serial.print("SENS_D4: "); Serial.print(analogRead(SENS_D4)); Serial.print("; ");
  Serial.print("SENS_D5: "); Serial.print(analogRead(SENS_D5)); Serial.print("; ");
  Serial.print("SENS_D6: "); Serial.print(analogRead(SENS_D6)); Serial.print("; ");
  Serial.println();
}

void ledOn(){
  digitalWrite(LED, HIGH);
}
void ledOff(){
  digitalWrite(LED, LOW);
}

void ledBlink(int time){
  ledOn();
  delay(time);
  ledOff();
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
  pinMode(LED, OUTPUT);
  ledBlink(250); delay(750); ledBlink(250); delay(750); ledBlink(250); delay(750);
}

void loop() {
  /*
  if (analogRead(SENS_D1) > 900 && (analogRead(SENS_D2) < 870)){
    while (analogRead(SENS_D3) < 900) {motorSteering(-100, 5);}
  }
  if (analogRead(SENS_D6) > 900 && (analogRead(SENS_D5) < 870)){
    while (analogRead(SENS_D4) < 900) {motorSteering(100, 5);}
  }
  */
  valD1 = constrain(analogRead(SENS_D1), 800, 900); valD2 = constrain(analogRead(SENS_D2), 800, 900); valD3 = constrain(analogRead(SENS_D3), 800, 900);
  valD4 = constrain(analogRead(SENS_D4), 800, 900); valD5 = constrain(analogRead(SENS_D5), 800, 900); valD6 = constrain(analogRead(SENS_D6), 800, 900);
  int errR = valD4 + valD5 + valD6; //800-900
  int errL = valD1 + valD2 + valD3;
  int err = errR - errL; //-200 : 200
  if ((errR + errL) < 4850) {
    while ((errR + errL) < 5000) {
      valD1 = constrain(analogRead(SENS_D1), 800, 900); valD2 = constrain(analogRead(SENS_D2), 800, 900); valD3 = constrain(analogRead(SENS_D3), 800, 900);
      valD4 = constrain(analogRead(SENS_D4), 800, 900); valD5 = constrain(analogRead(SENS_D5), 800, 900); valD6 = constrain(analogRead(SENS_D6), 800, 900);
      errR = valD4 + valD5 + valD6; //800-900
      errL = valD1 + valD2 + valD3;
      motorSteering(errLF * 100, 100);
      delay(20);
    }
  } else {
    motorSteering((err/2)*0.6 + (err - lastErr)*5 + (sum)*0.4, 240);
  }
  lastErr = err;
  sum = constrain(sum + err, -100, 100);
  diff -= err;
  float K = 0.1;
  errLF = errLF * (1 - K) + K * err;
  delay(10);
}