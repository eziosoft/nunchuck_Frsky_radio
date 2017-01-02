#include "Wire.h"
#include "WiiChuck.h"
#include "ppm_generator.h"
#include "telemetry.h"
#include "functions.h"
#include "Expo.h"


//#define DEBUG
/////////////////////////////////////////////////
int MIN_PULSE = 1000;
int MAX_PULSE = 2000;

int BAT_LOW_V = 330; //3,3V

/////////////////////////////////////////////////

int zeroPitch = 0;
int zeroRoll = 0;

WiiChuck chuck = WiiChuck();
rc::Expo g_expo;


int vBat = 0;
int blinkLed = 1;

long timer1 = 0;

void setup() {
  pinMode(13, OUTPUT);

  Serial.begin(9600);
  chuck.begin();
  chuck.update();
  chuck.calibrateJoy();

  ppm_setup();
  telemetry_setup();

  // we use 30% expo, dumb down the sensitivity in the center a bit, if we
  // want to make it more twitchy around the center we use a negative value
  g_expo = 90;



  sound(440, 100);
  delay(100);
  sound(600, 100);
  delay(100);
  sound(440, 100);
}

void loop() {
  if (vBat > BAT_LOW_V) {
    digitalWrite(13, blinkLed);
    blinkLed = blinkLed == 1 ? 0 : 1;
  }
  else
  {
    digitalWrite(13, LOW);

  }

  delay(20); //critical for nunchuck do not remove
  chuck.update();

  int x = map(chuck.readJoyX(), 0, 256, -256, 256);//normalized values
  int y = map(chuck.readJoyY(), 0, 256, -256, 256);

  x = g_expo.apply(x); //applying expo
  y = g_expo.apply(y);

  ppm[0] = map(y, -256, 256, MIN_PULSE, MAX_PULSE);
  ppm[1] = map(x, -256, 256, MIN_PULSE, MAX_PULSE);

  ppm[2] = map(chuck.readRoll() - zeroRoll, -90, 90, MIN_PULSE, MAX_PULSE);
  ppm[3] = map(chuck.readPitch() - zeroPitch + 90, 0, 180, MAX_PULSE, MIN_PULSE);

  if (chuck.buttonZ) {
    ppm[4] = MAX_PULSE;
  } else  {
    ppm[4] = MIN_PULSE;
  }

  if (chuck.buttonC) {
    //    ppm[5] = MAX_PULSE;
    //chuck.calibrateJoy();
    zeroRoll = chuck.readRoll();
    zeroPitch = chuck.readPitch();
    sound(800, 100);
  } else  {
    //    ppm[5] = MIN_PULSE;
  }

  for (int i = 0; i < CHANNEL_NUMBER; i++)
  {
    if (ppm[i] < MIN_PULSE) ppm[i] = MIN_PULSE;
    if (ppm[i] > MAX_PULSE) ppm[i] = MAX_PULSE;
  }





  //telemetry
//        if (telemetry_loop()) {
//          Serial.print("TX RSSI: ");
//          Serial.println(frsky.getLink_up());
//          Serial.print("Telemetry RSSI: ");
//          Serial.println(frsky.getLink_dn());
//        }

 // telemetryTest();


  //every sec
  if (millis() > timer1) {
    timer1 = millis() + 1000;
    vBat = map(analogRead(A7), 0, 1023, 0, 500);
    if (vBat < BAT_LOW_V && vBat > 200) { //>200 to avoid beeping without battery
      sound(1000, 100);
    }

    if ((zeroPitch == 0) && (zeroRoll == 0)) {
      sound(800, 100);
    }
    //    Serial.print("vBat ");
    //    Serial.println(vBat);
  }



#ifdef DEBUG
  for (int i = 0; i < CHANNEL_NUMBER; i++)
  {
    Serial.print(ppm[i]);
    Serial.print(',');
  }
  Serial.println();

#endif

}







//  Serial.print((int)chuck.readAccelX());
//  Serial.print((int)chuck.readAccelY());
//  Serial.print((int)chuck.readAccelZ());

