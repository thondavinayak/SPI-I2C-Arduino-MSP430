#include <Wire.h>
#include <SPI.h>

// CMPS12 I2C address and registers
#define CMPS12_ADDRESS 0x60
#define ANGLE_8  1

// LED pin
#define LED_PIN 2

// Sensor data variables
unsigned char high_byte, low_byte, angle8;
unsigned char pitch, roll;
unsigned int angle16;

char data = 'Z';

void setup() {
  // Serial and I2C init
  Serial.begin(9600);
  Wire.begin();

  // SPI Slave config
  pinMode(MISO, OUTPUT);     // MISO = output in SPI slave
  pinMode(LED_PIN, OUTPUT);  // LED output

  SPCR = 0b01000001;            // Enable SPI in slave mode  and the clock frequency is 16 mhz/16
  SPDR = 0x00;                  // initalize the data to 0 
}

void loop() {
  // ==== I2C READ from CMPS12 ====

  //Serial.println("here 1");
  
  Wire.beginTransmission(CMPS12_ADDRESS);
  Wire.write(ANGLE_8);  // Start reading from register 1
  Wire.endTransmission();

  //Serial.println("here 2");

  Wire.requestFrom(CMPS12_ADDRESS, 5);
  while (Wire.available() < 5);  // Wait until data is available

  //Serial.println("here 3");

  angle8 = Wire.read();
  high_byte = Wire.read();
  low_byte = Wire.read();
  pitch = Wire.read();
  roll = Wire.read();

  angle16 = (high_byte << 8) | low_byte;

  Serial.print("roll: ");
  Serial.print(roll);
  Serial.print("  pitch: ");
  Serial.print(pitch);
  Serial.print("  angle16: ");
  Serial.print(angle16 / 10);
  Serial.print(".");
  Serial.print(angle16 % 10);
  Serial.print("  angle8: ");
  Serial.println(angle8);


  // ==== SPI COMMUNICATION ====
  byte receivedByte = SPDR;  // Read what master sent previously

  char c;
  c = (char)receivedByte;
  
  Serial.print("Received byte: ");
  Serial.print(receivedByte, DEC);  
  Serial.print(" ");
  Serial.print(receivedByte);  
  Serial.print(" ");
  Serial.print((char)receivedByte); 
  Serial.print(" "); 

  // LED toggle logic (optional)
  if (receivedByte % 2 == 0) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF");
  }

  byte dataForTransmission = (byte)pitch; 
  //data = data + 1;

  Serial.print(" Transmitting ");
  Serial.print(dataForTransmission);
  Serial.println("");

  SPI.transfer(dataForTransmission);

  delay(500);  // Small delay
}
