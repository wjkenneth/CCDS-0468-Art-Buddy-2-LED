#include <Arduino.h>
#include <FastLED.h>
#include <SPI.h>
#include "main.h"
#include "animations.h"

// Fire animation
#define COOLING 55
#define SPARKING 120
#define FRAMES_PER_SECOND 30
bool gReverseDirection = false;
CRGBPalette16 gPal = HeatColors_p;
uint8_t heat[NUM_LEDS];   // Array of temperature readings at each simulation cell
uint8_t heat2[NUM_LEDS2];
bool initialized = false; // Flag to check if the heat array is initialized
bool initialized2 = false;

void fadeall()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i].nscale8(250);
    }
}

void fadeall2()
{
    for (int i = 0; i < NUM_LEDS2; i++)
    {
        leds2[i].nscale8(250);
    }
}

void Fire()
{
    if (!initialized)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            heat[i] = random(100, 150); // Random initial heat values
        }
        initialized = true; // Set the flag to true after initialization
    }

    // Add entropy to random number generator; we use a lot of it.
    random16_add_entropy(random());

    // Array of temperature readings at each simulation cell
    // static uint8_t heat[NUM_LEDS];

    // Step 1.  Cool down every cell a little
    for (int i = 0; i < NUM_LEDS; i++)
    {
        heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = NUM_LEDS - 1; k >= 2; k--)
    {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKING)
    {
        int y = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (int j = 0; j < NUM_LEDS; j++)
    {
        // Scale the heat value from 0-255 down to 0-240
        // for best results with color palettes.
        uint8_t colorindex = scale8(heat[j], 240);
        CRGB color = ColorFromPalette(gPal, colorindex);
        int pixelnumber;
        if (gReverseDirection)
        {
            pixelnumber = (NUM_LEDS - 1) - j;
        }
        else
        {
            pixelnumber = j;
        }
        leds[pixelnumber] = color;
    }

    FastLED.show(); // display this frame
}

void Beat()
{
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS - 1, 0, 0);
    uint8_t sinBeat2 = beatsin8(30, 0, NUM_LEDS - 1, 0, 85);
    uint8_t sinBeat3 = beatsin8(30, 0, NUM_LEDS - 1, 0, 170);

    if (uiState.gradientMenuSelection == 0)
    {
        leds[sinBeat] = CRGB::Blue;
        leds[sinBeat2] = CRGB::Red;
        leds[sinBeat3] = CRGB::White;
    }

    if (uiState.gradientMenuSelection == 1)
    {
        leds[sinBeat] = CRGB::HotPink;
        leds[sinBeat2] = CRGB::MediumOrchid;
        leds[sinBeat3] = CRGB::MediumPurple;
    }

    if (uiState.gradientMenuSelection == 2)
    {
        leds[sinBeat] = CRGB::Lime;
        leds[sinBeat2] = CRGB::SpringGreen;
        leds[sinBeat3] = CRGB::DarkGreen;
    }

    if (uiState.gradientMenuSelection == 3)
    {
        leds[sinBeat] = CRGB::Aqua;
        leds[sinBeat2] = CRGB::DeepSkyBlue;
        leds[sinBeat3] = CRGB::Blue;
    }

    fadeToBlackBy(leds, NUM_LEDS, 10);
    FastLED.show();
}

void Fade()
{
    EVERY_N_MILLISECONDS(10)
    {
        // Switch on an LED at random, choosing a random color from the palette
        leds[random8(0, NUM_LEDS - 1)] = ColorFromPalette(gPal, random8(), 255, LINEARBLEND);
    }

    // Fade all LEDs down by 1 in brightness each time this is called
    fadeToBlackBy(leds, NUM_LEDS, 1);

    FastLED.show();
}

void Blur()
{
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS - 1, 0, 0);
    uint8_t sinBeat2 = beatsin8(30, 0, NUM_LEDS - 1, 0, 85);
    uint8_t sinBeat3 = beatsin8(30, 0, NUM_LEDS - 1, 0, 170);

    if (uiState.gradientMenuSelection == 0)
    {
        leds[sinBeat] = CRGB::Blue;
        leds[sinBeat2] = CRGB::Red;
        leds[sinBeat3] = CRGB::White;
    }

    if (uiState.gradientMenuSelection == 1)
    {
        leds[sinBeat] = CRGB::HotPink;
        leds[sinBeat2] = CRGB::MediumOrchid;
        leds[sinBeat3] = CRGB::MediumPurple;
    }

    if (uiState.gradientMenuSelection == 2)
    {
        leds[sinBeat] = CRGB::Lime;
        leds[sinBeat2] = CRGB::SpringGreen;
        leds[sinBeat3] = CRGB::DarkGreen;
    }

    if (uiState.gradientMenuSelection == 3)
    {
        leds[sinBeat] = CRGB::Aqua;
        leds[sinBeat2] = CRGB::DeepSkyBlue;
        leds[sinBeat3] = CRGB::Blue;
    }

    EVERY_N_MILLISECONDS(10)
    {
        for (int i = 0; i < 4; i++)
        {
            blur1d(leds, NUM_LEDS, 50);
        }
    }

    FastLED.show();
}

void ColorChange()
{
    int i = 0;
    for (int j = 0; j < 6; j++)
    {
        if (i == 0)
            fill_solid(leds, NUM_LEDS, CRGB::Red);
        if (i == 1)
            fill_solid(leds, NUM_LEDS, CRGB::Yellow);
        if (i == 2)
            fill_solid(leds, NUM_LEDS, CRGB::Blue);
        if (i == 3)
            fill_solid(leds, NUM_LEDS, CRGB::HotPink);
        if (i == 4)
            fill_solid(leds, NUM_LEDS, CRGB::MediumOrchid);
        if (i == 5)
            fill_solid(leds, NUM_LEDS, CRGB::MediumPurple);

        i++;
        FastLED.show();
        delay(30);
        if (i == 6)
            i = 0;
        if (digitalRead(SWITCH) == HIGH) break;
    }
}

