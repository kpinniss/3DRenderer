#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Display.h"
#include "Vector.h"

#pragma region Fields
const int _nVecs = 729;
vec3_t _shapeVecs[729]; //9 x 9 x 9 cube
vec2_t _projectedVecs[729];
bool _isRunning = false;
float _fovFactor = 640; //scale up vectors
vec3_t _cameraPos = { .x = 0, .y = 0, .z = -5 };
vec3_t _rotation = { 0,0,0 };
int _prevFrameTime = 0;
#pragma endregion

#pragma region Modules

void setup(void) {
	_colorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * _windowWidth * _windowHeight);
	_colorBufferTexture = SDL_CreateTexture(
		_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		_windowWidth,_windowHeight
	);

	if (!_colorBuffer) {
		fprintf(stderr, "Error initializing buffer.\n");
		return false;
	}

	//start loading my array of vectors
	//from -1 to 1 (in this 9x9x9 cube)
	int count = 0;

	for (float x = -1; x <= 1; x += 0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				vec3_t newPoint = {.x = x, .y = y, .z = z};
				_shapeVecs[count++] = newPoint;
			}
		}
	}
}

void processInput(void) {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			_isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				_isRunning = false;
			}	
			break;
	}
}

vec2_t project(vec3_t vec) {
	//project and scale vecors (scale makes bigger smaller) fovFactor (Field of View Factor)
	vec2_t projectedPoint = {
		//devide by z for prospective devision adds depth.
		.x = (_fovFactor * vec.x) / vec.z, 
		.y = (_fovFactor * vec.y) / vec.z
	};
	return projectedPoint;
}

void update(void) {

	//delay a few miliseconds to keep consistant frame rate.
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), (_prevFrameTime + FRAME_TARGET_TIME))); //manual delay

	int delayTime = FRAME_TARGET_TIME - (SDL_GetTicks() - _prevFrameTime);
	
	if (delayTime > 0 && delayTime <= FRAME_TARGET_TIME) {
		SDL_Delay(delayTime);
	}

	_prevFrameTime = SDL_GetTicks();

	_rotation.x += 0.01;
	_rotation.y += 0.01;
	_rotation.z += 0.01;

	for (int i = 0; i < _nVecs; i++) {
		vec3_t vec = _shapeVecs[i];

		vec3_t transformedVec = rotateX(vec, _rotation.x);
		transformedVec = rotateY(transformedVec, _rotation.y);
		transformedVec = rotateZ(transformedVec, _rotation.z);

		//translates vector away from camera 
		transformedVec.z -= _cameraPos.z;

		//Project the current point
		vec2_t projectedVec = project(transformedVec);

		//cache point in array;
		_projectedVecs[i] = projectedVec;
	}
}

void render(void) {

	drawGrid();

	for (int i = 0; i < _nVecs; i++) {
		vec2_t vec = _projectedVecs[i];
		//move point towards middle of screen with window wxh
		drawRect(
			vec.x + (_windowWidth / 2),
			vec.y + (_windowHeight / 2),
			4, 4, _drawColor);
	}

	renderColorBuffer();
	clearBuffer(_gridBackGroundColor);

	SDL_RenderPresent(_renderer);
}

#pragma endregion

#pragma region MainModule

int main(int argc, char* args[]) {
	printf("App Started..");
	_isRunning = initWindow();
	setup();

	while (_isRunning) {
		processInput();
		update();
		render();
	}

	printf("Exiting App..");
	return 0;
}

#pragma endregion


