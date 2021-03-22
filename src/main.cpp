#include <Arduino.h>
#include <M5Stack.h>
#include "SdFat.h"

#define SPI_SPEED SD_SCK_MHZ(25) //Armin_PP 22.3 ##############  OK 4 10 20 25   too much: 29 30 40
// Optional reduced SPI frequency for reading TFT
// #define SPI_READ_FREQUENCY  16000000
// https://github.com/m5stack/M5Stack/blob/6cd8ce15e723c2043a3c01763320413ebff300d5/src/utility/In_eSPI_Setup.h#L266

// but in M5Stack.cpp original SD starts with 40 MHz ?!
// TF Card
  // if (SDEnable == true) {
  //   SD.begin(TFCARD_CS_PIN, SPI, 40000000);
  // }


SdFat32 sd; // for FAT16/FAT32
File32 file;
File32 root;

void setup()
{
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.textsize = 3;
  M5.Lcd.print(F("Hello World"));

  delay(1000);

  // Initialize the SD card.
  if (!sd.begin(TFCARD_CS_PIN, SPI_SPEED)) // TFCARD_CS_PIN is defined in M5Stack Config.h (Pin 4)
  {
    sd.initErrorHalt(&Serial);
  }

  // Open root directory
  if (!root.open("/"))
  {
    Serial.println(F("dir.open failed"));
  }
  // Open next file in root.
  // Warning, openNext starts at the current position of dir so a
  // rewind may be necessary in your application.
  while (file.openNext(&root, O_RDONLY))
  {
    file.printFileSize(&Serial);
    Serial.write(' ');
    file.printModifyDateTime(&Serial);
    Serial.write(' ');
    file.printName(&Serial);
    if (file.isDir())
    {
      // Indicate a directory.
      Serial.write('/');
    }
    Serial.println();
    file.close();
  }
  if (root.getError())
  {
    Serial.println(F("openNext failed"));
  }
  else
  {
    Serial.println(F("Done!"));
  }
}
//------------------------------------------------------------------------------
// Nothing happens in loop.
void loop() {}