void MovingDot()
{
    uint16_t sinBeat = beatsin16(30, 0, NUM_LEDS - 1, 0, 0); //NUM_LEDS - 1

    //leds[sinBeat] = CRGB::Blue;
    if (uiState.gradientMenuSelection == 0)
        leds[sinBeat].setRGB(255, 128, 0);
    if (uiState.gradientMenuSelection == 1)
        leds[sinBeat].setRGB(255, 0, 0);
    if (uiState.gradientMenuSelection == 2)
        leds[sinBeat].setRGB(0, 255, 0);
    if (uiState.gradientMenuSelection == 3)
        leds[sinBeat].setRGB(0, 0, 255);

    fadeToBlackBy(leds, NUM_LEDS, 10); //NUM_LEDS 
    FastLED.show();
}

void Cylon()
{
    static uint8_t hue = 0;
    // First slide the led in one direction
    for (int i = 0; i < NUM_LEDS; i++)
    {
        if (digitalRead(SWITCH) == HIGH) break;

        // Set the i'th led to red
        leds[i] = CHSV(hue++, 255, 255);

        // Show the leds
        FastLED.show();

        // now that we've shown the leds, reset the i'th led to black
        fadeall();

        if (digitalRead(SWITCH) == HIGH) break;
    }
}
void Boom()
{
    int red = 255;
    int green = 0;
    int blue = 0;

    EVERY_N_MILLISECONDS(100)
    {
        int cutoff = random8(30, 90);
        for (int i = 0; i < cutoff; i++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds[i].setRGB(213, 240, 76);
        }
        for (int j = cutoff; j < NUM_LEDS; j++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds[j].setRGB(red, green, blue);
        }
        FastLED.show();
    }
    
}
void FirstLight()
{
    EVERY_N_MILLISECONDS(200)
    {
        // Move a single white led
        for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1)
        {
            // Turn our current led on to white, then show the leds
            leds[whiteLed] = CRGB::White;

            // Show the leds (only one of which is set to white, from above)
            FastLED.show();

            // Turn our current led back to black for the next loop around
            leds[whiteLed] = CRGB::Black;
        }
    }
}

void Sparkle()
{
    int pixel = random(NUM_LEDS);
    int i = abs(pixel - 5);
    for (int j = i; j <= pixel; j++)
    {
        leds[j].setRGB(255, 0, 0);
    }
    FastLED.show();
    delay(10);
    for (int k = i; k <= pixel; k++)
    {
        leds[k].setRGB(0, 0, 0);
    }
}

byte *Wheel(byte WheelPosition)
{
    static byte c[3];

    if (WheelPosition < 85)
    {
        c[0] = WheelPosition * 3;
        c[1] = 255 - WheelPosition * 3;
        c[2] = 0;
    }
    else if (WheelPosition < 170)
    {
        WheelPosition -= 85;
        c[0] = 255 - WheelPosition * 3;
        c[1] = 0;
        c[2] = WheelPosition * 3;
    }
    else
    {
        WheelPosition -= 170;
        c[0] = 0;
        c[1] = WheelPosition * 3;
        c[2] = 255 - WheelPosition * 3;
    }

    return c;
}

void RainbowCycle()
{
    byte *c;
    uint16_t i, j;

    for (j = 0; j < 256; j++)
    {
        if (digitalRead(SWITCH) == HIGH) break;
        for (i = 0; i < NUM_LEDS; i++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
            leds[NUM_LEDS - 1 - i].setRGB(*c, *(c + 1), *(c + 2));
        }
        FastLED.show();
    }
}

void Strange()
{
    EVERY_N_MILLISECONDS(20)
    {
        for (int i = 71; i < 101; i++)
        {
            int random = random8(0, 10);
            leds[i] = CHSV(30, 255, 255);
            if (random % 5 == 1)
            {
                leds[i].setRGB(255, 128, 0);
            }
            else
            {
                leds[i].setRGB(0, 0, 0);
            }
        }

        for (int i = 45; i <= 65; i++)
        {
            leds[i] = CHSV(30, 255, 255);
            leds[i].setRGB(255, 128, 0);
        }

        for (int i = 20; i <= 40; i++)
        {
            int random = random8(0, 10);
            leds[i] = CHSV(30, 255, 255);
            if (random % 5 == 1)
            {
                leds[i].setRGB(255, 128, 0);
            }
            else
            {
                leds[i].setRGB(0, 0, 0);
            }
        }

        for (int i = 5; i <= 15; i++)
        {
            leds[i] = CHSV(30, 255, 255);
            leds[i].setRGB(255, 128, 0);
        }

        FastLED.show();
    }
}

void RainbowLines()
{
    for (int i = 71; i < 101; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(255, 0, 0);
    }

    for (int i = 45; i <= 65; i++)
    {
        leds[i] = CHSV(60, 255, 255);
        leds[i].setRGB(255, 255, 0);
    }

    for (int i = 20; i <= 40; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(0, 255, 0);
    }

    for (int i = 5; i <= 15; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(0, 0, 255);
    }
    FastLED.show();
}

void JagWhite()
{
    EVERY_N_MILLISECONDS(500)
    {
        for (int i = 71; i < 101; i++)
        {
            leds[i] = CHSV(30, 255, 255);
            leds[i].setRGB(255, 255, 255);
        }

        delay(100);
        FastLED.show(); // Send the updated color data to the LED strip
        fill_solid(leds, NUM_LEDS, CRGB::Black);

        for (int i = 45; i <= 65; i++)
        {
            leds[i] = CHSV(30, 255, 255);
            leds[i].setRGB(255, 255, 255);
        }
        delay(100);
        FastLED.show(); // Send the updated color data to the LED strip
        fill_solid(leds, NUM_LEDS, CRGB::Black);

        for (int i = 20; i <= 40; i++)
        {
            leds[i] = CHSV(30, 255, 255);
            leds[i].setRGB(255, 255, 255);
        }

        delay(100);
        FastLED.show(); // Send the updated color data to the LED strip
        fill_solid(leds, NUM_LEDS, CRGB::Black);

        for (int i = 5; i <= 15; i++)
        {
            leds[i] = CHSV(30, 255, 255);
            leds[i].setRGB(255, 255, 255);
        }
        FastLED.show();
    }
}

