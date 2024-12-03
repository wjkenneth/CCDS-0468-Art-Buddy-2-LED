#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "main.h"
#include "animations.h"
#include "display.h"
#include "bitmap.h"

// Define the rectangles array
Rectangle rectangles[4] = {
    {15, 30},   // Top Left
    {125, 30},  // Top Right
    {15, 75},   // Bottom Left
    {125, 75}   // Bottom Right
};

void start()
{
  // Draw start screen
  tft.fillScreen(ST77XX_BLACK);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);
  tft.setCursor(60, 10);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.print("Art Buddy LED Painter");

  // Draw instructions
  tft.setTextColor(ST77XX_WHITE);

  /*
  tft.setCursor(10, 25);
  tft.print("<- Display Bitmap [ Down ]");

  tft.setCursor(10, 40);
  tft.print("<- Settings [ UP ]");

  tft.setCursor(10, 55);
  tft.print("<- Back to Start Screen [ CTRL ]");

  tft.setCursor(10, 70);
  tft.print("<- Move [ JOYSTICK ] ");

  tft.setCursor(145, 85);
  tft.print("Select [ A ] ->");

  tft.setCursor(125, 100);
  tft.print("Next Page [ B ] ->");

  // Draw continue prompt
  tft.setTextColor(ST77XX_RED);
  tft.setCursor(35, 115);
  tft.setTextSize(2);
  tft.print("[A] TO CONTINUE");
  */

  // Draw instructions
  int selectionColor[] = {ST77XX_RED, ST77XX_RED, ST77XX_RED};
  selectionColor[uiState.startMenuSelection] = ST77XX_GREEN;

  drawMenuOption(15, 30, 100, 40, selectionColor[0], start_menu[0]);
  drawMenuOption(125, 30, 100, 40, selectionColor[1], start_menu[1]);
  drawMenuOption(15, 75, 210, 40, selectionColor[2], start_menu[2]);
}

void startChangeSelection(int curIndex, int nextIndex)
{
  // Change cur to unselected color
  if (curIndex == 2)
  {
    drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 210, 40, ST77XX_RED, start_menu[curIndex]);
  }
  else
  {
    drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 40, ST77XX_RED, start_menu[curIndex]);
  }

  // Change next to selected color
  if (nextIndex == 2)
  {
    drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 210, 40, ST77XX_GREEN, start_menu[nextIndex]);
  }
  else
  {
    drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 40, ST77XX_GREEN, start_menu[nextIndex]);
  }
}

void mainMenu() 
{
  int animationIndex = uiState.mainMenuPage * 4;
  int colorIndex = uiState.mainMenuSelection % 4;
  int selectionColor[] = {ST77XX_RED, ST77XX_RED, ST77XX_RED, ST77XX_RED};
  selectionColor[colorIndex] = ST77XX_GREEN;

  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title
  tft.setCursor(15, 10);
  tft.setTextSize(1);
  tft.print("[ A ] Select Primary Animation");

  // Draw menu options
  drawMenuOption(15, 30, 100, 40, selectionColor[0], all_animations[animationIndex]);
  drawMenuOption(125, 30, 100, 40, selectionColor[1], all_animations[animationIndex + 1]);
  drawMenuOption(15, 75, 100, 40, selectionColor[2], all_animations[animationIndex + 2]);
  drawMenuOption(125, 75, 100, 40, selectionColor[3], all_animations[animationIndex + 3]);

  // Navigation
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);

  tft.setCursor(10, 120);
  // tft.print("<- [ CTRL ] Start");

  tft.setCursor(118, 120);
  tft.print(String(uiState.mainMenuPage + 1));

  if (uiState.mainMenuPage != 5)
  {
    tft.setCursor(150, 120);
    tft.print("Next [ B ] ->");
  }
}

void mainMenuChangeSelection(int curIndex, int nextIndex)
{
    int selectionIndex = uiState.mainMenuPage * 4;

    // Change cur to unselected color
    drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 40, ST77XX_RED, all_animations[selectionIndex + curIndex]);

    // Change next to selected color
    drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 40, ST77XX_GREEN, all_animations[selectionIndex + nextIndex]);
}

