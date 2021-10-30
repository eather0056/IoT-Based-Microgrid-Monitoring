#include <Wire.h>
#include <ds3231.h>
#include<math.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(22, 23, 24, 25, 26, 27);

// Power fector variable decleation
float pulswidth = 0;
float powerfector = 0;
float phase = 0;

//Real power variable declearation
int x;
float V;
double AcsOffset=2.5;
double Sensibl=0.100; 
double I=0;
double Readvoltage=0;
double P;

// Reactive power Variable declearition
double Q;
double Reactivepower;

// Apparant Power Variable declearition
double S;
double  X;
double  y;
double B;
double D;

//Frequency variable declearation
const int pulsePin = 7; 
int pulseHigh;
int pulseLow; 
float pulseTotal;
float frequency;

// bills variable declaration 
double kw;
double kwh;
double Price;


// Time RTC
struct ts t; 
int inpt = 2;
int opt = 3 ;
const int temp = 10;
double T;

// 3 phase variable declaration 
double xold = 0;
float AkWh;
int s=0;

int V_1 =0;
int V_2 =0;  
int V_3 =0; 
 
int I_1 =0;  
int I_2 =0;  
int I_3 =0;  

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Energy Meter");
  pinMode(53, OUTPUT); 
  digitalWrite(53, HIGH);
  
  pinMode(A8,INPUT);
  pinMode(A9,INPUT);
  pinMode(pulsePin,INPUT);
  
  Wire.begin();
  DS3231_init(DS3231_INTCN);
    t.sec=0;
  pinMode(inpt,INPUT); 
  pinMode(opt,INPUT); 
 DS3231_set(t);
 
}

void loop() {
            // power fector
              pulswidth = pulseIn( 8 ,HIGH);
              phase = 2*180*50*pulswidth/1000000;           
              powerfector = cos(phase*3.1414/180);
              Serial.print("Phase     =");
              Serial.print(phase);
              Serial.println(" Degree");
              delay(500);
              Serial.print("Power Fector  =");
              Serial.println(powerfector);
              delay(400);
              
      // Voltages  
      x=analogRead(A8);
      V=(x* .304177);
      delay(500);
      Serial.print("Voltage  =");
      Serial.print(V); 
      Serial.println(" v");
      delay(400);
      
              // Current
              double Value = analogRead(A9);
              Readvoltage =(Value*5.0/1023);
              I=(Readvoltage-AcsOffset)/Sensibl;
              delay(500);
              Serial.print("Current  =");
              Serial.print(I);
              Serial.println(" Amp");
              delay(500);
              
      // Real Power
      P = V*I*powerfector;
      delay(400);
      Serial.print("Real Power   =");
      Serial.print(P);
      Serial.println(" Watt");
      delay(500);
      
              // Reactive Power
              Reactivepower = sin(phase*3.1414/180);
              Q = V*I*Reactivepower;
              Serial.print("Reactive Power   =");
              Serial.print(Q);
              Serial.println("  KVAR");
              
      // Apparant Power
      delay(400);
      X = pow(P, 2);
      y = pow(Q, 2);
      B = sqrt(X);
      D = sqrt(y);
      S =  B+D;
      Serial.print("Apparant Power   =");
      Serial.print(S);
      Serial.println("  KVA");
      
              // frequency 
              pulseHigh = pulseIn(pulsePin,HIGH);
              pulseLow = pulseIn(pulsePin,LOW);
              pulseTotal = pulseHigh + pulseLow; 
              frequency=1000000/pulseTotal;     
              Serial.print("Frequency   =");
              Serial.print(frequency);
              Serial.println(" Hz");
              delay(400);
              
      //RTC Time clocK
       DS3231_get(&t);
  
              //------Phase 1 Voltage --------- 
              V_1 = analogRead(A0); 
              float V0 = V_1*(5.0 / 1023.0);
              V0 = V0 * 220; 
   
      //------Phase 2 Voltage ---------
      V_2 = analogRead(A1); 
      float V1 = V_2 * (5.0 / 1023.0);
      V1 = V1 * 220;

            //------Phase 3 Voltage ---------
             V_3 = analogRead(A2); 
             float V2 = V_3 * (5.0 / 1023.0);
             V2 = V2 * 220;

    //------Phase 1 Current ---------
    I_1= analogRead(A4);
    float I0 = I_1 * (5.0 / 1023.0);
    I0 = I0 * 10;

            //------Phase 2 Current ---------
            I_2= analogRead(A5); 
            float I1 = I_2 * (5.0 / 1023.0);
            I1 = I1 * 10;

    //------Phase 3 Current ---------
    I_3= analogRead(A6); 
    float I2 = I_3 * (5.0 / 1023.0);
    I2 = I2 * 10;

            float P = V0*I0 + V1*I1 + V2*I2; 
                  P = P/1000;

   
         xold = t.sec;
      
      
            float KW = P* xold;
            KW = KW/3600;
            AkWh = KW + AkWh;
      
    
     if(digitalRead(inpt) == HIGH)
     {
      Serial.print("Time  =");
      Serial.print(t.sec);
      Serial.println("s");
      delay(400);
      Serial.print("Power  = ");
      Serial.print(P);
      Serial.println("W");
      delay(400);
      Serial.print("Power Kw  =");
      kw = P/1000;
      Serial.print(kw);
      Serial.println("kw");
      delay(400);
      Serial.print("Power kwh  =");
      T = t.sec;
      kwh = kw*(T/60);
      Serial.print(kwh);
      Serial.println("kwh");
      Price = kwh*15; 
      delay(400);
      Serial.print("Price    =");
      Serial.print(Price);
      Serial.println("Tk");
    }
    
        if(digitalRead(inpt)== LOW)
        {
        Serial.println("Nothing");
        }


Serial.println("///////////////3 Phase Information//////////////////");

  Serial.println(" ");  
  Serial.print("Voltage 1 = ");
  Serial.print(V0);
  Serial.println(" V");
  
  Serial.print("Voltage 2 = ");
  Serial.print(V1);
  Serial.println(" V");

  Serial.print("Voltage 3 = ");
  Serial.print(V2);
  Serial.println(" V");
  Serial.println(" ");
  Serial.print("Current 1 = ");
  Serial.print(I0);
  Serial.println(" AMPs");

  Serial.print("Current 2 = ");
  Serial.print(I1);
  Serial.println(" AMPs");
  
  Serial.print("Current 3 = ");
  Serial.print(I2);
  Serial.println(" AMPs");
  Serial.println(" ");    
  Serial.print("Power = ");
  Serial.print(P);
  Serial.println(" kW");
  
  Serial.print("Energy = ");
  Serial.print(AkWh);
  Serial.println(" kWh");

 Serial.println(" -------------- ");



      if (KW > 16){
       digitalWrite(53, LOW);
       delay(5000);
      }
      if (KW <= 16){
       digitalWrite(53, HIGH);
       
      }
      
        lcd.setCursor(0, 1);
        lcd.print(P);
        lcd.print("kW");
        lcd.print("-");
        lcd.print(AkWh);
        lcd.print("kWh");
        delay(2000);
}
