#include "Wire.h"
#include "WiiChuck.h"
#include "ppm_generator.h"
#include "telemetry.h"

/////////////////////////////////////////////////
int MIN_PULSE = 1000;
int MAX_PULSE = 2000;
/////////////////////////////////////////////////

WiiChuck chuck = WiiChuck();

int vBat = 0;
int blinkLed = 1;

long timer1 = 0;

void setup() {
  pinMode(13, OUTPUT);

  Serial.begin(115200);
  chuck.begin();
  chuck.update();
  chuck.calibrateJoy();

  ppm_setup();
  telemetry_setup();
}

void loop() {
  if (vBat > 330) {
    digitalWrite(13, blinkLed);
    blinkLed = blinkLed == 1 ? 0 : 1;
  }
  else
  {
    digitalWrite(13, LOW);
  }

  delay(20); //critical for nunchuck do not remove
  chuck.update();

  ppm[0] = map(chuck.readJoyY(), 0, 256, MIN_PULSE, MAX_PULSE);
  ppm[1] = map(chuck.readJoyX(), 0, 256, MIN_PULSE, MAX_PULSE);

  ppm[2] = map(chuck.readRoll(), -90, 90, MIN_PULSE, MAX_PULSE);
  ppm[3] = map(chuck.readAccelY(), 0, 180, MIN_PULSE, MAX_PULSE);

  if (chuck.buttonZ) {
    ppm[4] = MAX_PULSE;
  } else  {
    ppm[4] = MIN_PULSE;
  }

  if (chuck.buttonC) {
    ppm[5] = MAX_PULSE;
  } else  {
    ppm[5] = MIN_PULSE;
  }

  for (int i = 0; i < CHANNEL_NUMBER; i++)
  {
    if (ppm[i] < MIN_PULSE) ppm[i] = MIN_PULSE;
    if (ppm[i] > MAX_PULSE) ppm[i] = MAX_PULSE;
  }


  if (false) {
    for (int i = 0; i < CHANNEL_NUMBER; i++)
    {
      Serial.print(ppm[i]);
      if (i != CHANNEL_NUMBER - 1)
      {
        Serial.print(',');
      }
    }
    Serial.println();
  }


  //telemetry
  //      if (telemetry_loop()) {
  //        Serial.print("TX RSSI: ");
  //        Serial.println(frsky.getLink_up());
  //        Serial.print("Telemetry RSSI: ");
  //        Serial.println(frsky.getLink_dn());
  //      }

  //telemetryTest();


  //battery
  if (millis() > timer1) {
    timer1 = millis() + 10000;
    vBat = map(analogRead(A7), 0, 1023, 0, 500);
    Serial.print("vBat ");
    Serial.println(vBat);
  }



}

//  Serial.print(chuck.readJoyX());
//  Serial.print(", ");
//  Serial.print(chuck.readJoyY());
//  Serial.print(", ");
//  Serial.print(chuck.readRoll());
//  Serial.print(", ");
//  Serial.print(chuck.readPitch());


//  Serial.print((int)chuck.readAccelX());
//  Serial.print((int)chuck.readAccelY());
//  Serial.print((int)chuck.readAccelZ());