void mainMenu2()
{
  int animationIndex = uiState.mainMenuPage2 * 4;
  int colorIndex = uiState.mainMenuSelection2 % 4;
  int selectionColor[] = {ST77XX_RED, ST77XX_RED, ST77XX_RED, ST77XX_RED};
  selectionColor[colorIndex] = ST77XX_GREEN;

  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title
  tft.setCursor(15, 10);
  tft.setTextSize(1);
  tft.print("[ A ] Select Secondary Animation");

  // Draw menu options
  drawMenuOption(15, 30, 100, 40, selectionColor[0], all_animations_2[animationIndex]);
  drawMenuOption(125, 30, 100, 40, selectionColor[1], all_animations_2[animationIndex + 1]);
  drawMenuOption(15, 75, 100, 40, selectionColor[2], all_animations_2[animationIndex + 2]);
  drawMenuOption(125, 75, 100, 40, selectionColor[3], all_animations_2[animationIndex + 3]);

  // Navigation
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);

  tft.setCursor(10, 120);
  // tft.print("<- [ CTRL ] Start");

  tft.setCursor(118, 120);
  tft.print(String(uiState.mainMenuPage2 + 1));

  if (uiState.mainMenuPage2 != 5)
  {
    tft.setCursor(150, 120);
    tft.print("Next [ B ] ->");
  }
}

void mainMenuChangeSelection2(int curIndex, int nextIndex)
{
  int selectionIndex = uiState.mainMenuPage2 * 4;

  // Change cur to unselected color
  drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 40, ST77XX_RED, all_animations_2[selectionIndex + curIndex]);

  // Change next to selected color
  drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 40, ST77XX_GREEN, all_animations_2[selectionIndex + nextIndex]);
}

void animationMenu()
{
  int selectionIndex = uiState.mainMenuPage * 4 + uiState.mainMenuSelection;
  int selectionColor[] = {ST77XX_CYAN, ST77XX_CYAN, ST77XX_CYAN};
  selectionColor[uiState.animationMenuSelection] = ST77XX_GREEN;

  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_RED);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title and menu options
  tft.setCursor(15, 15);
  tft.setTextSize(1.5);

  if (selectionIndex == 1 || selectionIndex == 3 || selectionIndex == 5)
  {
    tft.print("Primary: " + String(all_animations[selectionIndex]) + " - " + String(all_gradients[uiState.gradientMenuSelection]));
    tft.setCursor(15, 27);
    tft.print("Secondary: " + String(all_animations_2[uiState.mainMenuPage2 * 4 + uiState.mainMenuSelection2]));
    drawMenuOption(15, 40, 100, 40, selectionColor[0], run_ani_menu[0]);
    drawMenuOption(125, 40, 100, 40, selectionColor[1], run_ani_menu[1]);
    drawMenuOption(15, 85, 210, 30, selectionColor[2], run_ani_menu[2]);
  } else {
    tft.print("Primary: " + String(all_animations[selectionIndex]));
    tft.setCursor(15, 27);
    tft.print("Secondary: " + String(all_animations_2[uiState.mainMenuPage2 * 4 + uiState.mainMenuSelection2]));
    drawMenuOption(15, 40, 100, 76, selectionColor[0], "Back");
    drawMenuOption(125, 40, 100, 76, selectionColor[1], "Run");
  }
}

void animationMenuChangeSelection(int curIndex, int nextIndex)
{
  int selectionIndex = uiState.mainMenuPage * 4 + uiState.mainMenuSelection;
  
  // Change cur to unselected color
  if (selectionIndex == 1 || selectionIndex == 3 || selectionIndex == 5)
  {
      if (curIndex == 2)
      {
          drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y + 10, 210, 30, ST77XX_CYAN, run_ani_menu[curIndex]);
      } else {
          drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y + 10, 100, 40, ST77XX_CYAN, run_ani_menu[curIndex]);
      }
  } else {
      drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y + 10, 100, 76, ST77XX_CYAN, run_ani_menu[curIndex]);
  }
  
  // Change next to selected color
  if (selectionIndex == 1 || selectionIndex == 3 || selectionIndex == 5)
  {
      if (nextIndex == 2)
      {
          drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y + 10, 210, 30, ST77XX_GREEN, run_ani_menu[nextIndex]);
      } else {
          drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y + 10, 100, 40, ST77XX_GREEN, run_ani_menu[nextIndex]);
      }
  } else {
      drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y + 10, 100, 76, ST77XX_GREEN, run_ani_menu[nextIndex]);
  }
}

