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
#endif