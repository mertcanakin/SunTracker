#include <virtuabotixRTC.h>
#include <PID_v1.h>
#define MotEnable 9 
#define MotFwd  10  
#define MotRev  11 

virtuabotixRTC myRTC(5, 4, 3);  //rst-->3 dat-->4 clk-->5
                      
int User_Input = 0; 
int RPV = 0;          // setpoint required potentiometer value
int month_angle[]={10,15,20,25,30,35,45,55,65,70,75,85,90};  //tilt angles

double kp = 5 , ki = 1 , kd = 0.01;     //pid values
double input = 0, output = 0, setpoint = 0;
PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);  

void setup() {
  pinMode(MotEnable, OUTPUT);
  pinMode(MotFwd, OUTPUT); 
  pinMode(MotRev, OUTPUT); 
  Serial.begin(9600); 
  
  myRTC.setDS1302Time(00, 1, 1,7, 27, 11, 2016); //date    
  TCCR1B = TCCR1B & 0b11111000 | 1;  // set 31KHz , prevent motor noise
  
  myPID.SetMode(AUTOMATIC);   //set pid in automatic mode
  myPID.SetSampleTime(1);  // refresh rate of pid controller
  myPID.SetOutputLimits(-125, 125); // this is the max pwm value to move motor
}

void loop() {
  myRTC.updateTime();
  
  User_Input=month_angle[myRTC.minutes];
  RPV = map (User_Input, 0, 90, 577, 935); // mapping degree 
  
  Serial.print("this is RPV - "); 
  Serial.println(RPV);               

  setpoint = analogRead(A3);                    //PID while work to achive this value consider as SET value
  input = analogRead(A2) ;           // data from encoder consider as a Process value
  Serial.print("pot - ");
  Serial.println(User_Input);
  myPID.Compute();                 // calculate new output
  pwmOut(output);  
}
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
