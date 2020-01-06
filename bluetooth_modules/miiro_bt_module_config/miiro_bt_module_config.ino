/*
 * RD9 Solutions MiiRo HM10 Bluetooth Module Configuration
 * Author: Tyrone van Balla
 * Date: January 2020
 */

#include <SoftwareSerial.h>

#define rx_pin 7
#define tx_pin 8

#define led_b_pin 17

SoftwareSerial mySerial (rx_pin, tx_pin); // RX, TX

char mac_address[5];
char mac_address_cpy[5];
char input;
const char *name_cmd_first = "AT+NAMEmiiro";
const char *baud_cmd = "AT+BAUD3";
char name_cmd[18];

int count;

void configure_hm10_module(void );

// the default baud rate for a new HM10 module is 9600 bps
// after runnint this configuration the baud rate will be set to 57600 bps
#define BAUD_RATE 57600

void setup () {

    // initialise the serial device for the bluetooth module
    mySerial.begin(BAUD_RATE);
    // initialise the serial device for the serial monitor (over USB)
    Serial.begin(BAUD_RATE);
    pinMode(led_b_pin, OUTPUT);
    
    Serial.println("RD9 MiiRo Bluetooth Module Configuration");

    // wait for things to settle
    delay(2000);
    
    // configure hm10 module
    configure_hm10_module();

    // flash blue led to indicate configuration complete
    analogWrite(led_b_pin, 255);
    delay(500);
    analogWrite(led_b_pin, 0);
    delay(500);
    analogWrite(led_b_pin, 255);
}

void loop() 
{

}

void configure_hm10_module(void )
{
    // change name of the bt module
    // name = miiabitwxyz, where wxyz = last 4 digits of MAC address
    analogWrite(led_b_pin, 0);

    Serial.println("Renaming HM-10 module. . .");
    // query HM10 mac address
    mySerial.write("AT+ADDR?");
    delay(100);

    // get last 4 characters of the mac address
    while (mySerial.available()) {
      for (count = 0; count < 20; count++)
      {
        if (count >= 16)
        {
          mac_address[count-16] = mySerial.read();
        }
        else
        {
          mySerial.read();
        }
      }
    }    

    // create the rename command
    strcpy(name_cmd, name_cmd_first);
    strcpy(mac_address_cpy, mac_address);
    strcat(name_cmd, mac_address);

    // request a rename of the HM-10 module
    mySerial.write(name_cmd);
    delay(100);

    // read the response from the name change command
    while(mySerial.available())
    {
      Serial.write(mySerial.read());
    }
    
    Serial.println(". . .Renaming complete!\n");
    
    // change baud rate to 57600bps
    Serial.println("Changing baud rate to 57600 bps. . .");
    mySerial.write(baud_cmd);
    delay(1000);
    // read the response from the name change command
    while(mySerial.available())
    {
      Serial.write(mySerial.read());
    }
    Serial.println(". . .baud rate changed to 57600 complete!");
    Serial.println("configuration complete!\n");
    Serial.print("Robot serial number: miiro");
    Serial.print(mac_address_cpy);
}
