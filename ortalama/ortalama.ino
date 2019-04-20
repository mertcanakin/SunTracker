#include <Servo.h>
Servo yatay_servo;
Servo dusey_servo;

int pos_y=90;
int pos_d=0;
int d_home=0;
int y_home=90;
int tolerans=5;

int ldr_kuzey_dogu=A1;
int ldr_guney_dogu=A2;
int ldr_kuzey_bati=A3;
int ldr_guney_bati=A4;


void setup() {
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);

  yatay_servo.attach(3);
  dusey_servo.attach(5);
  Serial.begin(9600); 
  dusey_servo.write(0);
  yatay_servo.write(90);
}

void loop() {

  int isik_kuzey_dogu = analogRead(A1);
  int isik_guney_dogu = analogRead(A2); 
  int isik_kuzey_bati = analogRead(A3); 
  int isik_guney_bati = analogRead(A4);  
  /*
  Serial.print("Kuzey Dogu: ");
  Serial.print(isik_kuzey_dogu);
  Serial.print(" | ");
  Serial.print("Guney Dogu: ");
  Serial.print(isik_guney_dogu);
  Serial.print(" | ");
  Serial.print("Kuzey Bati: ");
  Serial.print(isik_kuzey_bati);
  Serial.print(" | ");
  Serial.print("Guney Bati: ");
  Serial.println(isik_guney_bati);
  delay(50); */

  int dogu=(isik_kuzey_dogu+isik_guney_dogu)/2;
  int bati=(isik_kuzey_bati+isik_guney_bati)/2;

  int kuzey=(isik_kuzey_dogu+isik_kuzey_bati)/2;
  int guney=(isik_guney_dogu+isik_guney_bati)/2;

  int toplam=isik_kuzey_dogu+isik_guney_dogu+isik_kuzey_bati+isik_guney_bati;
  
  if((abs(dogu-bati)<=tolerans) || (abs(bati-dogu)<=tolerans)){}
  else{
    if(dogu>bati){
      pos_y=++pos_y;
    }
    if(dogu<bati){
      pos_y=--pos_y;
    }
  }
  
  if((abs(kuzey-guney)<=tolerans) || (abs(guney-kuzey)<=tolerans)){}
  else{
    if(kuzey>guney){
      pos_d=--pos_d;
    }
    if(kuzey<guney){
      pos_d=++pos_d;
    }
  }
  

  yatay_servo.write(pos_y); 
  delay(10);
  dusey_servo.write(pos_d); 
  delay(10);

  if(pos_d > 90) { pos_d = 90; } 
  if(pos_d < 10) { pos_d = 10; } 
  if(pos_y > 160) { pos_y = 160; } 
  if(pos_y < 10) { pos_y = 10; } 

if(toplam<100){
if(d_home != pos_d){
  dusey_servo.write(d_home);
  pos_d=d_home;
}
if(y_home != pos_y){
  yatay_servo.write(y_home);
  pos_y=y_home;
}

}


}
