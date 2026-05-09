#define RAYGUI_IMPLEMENTATION
#include "raylib_raygui.h"
#include "terminal/style_terminal.h"

int GuiScrollBarW(Rectangle bounds, int value, int minValue, int maxValue) {
	return GuiScrollBar(bounds, value, minValue, maxValue); 
}

void GuiLoadStyleTerminalW() { GuiLoadStyleTerminal(); }