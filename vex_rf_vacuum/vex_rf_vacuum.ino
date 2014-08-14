#include <Servo.h> 

/* M1 Speed Control */
#define E1  5
/* M2 Speed Control */
#define E2  6
/* M1 Direction Control */
#define M1  4
/* M1 Direction Control */
#define M2  7

#define DEFAULT_VAC_SPEED 150
#define MAX_VAC_SPEED     255

/* VEX FM Transmitter has 6 channels */
#define NUM_OF_CHANNELS 6 
#define RF_PORT         3

Servo MotorL;  
Servo MotorR;                
 
int pos = 200;    
int pos2 = -170;
int chan_val[NUM_OF_CHANNELS];
int vac_speed = DEFAULT_VAC_SPEED;


void setup()
{
  /* Serial Begin */
  Serial.begin(57600);
 
  /* Vex RF input port */ 
  pinMode(RF_PORT, INPUT);
  
  /* Vaccum control */
  pinMode(E1, OUTPUT); 
  pinMode(E2, OUTPUT); 
  pinMode(M1, OUTPUT); 
  pinMode(M2, OUTPUT); 
  
  MotorL.attach(10);
  MotorR.attach(11);
}

void loop()
{
  /* Wait for the beginning of the frame */
  while(pulseIn(RF_PORT, LOW) < 5000){}
  
  /* Loop to store all the channel position */
  for (int x = 0; x <= NUM_OF_CHANNELS-1; x++)
  {
    chan_val[x] = pulseIn(RF_PORT, LOW);
  }
  
  /* Loop to print and clear all the channel readings */
  for (int x = 0; x<= NUM_OF_CHANNELS-1; x++)
  {
    /* R-Joystick, Right cmd */
    if (chan_val[0] > 1200) {
      Serial.print("RJ: Right command: ");
      Serial.println(chan_val[0]);
      MotorL.write(pos2);
    }
    /* R-Joystick, Left cmd */
    else if (chan_val[0] < 1100 && chan_val[0] != 0) {
      Serial.print("RJ: Left command: ");
      Serial.println(chan_val[0]);
      MotorR.write(pos);
    }  
    /* R-Joystick, Down cmd */
    else if (chan_val[1] >= 1000) {
      Serial.print("RJ: Down command: ");
      Serial.println(chan_val[1]); //Print the chan_val
      MotorL.write(pos);
      MotorR.write(pos2);
    }
    /* R-Joystick, Up cmd */
    else if (chan_val[1] <= 800 && chan_val[1] != 0) {
      Serial.print("RJ: Up command: ");
      Serial.println(chan_val[1]);
      MotorL.write(pos2);
      MotorR.write(pos);
    }
    else {
      /* Stop motor */
      MotorL.write(90);
      MotorR.write(90);
    }
    
    /* L-Joystick, Down cmd */
    if (chan_val[2] >= 1000) {
      Serial.print("LJ: Down command: ");
      Serial.println(chan_val[2]); //Print the chan_val
      
      vac_speed -= 10;
      if (vac_speed <= DEFAULT_VAC_SPEED) {
        vac_speed = DEFAULT_VAC_SPEED;
        analogWrite(E1, vac_speed);
        analogWrite(E2, vac_speed);
        digitalWrite(M1, LOW);
        digitalWrite(M2, LOW);
        Serial.println("off");
      }
      else {
    
        analogWrite(E1, vac_speed);
        analogWrite(E2, vac_speed);
        digitalWrite(M1, HIGH);
        digitalWrite(M2, LOW);
      }
    }
    /* L-Joystick, Up cmd */
    else if (chan_val[2] <= 800 && chan_val[2] != 0) {
      Serial.print("LJ: Up command: ");
      Serial.println(chan_val[2]); //Print the chan_val
      
      analogWrite(E1, vac_speed);
      analogWrite(E2, vac_speed);
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      
      vac_speed += 10;
      if (vac_speed >= MAX_VAC_SPEED)
        vac_speed = MAX_VAC_SPEED;
    }
    else {
      analogWrite(E1, 0);
      analogWrite(E2, 0);
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      Serial.println("off");
    }
    /*
    else if (chan_val[3] >= 1000) { //L-Joystick, Right cmd
      Serial.print("LJ: Right command: ");
      Serial.println(chan_val[3]); //Print the chan_val
    }
    
    else if (chan_val[3] <= 800 && chan_val[3] != 0) { //L-Joystick, Left cmd
      Serial.print("LJ: Left command: ");
      Serial.println(chan_val[3]); //Print the chan_val
    }*/
   /* else {
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      
      vac_speed = DEFAULT_VAC_SPEED;
    }*/
    
    //Serial.print(chan_val[x]); //Print the chan_val
    //Serial.print(" ");
   // chan_val[x]=0; //Clear the chan_val afeter is printed
  }
}

