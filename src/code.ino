#include <Servo.h>
Servo steer;

#define ina 2
#define inb 3
#define en 4
#define pwm 5
#define steering 6
#define recho 7
#define rtrig 8
#define ctrig 9
#define cecho 10
#define ltrig 11
#define lecho 12
#define startbutton 13
#define s0 A0      
#define s1 A1
#define s2 A2
#define s3 A3
#define out A4
#define cs A5
#define rgreen A7


int repeatstart = 1 ;
int repeatturn = 0 ;
int repeatMove = 0;
int reason = 0 ;
int data;
int turns=0;
int finalspeed=0;
int c ;
long newtime=0;
long movetime=0;
long vg=0;
long before=0;
long after=0;
int lastc=0;
int dontstop=0;
char col;



  float rdis;
  float cdis;
  float ldis;

void setup() 
{
  pinMode(ina,OUTPUT);
  pinMode(inb,OUTPUT);
  pinMode(en,OUTPUT);
  pinMode(pwm,OUTPUT);
  pinMode(rtrig,OUTPUT);
  pinMode(recho,INPUT);
  pinMode(ctrig,OUTPUT);
  pinMode(cecho,INPUT);
  pinMode(ltrig,OUTPUT);
  pinMode(lecho,INPUT);
  pinMode(s0,OUTPUT);   
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);
  steer.attach(steering);
    
  Serial.begin(9600);
  digitalWrite(en,1);
  digitalWrite(s0,1);
  digitalWrite(s1,0);
  forward(0,0);
  delay(2000);
}





void loop()
{
  //test ('u');
  start();
//    ultra();int r = rdis;int l = ldis;
//     c = (((r - l)*1)+lastc*2);if (c>30){c = 30 ;}if (c < -30){c = -30 ;}lastc=(r-l);
//  forward(100,c);

}



void start ()
{
    while ( repeatstart ==1 )
  {ultra();int r = rdis;int l = ldis;
  if (reason==2 || repeatturn==0){if (r > 100 && cdis < 90){repeatstart = 0 ;reason = 2 ;repeatturn=1;}}
  if (reason==4 || repeatturn==0){if ( l > 100 && cdis < 90 ){repeatstart = 0 ;reason = 4 ;repeatturn=1;}}
   c = (((r - l)*1)+lastc*2);if (c>20){c = 20 ;}if (c < -20){c = -20 ;}lastc=(r-l);
  forward(100,c);Stop();}
ultra();
if (cdis<50){newtime=newtime+1500;forward(0,0); delay (200); forward(-60,0); delay(550);forward(0,0); delay(200); }
while (cdis < 100){ultra(); Stop();if ( reason == 2 ){forward (100,100);}else if (reason == 4){forward (100,-100);}}
if ( reason == 2 ){Move (100,100,150);}else if (reason == 4){Move (100,-100,250);}


Move(100,0,500);
ultra();
if(reason==2){while(rdis>90){ultra();forward(100,0);}}
else if(reason==4){while(ldis>90){ultra();forward(100,0);}}
repeatstart =1;


}


int forward (int Speed, int dir){
    if(dir==0){steer.write(65);}
    else if(dir>0){int degree=map(dir, 1, 100, 65, 0);steer.write(degree);}
    else if(dir<0){int degree=map(dir, -1, -100, 65, 130);steer.write(degree);}
    if(Speed==0){finalspeed=0;}
    else if(Speed>0){finalspeed= map(Speed, 1, 100, 1, 255);digitalWrite(ina,0);digitalWrite(inb,1);}
    else if(Speed<0){finalspeed= map(Speed, -1, -100, 1, 255);digitalWrite(ina,1);digitalWrite(inb,0);}
    analogWrite(pwm,finalspeed);
}
            
void ultra(){
  digitalWrite(rtrig,0);
  delay(5);
  digitalWrite(rtrig,1);
  delayMicroseconds(10);
  digitalWrite(rtrig,0);
  float rT=pulseIn(recho,1,10000);
  rdis=rT*0.034/2;

  digitalWrite(ctrig,0);
  delay(5);
  digitalWrite(ctrig,1);
  delayMicroseconds(10);
  digitalWrite(ctrig,0);
  float cT=pulseIn(cecho,1,10000);
  cdis=cT*0.034/2;

  digitalWrite(ltrig,0);
  delay(5);
  digitalWrite(ltrig,1); 
  delayMicroseconds(10);
  digitalWrite(ltrig,0);
  float lT=pulseIn(lecho,1,10000);
  ldis=lT*0.034/2;
  if(rdis==0){rdis=160;}if(cdis==0){cdis=160;}if(ldis==0){ldis=160;}
}

void color(){
   digitalWrite(s2,1);
   digitalWrite(s3,0);
   int W=pulseIn(out,0); 
   if (W>800){col='o';}
   else{col='w';}  
}

void test(char t){
  if(t=='c'){
   digitalWrite(s2,0);     
   digitalWrite(s3,0);
   Serial.print("R= "); 
   int R=pulseIn(out,0); 
   Serial.print(R); 

   digitalWrite(s2,1);
   digitalWrite(s3,1);
   Serial.print("     G= ");
   int G=pulseIn(out,0); 
   Serial.print(G); 

   digitalWrite(s2,0);
   digitalWrite(s3,1);
   Serial.print("     B= ");
   int B=pulseIn(out,0); 
   Serial.print(B); 
      
   digitalWrite(s2,1);
   digitalWrite(s3,0);
   Serial.print("     W= ");
   int W=pulseIn(out,0); 
   Serial.println(W);
    if (W>800){Serial.println('O');col='o'; }
    else{Serial.println('W');col='w'; }
  }else if(t=='u')
  {
    ultra();
    Serial.print(ldis);
    Serial.print("      ");
    Serial.print(cdis);
    Serial.print("      ");
    Serial.println(rdis);
  }
}

void Move(int Sp, int Dir, int Tim){
  movetime=millis();
  while((millis()-movetime)<Tim){
  forward(Sp, Dir);
  Stop();}
}

void Stop(){
      color();
      ultra();
      if (rdis<15 && reason == 4)
      {dontstop=1;}else{dontstop=0;}
      if (col=='o' && ((millis()-newtime)>2500) && dontstop==0){
        turns++;
        newtime=millis();
      }
      if(turns>11)
      {
        if(reason==2){forward(100,70);delay(350);}else{forward(100,-100);delay(400);}
        forward(100,0);
        delay(1600);
        forward(0,0);
        delay(1000000);
      }
   

}

void readraspi(){
  vg=0;
for(int i=0; i<200; i++)
{
   vg=vg+analogRead(rgreen);
}
vg=vg/200;
}

void raspi(){
  readraspi();
    if(vg>200 && vg<530)
      {before=millis();
       while(vg>200 && vg<530){
        raspi();
         forward(60,-100);
       }
       after=millis();
       forward(60,-100);
       delay(100);
       ultra();
       while(rdis>20){ultra();forward(60,0);}
       forward(80,0);
       delay(200);
       forward(60,100);
       delay(after-before);
 
      }else{forward(0,0);}
}
