#include <Wire.h>  // i2c communication library
#include "AHRSProtocol.h"             // navX-Sensor Register Definition header file

byte data[512];

#define ITERATION_DELAY_MS                   10
#define NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT  0x32
#define NUM_BYTES_TO_READ                    8

void setup()
{
  Serial.begin(115200);
  Wire.begin(); // join i2c bus (address optional for master)

  for ( int i = 0; i < sizeof(data); i++ ) {
    data[i] = 0;
  }
}

int register_address = NAVX_REG_FUSED_HEADING_L;

void loop()
{
  int i = 0;
  /* Transmit I2C data request */
  Wire.beginTransmission(NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT); // Begin transmitting to navX-Sensor
  Wire.write(register_address);                                // Sends starting register address
  Wire.write(NUM_BYTES_TO_READ);                               // Send number of bytes to read
  Wire.endTransmission();                                      // Stop transmitting

  /* Receive the echoed value back */
  Wire.beginTransmission(NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT); // Begin transmitting to navX-Sensor
  Wire.requestFrom(NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT, NUM_BYTES_TO_READ);    // Send number of bytes to read
  delay(1);
  while (Wire.available()) {                                   // Read data (slave may send less than requested)
    data[i++] = Wire.read();
  }
  Wire.endTransmission();                                      // Stop transmitting
//  for (int i = 0; i < 8; i++)
//  {
//    Serial.print(IMURegisters::decodeProtocolSignedHundredthsFloat((char *)&data[i]));
//    Serial.print("  ");
//  }
//  Serial.println();
  /* Decode received data to floating-point orientation values */
  float Fused_Heading =     IMURegisters::decodeProtocolSignedHundredthsFloat((char *)&data[0]);   // The cast is needed on arduino

  Serial.print(" Fused_heading:  ");
  Serial.print(Fused_Heading, 2);
  Serial.println("");

  delay(ITERATION_DELAY_MS);
}
