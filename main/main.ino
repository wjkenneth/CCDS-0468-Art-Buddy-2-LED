#define FASTLED_RP2040_CLOCKLESS_M0_FALLBACK 0
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <FastLED.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <LittleFS.h>
#include "main.h"
#include "animations.h"
#include "motor.h"
#include "display.h"
#include "controls.h"
#include "bitmap.h"

unsigned long last = 0;

// Initialize the Adafruit_ST7789 display object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Initialize the NeoPixel strip object
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];

// Initialize the UIState object
UIState uiState;

// SD connection flag
bool sdConnected = false;

void setup()
{
  delay(5000);
  // Init serial communication
  Serial.begin(9600);
  Serial.println("Initialized");

  SPI1.setRX(SD_MISO);
  SPI1.setTX(SD_MOSI);
  SPI1.setSCK(SD_SCLK);
  SPI1.begin();

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS, SPI1))
  {
    Serial.println("initialization failed!");
  }
  else
  {
    Serial.println("initialization done.");
    sdConnected = true;
    uiState.storageModeSelection = 0; // Reset selection
    fileCount = 0;                    // Reset fileCount
    sdListDir();
  }

  SPI1.end();

  // Deactivate SD card
  digitalWrite(SD_CS, HIGH);

  // Init Button Inputs
  pinMode(KEY_A, INPUT_PULLUP);
  pinMode(KEY_B, INPUT_PULLUP);
  pinMode(KEY_UP, INPUT_PULLUP);
  pinMode(KEY_DOWN, INPUT_PULLUP);
  pinMode(KEY_LEFT, INPUT_PULLUP);
  pinMode(KEY_RIGHT, INPUT_PULLUP);
  pinMode(KEY_CTRL, INPUT_PULLUP);

  // Init TFT Display
  tft.init(135, 240);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(3);

  // Init Switch and Motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);

  // Initialize the UI state
  EEPROM.begin(sizeof(UIState));

  if (!loadUIState())
  {
    uiState = UIState();
    uiState.currentState = START_SCREEN;
  }

  // Initialize the LED strip
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_PIN2>(leds2, NUM_LEDS2);

  // Initialize file system
  if (!LittleFS.begin())
  {
    Serial.println("Failed to mount filesystem!");
    return;
  }
  listDir();

  Serial.println("Filesystem mounted successfully!");

  // Start the UI
  if (uiState.currentState != ANIMATION_MENU)
  {
    Serial.println("Starting UI");
    start();
    uiState.mainMenuSelection = 0;
    uiState.mainMenuPage = 0;
    uiState.mainMenuSelection2 = 0;
    uiState.mainMenuPage2 = 0;
    uiState.animationMenuSelection = 0;
    uiState.gradientMenuSelection = 0;
    uiState.settingsMenuSelection = 0;
    uiState.currentState = START_SCREEN;
  }
  else
  {
    animationMenu();
  }
}

void loop()
{
  // Handle menu states
  switch (uiState.currentState)
  {
  case START_SCREEN:
    handleStartScreen();
    break;
  case MAIN_MENU:
    handleMainMenu();
    break;
  case MAIN_MENU_2:
    handleMainMenu2();
    break;
  case ANIMATION_MENU:
    handleAnimationMenu();
    break;
  case GRADIENT_MENU:
    handleGradientMenu();
    break;
  case SETTINGS_MENU:
    handleSettingsMenu();
    break;
  case CHANGE_SETTINGS:
    handleChangeSettingsMenu();
    break;
  case BITMAP_MENU:
    handleBitmapMenu();
    break;
  case USB_MODE:
    handleUsbMode();
    break;
  case STORAGE_MODE:
    handleStorageMode();
    break;
  case BITMAP_SECONDARY_MENU:
    handleBitmapSecondaryMenu();
    break;
  case BITMAP_ANIMATION_MENU:
    handleBitmapAnimationMenu();
    break;
  case USB_ANIMATION_MENU:
    handleUsbAnimationMenu();
    break;
  }
}

void handleSaveSettings()
{
  if (digitalRead(KEY_A) == LOW)
  {
    saveUIState();
    uiState.currentState = SETTINGS_MENU;
    settingsMenu();
  }
}

