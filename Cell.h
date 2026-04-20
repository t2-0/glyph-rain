#pragma once
#include <vector>
#include <string>
#include "raylib_raygui.h"

using std::vector;
using std::string;
using std::pair;

struct Cell {
	Cell(char c, Vector2 pos, Color color) : pos{ pos }, color{ color } {
		this->c[0] = c;
		this->c[1] = '\0';
	}

	Vector2 pos;
	Color color;

	char c[2];
};