#include <PID_v1.h>

#define MotEnable 9 
#define MotFwd  10  
#define MotRev  11 

#define MotEnable2 6 
#define MotFwd2  8
#define MotRev2  7

int tolerance=40; 
int tolerance2=40; 

int north=A0;
int south=A1;
int east=A2;
int west=A3;

double kp = 5 , ki = 1 , kd = 0.01;     //pid parameters

double input = 0, output = 0, setpoint = 0;     
double input2 = 0, output2 = 0, setpoint2 = 0;

PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);     //PID for tilt 
PID myPID2(&input2, &output2, &setpoint2, kp, ki, kd, DIRECT);  //PID for azimuth


void setup() {

  pinMode(east,INPUT);
  pinMode(west,INPUT);
  pinMode(north,INPUT);
  pinMode(south,INPUT);
  
  //Motor pins:
  pinMode(MotEnable, OUTPUT);
  pinMode(MotFwd, OUTPUT); 
  pinMode(MotRev, OUTPUT); 
  pinMode(MotEnable2, OUTPUT);
  pinMode(MotFwd2, OUTPUT); 
  pinMode(MotRev2, OUTPUT); 

  Serial.begin(9600);  //Opening serial port
  
  
  TCCR1B = TCCR1B & 0b11111000 | 1;  // set 31KHz to prevent motor noise
  
  myPID.SetMode(AUTOMATIC);            //set pid in automatic mode
  myPID.SetSampleTime(1);              // refresh rate of pid controller
  myPID.SetOutputLimits(-90, 90);    // this is the max pwm value to move motor

  myPID2.SetMode(AUTOMATIC);  
  myPID2.SetSampleTime(1);  
  myPID2.SetOutputLimits(-70, 70); 
}

void loop() {
 
  int north_value=analogRead(A1);
  int south_value=analogRead(A3);
  int east_value=analogRead(A0);
  int west_value=analogRead(A2);   

  setpoint=north_value;
  setpoint2=west_value;
  
  input = south_value ;     // data from potentiometer
  input2 = east_value;
   
  myPID.Compute();     // calculate new output
  myPID2.Compute();
  
  pwmOut(output);    
  pwmOut2(output2); 

  if(abs(north_value-south_value)<tolerance){
    finish();
    
  }
   if(abs(east_value-west_value)<tolerance2){
    finish2();
  }
  if(north_value<40 and south_value<40){
    finish();

  }

  if(east_value<40 and west_value<40){
    finish2();
  }
  

  Serial.print("east: ");
  Serial.print(east_value);
  Serial.print(" || ");
  Serial.print("west: ");
  Serial.print(west_value);
  Serial.print(" || ");
  Serial.print("north: ");
  Serial.print(north_value);
  Serial.print(" || ");
  Serial.print("south: ");
  Serial.print(south_value);
  Serial.println(" || ");

  
}
//------Motor Control For Tilt Angles------

void pwmOut(int out) {                               
  if (out > 0) {                           
    analogWrite(MotEnable, out);         
    forward();                          
  }
  else {
    analogWrite(MotEnable, abs(out));                    
    reverse();                           
  }
  

}

void forward () {
  digitalWrite(MotFwd, HIGH); 
  digitalWrite(MotRev, LOW); 
  
}

void reverse () {
  digitalWrite(MotFwd, LOW); 
  digitalWrite(MotRev, HIGH); 
  
}
void finish () {
  digitalWrite(MotFwd, LOW); 
  digitalWrite(MotRev, LOW); 
  
}

//------Motor Control For Azimuth Angles------


void pwmOut2(int out2) {                               
  if (out2 > 0) {                           
    analogWrite(MotEnable2, out2);         
    forward2();                          
  }
  else {
    analogWrite(MotEnable2, abs(out2));                    
    reverse2();                           
  }
  

}

void forward2 () {
  digitalWrite(MotFwd2, HIGH); 
  digitalWrite(MotRev2, LOW); 
  
}

void reverse2() {
  digitalWrite(MotFwd2, LOW); 
  digitalWrite(MotRev2, HIGH); 
  
}
void finish2 () {
  digitalWrite(MotFwd2, LOW); 
  digitalWrite(MotRev2, LOW); 
  
}