bool loadUIState()
{
  UIState loadedState;
  EEPROM.get(EEPROM_ADDRESS, loadedState);

  if (loadedState.validationToken == 0xABDECBCF)
  {
    uiState = loadedState;
    return true;
  }
  return false;
}

void saveUIState()
{
  EEPROM.put(EEPROM_ADDRESS, uiState);
  EEPROM.commit();
  Serial.println("Saved UI State");
}

void handleStartScreen()
{
  // Joystick Controls
  handleRight(uiState.startMenuSelection, 2, startChangeSelection);
  handleLeft(uiState.startMenuSelection, startChangeSelection);
  handleUp(uiState.startMenuSelection, startChangeSelection);
  handleDown(uiState.startMenuSelection, 2, startChangeSelection);
  if (uiState.startMenuSelection == 0)
  {
    handleChangeMenu(KEY_A, uiState.currentState, MAIN_MENU, mainMenu);
  } else if (uiState.startMenuSelection == 1)
  {
    handleChangeMenu(KEY_A, uiState.currentState, BITMAP_MENU, bitmapMenu);
  } else
  {
    handleChangeMenu(KEY_A, uiState.currentState, SETTINGS_MENU, settingsMenu);
  }
  
  uiState.mainMenuSelection = 0;
  uiState.mainMenuPage = 0;
  uiState.mainMenuSelection2 = 0;
  uiState.mainMenuPage2 = 0;
  uiState.animationMenuSelection = 0;
  uiState.gradientMenuSelection = 0;
  uiState.settingsMenuSelection = 0;
}

void handleMainMenu()
{
  uiState.gradientMenuSelection = 0;
  uiState.animationMenuSelection = 0;

  // Joystick Controls
  handleRight(uiState.mainMenuSelection, 3, mainMenuChangeSelection);
  handleLeft(uiState.mainMenuSelection, mainMenuChangeSelection);
  handleUp(uiState.mainMenuSelection, mainMenuChangeSelection, 2);
  handleDown(uiState.mainMenuSelection, 3, mainMenuChangeSelection, 2);

  // Selection Controls
  handleChangeMenu(KEY_A, uiState.currentState, MAIN_MENU_2, mainMenu2);
  handleChangeMenu(KEY_CTRL, uiState.currentState, START_SCREEN, start);

  // Page Controls
  handleNextPage(uiState.mainMenuPage, uiState.mainMenuSelection, 5, mainMenu);
}

void handleMainMenu2()
{
  
  uiState.gradientMenuSelection = 0;
  uiState.animationMenuSelection = 0;

  // Joystick Controls
  handleRight(uiState.mainMenuSelection2, 3, mainMenuChangeSelection2);
  handleLeft(uiState.mainMenuSelection2, mainMenuChangeSelection2);
  handleUp(uiState.mainMenuSelection2, mainMenuChangeSelection2, 2);
  handleDown(uiState.mainMenuSelection2, 3, mainMenuChangeSelection2, 2);

  // Selection Controls
  handleChangeMenu(KEY_A, uiState.currentState, ANIMATION_MENU, animationMenu);
  handleChangeMenu(KEY_CTRL, uiState.currentState, MAIN_MENU, mainMenu);

  // Page Controls
  handleNextPage(uiState.mainMenuPage2, uiState.mainMenuSelection2, 5, mainMenu2);
}

