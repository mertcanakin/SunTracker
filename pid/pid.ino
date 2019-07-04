#include <PID_v1.h>
int motor_p1 = 10;
int motor_p2 = 9;
int pwmPin = 11;

int current_angle;
int required_angle;
int error;
int remapped_error;
int east;
int west;
int total;

int ldr_ne=A3;
int ldr_se=A2;
int ldr_nw=A1;
int ldr_sw=A0;

int tolerance=5; 

 
//Define Variables we'll be connecting to
double Setpoint, Input, Output;
 
//Specify the links and initial tuning parameters
double Kp=2, Ki=5, Kd=1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
 
void setup()
{
 //initialize the variables we're linked to
  Serial.begin(9600);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(motor_p1, OUTPUT);
  pinMode(motor_p2, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetTunings(Kp, Ki, Kd);
}
 
void loop()
{
  int light_ne = analogRead(A3);
  int light_se = analogRead(A2); 
  int light_nw = analogRead(A1); 
  int light_sw = analogRead(A0);  

  east=(light_ne+light_se);
  west=(light_nw+light_sw);

  total=(east+west)/2;
  Setpoint=total;
  if((abs(east-west)<=tolerance) || (abs(west-east)<=tolerance)){}
  else{
    if(east<west){
      digitalWrite(motor_p1, HIGH);                      
    digitalWrite(motor_p2, LOW);
    }
    if(east>west){
      digitalWrite(motor_p1, LOW);                      
    digitalWrite(motor_p2, HIGH);
      
    }
  }
 Input = east;
 myPID.Compute();
 analogWrite(pwmPin, Output);
}
