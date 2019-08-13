//Sun Tracker System



#include <virtuabotixRTC.h>
#include <PID_v1.h>

#define MotEnable 9 
#define MotFwd  10  
#define MotRev  11 

#define MotEnable2 6 
#define MotFwd2  8
#define MotRev2  7

virtuabotixRTC myRTC(5, 4, 3);  //rst-->3 dat-->4 clk-->5
                      
int User_Input = 0;   //For getting tilt angles
int User_Input2 = 0;  //For getting azimuth angles

int RPV = 0;          // setpoint required potentiometer value(tilt)
int RPV2 = 0;         // setpoint required potentiometer value(azimuth)

int month_angle[]={10,15,20,25,30,35,45,55,65,70,75,85,90};  //tilt angles
int month_angle2[]={10,15,20,25,30,35,45,55,65,70,75,85,90}; //azimuth angles

double kp = 5 , ki = 1 , kd = 0.01;     //pid parameters

double input = 0, output = 0, setpoint = 0;     
double input2 = 0, output2 = 0, setpoint2 = 0;

PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);     //PID for tilt 
PID myPID2(&input2, &output2, &setpoint2, kp, ki, kd, DIRECT);  //PID for azimuth


void setup() {

  //Motor pins:
  pinMode(MotEnable, OUTPUT);
  pinMode(MotFwd, OUTPUT); 
  pinMode(MotRev, OUTPUT); 
  pinMode(MotEnable2, OUTPUT);
  pinMode(MotFwd2, OUTPUT); 
  pinMode(MotRev2, OUTPUT); 

  Serial.begin(9600);  //Opening serial port
  
  myRTC.setDS1302Time(00, 1, 1,7, 13, 8, 2019);   //Date
  
  TCCR1B = TCCR1B & 0b11111000 | 1;  // set 31KHz to prevent motor noise
  
  myPID.SetMode(AUTOMATIC);            //set pid in automatic mode
  myPID.SetSampleTime(1);              // refresh rate of pid controller
  myPID.SetOutputLimits(-125, 125);    // this is the max pwm value to move motor

  myPID2.SetMode(AUTOMATIC);  
  myPID2.SetSampleTime(1);  
  myPID2.SetOutputLimits(-75, 75); 
}

void loop() {
  myRTC.updateTime();   //updating time
  
  User_Input=month_angle[myRTC.month];     //Tilt angles by month
  User_Input2=month_angle2[myRTC.month];   //Azimuth angles by month
  
  RPV = map (User_Input, 0, 90, 577, 935);    //Mapping  
  RPV2 = map (User_Input2, 0, 90, 300, 700); 
              
  setpoint = RPV;      //Setpoint value
  setpoint2 = RPV2;
  
  input = analogRead(A0) ;     // data from potentiometer
  input2 = analogRead(A1) ;
  
  myPID.Compute();     // calculate new output
  myPID2.Compute();
  
  pwmOut(output);    
  pwmOut2(output2); 

  Serial.print("Tilt Pot: ");
  Serial.print(input);
  Serial.print(" || ");
  Serial.print("Azimuth Pot: ");
  Serial.print(input2);
  Serial.print(" || ");
  Serial.print("RPV: ");
  Serial.print(RPV);
  Serial.print(" || ");
  Serial.print("RPV2: ");
  Serial.println(RPV2);
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
