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
uint32_t _gridBackGroundColor = 0x000000;
uint32_t _gridColor = 0x808080;
uint32_t _drawColor = 0xffd700;

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
	free(_colorBuffer);
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

#pragma endregion