void gradientMenu()
{
  int selectionColor[] = {ST77XX_RED, ST77XX_RED, ST77XX_RED, ST77XX_RED};
  selectionColor[uiState.gradientMenuSelection] = ST77XX_GREEN;

  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title
  tft.setCursor(15, 10);
  tft.setTextSize(1);
  tft.print("[ A ] Select Gradient");

  // Draw menu options
  drawMenuOption(15, 30, 100, 40, selectionColor[0], all_gradients[0]);
  drawMenuOption(125, 30, 100, 40, selectionColor[1], all_gradients[1]);
  drawMenuOption(15, 75, 100, 40, selectionColor[2], all_gradients[2]);
  drawMenuOption(125, 75, 100, 40, selectionColor[3], all_gradients[3]);

  // Draw instructions
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(165, 120);
  tft.print("Back [ B ]");
}

void gradientMenuChangeSelection(int curIndex, int nextIndex)
{
  // Change cur to unselected color
  drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 40, ST77XX_RED, all_gradients[curIndex]);

  // Change next to selected color
  drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 40, ST77XX_GREEN, all_gradients[nextIndex]);
}

void settingsMenu()
{
  int selectionColor[] = {ST77XX_RED, ST77XX_RED};
  selectionColor[uiState.settingsMenuSelection] = ST77XX_GREEN;

  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title
  tft.setCursor(15, 10);
  tft.setTextSize(1);
  tft.print("[ A ] Select Settings");

  // Draw menu options
  drawMenuOption(15, 30, 100, 86, selectionColor[0], settings_menu[0]);
  drawMenuOption(125, 30, 100, 86, selectionColor[1], settings_menu[1]);

  // Draw instructions
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(165, 120);
  tft.print("Back [ B ]");
}

void settingsMenuChangeSelection(int curIndex, int nextIndex)
{
  // Change cur to unselected color
  drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 86, ST77XX_RED, settings_menu[curIndex]);

  // Change next to selected color
  drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 86, ST77XX_GREEN, settings_menu[nextIndex]);
}

void changeSettingsMenu()
{
  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title
  tft.setCursor(15, 10);
  tft.setTextSize(1);
  tft.print("[ UP ] [ DOWN ] Change Settings");

  // Draw settings
  tft.setCursor(15, 60);
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(2);
  if(uiState.settingsMenuSelection == 0)
  {
    tft.print(String(settings_menu[uiState.settingsMenuSelection]) + " " + String(uiState.brightnessSetting[uiState.brightnessSelection]));
  }
  else
  {
    tft.print(String(settings_menu[uiState.settingsMenuSelection]) + " " + String(uiState.speedSetting[uiState.speedSelection]));
  }

  Serial.println("brightnessSelection" + uiState.brightnessSelection);
  Serial.println("speedSelection" + uiState.speedSelection);

  // Draw instructions
  tft.setTextColor(ST77XX_WHITE);

  tft.setCursor(15, 120);
  tft.setTextSize(1);
  tft.print("[ A ] Save [ B ] Back");
}

void changeSettingsMenuChangeSelection(int curIndex, int nextIndex)
{
  // Update settings value
  tft.setCursor(15, 60);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(2);
  if(uiState.settingsMenuSelection == 0)
  {
    tft.print(String(settings_menu[uiState.settingsMenuSelection]) + " " + String(uiState.brightnessSetting[curIndex]));
  }
  else
  {
    tft.print(String(settings_menu[uiState.settingsMenuSelection]) + " " + String(uiState.speedSetting[curIndex]));
  }

  tft.setCursor(15, 60);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  if (uiState.settingsMenuSelection == 0)
  {
    tft.print(String(settings_menu[uiState.settingsMenuSelection]) + " " + String(uiState.brightnessSetting[nextIndex]));
  }
  else
  {
    tft.print(String(settings_menu[uiState.settingsMenuSelection]) + " " + String(uiState.speedSetting[nextIndex]));
  }
}

