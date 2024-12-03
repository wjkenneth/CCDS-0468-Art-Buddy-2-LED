#ifndef BITMAP_H
#define BITMAP_H

#include <Arduino.h>

// Define maximum number of files
#define MAX_FILES 100

// Define file info structure
struct FileInfo
{
    String name;
    size_t size;
    bool isDirectory;
};

// Function prototypes
void listDir();
void sdListDir();
void deleteFile();
void deleteUSB();
void displayBMP();
void displayUSB();

// Bitmap Menu
static const char bitmap_1[] PROGMEM = "Select from SD Card";
static const char bitmap_2[] PROGMEM = "Select from Storage";

static const char *const bitmap_menu[] PROGMEM = {
    bitmap_1, bitmap_2};

// Run Animation Menu
static const char run_bitmap_ani_1[] PROGMEM = "Back";
static const char run_bitmap_ani_2[] PROGMEM = "Run";
static const char run_bitmap_ani_3[] PROGMEM = "Delete";

static const char *const run_bitmap_ani_menu[] PROGMEM = {
    run_bitmap_ani_1, run_bitmap_ani_2, run_bitmap_ani_3};

// Run Animation Menu
static const char run_usb_ani_1[] PROGMEM = "Back";
static const char run_usb_ani_2[] PROGMEM = "Run";

static const char *const run_usb_ani_menu[] PROGMEM = {
    run_usb_ani_1, run_usb_ani_2, };

// Declare global variables
extern int fileCount;
extern FileInfo files[MAX_FILES];

extern int sdFileCount;
extern FileInfo sdFiles[MAX_FILES];

#endif