void Stars()
{
    EVERY_N_MILLISECONDS(20)
    {
        for (int i = 0; i < 101; i++)
        {
            int random = random8(0, 10);
            leds[i] = CHSV(30, 255, 255);
            if (random % 5 == 1)
            {
                leds[i].setRGB(255, 255, 255);
            }
            else
            {
                leds[i].setRGB(0, 0, 0);
            }
        }
        FastLED.show();
    }
}

void Pride()
{
    for (int i = 91; i < 101; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(255, 0, 0);
    }

    for (int i = 75; i <= 85; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(255, 165, 0);
    }

    for (int i = 60; i <= 70; i++)
    {
        leds[i] = CHSV(60, 255, 255);
        leds[i].setRGB(255, 255, 0);
    }

    for (int i = 45; i <= 55; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(0, 255, 0);
    }

    for (int i = 30; i <= 40; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(0, 0, 255);
    }

    for (int i = 5; i <= 25; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(160, 32, 240);
    }
    FastLED.show();
}

void ColorFrenzy()
{
    int i = 0;
    for (int j = 0; j < 6; j++)
    {
        if (i == 0)
            fill_solid(leds, NUM_LEDS, CRGB::Red);
        if (i == 1)
            fill_solid(leds, NUM_LEDS, CRGB::Yellow);
        if (i == 2)
            fill_solid(leds, NUM_LEDS, CRGB::Blue);
        if (i == 3)
            fill_solid(leds, NUM_LEDS, CRGB::HotPink);
        if (i == 4)
            fill_solid(leds, NUM_LEDS, CRGB::MediumOrchid);
        if (i == 5)
            fill_solid(leds, NUM_LEDS, CRGB::MediumPurple);
        i++;
        FastLED.show();
        delay(30);
        if (i == 6)
            i = 0;
        if (digitalRead(SWITCH) == HIGH) break;
    }
}

void Histo()
{
    EVERY_N_MILLISECONDS(500)
    {
        int cutoff = random8(30, 60);
        for (int i = 0; i < cutoff; i++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds[i].setRGB(255, 255, 255);
        }
        for (int j = cutoff; j < NUM_LEDS; j++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            int cutoff2 = random8(1, 10);
            if (cutoff2 % 3 == 0)
                leds[j].setRGB(255, 0, 0);
            if (cutoff2 % 3 == 1)
                leds[j].setRGB(0, 255, 0);
            if (cutoff2 % 3 == 2)
                leds[j].setRGB(0, 0, 255);
        }
        FastLED.show();
    }
}

void RandHisto()
{
    EVERY_N_MILLISECONDS(500)
    {
        int cutoffsize = random8(40, NUM_LEDS / 2);
        int cutoffcolor = random8(1, 10);
        for (int j = cutoffsize; j < NUM_LEDS / 2; j++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            if (cutoffcolor % 3 == 0)
                leds[j].setRGB(255, 0, 0);
            if (cutoffcolor % 3 == 1)
                leds[j].setRGB(0, 255, 0);
            if (cutoffcolor % 3 == 2)
                leds[j].setRGB(0, 0, 255);
        }

        for (int k = NUM_LEDS / 2; k < NUM_LEDS / 2 + cutoffsize; k++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            if (cutoffcolor % 3 == 0)
                leds[k].setRGB(255, 0, 0);
            if (cutoffcolor % 3 == 1)
                leds[k].setRGB(0, 255, 0);
            if (cutoffcolor % 3 == 2)
                leds[k].setRGB(0, 0, 255);
        }
        FastLED.show();
    }
}

void Bang()
{
    for (int i = 91; i < 101; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(255, 0, 0);
    }
    static uint8_t hue = 0;
    // First slide the led in one direction
    for (int i = 40; i < 80; i++)
    {
        // Set the i'th led to red
        leds[i] = CHSV(hue++, 255, 255);
        // Show the leds
        FastLED.show();
        if (digitalRead(SWITCH) == HIGH) break;
        // now that we've shown the leds, reset the i'th led to black
        fadeall();
    }

    for (int i = 20; i <= 30; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(255, 0, 0);
    }
}

unsigned long previousMillisRL = 0; // Stores last time LEDs were updated
int countRL = 0;                    // Stores count for incrementing up to the NUM_LEDs

void RunningLights()
{
    unsigned long currentMillis = millis(); // Get the time
    if (currentMillis - previousMillisRL >= 1200)
    {
        previousMillisRL = currentMillis; // Save the last time the LEDs were updated
        countRL = 0;                      // Reset the count to 0 after each interval
    }
    else
    {
        if (countRL < NUM_LEDS)
        {
            leds[countRL % (NUM_LEDS + 1)].setRGB(255, 255, 255); // Set LEDs with the color value
            countRL++;
        }
    }
    fadeToBlackBy(leds, NUM_LEDS, random(10, 60)); // Fade the tail LEDs to black
    FastLED.show();
}

unsigned long previousMillisRL2 = 0; // Stores last time LEDs were updated
int countRL2 = 0;                    // Stores count for incrementing up to the NUM_LEDs

void RunningLights2()
{
    unsigned long currentMillis = millis(); // Get the time
    if (currentMillis - previousMillisRL2 >= 1200)
    {
        previousMillisRL2 = currentMillis; // Save the last time the LEDs were updated
        countRL2 = 0;                      // Reset the count to 0 after each interval
    }
    else
    {
        if (countRL2 < NUM_LEDS2)
        {
            leds2[countRL2 % (NUM_LEDS2 + 1)].setRGB(255, 255, 255); // Set LEDs with the color value
            countRL2++;
        }
    }
    fadeToBlackBy(leds2, NUM_LEDS2, random(10, 60)); // Fade the tail LEDs to black
    FastLED.show();
}

unsigned long previousTimeM = 0; // Stores the last time an update occurred
const int meteorInterval = 30;  // Interval between each frame of the meteor animation
int currentLEDM = 0;             // Tracks the current LED position

