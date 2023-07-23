#ifndef UTILS_H
#define UTILS_H

extern bool startsWith(const char* a, const char* b);


typedef uint32_t color_t;

enum HexColor {
	WHITE = 0xC0FFEE,
	YELLOW = 0xFFFE33,
	RED = 0xCC2400,
	BLUE = 0x0843e2,
	NAVYBLUE = 0x2a52be,
	BLACK = 0x000000,
	DARKGRAY = 0x999999,
	GRAY = 0x444444,
	PURPLE = 0xa284eb
} _hexColor;
#endif