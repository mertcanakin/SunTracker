
double kp=2;
double ki=5;
double kd=1;
int bati;
int dogu;

int in1=9;
int in2=10;
int en=11;

int kuz_d=A1;
int kuz_b=A0;
int gun_d=A3;
int gun_b=A2;

unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double input, output;
int Setpoint;
double cumError, rateError;

void setup(){
  Serial.begin(9600);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
}
void loop(){
  int isik_kuzey_dogu = analogRead(A1);
  int isik_guney_dogu = analogRead(A3); 
  int isik_kuzey_bati = analogRead(A0); 
  int isik_guney_bati = analogRead(A2);  
  bati=(isik_kuzey_bati+isik_guney_bati)/2;
  dogu=(isik_kuzey_dogu+isik_guney_dogu)/2;
  input = analogRead(A4);                //read from rotary encoder connected to A0
  output = computePID(input);
        
        delay(100);
          if (dogu<bati){
            digitalWrite(in1, HIGH);                      
            digitalWrite(in2, LOW);
            analogWrite(11, output);
  }       if (dogu>bati){
             digitalWrite(in1, LOW);                      
             digitalWrite(in2, HIGH);
             analogWrite(11, output);
  }

         
        Serial.print(" Motor Angle= ");
        Serial.print (input);
        Serial.print("-----");
        Serial.print("Setpoint= ");
        Serial.println(Setpoint);
        delay(100);
        Serial.println(output);
}
double computePID(double inp){ 
        Setpoint=(dogu+bati)/2;  
        currentTime = millis();                //get current time
        elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
        
        error = Setpoint - inp;                                // determine error
        cumError += error * elapsedTime;                // compute integral
        rateError = (error - lastError)/elapsedTime;   // compute derivative
 
        double out = kp*error + ki*cumError + kd*rateError;                //PID output               
 
        lastError = error;                                //remember current error
        previousTime = currentTime;                        //remember current time
 
        return out;                                        //have function return the PID output
}