void handleAnimationMenu()
{
  // Gradient Specific Controls
  if (uiState.mainMenuSelection == 1 || uiState.mainMenuSelection == 3 || uiState.mainMenuSelection == 5)
  {
    // Joystick Controls
    handleRight(uiState.animationMenuSelection, 2, animationMenuChangeSelection);
    handleUp(uiState.animationMenuSelection, animationMenuChangeSelection);
    handleDown(uiState.animationMenuSelection, 2, animationMenuChangeSelection);
    if (uiState.animationMenuSelection == 1)
    {
      handleChangeMenu(KEY_A, uiState.currentState, RUN_MENU, runMenu);
    }
    if (uiState.animationMenuSelection == 2)
    {
      handleChangeMenu(KEY_A, uiState.currentState, GRADIENT_MENU, gradientMenu);
    }
  }
  else
  {
    // Joystick Controls
    handleRight(uiState.animationMenuSelection, 1, animationMenuChangeSelection);
    if (uiState.animationMenuSelection == 1)
    {
      handleChangeMenu(KEY_A, uiState.currentState, RUN_MENU, runMenu);
    }
  }

  // Joystick Controls for both cases
  handleLeft(uiState.animationMenuSelection, animationMenuChangeSelection);

  // Selection Controls for both cases
  if (uiState.animationMenuSelection == 0)
  {
    handleChangeMenu(KEY_A, uiState.currentState, MAIN_MENU, mainMenu);
  }
}

void handleGradientMenu()
{
  // Joystick Controls
  handleRight(uiState.gradientMenuSelection, 3, gradientMenuChangeSelection);
  handleLeft(uiState.gradientMenuSelection, gradientMenuChangeSelection);
  handleUp(uiState.gradientMenuSelection, gradientMenuChangeSelection, 2);
  handleDown(uiState.gradientMenuSelection, 3, gradientMenuChangeSelection, 2);

  // Selection Controls
  handleChangeMenu(KEY_A, uiState.currentState, ANIMATION_MENU, animationMenu);
  handleChangeMenu(KEY_B, uiState.currentState, ANIMATION_MENU, animationMenu);
}

void handleSettingsMenu()
{
  FastLED.clear();
  FastLED.show();

  // Joystick Controls
  handleRight(uiState.settingsMenuSelection, 1, settingsMenuChangeSelection);
  handleLeft(uiState.settingsMenuSelection, settingsMenuChangeSelection);

  // Selection Controls
  handleChangeMenu(KEY_A, uiState.currentState, CHANGE_SETTINGS, changeSettingsMenu);
  handleChangeMenu(KEY_B, uiState.currentState, START_SCREEN, start);

  unsigned long currentTime = millis();
}

void handleChangeSettingsMenu()
{
  if(uiState.settingsMenuSelection == 0){
    // +
    handleSettingsUp(uiState.brightnessSelection, ((sizeof(uiState.brightnessSetting) / sizeof(uiState.brightnessSetting[0])) - 1), changeSettingsMenuChangeSelection);
    // -
    handleSettingsDown(uiState.brightnessSelection, changeSettingsMenuChangeSelection);
  } else {
    // +
    handleSettingsUp(uiState.speedSelection, ((sizeof(uiState.speedSetting) / sizeof(uiState.speedSetting[0])) - 1), changeSettingsMenuChangeSelection);
    // -
    handleSettingsDown(uiState.speedSelection, changeSettingsMenuChangeSelection);
  }
  

  // Selection Controls
  handleSaveSettings();
  handleChangeMenu(KEY_B, uiState.currentState, SETTINGS_MENU, settingsMenu);
  handleChangeMenu(KEY_CTRL, uiState.currentState, CHANGE_SETTINGS, spinMotor);
}

void handleBitmapMenu()
{
  if(uiState.bitmapMenuSelection == 0)
  {
    handleChangeMenu(KEY_A, uiState.currentState, USB_MODE, usbMode);
    
  }
  else if(uiState.bitmapMenuSelection == 1)
  {
    handleChangeMenu(KEY_A, uiState.currentState, STORAGE_MODE, storageMode);
  }

  handleChangeMenu(KEY_B, uiState.currentState, START_SCREEN, start);

  handleRight(uiState.bitmapMenuSelection, 1, bitmapMenuChangeSelection);
  handleLeft(uiState.bitmapMenuSelection, bitmapMenuChangeSelection);
}

void handleUsbMode()
{
  handleChangeMenu(KEY_A, uiState.currentState, BITMAP_SECONDARY_MENU, mainMenu2);
  handleChangeMenu(KEY_B, uiState.currentState, BITMAP_MENU, bitmapMenu);

  handleUp(uiState.usbModeSelection, usbModeChangeSelection);
  handleDown(uiState.usbModeSelection, sdFileCount - 1, usbModeChangeSelection);
}

