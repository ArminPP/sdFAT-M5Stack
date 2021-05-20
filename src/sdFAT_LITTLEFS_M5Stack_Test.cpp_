#include <Arduino.h>
#include <M5Stack.h>
// #define FS_NO_GLOBALS

#include "SdFat.h"
#include <LITTLEFS.h>

#define DEBUG //
#include <Debug.h>


// INFO  mklittlefs.exe has to be included to the project,
// INFO  otherwise file(s) in \data folder would not uploaded correctly!

// Optional reduced SPI frequency for reading TFT
// #define SPI_READ_FREQUENCY  16000000
// https://github.com/m5stack/M5Stack/blob/6cd8ce15e723c2043a3c01763320413ebff300d5/src/utility/In_eSPI_Setup.h#L266

// but in M5Stack.cpp original SD starts with 40 MHz ?!
// SD.begin(TFCARD_CS_PIN, SPI, 40000000);
// https://github.com/m5stack/M5Stack/blob/6cd8ce15e723c2043a3c01763320413ebff300d5/src/M5Stack.cpp#L27
#define SPI_SPEED SD_SCK_MHZ(25)                                    //ArminPP 22.03 ##############  OK 4 10 20 25   too much: 29 30 40
#define SD_CONFIG SdSpiConfig(TFCARD_CS_PIN, SHARED_SPI, SPI_SPEED) // TFCARD_CS_PIN is defined in M5Stack Config.h (Pin 4)

// HACK SdFat\src\SdFatConfig.h Line 100: #define ENABLE_DEDICATED_SPI 0

SdFat32 sd; // for FAT16/FAT32
File32 file;
File32 root;

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("LITTLEFS - failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

bool dumpFile(const char *FileName)
{
  fs::File dataFile = LITTLEFS.open(FileName);
  if (dataFile)
  {
    while (dataFile.available())
    {
      byte c = dataFile.read();
      // Serial.write(c);
      DWRITE(c);
      M5.Lcd.write(c);
    }
    dataFile.close();
    Serial.println();
    M5.Lcd.println();
    return true;
  }
  // if the file isn't open, pop up an error:
  else
  {
    Serial.print(F("LITTLEFS Error opening "));
    Serial.println(FileName);
    M5.Lcd.print(F("LITTLEFS Error opening "));
    M5.Lcd.println(FileName);
    return false;
  }
}

void setup()
{
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.textsize = 2;
  M5.Lcd.println(F("sdFAT AND LittleFS \n test with M5Stack"));
  Serial.println(F("sdFAT AND LittleFS \n test with M5Stack"));

  int z = 8778;
  DEBUG_PRINT_H(z);
  Serial.println(F(""));
  DEBUG_PRINTLN_H(PI);
  DPRINTLN("1 You are probably aware that PI = ");
  DPRINTLN_H("2 You are probably aware that PI = ");
  DPRINTLNF("3 You are probably aware that PI = ");
  DPRINTLNF_H("4 You are probably aware that PI = ");

  delay(1000);
  int fNameLength = 25;

  //if (!sd.begin(TFCARD_CS_PIN, SPI_SPEED)) // TFCARD_CS_PIN is defined in M5Stack Config.h (Pin 4)
  if (!sd.begin(SD_CONFIG)) // TFCARD_CS_PIN is defined in M5Stack Config.h (Pin 4)
  {
    sd.initErrorHalt(&Serial);
  }

  if (!LITTLEFS.begin(true))
  {
    Serial.println("LITTLEFS Mount Failed");
    M5.Lcd.println("LITTLEFS Mount Failed");
    // return;
  }
  else
  {
    Serial.println(F("LITTLEFS Mount done!"));
    M5.Lcd.println(F("LITTLEFS Mount done!"));
  }

  Serial.println(F("LITTLEFS list and dump Settings.ini"));
  M5.Lcd.println(F("LITTLEFS list and \n dump Settings.ini"));
  listDir(LITTLEFS, "/", 3);
  dumpFile("/Settings.ini");

  if (!root.open("/"))
  {
    Serial.println(F("sdFAT dir.open failed"));
    M5.Lcd.println(F("sdFAT dir.open failed"));
  }
  else
  {
    Serial.println(F("Open sdFAT dir"));
    M5.Lcd.println(F("Open sdFAT dir"));
  }

  while (file.openNext(&root, O_RDONLY))
  {
    file.printFileSize(&Serial);
    M5.Lcd.print(file.fileSize());
    M5.Lcd.print(" ");
    Serial.write(' ');
    file.printModifyDateTime(&Serial);
    Serial.write(' ');
    M5.Lcd.print(" ");
    file.printName(&Serial);
    char f_name[fNameLength];
    file.getName(f_name, fNameLength);
    M5.Lcd.print(f_name);
    if (file.isDir())
    {
      Serial.write('/');
      M5.Lcd.print("/");
    }
    Serial.println();
    M5.Lcd.println();
    file.close();
  }
  if (root.getError())
  {
    Serial.println(F("sdFAT openNext failed"));
  }
  else
  {
    Serial.println(F("sdFAT Done!"));
    M5.Lcd.println(F("sdFAT Done!"));
  }
}

void loop() {}