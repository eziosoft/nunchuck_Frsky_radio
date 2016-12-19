//test
#include "Wire.h"
#include "WiiChuck.h"
#include "ppm_generator.h"
#include "telemetry.h"

/////////////////////////////////////////////////
#define MIN_PULSE = 1000;
#define MAX_PULSE = 2000;
/////////////////////////////////////////////////

WiiChuck chuck = WiiChuck();

void setup() {
  Serial.begin(115200);
  chuck.begin();
  chuck.update();
  chuck.calibrateJoy();

  ppm_setup();
  telemetry_setup();
}

void loop() {
  delay(20);
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

  for (int i = 0; i < CHANNEL_NUMBER; i++)
  {
    Serial.print(ppm[i]);
    if (i != CHANNEL_NUMBER - 1)
    {
      Serial.print(',');
    }
  }
  Serial.println();




  //telemetry
  if (telemetry_loop()) {
    Serial.print("TX RSSI: ");
    Serial.println(frsky.getLink_up());
    Serial.print("Telemetry RSSI: ");
    Serial.println(frsky.getLink_dn());
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
  //  if (chuck.buttonZ) {
  //    Serial.print("Z");
  //  } else  {
  //    Serial.print("-");
  //  }

  //  if (chuck.buttonC) {
  //    Serial.print("C");
  //  } else  {
  //    Serial.print("-");
  //  }






}
