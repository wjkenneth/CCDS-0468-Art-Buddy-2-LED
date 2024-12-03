#ifndef CONTROLS_H
#define CONTROLS_H

#include "main.h"

// Function declarations
void handleRight(int &currentSelection, int maxSelection, void (*updateMenu)(int curIndex, int nextIndex));
void handleLeft(int &currentSelection, void (*updateMenu)(int curIndex, int nextIndex));
void handleUp(int &currentSelection, void (*updateMenu)(int curIndex, int nextIndex), int increment = 1);
void handleDown(int &currentSelection, int maxSelection, void (*updateMenu)(int curIndex, int nextIndex), int increment = 1);
void handleNextPage(int &currentPage, int &currentSelection, int maxPage, void (*updateMenu)());
void handlePreviousPage(int &currentPage, int &currentSelection, void (*updateMenu)());
void handleChangeMenu(int key, MenuState &currentState, MenuState newState, void (*updateMenu)());
void handleSettingsUp(int &currentSelection, int maxSelection, void (*updateMenu)(int curIndex, int nextIndex), int increment = 1);
void handleSettingsDown(int &currentSelection, void (*updateMenu)(int curIndex, int nextIndex), int increment = 1);

#endif // CONTROLS_H