void bitmapMenu()
{
  int selectionColor[] = {ST77XX_CYAN, ST77XX_CYAN};
  selectionColor[uiState.bitmapMenuSelection] = ST77XX_GREEN;

  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title
  tft.setCursor(15, 10);
  tft.setTextSize(1);
  tft.print("Bitmap");

  drawMenuOption(15, 30, 100, 86, selectionColor[0], bitmap_menu[0]);
  drawMenuOption(125, 30, 100, 86, selectionColor[1], bitmap_menu[1]);
}

void bitmapMenuChangeSelection(int curIndex, int nextIndex)
{
  // Change cur to unselected color
  drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 86, ST77XX_CYAN, bitmap_menu[curIndex]);

  // Change next to selected color
  drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 86, ST77XX_GREEN, bitmap_menu[nextIndex]);
}

void usbMode()
{
  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title
  tft.setTextColor(ST77XX_RED);
  tft.setCursor(15, 10);
  tft.setTextSize(1);
  if (sdConnected){
    tft.print("SD Mode - Connected");
  } else {
    tft.print("SD Mode - Not Connected");
    tft.setCursor(15, 30);
    tft.print("Please Connect and Restart");
  }

  for (int i = 0; i < sdFileCount; i++)
  {
    float sizeInMB = sdFiles[i].size / 1024.0;
    Serial.println(sdFiles[i].name + " " + String(sizeInMB, 1) + " KB"); // Draw menu options
  }

  tft.setTextColor(ST77XX_WHITE);
  for (int i = 0; i < sdFileCount; i++)
  {
    int color = ST77XX_WHITE;
    if (i == uiState.usbModeSelection)
    {
      color = ST77XX_GREEN;
    }
    tft.fillRect(15, 30 + i * 20, 5, 5, color);
    tft.setTextColor(color);
    tft.setCursor(30, 30 + i * 20);
    float sizeInMB = sdFiles[i].size / 1024.0;
    tft.print(sdFiles[i].name + " " + String(sizeInMB, 1) + " KB"); // Draw menu options
  }
}

void usbModeChangeSelection(int curIndex, int nextIndex)
{
  // Change cur to unselected color
  tft.fillRect(15, 30 + curIndex * 20, 5, 5, ST77XX_WHITE);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(30, 30 + curIndex * 20);
  float sizeInMB = sdFiles[curIndex].size / 1024.0;
  tft.print(sdFiles[curIndex].name + " " + String(sizeInMB, 1) + " KB"); // Draw menu options

  // Change next to selected color
  tft.fillRect(15, 30 + nextIndex * 20, 5, 5, ST77XX_GREEN);
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(30, 30 + nextIndex * 20);
  sizeInMB = sdFiles[nextIndex].size / 1024.0;
  tft.print(sdFiles[nextIndex].name + " " + String(sizeInMB, 1) + " KB"); // Draw menu options
}

void storageMode()
{
  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title
  tft.setTextColor(ST77XX_RED);
  tft.setCursor(15, 10);
  tft.setTextSize(1);
  tft.print("Storage Mode");

  tft.setTextColor(ST77XX_WHITE);
  for(int i = 0; i < fileCount; i++)
  {
    int color = ST77XX_WHITE;
    if(i == uiState.storageModeSelection)
    {
      color = ST77XX_GREEN;
    }
    tft.fillRect(15, 30 + i * 20, 5, 5, color);
    tft.setTextColor(color);
    tft.setCursor(30, 30 + i * 20);
    float sizeInMB = files[i].size / 1024.0;
    tft.print(files[i].name + " " + String(sizeInMB, 1) + " KB"); // Draw menu options
  }
}

void storageModeChangeSelection(int curIndex, int nextIndex)
{
  // Change cur to unselected color
  tft.fillRect(15, 30 + curIndex * 20, 5, 5, ST77XX_WHITE);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(30, 30 + curIndex * 20);
  float sizeInMB = files[curIndex].size / 1024.0;
  tft.print(files[curIndex].name + " " + String(sizeInMB, 1) + " KB"); // Draw menu options

  // Change next to selected color
  tft.fillRect(15, 30 + nextIndex * 20, 5, 5, ST77XX_GREEN);
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(30, 30 + nextIndex * 20);
  sizeInMB = files[nextIndex].size / 1024.0;
  tft.print(files[nextIndex].name + " " + String(sizeInMB, 1) + " KB"); // Draw menu options
}

