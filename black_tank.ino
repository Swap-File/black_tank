/*To control the rover, Copy and paste the code below into the Arduino software*/
int E1 = 6; //M1 Speed Control
int E2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control

#include <Wire.h>
#include <ArduinoNunchuk.h>

ArduinoNunchuk nunchuk = ArduinoNunchuk();

void setup(void)
{
  int i;
  for (i = 5; i <= 8; i++) pinMode(i, OUTPUT);
  Serial.begin(115200);
  Serial.println("setup");
  nunchuk.init();

}

int leftspeed = 0; //255 is maximum speed
int rightspeed = 0;
boolean leftdirection = false;
boolean rightdirection = false;

void loop(void)
{

  nunchuk.update();

  if (nunchuk.pluggedin) {

    int tempx = nunchuk.analogX;
    int tempy = nunchuk.analogY;

    tempx  -= 128;
    tempy -= 128;

    int mainspeed = abs(tempy) * 2;
    int altspeed = (1 - (((float)abs(tempx) * 2)) / 256) *  mainspeed;

    int mainspeedb = abs(tempx) * 2;
    int altspeedb = (1 - (((float)abs(tempy) * 2)) / 256) *  mainspeedb;

    Serial.print(nunchuk.analogAngle);
    Serial.print(' ');
    Serial.print(mainspeed);
    Serial.print(' ');
    Serial.print(altspeed);
    Serial.print(' ');
    Serial.print(mainspeedb);
    Serial.print(' ');
    Serial.print(altspeedb);

    mainspeed = constrain(mainspeed , 0, 255);
    altspeed = constrain(altspeed , 0, 255);
    if (mainspeed == 254) mainspeed = 255;
    if (altspeed == 254) altspeed = 255;

   mainspeedb = constrain(mainspeedb , 0, 255);
    altspeedb = constrain(altspeedb , 0, 255);
    if (mainspeedb == 254) mainspeedb = 255;
    if (altspeedb == 254) altspeedb = 255;
    
    leftspeed = 0;
    rightspeed = 0;

    if (nunchuk.analogAngle < 0 && nunchuk.analogAngle >= -45) {   //left
      
      rightdirection = false ;
      leftdirection =  true ;
      rightspeed = mainspeedb;
      leftspeed = altspeedb;
      
    } else if (nunchuk.analogAngle < -45  &&  nunchuk.analogAngle >= -90) { //fwd left
      rightdirection = false;
      leftdirection = false;
      leftspeed = altspeed;
      rightspeed = mainspeed;
    } else if (nunchuk.analogAngle < -90  &&  nunchuk.analogAngle >= -135) { //fwd right
      rightdirection = false;
      leftdirection = false;
      leftspeed = mainspeed;
      rightspeed = altspeed;
    } else if (nunchuk.analogAngle < -135  &&  nunchuk.analogAngle >= -180) {  //right

      rightdirection = true;
      leftdirection = false;
      leftspeed = mainspeedb;
      rightspeed = altspeedb;

    } else if (nunchuk.analogAngle >= 0  &&  nunchuk.analogAngle < 45) {  //left
       rightdirection = false ;
      leftdirection =  true ;
      rightspeed = mainspeedb;
      leftspeed = altspeedb;
      
    } else if (nunchuk.analogAngle >= 45  &&  nunchuk.analogAngle < 90) { //back left
      rightdirection = true;
      leftdirection = true;
      leftspeed = altspeed;
      rightspeed = mainspeed;
    } else if (nunchuk.analogAngle >= 90  &&  nunchuk.analogAngle < 135) { //back right
      rightdirection = true;
      leftdirection = true;
      leftspeed = mainspeed;
      rightspeed = altspeed;
    } else if (nunchuk.analogAngle >= 135  &&  nunchuk.analogAngle < 180) {  //right
      rightdirection = true;
      leftdirection = false;
      leftspeed = mainspeedb;
      rightspeed = altspeedb;
    }

if (nunchuk.zButton){
  rightdirection = !rightdirection;
}
if (nunchuk.cButton){
    leftdirection = !leftdirection;
}

    Serial.print(' ');
    Serial.print(leftspeed);
    Serial.print(' ');
    Serial.println(rightspeed);

  } else {

    leftspeed = 0;
    rightspeed = 0;
  }

  digitalWrite(M1, leftdirection);
  digitalWrite(M2, rightdirection);
  analogWrite (E1, leftspeed);
  analogWrite (E2, rightspeed);

  delay(20);

}

