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

void setup() {
  Serial.begin(9600);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(motor_p1, OUTPUT);
  pinMode(motor_p2, OUTPUT);
  pinMode(pwmPin, OUTPUT);
}

void loop() {

  int light_ne = analogRead(A3);
  int light_se = analogRead(A2); 
  int light_nw = analogRead(A1); 
  int light_sw = analogRead(A0);  
 /* 
  Serial.print("North East: ");
  Serial.print(light_ne);
  Serial.print(" | ");
  Serial.print("South East: ");
  Serial.print(light_se);
  Serial.print(" | ");
  Serial.print("North West: ");
  Serial.print(light_nw);
  Serial.print(" | ");
  Serial.print("South West: ");
  Serial.println(light_sw);
  delay(50);
*/
  east=(light_ne+light_se);
  west=(light_nw+light_sw);

  total=(east+west)/2;
 // int north=(light_ne+light_nw)/2;
 // int south=(light_se+light_sw)/2;
 Serial.println(total);
  read_current_angle();
  read_required_angle();
   if((abs(east-west)<=tolerance) || (abs(west-east)<=tolerance)){}
  else{
    if(east>west){
      digitalWrite(motor_p1, HIGH);                      
    digitalWrite(motor_p2, LOW);
    }
    if(east<west){
      digitalWrite(motor_p1, LOW);                      
    digitalWrite(motor_p2, HIGH);
      
    }
  }

  calc_error(current_angle,required_angle);
  analogWrite(pwmPin,remapped_error);
  
}

void calc_error(int current_angle,int required_angle){
  error=abs(current_angle-required_angle);
  remapped_error=map(error,60,120,200,255);
  if(remapped_error<tolerance){
    remapped_error=0;
  }}
void read_current_angle() {
  int non_current_angle = analogRead(A4);
  current_angle=map(non_current_angle,0,1023,60,120);
  if (current_angle<200){
   current_angle=200; 
  } if (current_angle>800) {
   current_angle=800; 
  
}}

void read_required_angle() {
  required_angle = total;
  if (required_angle<60){
   required_angle=60; 
  } if (required_angle>120) {
   required_angle=120; 
  
}}
