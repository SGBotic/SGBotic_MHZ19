#include <SGBotic_MHZ19.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);  // RX, TX . You can choose other pins if you prefer.
SGBotic_MHZ19 co2 = SGBotic_MHZ19(&mySerial);

// Hardware serial,
//SGBotic_MHZ19 co2 = SGBotic_MHZ19(&Serial1);


void setup() {

  Serial.begin(9600);
  Serial.print("MH-Z19 takes 3 seconds to warming up...  ");
  //delay(3000);
  co2.begin(9600);
  co2.setAutoCalibration(false);

}

void loop() {
  int ppm = co2.readPPM();

  Serial.print("co2: "); Serial.print(ppm);Serial.println("ppm");

  delay(2000);
}