void Meteor()
{
    unsigned long currentTime = millis();

    // Check if it's time to update the LEDs
    if (currentTime - previousTimeM >= meteorInterval)
    {
        previousTimeM = currentTime;

        // Fade brightness of all LEDs one step
        for (int j = 0; j < NUM_LEDS; j++)
        {
            if (random(10) > 5)
            {
                leds[j].fadeToBlackBy(64);
            }
        }

        // Draw the meteor trail
        for (int j = 0; j < 10; j++)
        {
            int pos = currentLEDM - j;
            if (pos >= 0 && pos < NUM_LEDS)
            {
                leds[pos].setRGB(255, 255, 255);
            }
        }

        FastLED.show(); // Update the LEDs

        // Move to the next LED position
        currentLEDM++;
        if (currentLEDM >= NUM_LEDS)
        {
            currentLEDM = 0; // Reset to the beginning after reaching the end
        }
    }
}

unsigned long previousTimeM2 = 0; // Stores the last time an update occurred
const int meteorInterval2 = 30;   // Interval between each frame of the meteor animation
int currentLEDM2 = 0;             // Tracks the current LED position

void Meteor2()
{
    unsigned long currentTime = millis();

    // Check if it's time to update the LEDs
    if (currentTime - previousTimeM2 >= meteorInterval)
    {
        previousTimeM2 = currentTime;

        // Fade brightness of all LEDs one step
        for (int j = 0; j < NUM_LEDS2; j++)
        {
            if (random(10) > 5)
            {
                leds2[j].fadeToBlackBy(64);
            }
        }

        // Draw the meteor trail
        for (int j = 0; j < 2; j++)
        {
            int pos = currentLEDM2 - j;
            if (pos >= 0 && pos < NUM_LEDS2)
            {
                leds2[pos].setRGB(255, 255, 255);
            }
        }

        FastLED.show(); // Update the LEDs

        // Move to the next LED position
        currentLEDM2++;
        if (currentLEDM2 >= NUM_LEDS2)
        {
            currentLEDM2 = 0; // Reset to the beginning after reaching the end
        }
    }
}

unsigned long previousTimeS = 0; // Tracks the last update time
const int chaseIntervalS = 30;   // Time interval between each step
int SplitJ = random(0,256);                      // Position in the color wheel
int SplitQ = 0;                      // Offset for the chase effect

void Split()
{
    unsigned long currentTime = millis();

    // Check if it's time to update the LEDs
    if (currentTime - previousTimeS >= chaseIntervalS)
    {
        previousTimeS = currentTime;

        // Turn every third pixel on
        byte *c;
        for (int i = 0; i < NUM_LEDS; i = i + 3)
        {
            c = Wheel((i + SplitJ) % 255);
            leds[i + SplitQ].setRGB(*c, *(c + 1), *(c + 2));
            leds[i + SplitQ + 1].setRGB(*c, *(c + 1), *(c + 2));
        }
        FastLED.show(); // Update the LEDs

        // Turn every third pixel off to create the chase effect
        for (int i = 0; i < NUM_LEDS; i = i + 3)
        {
            leds[i + SplitQ].setRGB(0, 0, 0);
            leds[i + SplitQ + 1].setRGB(*c, *(c + 1), *(c + 2));
        }

        // Update the chase offset and wheel position
        SplitQ = (SplitQ + 1) % 3; // Cycle q between 0, 1, and 2
        if (SplitQ == 0)
        {
            SplitJ = (SplitJ + 1) % 256; // Advance the color wheel
        }
    }
}

unsigned long previousTimeS2 = 0; // Tracks the last update time
const int chaseIntervalS2 = 30;   // Time interval between each step
int SplitJ2 = random(0,256);                  // Position in the color wheel
int SplitQ2 = 0;                  // Offset for the chase effect

void Split2()
{
    unsigned long currentTime = millis();

    // Check if it's time to update the LEDs
    if (currentTime - previousTimeS2 >= chaseIntervalS2)
    {
        previousTimeS2 = currentTime;

        // Turn every third pixel on
        byte *c;
        for (int i = 0; i < NUM_LEDS2; i = i + 3)
        {
            c = Wheel((i + SplitJ2) % 255);
            leds2[i + SplitQ2].setRGB(*c, *(c + 1), *(c + 2));
            leds2[i + SplitQ2 + 1].setRGB(*c, *(c + 1), *(c + 2));
        }
        FastLED.show(); // Update the LEDs

        // Turn every third pixel off to create the chase effect
        for (int i = 0; i < NUM_LEDS2; i = i + 3)
        {
            leds2[i + SplitQ2].setRGB(0, 0, 0);
            leds2[i + SplitQ2 + 1].setRGB(*c, *(c + 1), *(c + 2));
        }

        // Update the chase offset and wheel position
        SplitQ2 = (SplitQ2 + 1) % 3; // Cycle q between 0, 1, and 2
        if (SplitQ2 == 0)
        {
            SplitJ2 = (SplitJ2 + 1) % 256; // Advance the color wheel
        }
    }
}

