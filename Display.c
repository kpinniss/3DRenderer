#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Display.h"

#pragma region Fields

SDL_Window* _window = NULL;
SDL_Renderer* _renderer = NULL;

SDL_Texture* _colorBufferTexture = NULL;
uint32_t* _colorBuffer = NULL;

int _windowWidth = 800;
int _windowHeight = 600;
uint32_t _gridBackGroundColor = GRAY;
uint32_t _gridColor = DARKGRAY;
uint32_t _drawColor = YELLOW;
uint32_t _drawColor2 = NAVYBLUE;

#pragma endregion

#pragma region Modules

bool initWindow(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}
	//check size of screen for full screen
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	//update window WxH
	_windowWidth = displayMode.w;
	_windowHeight = displayMode.h;

	//create window
	_window = SDL_CreateWindow(
		"3D Renderer",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		_windowWidth,
		_windowHeight,
		SDL_WINDOW_BORDERLESS
	);

	if (!_window) {
		fprintf(stderr, "Error creating SDL Window.\n");
		return false;
	}

	//create renderer
	_renderer = SDL_CreateRenderer(_window, -1, 0);

	if (!_renderer) {
		fprintf(stderr, "Error creating SDL Renderer.\n");
		return false;
	}

	SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void renderColorBuffer() {
	//copies color buffer to texture
	SDL_UpdateTexture(
		_colorBufferTexture,
		NULL,
		_colorBuffer,
		(int)_windowWidth * sizeof(uint32_t)
	);
	SDL_RenderCopy(_renderer, _colorBufferTexture, NULL, NULL);
}

void clearBuffer(uint32_t color) {
	for (int y = 0; y < _windowHeight; y++) {
		for (int x = 0; x < _windowWidth; x++) {
			uint32_t bufferPos = ((_windowWidth * y) + x);
			_colorBuffer[bufferPos] = color;
		}
	}
}

void destroyWindow(void) {
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void drawGrid() {

	//draw grid lines
	//for (int y = 0; y < _windowHeight; y++) {
	//	for (int x = 0; x < _windowWidth; x++) {
	//		//if divisible by 10 draw
	//		if (x % 10 == 0 || y % 10 == 0) {
	//			_colorBuffer[((_windowWidth * y) + x)] = _gridColor;
	//		}
	//	}
	//}

	//draw grid pos dots
	for (int y = 0; y < _windowHeight; y += 10) {
		for (int x = 0; x < _windowWidth; x += 10) {
			_colorBuffer[((_windowWidth * y) + x)] = _gridColor;
		}
	}
}

void drawPixel(int x, int y, uint32_t color) {
	if (x >= 0 && x < _windowWidth && y >= 0 && y < _windowHeight) {
		_colorBuffer[(_windowWidth * y) + x] = color;
	}
}

void drawRect(int x, int y, int width, int height, uint32_t color) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int current_x = x + i;
			int current_y = y + j;
			drawPixel(current_x, current_y, color);
		}
	}
}

void drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
	int deltaX = (x2 - x1);
	int deltaY = (y2 - y1);
	int abs_deltaX = abs(deltaX);
	int abs_deltaY = abs(deltaY);

	int sideLength = abs_deltaX >= abs_deltaY ? abs_deltaX : abs_deltaY;

	//find increment values by dividing delta / sideLength
	float incX = deltaX / (float)sideLength;
	float incY = deltaY / (float)sideLength;

	float currentX = x1;
	float currentY = y1;

	for (int i = 0; i < sideLength; i++) {
		drawPixel(round(currentX), round(currentY), color);
		currentX += incX;
		currentY += incY;
	}
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x3, y3, color);
	drawLine(x3, y3, x1, y1, color);
}

void intSwap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void sortVerices(int x0, int y0, int x1, int y1, int x2, int y2) {
	if (y0 > y1) {
		intSwap(&y0, &y1);
		intSwap(&x0, &x1);
	}
	if (y1 > y2) {
		intSwap(&y1, &y2);
		intSwap(&x1, &x2);
	}
	if (y0 > y1) {
		intSwap(&y0, &y1);
		intSwap(&x0, &x1);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat top
///////////////////////////////////////////////////////////////////////////////
//
//  (x0,y0)------(x1,y1)
//      \         /
//       \       /
//        \     /
//         \   /
//          \ /
//        (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void fillBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	float invSlope1 = (float)(x2 - x0) / (y2 - y0);
	float invSlope2 = (float)(x2 - x1) / (y2 - y1);

	// Start x_start and x_end from the bottom vertex (x2,y2)
	float xStart = x2;
	float xEnd = x2;

	// Loop all the scanlines from bottom to top
	for (int y = y2; y >= y0; y--) {
		drawLine(xStart, y, xEnd, y, color);
		xStart -= invSlope1;
		xEnd -= invSlope2;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat bottom
///////////////////////////////////////////////////////////////////////////////
//
//        (x0,y0)
//          / \
//         /   \
//        /     \
//       /       \
//      /         \
//  (x1,y1)------(x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void fillTopTraingle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	//find changing inverse of the slope
	float invSlope1 = (float)(x1 - x0) / (y1 - y0);
	float invSlope2 = (float)(x2 - x0) / (y2 - y0);
	
	//start xStart adn xEnd from the top of vertex
	float xStart = x0;
	float xEnd = x0;

	//iterate over scan lines
	for (int y = y0; y <= y2; y++) {
		drawLine(xStart, y, xEnd, y, color);
		xStart += invSlope1;
		xEnd += invSlope2;
	}
}


///////////////////////////////////////////////////////////////////////////////
// Draw a filled triangle with the flat-top/flat-bottom method
// We split the original triangle in two, half flat-bottom and half flat-top
///////////////////////////////////////////////////////////////////////////////
//
//          (x0,y0)
//            / \
//           /   \
//          /     \
//         /       \
//        /         \
//   (x1,y1)------(Mx,My)
//       \_           \
//          \_         \
//             \_       \
//                \_     \
//                   \    \
//                     \_  \
//                        \_\
//                           \
//                         (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	//sort vertices by y-coordinate ascending (y0 < y1 < y2)
	if (y0 > y1) {
		intSwap(&y0, &y1);
		intSwap(&x0, &x1);
	}
	if (y1 > y2) {
		intSwap(&y1, &y2);
		intSwap(&x1, &x2);
	}
	if (y0 > y1) {
		intSwap(&y0, &y1);
		intSwap(&x0, &x1);
	}

	if (y1 == y2) {
		fillTopTraingle(x0, y0, x1, y1, x2, y2, color);
	}
	if (y0 == y1) {
		fillBottomTriangle(x0, y0, x1, y1, x2, y2, color);
	}
	else {
		//calculate new vertex using triangle similarity
		int My = y1;
		int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0) + x0);

		//draw flat bottom & flat top
		fillTopTraingle(x0, y0, x1, y1, Mx, My, color);
		fillBottomTriangle(x1, y1, Mx, My, x2, y2, color);
	}
}
#pragma endregion