void bitmapAnimationMenu()
{
  int selectionColor[] = {ST77XX_CYAN, ST77XX_CYAN, ST77XX_CYAN};
  selectionColor[uiState.bitmapAnimationMenuSelection] = ST77XX_GREEN;

  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_RED);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title and menu options
  tft.setCursor(15, 15);
  tft.setTextSize(1.5);

  float sizeInMB = files[uiState.storageModeSelection].size / 1024.0;
  tft.print(files[uiState.storageModeSelection].name + " " + String(sizeInMB, 1) + " KB");
  
  drawMenuOption(15, 30, 100, 40, selectionColor[0], run_bitmap_ani_menu[0]);
  drawMenuOption(125, 30, 100, 40, selectionColor[1], run_bitmap_ani_menu[1]);
  drawMenuOption(15, 75, 210, 40, selectionColor[2], run_bitmap_ani_menu[2]);
}

void bitmapAnimationMenuChangeSelection(int curIndex, int nextIndex)
{
  // Change cur to unselected color
  if (curIndex == 2)
  {
    drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 210, 40, ST77XX_CYAN, run_bitmap_ani_menu[curIndex]);
  }
  else
  {
    drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 40, ST77XX_CYAN, run_bitmap_ani_menu[curIndex]);
  }

  // Change next to selected color
  if (nextIndex == 2)
  {
    drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 210, 40, ST77XX_GREEN, run_bitmap_ani_menu[nextIndex]);
  }
  else
  {
    drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 40, ST77XX_GREEN, run_bitmap_ani_menu[nextIndex]);
  }
}

void usbAnimationMenu()
{
  int selectionColor[] = {ST77XX_CYAN, ST77XX_CYAN};
  selectionColor[uiState.usbAnimationMenuSelection] = ST77XX_GREEN;

  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_RED);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title and menu options
  tft.setCursor(15, 15);
  tft.setTextSize(1.5);

  float sizeInMB = sdFiles[uiState.usbModeSelection].size / 1024.0;
  tft.print(sdFiles[uiState.usbModeSelection].name + " " + String(sizeInMB, 1) + " KB");

  drawMenuOption(15, 30, 100, 86, selectionColor[0], run_usb_ani_menu[0]);
  drawMenuOption(125, 30, 100, 86, selectionColor[1], run_usb_ani_menu[1]);
}

void usbAnimationMenuChangeSelection(int curIndex, int nextIndex)
{
  // Change cur to unselected color
  drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 86, ST77XX_CYAN, run_usb_ani_menu[curIndex]);

  // Change next to selected color
  drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 86, ST77XX_GREEN, run_usb_ani_menu[nextIndex]);
}

void drawMenuOption(int x, int y, int w, int h, int color, String text)
{
  tft.drawRect(x, y, w, h, color);
  tft.setCursor(x + 15, y + 10);
  tft.setTextColor(color);
  tft.setTextSize(1);
  drawWrappedText(text, x + 15, y + 10, w - 30, h);
}

void drawWrappedText(String text, int x, int y, int maxWidth, int maxHeight)
{
  int16_t x1, y1;
  uint16_t w, h;
  int cursorY = y;
  int lineHeight = 16; // Adjust based on your text size and font

  while (text.length() > 0)
  {
    int len = text.length();
    String line = "";

    // Iterate over the text to find where to split it
    for (int i = 0; i < len; i++)
    {
      tft.getTextBounds(text.substring(0, i + 1), x, cursorY, &x1, &y1, &w, &h);
      if (w > maxWidth)
      {
        break;
      }
      line = text.substring(0, i + 1);
    }

    // Print the line and remove it from the string
    tft.setCursor(x, cursorY);
    tft.print(line);
    text = text.substring(line.length());

    // Move to the next line
    cursorY += lineHeight;

    // Check if the cursor exceeds the screen height
    if (cursorY + lineHeight > maxHeight)
    {
      break; // Stop printing if there's no space left
    }
  }
}