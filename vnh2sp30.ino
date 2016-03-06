
/* for pin connections on motor shield please refer to:
VNH2SP30 SparkFun : https://www.sparkfun.com/products/10182
VNH2SP30 Data sheet: http://cdn.sparkfun.com/datasheets/Dev/Arduino/Shields/10832.pdf 
VNH2SP30 Schematics: http://cdn.sparkfun.com/datasheets/Dev/Arduino/Shields/MonsterMoto-Shield-v12.pdf

You may also reffer to my second video for system explenation

Motor Board breakout pins
A0 - EN1 need to be H D4 - INA-2 - motor 2 direction and Break
A1 - EN2 need to be H D5 - PWM-1 - motor 1 PWM from controller
A2 - CS1 current Sense D6 - PWM-2 - motor 2 PWM from controller
A3 - CS2 current Sense D7 - INA-1 - motor 1 direction and Break
5 VDC D8 - INB-1 - motor 1 direction and Break
GND D9 - INB-2 - motor 2 direction and Break
Vcc 5.5 - 16Vdc to drivers and motors.
*/
int potPin_NS = A0; // Notrh/South Potentiometer input to A0.
int potValue_NS = 0; // Notrh/South Potentiometer Value .

int potPin_LR = A1; // Left/Right Potentiometer input to A1.
int potValue_LR = 0; // Left/Right Potentiometer Value at pin A1.

//Right Motor
int dir_Right_A1 = 3; // Direction/Break command pin 3.
int dir_Right_B1 = 4; // Direction/Break command pin 4.
int Right_Wheel_Pin = 5; // PWM Output on pin 5 NS==Right, LR==Left.
int Right_Wheel_Value=0; // PWM value at pin 5.

int Speed_Value =0; // Speed_Value for both Left and Right wheels.

//Left Motor 
int dir_Left_A2 = 7; // Direction/Break command pin 7.
int dir_Left_B2 = 8; // Direction/Break command pin 8.
int Left_Wheel_Pin = 9; // PWM Output on pin 9.
int Left_Wheel_Value = 0; // PWM value at pin 9.

int K_right = 0; // analog RIGHT multiplier.
int K_left = 0; // analog LEFT multiplier.
int KK_right = 0; // analog RIGHT Wheeel Corrected Value.
int KK_left = 0; // analog LEFT Wheel corrrected Value.

void setup() 
{
stop (); //stop rutine.
FORWARD (); //Forwad rutine.
REVERSE (); //Reverse rutine.
BREAK (); //Break rutine.

pinMode (potPin_NS, INPUT); // analog input north/south or forward/revers.
pinMode (dir_Right_A1, OUTPUT); // Both A1 & B1 for Right-Motor, are needed for motor direction,
pinMode (dir_Right_B1, OUTPUT);
pinMode (Right_Wheel_Pin, OUTPUT); // PWM Right wheel.

pinMode (potPin_LR, INPUT); // analog input left/right direction.
pinMode (dir_Left_A2, OUTPUT); // Both A1 & B1 for Left-Motor, are needed for motor direction.
pinMode (dir_Left_B2, OUTPUT);
pinMode (Left_Wheel_Pin, OUTPUT); // PWM for left wheel.

Serial.begin (9600);

}

void loop() 
{ 
  //--------------- LR routin ---------------
  potValue_NS = analogRead (potPin_NS);
  potValue_LR = analogRead (potPin_LR); 

  if ((potValue_NS >=501 && potValue_NS <=520)&&(potValue_LR >=501 && potValue_LR <=520)) //dead band of potentiometers at center.
  {
    stop(); //start "stop" routine.
    delay (50); //delat 50mSec. for soft break.
    BREAK (); // "break" both motors.
  }
  
  if (potValue_LR <=500) // JoyStick LEFT Shifted. LEFT=0<<======|500-520|-------1024=RIGHT
  {
    K_right = map (potValue_LR, 500, 385, 0, 180); // set right motor potentiometer value from 500-385 to 0-220 accordinglly.
  }
  
  if (potValue_LR >=521) // JoyStic RIGHT shifted. LEFT=0--------|500-520|=======>>1024=RIGHT
  {
    K_left=map (potValue_LR, 521, 640, 0, 180); // set right motor potentiometer value from 500-385 to 0-220 accordinglly.
  }
  
  // --------------- OPERATION routin ---------------
  if (potValue_NS >=521) // 520|=====>>1024=FORWARD
  {
    FORWARD();
  }
  
  if (potValue_NS <=500) // REVERSE = 0<<=====501
  {
    REVERSE ();
  }
    //Serial.println ("L/R potValue:");
    //Serial.println (potPin_NS);
    //delay (100);
    // Serial.println ("LEFT WHEEL:");
    Serial.println (Left_Wheel_Pin);
    // delay (100);
}

void stop()
{
  analogWrite (Right_Wheel_Value, 0); //pwm out pin 5
  analogWrite (Left_Wheel_Value, 0); //pwm out pin 9 
}
  
void FORWARD ()
{
  digitalWrite (dir_Right_A1, HIGH); //FORWARD right wheel
  digitalWrite (dir_Right_B1, LOW); //FORWARD right wheel
  digitalWrite (dir_Left_A2, LOW); //FORWARD left wheel
  digitalWrite (dir_Left_B2, HIGH); //FORWARD left wheel 
  
  Right_Wheel_Value=map (potValue_NS, 521, 645, 0, 254); 
  Left_Wheel_Value =map (potValue_NS, 521, 645, 0, 254);
  
  KK_right = (Right_Wheel_Value - K_right); // Right wheel forward-command - (minus) stick right-command for turning.
  
  if (KK_right < 0) // combind, calculated value for wheel.
  {
    analogWrite (Right_Wheel_Pin, 0);
  }
  else 
  {
    analogWrite( Right_Wheel_Pin , KK_right);
  } 
  
  KK_left = (Left_Wheel_Value - K_left);
  
  if (KK_left < 0)
  {
    analogWrite (Left_Wheel_Pin, 0);
  }
  
  else 
  {
  analogWrite( Left_Wheel_Pin, KK_left);
  } 
}
  
  
void REVERSE()
{
  digitalWrite (dir_Right_A1, LOW); //REVERSE right wheel
  digitalWrite (dir_Right_B1, HIGH); //REVERSE right wheel
  digitalWrite (dir_Left_A2, HIGH); //REVERSE left wheel
  digitalWrite (dir_Left_B2, LOW); //REVERSE left wheel
  
  Right_Wheel_Value=map (potValue_NS, 500, 370, 0, 254);
  Left_Wheel_Value =map (potValue_NS, 500, 370, 0, 254);
  
  KK_right = Right_Wheel_Value - K_right; 
  
  if (KK_right <0)
  {
    analogWrite (Right_Wheel_Pin, 0);
  } 
  
  else {
    analogWrite( Right_Wheel_Pin , KK_right);
  }
  
  KK_left =(Left_Wheel_Value - K_left);
  
  if (KK_left < 0)
  {
    analogWrite (Left_Wheel_Pin, 0);
  }
  
  else 
  {
  analogWrite (Left_Wheel_Pin, KK_left);
  }
}


void BREAK ()
{
  digitalWrite (dir_Right_A1, LOW); //Break right wheel
  digitalWrite (dir_Right_B1, LOW); //Break right wheel
  digitalWrite (dir_Left_A2, LOW); //Break left wheel
  digitalWrite (dir_Left_B2, LOW); //Break left wheel 
}
