int IN1_A=7;
int IN2_A=8;
int IN1_B=9;
int IN2_B=10;
int servo_height_pin=A7;
int echo_pin = 2;
int trigger_pin=3;
int left_sensor_pin = A1;
int right_sensor_pin = A0;
// For two motors instance at once
#include <L298NX2.h>
#include <Servo.h>
L298NX2 motor(IN1_A,IN2_A,IN1_B,IN2_B);

Servo servo_height;

void move_stop(){
  Serial.println("moveStop() called");

  motor.stopA();
  motor.stopB();
}

void turn_right(int time){
  Serial.print("turn_right(");
  Serial.print(time);
  Serial.println(") called");

  motor.forwardForA(time);
  motor.backwardForB(time);
  printSomeInfo();
  motor.resetA();
  motor.resetB();
}

void turn_left(int time){
  Serial.print("turn_left(");
  Serial.print(time);
  Serial.println(") called");

  motor.forwardForB(time);
  motor.backwardForA(time);
  printSomeInfo();
  motor.resetA();
  motor.resetB();
}

// void increase_speed(){

//   if(motor.getSpeedA() > 250 - 10){
//     motor.setSpeedA(motor.getSpeedA() + 10);    
//   }

//   else{
//     motor.setSpeedA(250);  
//   }

//   if(motor.getSpeedB() > 250 - 10){
//     motor.setSpeedB(motor.getSpeedB() + 10);    
//   }

//   else{
//     motor.setSpeedB(250);  
//   }
  
// }

// void decrease_speed(){

//   if(motor.getSpeed() > 10){
//     motor.setSpeed(motor.getSpeed() - 10);    
//   }

//   else{
//     motor.setSpeed(0);  
//   }
  
// }
void printSomeInfo()
{
  Serial.print("Motor A is moving = ");
  Serial.print(motor.isMovingA() ? "YES" : "NO");
  Serial.print(" at speed = ");
  Serial.println(motor.getSpeedA());
  Serial.print("Motor B is moving = ");
  Serial.print(motor.isMovingB() ? "YES" : "NO");
  Serial.print(" at speed = ");
  Serial.println(motor.getSpeedB());
}
void move_forward(int time){
  Serial.print("move_forward(");
  Serial.print(time);
  Serial.println(") called");

  motor.forwardForA(time);
  motor.forwardForB(time);
  printSomeInfo();
  motor.resetA();
  motor.resetB();
}

void move_backwards(int time){
  Serial.print("move_backwards(");
  Serial.print(time);
  Serial.println(") called");

  motor.backwardForA(time);
  motor.backwardForB(time);
  printSomeInfo();
  motor.resetA();
  motor.resetB();
}

void fire(/*distance to bucket, water*/){
  
}

void setup() {
  // put your setup code here, to run once:
  pinMode(echo_pin,INPUT);
  pinMode(trigger_pin,OUTPUT);
  Serial.begin(9600);
  servo_height.attach(servo_height_pin);
}

void change_path() {
  Serial.println("change_path() called");

  move_stop();   // stop forward movement
  // check distance to the right
  turn_right(100);
  delay(500);
  double rightDistance = calc_distance(); //set right distance
  delay(500);
  turn_left(200);  // check distace to the left
  delay(700);
  double leftDistance = calc_distance(); //set left distance
  delay(500);
   //return to center
  turn_right(100);
  delay(100);
  if (leftDistance - rightDistance >= 1){
    turn_left(100);
  }else if(leftDistance - rightDistance <= -1){
    turn_right(100);
  }
  delay(100);
}

double calc_distance(){
  digitalWrite(trigger_pin,LOW);
  delay(10);
  digitalWrite(trigger_pin,HIGH);
  delay(10);
  digitalWrite(trigger_pin,LOW);
  double duration=pulseIn(echo_pin,HIGH);
  double distance = duration*0.034/2;
  Serial.println();
  return distance;  
}

int direction_sensing(int sensitivity) {  //Returns 0 when the robot should turn left, 1 when it should go straight, and 2 when it should turn right
  int left_sensor_val = 0;
  int right_sensor_val = 0;
  left_sensor_val = analogRead(left_sensor_pin);
  right_sensor_val = analogRead(right_sensor_pin);

 // Serial.print(left_sensor_val);
//  Serial.print(", ");
 // Serial.println(right_sensor_val);
 
  if(abs(left_sensor_val - right_sensor_val) <= sensitivity)
    return(1);
  else if(left_sensor_val > right_sensor_val)
    return(0);
  else if(right_sensor_val > left_sensor_val)
    return(2);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("----------------------------------------------------");
  if(calc_distance() <= 30){
    change_path();
  }else{
    move_forward(500);
  }

  

}