void handleStorageMode()
{
  handleChangeMenu(KEY_A, uiState.currentState, BITMAP_SECONDARY_MENU, mainMenu2);
  handleChangeMenu(KEY_B, uiState.currentState, BITMAP_MENU, bitmapMenu);

  handleUp(uiState.storageModeSelection, storageModeChangeSelection);
  handleDown(uiState.storageModeSelection, fileCount - 1, storageModeChangeSelection);
}

void handleBitmapSecondaryMenu()
{
  uiState.gradientMenuSelection = 0;
  uiState.animationMenuSelection = 0;

  // Joystick Controls
  handleRight(uiState.mainMenuSelection2, 3, mainMenuChangeSelection2);
  handleLeft(uiState.mainMenuSelection2, mainMenuChangeSelection2);
  handleUp(uiState.mainMenuSelection2, mainMenuChangeSelection2, 2);
  handleDown(uiState.mainMenuSelection2, 3, mainMenuChangeSelection2, 2);

  // Selection Controls
  if (uiState.bitmapMenuSelection == 0)
  {
    handleChangeMenu(KEY_A, uiState.currentState, USB_ANIMATION_MENU, usbAnimationMenu);
    handleChangeMenu(KEY_CTRL, uiState.currentState, USB_MODE, usbMode);
  }
  else if (uiState.bitmapMenuSelection == 1)
  {
    handleChangeMenu(KEY_A, uiState.currentState, BITMAP_ANIMATION_MENU, bitmapAnimationMenu);
    handleChangeMenu(KEY_CTRL, uiState.currentState, STORAGE_MODE, storageMode);
  }

  // Page Controls
  handleNextPage(uiState.mainMenuPage2, uiState.mainMenuSelection2, 4, mainMenu2);
}

void handleBitmapAnimationMenu()
{
  // Joystick Controls
  handleRight(uiState.bitmapAnimationMenuSelection, 2, bitmapAnimationMenuChangeSelection);
  handleLeft(uiState.bitmapAnimationMenuSelection, bitmapAnimationMenuChangeSelection);
  handleUp(uiState.bitmapAnimationMenuSelection, bitmapAnimationMenuChangeSelection);
  handleDown(uiState.bitmapAnimationMenuSelection, 2, bitmapAnimationMenuChangeSelection);
  if (uiState.bitmapAnimationMenuSelection == 0)
  {
    handleChangeMenu(KEY_A, uiState.currentState, STORAGE_MODE, storageMode);
  }
  if (uiState.bitmapAnimationMenuSelection == 1)
  {
    handleChangeMenu(KEY_A, uiState.currentState, BITMAP_RUN_MENU, runBitmapMenu);
  }
  if (uiState.bitmapAnimationMenuSelection == 2)
  {
    handleChangeMenu(KEY_A, uiState.currentState, STORAGE_MODE, deleteFile);
  }
}

void handleUsbAnimationMenu()
{
  // Joystick Controls
  handleRight(uiState.usbAnimationMenuSelection, 1, usbAnimationMenuChangeSelection);
  handleLeft(uiState.usbAnimationMenuSelection, usbAnimationMenuChangeSelection);
  handleUp(uiState.usbAnimationMenuSelection, usbAnimationMenuChangeSelection);
  handleDown(uiState.usbAnimationMenuSelection, 1, usbAnimationMenuChangeSelection);
  if (uiState.usbAnimationMenuSelection == 0)
  {
    handleChangeMenu(KEY_A, uiState.currentState, USB_MODE, usbMode);
  }
  if (uiState.usbAnimationMenuSelection == 1)
  {
    handleChangeMenu(KEY_A, uiState.currentState, BITMAP_RUN_MENU, runUsbMenu);
  }
}

void displayBrightnessLevel()
{
  FastLED.setBrightness(uiState.brightnessValue[uiState.brightnessSelection]);

  fill_solid(leds, NUM_LEDS, CRGB::Red);

  FastLED.show();
}

