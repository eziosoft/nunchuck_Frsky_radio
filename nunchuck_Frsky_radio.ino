#include "Wire.h"
#include "WiiChuck.h"
#include "ppm_generator.h"
#include "telemetry.h"

WiiChuck chuck = WiiChuck();

void setup() {
  //nunchuck_init();
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

  ppm[0] = map(chuck.readJoyY(), 0, 256, 1000, 2000);
  ppm[1] = map(chuck.readJoyX(), 0, 256, 1000, 2000);

  ppm[2] = map(chuck.readRoll(), -90, 90, 1000, 2000);
  ppm[3] = map(chuck.readAccelY(), 0, 180, 1000, 2000);

  for (int i = 0; i < CHANNEL_NUMBER; i++)
  {
    if (ppm[i] < 1000) ppm[i] = 1000;
    if (ppm[i] > 2000) ppm[i] = 2000;
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
