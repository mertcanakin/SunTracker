#include <PID_v1.h>

const int mot_en=11; //enable pin
const int mot_fw=10; //forward pin
const int mot_bw=9; //backward pin

int user_input=0;
int enc_pin1=3;
int enc_pin2=4;
volatile int last_encoded=0;  //last value of encoder
volatile long enc_val=0;  //current value
int pulse_rev=1600;   //pulse revolution
int angle=360;     //maximum degree
int req_enc_val=0;  //required encoder value
int last_msb=0;  //most significant bit
int last_lsb=0;  //least significant bit


double kp = 5 , ki = 1 , kd = 0.01;             
double input = 0, output = 0, setpoint = 0;
PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);  

void setup(){
  pinMode(mot_en,OUTPUT);
  pinMode(mot_fw,OUTPUT);
  pinMode(mot_bw,OUTPUT);

  pinMode(enc_pin1,INPUT_PULLUP);
  pinMode(enc_pin2,INPUT_PULLUP);

  digitalWrite(enc_pin1,HIGH);
  digitalWrite(enc_pin2,HIGH);

  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);

  Serial.begin(9600);
  
  TCCR1B = TCCR1B & 0b11111000 | 1;  
  myPID.SetMode(AUTOMATIC);  
  myPID.SetSampleTime(1);  
  myPID.SetOutputLimits(-125, 125); 
}

void loop(){
  user_input=50; //you can change this value
  req_enc_val=map(user_input,0,360,0,1600);

  Serial.print("Required encoder value: ");
  Serial.println(req_enc_val);

  setpoint=req_enc_val;
  input=enc_val;

  Serial.print("Encoder Value: ");
  Serial.println(enc_val);

  myPID.Compute();
  pwmOut(output);
 
}

void pwmOut(int out) {                               
  if (out > 0) {                         
    analogWrite(mot_en, out);         
    forward();                          
  }
  else {
    analogWrite(mot_en, abs(out));                    
    reverse();                            
  }
}
void updateEncoder(){
  int MSB = digitalRead(enc_pin1); 
  int LSB = digitalRead(enc_pin2); 

  int encoded = (MSB << 1) |LSB; 
  int sum  = (last_encoded << 2) | encoded; 

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) enc_val ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) enc_val --;

  last_encoded = encoded; 

}

void forward () {
  digitalWrite(mot_fw, HIGH); 
 digitalWrite(mot_bw, LOW); 
  
}

void reverse () {
  digitalWrite(mot_fw, LOW); 
 digitalWrite(mot_bw, HIGH); 
  
}
void finish () {
  digitalWrite(mot_fw, LOW); 
 digitalWrite(mot_bw, LOW); 
  
}
