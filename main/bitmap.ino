#include <Arduino.h>
#include <LittleFS.h>
#include <SPI.h>
#include <FastLED.h>
#include <SD.h>
#include "main.h"
#include "bitmap.h"
#include "display.h"
#include "motor.h"

// Define file info array
FileInfo files[MAX_FILES];
int fileCount = 0;

// SD
FileInfo sdFiles[MAX_FILES];
int sdFileCount = 0;

void listDir()
{
    File root = LittleFS.open("/", "r");
    if (!root)
    {
        Serial.println("Failed to open root directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println("Root is not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file && fileCount < MAX_FILES)
    {
        files[fileCount].name = file.name();
        files[fileCount].size = file.size();
        files[fileCount].isDirectory = file.isDirectory();
        Serial.println(files[fileCount].name + " " + files[fileCount].size + " " + files[fileCount].isDirectory);

        fileCount++;
        file = root.openNextFile();
    }

    root.close();
}

void sdListDir()
{
    File root = SD.open("/", FILE_READ);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }

    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file && sdFileCount < MAX_FILES)
    {
        // Ignore hidden files (those starting with a dot)
        if (file.name()[0] != '.')
        {
            sdFiles[sdFileCount].name = file.name();
            sdFiles[sdFileCount].size = file.size();
            sdFiles[sdFileCount].isDirectory = file.isDirectory();
            sdFileCount++;
        }

        file.close();               // Close the file after processing
        file = root.openNextFile(); // Open the next file
    }

    root.close();
}

void deleteFile()
{
    String path = "/" + String(files[uiState.storageModeSelection].name);

    if (LittleFS.remove(path))
    {
        Serial.println(String(path) + " deleted successfully.");
        // List files again to verify deletion
        uiState.storageModeSelection = 0; // Reset selection
        fileCount = 0; // Reset fileCount
        listDir();
    }
    else
    {
        Serial.println("Failed to delete " + String(path));
    }
    storageMode();
}

void deleteUSB()
{
    if (LittleFS.remove("/savedBMP.bmp"))
    {
        Serial.println("savedBMP deleted successfully.");
        // List files again to verify deletion
        uiState.storageModeSelection = 0; // Reset selection
        fileCount = 0;                    // Reset fileCount
        listDir();
    }
    else
    {
        Serial.println("Failed to delete savedBMP");
    }
}

void displayBMP()
{
    String path = "/" + String(files[uiState.storageModeSelection].name);
    File bmpFile = LittleFS.open(path.c_str(), "r");

    if (!bmpFile)
    {
        Serial.println("Failed to open BMP file");
        return;
    }

    uint8_t header[54];
    bmpFile.seek(0);
    bmpFile.read(header, 54);
    for (int i = 0; i < 54; i++)
    {
        Serial.print(header[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    bmpFile.seek(18);
    int width;
    bmpFile.read((uint8_t *)&width, 4);

    bmpFile.seek(22);
    int height;
    bmpFile.read((uint8_t *)&height, 4);

    // Calculate row size (including padding)
    int rowSize = ((width * 3 + 3) & ~3);

    // Calculate expected pixel data size
    int pixelDataSize = rowSize * height;

    // Get the total file size
    int fileSize = bmpFile.size();

    // Calculate the header size
    int headerSize = fileSize - pixelDataSize;

    // Calculate delay
    int delayTime = uiState.durationValue[uiState.speedSelection] / width;

    unsigned long startTime = millis();
    unsigned long currentTime = millis();
    unsigned long duration = uiState.durationValue[uiState.speedSelection];

    runMotor();

    // Reading the file column by column
    for (int col = 0; col < width + 0; col++)
    {
        animationFunctions2[uiState.mainMenuPage2 * 4 + uiState.mainMenuSelection2]();
        for (int row = 0; row < height; row++)
        {
            currentTime = millis();

            int pos = headerSize + (height - 1 - row) * rowSize + col * 3;
            bmpFile.seek(pos);

            uint8_t bmpLine[3];
            bmpFile.read(&bmpLine[0], 3);

            leds[row] = CRGB(bmpLine[2], bmpLine[1], bmpLine[0]);

            if (currentTime - startTime > duration || digitalRead(KEY_B) == LOW)
            {
                
                bmpFile.close();
                return;
            }
        }

        FastLED.show();
        delay(delayTime);
    }
    
    bmpFile.close();
}

void displayUSB()
{
    File bmpFile = LittleFS.open("/savedBMP.bmp", "r");

    if (!bmpFile)
    {
        Serial.println("Failed to open BMP file");
        return;
    }

    uint8_t header[54];
    bmpFile.seek(0);
    bmpFile.read(header, 54);
    for (int i = 0; i < 54; i++)
    {
        Serial.print(header[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    bmpFile.seek(18);
    int width;
    bmpFile.read((uint8_t *)&width, 4);

    bmpFile.seek(22);
    int height;
    bmpFile.read((uint8_t *)&height, 4);

    // Calculate row size (including padding)
    int rowSize = ((width * 3 + 3) & ~3);

    // Calculate expected pixel data size
    int pixelDataSize = rowSize * height;

    // Get the total file size
    int fileSize = bmpFile.size();

    // Calculate the header size
    int headerSize = fileSize - pixelDataSize;

    // Calculate delay
    int delayTime = uiState.durationValue[uiState.speedSelection] / width;

    unsigned long startTime = millis();
    unsigned long currentTime = millis();
    unsigned long duration = uiState.durationValue[uiState.speedSelection];

    runMotor();

    // Reading the file column by column
    for (int col = 0; col < width + 0; col++)
    {
        animationFunctions2[uiState.mainMenuPage2 * 4 + uiState.mainMenuSelection2]();
        for (int row = 0; row < height; row++)
        {
            currentTime = millis();

            int pos = headerSize + (height - 1 - row) * rowSize + col * 3;
            bmpFile.seek(pos);

            uint8_t bmpLine[3];
            bmpFile.read(&bmpLine[0], 3);

            leds[row] = CRGB(bmpLine[2], bmpLine[1], bmpLine[0]);

            if (currentTime - startTime > duration || digitalRead(KEY_B) == LOW)
            {

                bmpFile.close();
                return;
            }
        }

        FastLED.show();
        delay(delayTime);
    }

    bmpFile.close();
}