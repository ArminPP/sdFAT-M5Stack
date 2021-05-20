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

SdFat32 sdFAT; // for FAT16/FAT32

char *getAllFreeHeap()
{
  static char freeH[80]{}; // returns the formated free heap space
  sprintf(freeH, "Size: %.2f kB Free: %.2f kB Min: %.2f kB Max: %.2f kB",
          ESP.getHeapSize() / 1024.0,
          ESP.getFreeHeap() / 1024.0,
          ESP.getMinFreeHeap() / 1024.0,
          ESP.getMaxAllocHeap() / 1024.0);
  return freeH;
}

void startFileSystems()
{
  if (!sdFAT.begin(SD_CONFIG)) // TFCARD_CS_PIN is defined in M5Stack Config.h (Pin 4)
  {
    sdFAT.initErrorHalt(&Serial);
  }
  else
  {
    Serial.println(F("sdFAT Mount done!"));
    M5.Lcd.println(F("sdFAT Mount done!"));
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
}

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
  file.close();
}

void listDir_2(const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\r\n", dirname);

  fs::File root = LITTLEFS.open(dirname);
  if (!root)
  {
    Serial.println("LITTLEFS 2 - failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println(" - not a directory");
    return;
  }

  fs::File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir_2(file.name(), levels - 1);
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
  file.close();
}

void listDir32(const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\r\n", dirname);

  File32 root = sdFAT.open(dirname, O_RDONLY);
  // File32 root = fs.open(dirname);
  if (!root)
  {
    Serial.println("sdFAT - failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println(" - not a directory");
    return;
  }

  // File32 file = root.openNextFile();
  File32 file;
  file.openNext(&root, O_RDONLY);
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir32(file.name(), levels - 1);
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
  file.close();
}

void listDir32_2(const char *dirname)
{
  File32 root = sdFAT.open(dirname, O_RDONLY);
  if (!root)
  {
    Serial.println("open root");
  }
  File32 file;
  file.openNext(&root, O_RDONLY);

  while (file)
  {
    Serial.print("  FILE: ");
    file.printName();
    Serial.print("\tSIZE: ");
    Serial.println(file.size());
    Serial.println("");
    file.close();
  }
}

void setup()
{
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.textsize = 2;
  M5.Lcd.println(F("sdFAT AND LittleFS \n test with M5Stack"));
  Serial.println(F("sdFAT AND LittleFS \n test with M5Stack"));

  DEBUG_PRINTLN_H(PI);

  startFileSystems();

  for (int i = 0; i <= 5; i++)
  {
    listDir(LITTLEFS, "/", 3);
    Serial.println(getAllFreeHeap());
    Serial.println(F("LITTLEFS list dir ----------------------------"));
    // M5.Lcd.println(F("LITTLEFS list and \n dump Settings.ini"));
    delay(1000);
    listDir_2("/", 3);
    Serial.println(getAllFreeHeap());
    Serial.println(F("LITTLEFS list 2 dir ----------------------------"));
    delay(1000);
    listDir32("/", 3);
    Serial.println(getAllFreeHeap());
    Serial.println(F("sdFAT list dir ----------------------------"));
    // M5.Lcd.println(F("sdFAT list and \n dump Settings.ini"));
    delay(1000);
    listDir32_2("/");
    Serial.println(getAllFreeHeap());
    Serial.println(F("sdFAT list 2 dir ----------------------------"));
    // M5.Lcd.println(F("sdFAT list and \n dump Settings.ini"));
    delay(1000);
  }
}

void loop() {}