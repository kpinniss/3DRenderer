#pragma once
#ifndef  DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Utils.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

enum CullMethod {
	CULL_NONE,
	CULL_BACKFACE
} _cullMethod;

enum RenderMethod {
	RENDER_WIRE,
	RENDER_WIRE_VERTEX,
	RENDER_FILL_TRIANGLE,
	RENDER_FILL_TRIANGLE_WIRE
} _renderMethod;

extern SDL_Window* _window;
extern SDL_Renderer* _renderer;
extern SDL_Texture* _colorBufferTexture;
extern uint32_t* _colorBuffer;
extern int _windowWidth;
extern int _windowHeight;
uint32_t _gridBackGroundColor;
uint32_t _gridColor;
uint32_t _drawColor;
uint32_t _drawColor2;

bool initWindow(void);
void drawGrid();
void drawPixel(int x, int y, color_t color);
void renderColorBuffer();
void clearBuffer(uint32_t color);
void destroyWindow(void);
void drawRect(int posX, int posY, int w, int h, color_t color);
void drawLine(int x1, int y1, int x2, int y2, color_t color);
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, color_t color);
void fillBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);
void fillTopTraingle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);
void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);
#endif