void runMenu()
{
  // Display running animation
  tft.fillScreen(ST77XX_RED);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(90, 60);
  tft.print("Running");
  tft.setCursor(85, 80);
  tft.print("Animation");

  FastLED.setBrightness(uiState.brightnessValue[uiState.brightnessSelection]);

  unsigned long startTime = millis();
  unsigned long currentTime = millis();
  unsigned long duration = uiState.durationValue[uiState.speedSelection];
  int cc = 0;

  // Run the selected animation
  runMotor();

  while (true)
  {
    // Check if the duration has passed
    currentTime = millis();
    // Serial.print("Count: " + String(cc) + " Time: ");
    // Serial.println(currentTime - startTime);
    // Serial.println(all_animations[i]);
    cc++;

    // Run code here
    animationFunctions[uiState.mainMenuPage * 4 + uiState.mainMenuSelection]();
    animationFunctions2[uiState.mainMenuPage2 * 4 + uiState.mainMenuSelection2]();
    // animationFunctions[i]();
    // animationFunctions2[i]();

    delay(uiState.durationValue[uiState.speedSelection] / 360);

    if (currentTime - startTime > duration || digitalRead(KEY_B) == LOW)
    {
      break;
    }
  }

    stopMotor();
    FastLED.clear();
    FastLED.show();

  // Return to the animation menu
  uiState.currentState = ANIMATION_MENU;
  animationMenu();
  saveUIState();
}

void runBitmapMenu()
{
  // Display running animation
  tft.fillScreen(ST77XX_RED);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(90, 60);
  tft.print("Running");
  tft.setCursor(85, 80);
  tft.print("Animation");

  FastLED.setBrightness(uiState.brightnessValue[uiState.brightnessSelection]);

  // Run the selected animation
  displayBMP();

  stopMotor();
  FastLED.clear();
  FastLED.show();
  Serial.println("done");
  uiState.currentState = BITMAP_ANIMATION_MENU;
  bitmapAnimationMenu();
  // saveUIState();
}

void runUsbMenu()
{
  transferBMP();

  // Display running animation
  tft.fillScreen(ST77XX_RED);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(90, 60);
  tft.print("Running");
  tft.setCursor(85, 80);
  tft.print("Animation");

  FastLED.setBrightness(uiState.brightnessValue[uiState.brightnessSelection]);

  // Run the selected animation
  displayUSB();

  stopMotor();
  FastLED.clear();
  FastLED.show();

  // Delete temp BMP
  deleteUSB();

  uiState.currentState = USB_ANIMATION_MENU;
  usbAnimationMenu();
  // saveUIState();
}

void transferBMP()
{
  // Deactivate TFT to prevent interference on SPI
  digitalWrite(TFT_CS, HIGH);
  // Activate SD card
  digitalWrite(SD_CS, LOW);

  SPI1.setRX(SD_MISO);
  SPI1.setTX(SD_MOSI);
  SPI1.setSCK(SD_SCLK);
  SPI1.begin();

  // Open the BMP file from SD card
  File bmpFile = SD.open(sdFiles[uiState.usbModeSelection].name, FILE_READ);
  if (!bmpFile)
  {
    Serial.println("Failed to open BMP file");
    digitalWrite(SD_CS, HIGH);
    return;
  }

  // Create a new file in LittleFS
  File littleFsFile = LittleFS.open("/savedBMP.bmp", "w+");
  if (!littleFsFile)
  {
    Serial.println("Failed to open LittleFS file");
    bmpFile.close();
    digitalWrite(SD_CS, HIGH);
    return;
  }

  // Read and transfer data from SD card to LittleFS
  uint8_t buffer[64];
  size_t bytesRead;
  while ((bytesRead = bmpFile.read(buffer, sizeof(buffer))) > 0)
  {
    littleFsFile.write(buffer, bytesRead); // Write to LittleFS
  }

  Serial.println("File copied to LittleFS successfully");

  // Close both files
  bmpFile.close();
  littleFsFile.close();

  SPI1.end();

  // Deactivate SD card
  digitalWrite(SD_CS, HIGH);
  // Reactivate TFT after accessing SD card
  digitalWrite(TFT_CS, LOW);

  tft.init(135, 240);
  tft.setRotation(3);
}