void ColorFlame()
{
    static byte heat[NUM_LEDS]; // Array of temperature readings at each simulation cell
    int cooldown;

    // 1) Slight cool down for each cell
    for (int i = 0; i < NUM_LEDS; i++)
    {
        cooldown = random(0, ((COOLING * 10) / NUM_LEDS) + 2);

        if (cooldown > heat[i])
        {
            heat[i] = 0;
        }
        else
        {
            heat[i] = heat[i] - cooldown;
        }
    }

    // 2) Heat from each cell drifts up and diffuses slightly
    for (int k = (NUM_LEDS - 1); k >= 2; k--)
    {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // 3) Randomly ignite new Sparks near bottom of the flame
    if (random(255) < SPARKING)
    {
        int y = random(7);
        heat[y] = heat[y] + random(160, 255);
    }

    // 4) Convert heat cells to LED colors
    for (int n = 0; n < NUM_LEDS; n++)
    {
        // Temperature ranges from 0 (black/cold) to 255 (white/hot)
        // Rescale heat from 0-255 to 0-191
        byte temperature = heat[n];
        byte t192 = round((temperature / 255.0) * 191);

        // Calculate ramp up from
        byte heatramp = t192 & 0x3F; // 0...63
        heatramp <<= 2;              // scale up to 0...252

        int BottomColor[8][3] = {
            // Array of Bottom flame colors (for spark)
            {heatramp / 2, heatramp / 2, 255}, // 0 - blue sparks on white flame
            {255, 255, heatramp},              // 1 - white/yellow sparks on red flame
            {255, heatramp, heatramp},         // 2 - white/red sparks on yellow flame
            {heatramp, heatramp, 255},         // 3 - white/blue sparks on green flame
            {heatramp, heatramp, 255},         // 4 - white/blue sparks on cyan flame
            {255, 255, heatramp},              // 5 - white/yellow sparks on blue flame
            {255, heatramp, heatramp},         // 6 - white/red sparks on purple flame
            {255, heatramp, heatramp},         // 7 - white/red sparks on pink flame
        };
        int MiddleColor[8][3] = {
            // Array of Middle flame colors
            {heatramp / 2, heatramp / 2, heatramp},     // 0 - white/blue
            {255, heatramp, 0},                         // 1 - red/yellow
            {heatramp, heatramp, 0},                    // 2 - yellow
            {0, 255, heatramp / 2},                     // 3 - green/blue
            {0, heatramp, heatramp},                    // 4 - cyan
            {0, heatramp, 255},                         // 5 - blue/green
            {heatramp / 3, 0, heatramp / 2},            // 6 - purple
            {heatramp, heatramp / 4, heatramp * 2 / 3}, // 7 - pink
        };
        int TopColor[8][3] = {
            // Array of Top flame colors
            {heatramp, heatramp, heatramp},             // 0 - white
            {heatramp, 0, 0},                           // 1 - red
            {heatramp, heatramp, 0},                    // 2 - yellow
            {0, heatramp, 0},                           // 3 - green
            {0, heatramp, heatramp},                    // 4 - cyan
            {0, 0, heatramp},                           // 5 - blue
            {heatramp / 3, 0, heatramp / 2},            // 6 - purple
            {heatramp, heatramp / 4, heatramp * 2 / 3}, // 7 - pink
        };

        int Pixel = n;

        // Set Pixels according to the three regions of the flame:
        if (t192 > 0x80)
        { // hottest (bottom of flame, heatramp between yellow and white)
            leds[Pixel].setRGB(round(BottomColor[5][0]), round(BottomColor[5][1]), round(BottomColor[5][2]));
        }
        else if (t192 > 0x40)
        { // middle (midde of flame, heatramp with analogous Color)
            leds[Pixel].setRGB(round(MiddleColor[5][0]), round(MiddleColor[5][1]), round(MiddleColor[5][2]));
        }
        else
        { // coolest (top of flame, heatramp heatramp with monochromatic Color)
            leds[Pixel].setRGB(round(TopColor[5][0]), round(TopColor[5][1]), round(TopColor[5][2]));
        }
    }

    FastLED.show();
}

void ColorFlame2()
{
    static byte heat[NUM_LEDS2]; // Array of temperature readings at each simulation cell
    int cooldown;

    // 1) Slight cool down for each cell
    for (int i = 0; i < NUM_LEDS2; i++)
    {
        cooldown = random(0, ((COOLING * 10) / NUM_LEDS2) + 2);

        if (cooldown > heat[i])
        {
            heat[i] = 0;
        }
        else
        {
            heat[i] = heat[i] - cooldown;
        }
    }

    // 2) Heat from each cell drifts up and diffuses slightly
    for (int k = (NUM_LEDS2 - 1); k >= 2; k--)
    {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // 3) Randomly ignite new Sparks near bottom of the flame
    if (random(255) < SPARKING)
    {
        int y = random(7);
        heat[y] = heat[y] + random(160, 255);
    }

    // 4) Convert heat cells to LED colors
    for (int n = 0; n < NUM_LEDS2; n++)
    {
        // Temperature ranges from 0 (black/cold) to 255 (white/hot)
        // Rescale heat from 0-255 to 0-191
        byte temperature = heat[n];
        byte t192 = round((temperature / 255.0) * 191);

        // Calculate ramp up from
        byte heatramp = t192 & 0x3F; // 0...63
        heatramp <<= 2;              // scale up to 0...252

        int BottomColor[8][3] = {
            // Array of Bottom flame colors (for spark)
            {heatramp / 2, heatramp / 2, 255}, // 0 - blue sparks on white flame
            {255, 255, heatramp},              // 1 - white/yellow sparks on red flame
            {255, heatramp, heatramp},         // 2 - white/red sparks on yellow flame
            {heatramp, heatramp, 255},         // 3 - white/blue sparks on green flame
            {heatramp, heatramp, 255},         // 4 - white/blue sparks on cyan flame
            {255, 255, heatramp},              // 5 - white/yellow sparks on blue flame
            {255, heatramp, heatramp},         // 6 - white/red sparks on purple flame
            {255, heatramp, heatramp},         // 7 - white/red sparks on pink flame
        };
        int MiddleColor[8][3] = {
            // Array of Middle flame colors
            {heatramp / 2, heatramp / 2, heatramp},     // 0 - white/blue
            {255, heatramp, 0},                         // 1 - red/yellow
            {heatramp, heatramp, 0},                    // 2 - yellow
            {0, 255, heatramp / 2},                     // 3 - green/blue
            {0, heatramp, heatramp},                    // 4 - cyan
            {0, heatramp, 255},                         // 5 - blue/green
            {heatramp / 3, 0, heatramp / 2},            // 6 - purple
            {heatramp, heatramp / 4, heatramp * 2 / 3}, // 7 - pink
        };
        int TopColor[8][3] = {
            // Array of Top flame colors
            {heatramp, heatramp, heatramp},             // 0 - white
            {heatramp, 0, 0},                           // 1 - red
            {heatramp, heatramp, 0},                    // 2 - yellow
            {0, heatramp, 0},                           // 3 - green
            {0, heatramp, heatramp},                    // 4 - cyan
            {0, 0, heatramp},                           // 5 - blue
            {heatramp / 3, 0, heatramp / 2},            // 6 - purple
            {heatramp, heatramp / 4, heatramp * 2 / 3}, // 7 - pink
        };

        int Pixel = n;

        // Set Pixels according to the three regions of the flame:
        if (t192 > 0x80)
        { // hottest (bottom of flame, heatramp between yellow and white)
            leds2[Pixel].setRGB(round(BottomColor[7][0]), round(BottomColor[7][1]), round(BottomColor[7][2]));
        }
        else if (t192 > 0x40)
        { // middle (midde of flame, heatramp with analogous Color)
            leds2[Pixel].setRGB(round(MiddleColor[7][0]), round(MiddleColor[7][1]), round(MiddleColor[7][2]));
        }
        else
        { // coolest (top of flame, heatramp heatramp with monochromatic Color)
            leds2[Pixel].setRGB(round(TopColor[7][0]), round(TopColor[7][1]), round(TopColor[7][2]));
        }
    }

    FastLED.show();
}

// Secondary Animations
void Fire2()
{
    if (!initialized2)
    {
        for (int i = 0; i < NUM_LEDS2; i++)
        {
            heat2[i] = random(100, 150); // Random initial heat values
        }
        initialized2 = true; // Set the flag to true after initialization
    }

    // Add entropy to random number generator; we use a lot of it.
    random16_add_entropy(random());

    // Array of temperature readings at each simulation cell
    // static uint8_t heat[NUM_LEDS];

    // Step 1.  Cool down every cell a little
    for (int i = 0; i < NUM_LEDS2; i++)
    {
        heat2[i] = qsub8(heat2[i], random8(0, ((COOLING * 10) / NUM_LEDS2) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = NUM_LEDS2 - 1; k >= 2; k--)
    {
        heat2[k] = (heat2[k - 1] + heat2[k - 2] + heat2[k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKING)
    {
        int y = random8(7);
        heat2[y] = qadd8(heat2[y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (int j = 0; j < NUM_LEDS2; j++)
    {
        // Scale the heat value from 0-255 down to 0-240
        // for best results with color palettes.
        uint8_t colorindex = scale8(heat2[j], 240);
        CRGB color = ColorFromPalette(gPal, colorindex);
        int pixelnumber;
        if (gReverseDirection)
        {
            pixelnumber = (NUM_LEDS - 1) - j;
        }
        else
        {
            pixelnumber = j;
        }
        leds2[pixelnumber] = color;
    }

    FastLED.show(); // display this frame
}

void Beat2()
{
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS2 - 1, 0, 0);
    uint8_t sinBeat2 = beatsin8(30, 0, NUM_LEDS2 - 1, 0, 85);
    uint8_t sinBeat3 = beatsin8(30, 0, NUM_LEDS2 - 1, 0, 170);

    if (uiState.gradientMenuSelection == 0)
    {
        leds2[sinBeat] = CRGB::Blue;
        leds2[sinBeat2] = CRGB::Red;
        leds2[sinBeat3] = CRGB::White;
    }

    if (uiState.gradientMenuSelection == 1)
    {
        leds2[sinBeat] = CRGB::HotPink;
        leds2[sinBeat2] = CRGB::MediumOrchid;
        leds2[sinBeat3] = CRGB::MediumPurple;
    }

    if (uiState.gradientMenuSelection == 2)
    {
        leds2[sinBeat] = CRGB::Lime;
        leds2[sinBeat2] = CRGB::SpringGreen;
        leds2[sinBeat3] = CRGB::DarkGreen;
    }

    if (uiState.gradientMenuSelection == 3)
    {
        leds2[sinBeat] = CRGB::Aqua;
        leds2[sinBeat2] = CRGB::DeepSkyBlue;
        leds2[sinBeat3] = CRGB::Blue;
    }

    fadeToBlackBy(leds2, NUM_LEDS2, 10);
    FastLED.show();
}

void Fade2()
{
    EVERY_N_MILLISECONDS(10)
    {
        // Switch on an LED at random, choosing a random color from the palette
        leds2[random8(0, NUM_LEDS2 - 1)] = ColorFromPalette(gPal, random8(), 255, LINEARBLEND);
    }

    // Fade all LEDs down by 1 in brightness each time this is called
    fadeToBlackBy(leds2, NUM_LEDS2, 1);

    FastLED.show();
}

void Blur2()
{
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS2 - 1, 0, 0);
    uint8_t sinBeat2 = beatsin8(30, 0, NUM_LEDS2 - 1, 0, 85);
    uint8_t sinBeat3 = beatsin8(30, 0, NUM_LEDS2 - 1, 0, 170);

    if (uiState.gradientMenuSelection == 0)
    {
        leds2[sinBeat] = CRGB::Blue;
        leds2[sinBeat2] = CRGB::Red;
        leds2[sinBeat3] = CRGB::White;
    }

    if (uiState.gradientMenuSelection == 1)
    {
        leds2[sinBeat] = CRGB::HotPink;
        leds2[sinBeat2] = CRGB::MediumOrchid;
        leds2[sinBeat3] = CRGB::MediumPurple;
    }

    if (uiState.gradientMenuSelection == 2)
    {
        leds2[sinBeat] = CRGB::Lime;
        leds2[sinBeat2] = CRGB::SpringGreen;
        leds2[sinBeat3] = CRGB::DarkGreen;
    }

    if (uiState.gradientMenuSelection == 3)
    {
        leds2[sinBeat] = CRGB::Aqua;
        leds2[sinBeat2] = CRGB::DeepSkyBlue;
        leds2[sinBeat3] = CRGB::Blue;
    }

    EVERY_N_MILLISECONDS(10)
    {
        for (int i = 0; i < 4; i++)
        {
            blur1d(leds2, NUM_LEDS2, 50);
        }
    }

    FastLED.show();
}

void ColorChange2()
{
    int i = 0;
    for (int j = 0; j < 6; j++)
    {
        if (i == 0)
            fill_solid(leds2, NUM_LEDS2, CRGB::Red);
        if (i == 1)
            fill_solid(leds2, NUM_LEDS2, CRGB::Yellow);
        if (i == 2)
            fill_solid(leds2, NUM_LEDS2, CRGB::Blue);
        if (i == 3)
            fill_solid(leds2, NUM_LEDS2, CRGB::HotPink);
        if (i == 4)
            fill_solid(leds2, NUM_LEDS2, CRGB::MediumOrchid);
        if (i == 5)
            fill_solid(leds2, NUM_LEDS2, CRGB::MediumPurple);

        i++;
        FastLED.show();
        delay(30);
        if (i == 6)
            i = 0;
        if (digitalRead(SWITCH) == HIGH) break;
    }
}

void MovingDot2()
{
    uint16_t sinBeat = beatsin16(30, 0, NUM_LEDS2 - 1, 0, 0); // NUM_LEDS - 1

    // leds[sinBeat] = CRGB::Blue;
    if (uiState.gradientMenuSelection == 0)
        leds2[sinBeat].setRGB(255, 128, 0);
    if (uiState.gradientMenuSelection == 1)
        leds2[sinBeat].setRGB(255, 0, 0);
    if (uiState.gradientMenuSelection == 2)
        leds2[sinBeat].setRGB(0, 255, 0);
    if (uiState.gradientMenuSelection == 3)
        leds2[sinBeat].setRGB(0, 0, 255);

    fadeToBlackBy(leds2, NUM_LEDS2, 10); // NUM_LEDS
    FastLED.show();
}

void Cylon2()
{
    static uint8_t hue = 0;
    // First slide the led in one direction
    for (int i = 0; i < NUM_LEDS2; i++)
    {
        if (digitalRead(SWITCH) == HIGH) break;

        // Set the i'th led to red
        leds2[i] = CHSV(hue++, 255, 255);

        // Show the leds
        FastLED.show();

        // now that we've shown the leds, reset the i'th led to black
        fadeall2();

        if (digitalRead(SWITCH) == HIGH) break;
    }
}

void Boom2()
{
    int red = 255;
    int green = 0;
    int blue = 0;

    EVERY_N_MILLISECONDS(100)
    {
        int cutoff = random8(20, 30);
        for (int i = 0; i < cutoff; i++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds2[i].setRGB(213, 240, 76);
            // leds2[i].setRGB(0, 0, 0);
        }
        for (int j = cutoff; j < 45; j++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds2[j].setRGB(255, 0, 0);
        }
        FastLED.show();
    }
    
}

void FirstLight2()
{
    EVERY_N_MILLISECONDS(200)
    {
        // Move a single white led
        for (int whiteLed = 0; whiteLed < NUM_LEDS2; whiteLed = whiteLed + 1)
        {
            // Turn our current led on to white, then show the leds
            leds2[whiteLed] = CRGB::White;

            // Show the leds (only one of which is set to white, from above)
            FastLED.show();

            // Turn our current led back to black for the next loop around
            leds2[whiteLed] = CRGB::Black;
        }
    }
}

void Sparkle2()
{
    int pixel = random(NUM_LEDS2);
    int i = abs(pixel - 5);
    for (int j = i; j <= pixel; j++)
    {
        leds2[j].setRGB(0, 255, 0);
    }
    FastLED.show();
    delay(10);
    for (int k = i; k <= pixel; k++)
    {
        leds2[k].setRGB(0, 0, 0);
    }
}

void RainbowCycle2()
{
    byte *c;
    uint16_t i, j;

    for (j = 0; j < 256; j++)
    {
        // if (digitalRead(SWITCH) == HIGH) break;
        for (i = 0; i < NUM_LEDS2; i++)
        {
            // if (digitalRead(SWITCH) == HIGH) break;
            c = Wheel(((i * 256 / NUM_LEDS2) + j) & 255);
            leds2[NUM_LEDS2 - 1 - i].setRGB(*c, *(c + 1), *(c + 2));
        }
        FastLED.show();
    }
}

void Strange2()
{
    EVERY_N_MILLISECONDS(20)
    {
        for (int i = 35; i < 45; i++)
        {
            int random = random8(0, 10);
            leds2[i] = CHSV(30, 255, 255);
            if (random % 5 == 1)
            {
                leds2[i].setRGB(0, 255, 0);
            }
            else
            {
                leds2[i].setRGB(0, 0, 0);
            }
        }

        for (int i = 25; i <= 30; i++)
        {
            leds2[i] = CHSV(30, 255, 255);
            leds2[i].setRGB(0, 255, 0);
        }

        for (int i = 10; i <= 20; i++)
        {
            int random = random8(0, 10);
            leds2[i] = CHSV(30, 255, 255);
            if (random % 5 == 1)
            {
                leds2[i].setRGB(0, 255, 0);
            }
            else
            {
                leds2[i].setRGB(0, 0, 0);
            }
        }

        for (int i = 0; i <= 5; i++)
        {
            leds2[i] = CHSV(30, 255, 255);
            leds2[i].setRGB(0, 255, 0);
        }

        FastLED.show();
    }
}

void RainbowLines2()
{
    for (int i = 35; i < 45; i++)
    {
        leds2[i] = CHSV(30, 255, 255);
        leds2[i].setRGB(255, 0, 0);
    }

    for (int i = 25; i <= 30; i++)
    {
        leds2[i] = CHSV(60, 255, 255);
        leds2[i].setRGB(255, 255, 0);
    }

    for (int i = 10; i <= 20; i++)
    {
        leds2[i] = CHSV(30, 255, 255);
        leds2[i].setRGB(0, 255, 0);
    }

    for (int i = 0; i <= 5; i++)
    {
        leds2[i] = CHSV(30, 255, 255);
        leds2[i].setRGB(0, 0, 255);
    }
    FastLED.show();
}

void JagWhite2()
{
    EVERY_N_MILLISECONDS(500)
    {
        for (int i = 35; i < 45; i++)
        {
            leds2[i] = CHSV(30, 255, 255);
            leds2[i].setRGB(255, 255, 255);
        }

        delay(100);
        FastLED.show(); // Send the updated color data to the LED strip
        fill_solid(leds2, NUM_LEDS2, CRGB::Black);

        for (int i = 25; i <= 30; i++)
        {
            leds2[i] = CHSV(30, 255, 255);
            leds2[i].setRGB(255, 255, 255);
        }
        delay(100);
        FastLED.show(); // Send the updated color data to the LED strip
        fill_solid(leds2, NUM_LEDS2, CRGB::Black);

        for (int i = 10; i <= 20; i++)
        {
            leds2[i] = CHSV(30, 255, 255);
            leds2[i].setRGB(255, 255, 255);
        }

        delay(100);
        FastLED.show(); // Send the updated color data to the LED strip
        fill_solid(leds2, NUM_LEDS2, CRGB::Black);

        for (int i = 0; i <= 5; i++)
        {
            leds2[i] = CHSV(30, 255, 255);
            leds2[i].setRGB(255, 255, 255);
        }
        FastLED.show();
    }
}

void Stars2()
{
    EVERY_N_MILLISECONDS(20)
    {
        for (int i = 15; i < 45; i++)
        {
            int random = random8(0, 10);
            leds2[i] = CHSV(30, 255, 255);
            if (random % 5 == 1)
            {
                leds2[i].setRGB(255, 255, 255);
            }
            else
            {
                leds2[i].setRGB(0, 0, 0);
            }
        }
        FastLED.show();
    }
}

void Pride2()
{
    for (int i = 38; i < 45; i++)
    {
        leds2[i] = CHSV(30, 255, 255);
        leds2[i].setRGB(255, 0, 0);
    }

    for (int i = 31; i <= 36; i++)
    {
        leds2[i] = CHSV(30, 255, 255);
        leds2[i].setRGB(255, 165, 0);
    }

    for (int i = 24; i <= 29; i++)
    {
        leds2[i] = CHSV(60, 255, 255);
        leds2[i].setRGB(255, 255, 0);
    }

    for (int i = 17; i <= 22; i++)
    {
        leds2[i] = CHSV(30, 255, 255);
        leds2[i].setRGB(0, 255, 0);
    }

    for (int i = 10; i <= 15; i++)
    {
        leds2[i] = CHSV(30, 255, 255);
        leds2[i].setRGB(0, 0, 255);
    }

    for (int i = 3; i <= 8; i++)
    {
        leds2[i] = CHSV(30, 255, 255);
        leds2[i].setRGB(160, 32, 240);
    }
    FastLED.show();
}

void ColorFrenzy2()
{
    int i = 0;
    for (int j = 0; j < 6; j++)
    {
        if (i == 0)
            fill_solid(leds2, NUM_LEDS2, CRGB::Red);
        if (i == 1)
            fill_solid(leds2, NUM_LEDS2, CRGB::Yellow);
        if (i == 2)
            fill_solid(leds2, NUM_LEDS2, CRGB::Blue);
        if (i == 3)
            fill_solid(leds2, NUM_LEDS2, CRGB::HotPink);
        if (i == 4)
            fill_solid(leds2, NUM_LEDS2, CRGB::MediumOrchid);
        if (i == 5)
            fill_solid(leds2, NUM_LEDS2, CRGB::MediumPurple);
        i++;
        FastLED.show();
        delay(30);
        if (i == 6)
            i = 0;
        if (digitalRead(SWITCH) == HIGH) break;
    }
}

void Histo2()
{
    EVERY_N_MILLISECONDS(200)
    {
        int cutoff = random8(10, 15);
        for (int i = 0; i < cutoff; i++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds2[i].setRGB(255, 255, 255);
        }
        for (int j = cutoff; j < NUM_LEDS2; j++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            int cutoff2 = random8(1, 10);
            if (cutoff2 % 3 == 0)
                leds2[j].setRGB(255, 0, 0);
            if (cutoff2 % 3 == 1)
                leds2[j].setRGB(0, 255, 0);
            if (cutoff2 % 3 == 2)
                leds2[j].setRGB(0, 0, 255);
        }
        FastLED.show();
    }
}

void RandHisto2()
{
    EVERY_N_MILLISECONDS(200)
    {
        int cutoffsize = random8(40, NUM_LEDS2 / 2);
        int cutoffcolor = random8(1, 10);
        for (int j = cutoffsize; j < NUM_LEDS2 / 2; j++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            if (cutoffcolor % 3 == 0)
                leds2[j].setRGB(255, 0, 0);
            if (cutoffcolor % 3 == 1)
                leds2[j].setRGB(0, 255, 0);
            if (cutoffcolor % 3 == 2)
                leds2[j].setRGB(0, 0, 255);
        }

        for (int k = NUM_LEDS2 / 2; k < NUM_LEDS2 / 2 + cutoffsize; k++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            if (cutoffcolor % 3 == 0)
                leds2[k].setRGB(255, 0, 0);
            if (cutoffcolor % 3 == 1)
                leds2[k].setRGB(0, 255, 0);
            if (cutoffcolor % 3 == 2)
                leds2[k].setRGB(0, 0, 255);
        }
        FastLED.show();
    }
}

void Bang2()
{
    for (int i = 35; i < 45; i++)
    {
        leds2[i] = CHSV(30, 255, 255);
        leds2[i].setRGB(255, 0, 0);
    }
    static uint8_t hue = 0;
    // First slide the led in one direction
    for (int i = 10; i < 30; i++)
    {
        // Set the i'th led to red
        leds2[i] = CHSV(hue++, 255, 255);
        // Show the leds
        FastLED.show();
        if (digitalRead(SWITCH) == HIGH) break;
        // now that we've shown the leds, reset the i'th led to black
        fadeall2();
    }

    for (int i = 0; i <= 5; i++)
    {
        leds2[i] = CHSV(30, 255, 255);
        leds2[i].setRGB(255, 0, 0);
    }
}

// Array of function pointers
const AnimationFunction animationFunctions[] = {
    Fire,
    Beat,
    Fade,
    Blur,
    ColorChange,
    MovingDot,
    Cylon,
    Boom,
    FirstLight,
    Sparkle,
    RainbowCycle,
    Strange,
    RainbowLines,
    JagWhite,
    Stars,
    Pride,
    ColorFrenzy,
    Histo,
    RandHisto,
    Bang,
    RunningLights,
    Meteor,
    Split,
    ColorFlame
};

// Array of function pointers
const AnimationFunction animationFunctions2[] = {
    Fire2,
    Beat2,
    Fade2,
    Blur2,
    ColorChange2,
    MovingDot2,
    Cylon2,
    Boom2,
    FirstLight2,
    Sparkle2,
    RainbowCycle2,
    Strange2,
    RainbowLines2,
    JagWhite2,
    Stars2,
    Pride2,
    ColorFrenzy2,
    Histo2,
    RandHisto2,
    Bang2,
    RunningLights2,
    Meteor2,
    Split2,
    ColorFlame2
};
