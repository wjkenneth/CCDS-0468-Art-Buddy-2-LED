#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <FastLED.h>
#include <SPI.h>
#include "animations.h"

// Define button pins
#define KEY_A 15
#define KEY_B 17
#define KEY_DOWN 18
#define KEY_UP 2
#define KEY_RIGHT 20
#define KEY_LEFT 16
#define KEY_CTRL 3

// Define TFT pins
#define TFT_CS 9
#define TFT_RST 12
#define TFT_DC 8
#define TFT_MOSI 11
#define TFT_SCLK 10
#define TFT_BL 13

// Define NeoPixel pins
#define LED_PIN 22
#define LED_PIN2 21

// Define switch pin
#define SWITCH 13

// Define Motor pins
#define EN 5
#define IN1 6
#define IN2 7

// Define SD pins
#define SD_CS 0
#define SD_SCLK 10
#define SD_MOSI 11
#define SD_MISO 12

// Define save address
#define EEPROM_ADDRESS 0

// Define menu states
enum MenuState
{
    START_SCREEN,
    MAIN_MENU,
    MAIN_MENU_2,
    ANIMATION_MENU,
    GRADIENT_MENU,
    RUN_MENU,
    SETTINGS_MENU,
    CHANGE_SETTINGS,
    BITMAP_MENU,
    USB_MODE,
    STORAGE_MODE,
    BITMAP_SECONDARY_MENU,
    BITMAP_ANIMATION_MENU,
    USB_ANIMATION_MENU,
    BITMAP_RUN_MENU
};

struct UIState
{
    uint32_t validationToken = 0xABDECBCF;
    int startMenuSelection = 0;
    int mainMenuSelection = 0;
    int mainMenuPage = 0;
    int mainMenuSelection2 = 0;
    int mainMenuPage2 = 0;
    int animationMenuSelection = 0;
    int gradientMenuSelection = 0;
    int settingsMenuSelection = 0;
    int bitmapMenuSelection = 0;
    int usbModeSelection = 0;
    int storageModeSelection = 0;
    int bitmapAnimationMenuSelection = 0;
    int usbAnimationMenuSelection = 0;
    int brightnessSelection = 3;
    int brightnessSetting[6] = {0, 1, 2, 3, 4, 5};
    int brightnessValue[6] = {0, 51, 102, 153, 204, 255};
    int speedSelection = 2;
    int speedSetting[5] = {1, 2, 3, 4, 5};
    int speedValue[5] = {41, 51 , 66, 88, 135};
    int direction = 1;
    int durationValue[5] = {6000, 5000, 4000, 3000, 2000};
    MenuState currentState = START_SCREEN;
};

// Settings Menu
static const char settings_1[] PROGMEM = "Brightness";
static const char settings_2[] PROGMEM = "M Speed";

static const char *const settings_menu[] PROGMEM = {
    settings_1, settings_2};

// Define Start Menu
static const char start_menu_1[] PROGMEM = "Animation";
static const char start_menu_2[] PROGMEM = "Bitmap";
static const char start_menu_3[] PROGMEM = "Settings";

static const char *const start_menu[] PROGMEM = {
    start_menu_1, start_menu_2, start_menu_3};

// Function prototypes
void handleSaveSettings();
void saveUIState();
bool loadUIState();
void handleStartScreen();
void handleMainMenu();
void handleMainMenu2();
void handleAnimationMenu();
void handleGradientMenu();
void handleSettingsMenu();
void handleChangeSettingsMenu();
void handleBitmapMenu();
void handleUsbMode();
void handleStorageMode();
void handleBitmapSecondaryMenu();
void handleBitmapAnimationMenu();
void handleUsbAnimationMenu();
void displayBrightnessLevel();
void runMenu();
void runBitmapMenu();
void runUsbMenu();
void transferBMP();

// Declare global variables
extern Adafruit_ST7789 tft;
extern UIState uiState;
extern CRGB leds[NUM_LEDS];
extern CRGB leds2[NUM_LEDS2];
extern bool sdConnected;

#endif