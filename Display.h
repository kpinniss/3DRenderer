#pragma once
#ifndef  DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window* _window;
extern SDL_Renderer* _renderer;
extern SDL_Texture* _colorBufferTexture;
extern uint32_t* _colorBuffer;
extern int _windowWidth;
extern int _windowHeight;
uint32_t _gridBackGroundColor;
uint32_t _gridColor;
uint32_t _drawColor;

bool initWindow(void);
void drawGrid();
void drawPixel(int x, int y, uint32_t color);
void renderColorBuffer();
void clearBuffer(uint32_t color);
void destroyWindow(void);
void drawRect(int posX, int posY, int w, int h, uint32_t color);
void drawLine(int x1, int y1, int x2, int y2);
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
void fillBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2);
void fillTopTraingle(int x0, int y0, int x1, int y1, int x2, int y2);
void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2);
#endif