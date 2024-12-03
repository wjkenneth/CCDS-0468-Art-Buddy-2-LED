#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Arduino.h>

#define NUM_LEDS 100
#define NUM_LEDS2 44
#define OFF 0

// Function prototypes
void Fire();
void Beat();
void Fade();
void Blur();
void ColorChange();
void MovingDot();
void Cylon();
void Boom();
void FirstLight();
void Sparkle();
void RainbowCycle();
void Strange();
void RainbowLines();
void JagWhite();
void Stars();
void Pride();
void ColorFrenzy();
void Histo();
void RandHisto();
void Bang();
void RunningLights();
void Meteor();
void Split();
void ColorFlame();

void Fire2();
void Beat2();
void Fade2();
void Blur2();
void ColorChange2();
void MovingDot2();
void Cylon2();
void Boom2();
void FirstLight2();
void Sparkle2();
void RainbowCycle2();
void Strange2();
void RainbowLines2();
void JagWhite2();
void Stars2();
void Pride2();
void ColorFrenzy2();
void Histo2();
void RandHisto2();
void Bang2();
void RunningLights2();
void Meteor2();
void Split2();
void ColorFlame2();

// Type definition for animation function pointer
typedef void (*AnimationFunction)();

// Array of function pointers
extern const AnimationFunction animationFunctions[];
extern const AnimationFunction animationFunctions2[];

// Animations
static const char ani_1[] PROGMEM = "Fire";
static const char ani_2[] PROGMEM = "Beat";
static const char ani_3[] PROGMEM = "Fade";
static const char ani_4[] PROGMEM = "Blur";
static const char ani_5[] PROGMEM = "Color Change";
static const char ani_6[] PROGMEM = "Moving Dot";
static const char ani_7[] PROGMEM = "Cylon";
static const char ani_8[] PROGMEM = "Boom";
static const char ani_9[] PROGMEM = "FirstLight";
static const char ani_10[] PROGMEM = "Sparkle";
static const char ani_11[] PROGMEM = "RainbowCycle";
static const char ani_12[] PROGMEM = "Strange";
static const char ani_13[] PROGMEM = "RaingbowLines";
static const char ani_14[] PROGMEM = "JagWhite";
static const char ani_15[] PROGMEM = "Stars";
static const char ani_16[] PROGMEM = "Pride";
static const char ani_17[] PROGMEM = "ColorFrenzy";
static const char ani_18[] PROGMEM = "Histo";
static const char ani_19[] PROGMEM = "RandHisto";
static const char ani_20[] PROGMEM = "Bang";
static const char ani_21[] PROGMEM = "RunningLights";
static const char ani_22[] PROGMEM = "Meteor";
static const char ani_23[] PROGMEM = "Split";
static const char ani_24[] PROGMEM = "ColorFlame";

static const char *const all_animations[] PROGMEM = {
    ani_1, ani_2, ani_3, ani_4, ani_5, ani_6, ani_7, ani_8, ani_9, ani_10,
    ani_11, ani_12, ani_13, ani_14, ani_15, ani_16, ani_17, ani_18, ani_19, ani_20, ani_21, ani_22, ani_23, ani_24};

// Secondary Animations
static const char ani2_1[] PROGMEM = "Fire";
static const char ani2_2[] PROGMEM = "Beat";
static const char ani2_3[] PROGMEM = "Fade";
static const char ani2_4[] PROGMEM = "Blur";
static const char ani2_5[] PROGMEM = "Color Change";
static const char ani2_6[] PROGMEM = "Moving Dot";
static const char ani2_7[] PROGMEM = "Cylon";
static const char ani2_8[] PROGMEM = "Boom";
static const char ani2_9[] PROGMEM = "FirstLight";
static const char ani2_10[] PROGMEM = "Sparkle";
static const char ani2_11[] PROGMEM = "RainbowCycle";
static const char ani2_12[] PROGMEM = "Strange";
static const char ani2_13[] PROGMEM = "RaingbowLines";
static const char ani2_14[] PROGMEM = "JagWhite";
static const char ani2_15[] PROGMEM = "Stars";
static const char ani2_16[] PROGMEM = "Pride";
static const char ani2_17[] PROGMEM = "ColorFrenzy";
static const char ani2_18[] PROGMEM = "Histo";
static const char ani2_19[] PROGMEM = "RandHisto";
static const char ani2_20[] PROGMEM = "Bang";
static const char ani2_21[] PROGMEM = "RunningLights";
static const char ani2_22[] PROGMEM = "Meteor";
static const char ani2_23[] PROGMEM = "Split";
static const char ani2_24[] PROGMEM = "ColorFlame";


static const char *const all_animations_2[] PROGMEM = {
    ani2_1, ani2_2, ani2_3, ani2_4, ani2_5, ani2_6, ani2_7, ani2_8, ani2_9, ani2_10,
    ani2_11, ani2_12, ani2_13, ani2_14, ani2_15, ani2_16, ani2_17, ani2_18, ani2_19, ani2_20, ani2_21, ani2_22, ani2_23, ani2_24};

// Gradient Menu
static const char grad_1[] PROGMEM = "Gradient 1";
static const char grad_2[] PROGMEM = "Gradient 2";
static const char grad_3[] PROGMEM = "Gradient 3";
static const char grad_4[] PROGMEM = "Gradient 4";

static const char *const all_gradients[] PROGMEM = {
    grad_1, grad_2, grad_3, grad_4};

// Run Animation Menu
static const char run_ani_1[] PROGMEM = "Back";
static const char run_ani_2[] PROGMEM = "Run";
static const char run_ani_3[] PROGMEM = "Config Ani";

static const char *const run_ani_menu[] PROGMEM = {
    run_ani_1, run_ani_2, run_ani_3};

// Calibration Menu
static const char cali_1[] PROGMEM = "Clockwise";
static const char cali_2[] PROGMEM = "Anti-Clockwise";
static const char cali_3[] PROGMEM = "Mini Clockwise";
static const char cali_4[] PROGMEM = "Back";

static const char *const cali_menu[] PROGMEM = {
    cali_1, cali_2, cali_3, cali_4};

#endif
