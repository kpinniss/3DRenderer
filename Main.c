#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Display.h"
#include "Vector.h"
#include "Mesh.h"

#pragma region Fields

float _fovFactor = 640;
triangle_t trianglesToRender[N_MESH_FACES];
vec3_t _cameraPos = { .x = 0, .y = 0, .z = -5 };
vec3_t _rotation = { 0,0,0 };

bool _isRunning = false;
int _prevFrameTime = 0;

#pragma endregion

#pragma region Modules

void setup(void) {
	_colorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * _windowWidth * _windowHeight);
	_colorBufferTexture = SDL_CreateTexture(
		_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		_windowWidth, _windowHeight
	);

	if (!_colorBuffer) {
		fprintf(stderr, "Error initializing buffer.\n");
		return false;
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

	//iterate over triangle faces
	for (int i = 0; i < N_MESH_FACES; i++) {
		face_t currentMeshFace = meshFaces[i];
		vec3_t faceVertices[T_SIZE];
		faceVertices[0] = meshVertices[(currentMeshFace.a - 1)];
		faceVertices[1] = meshVertices[(currentMeshFace.b - 1)];
		faceVertices[2] = meshVertices[(currentMeshFace.c - 1)];

		triangle_t projectedTriangle;

		// apply transform to vertices
		for (int j = 0; j < T_SIZE; j++) {
			vec3_t transformedVertex = faceVertices[j];

			//transform
			transformedVertex = rotateX(transformedVertex, _rotation.x);
			transformedVertex = rotateY(transformedVertex, _rotation.y);
			transformedVertex = rotateZ(transformedVertex, _rotation.z);

			//translate the vertex away from camera
			transformedVertex.z -= _cameraPos.z;

			//project current vertex
			vec2_t projectedVertex = project(transformedVertex);

			//scale and translate to center screen
			projectedVertex.x += (_windowWidth / 2);
			projectedVertex.y += (_windowHeight / 2);

			projectedTriangle.points[j] = projectedVertex;
		}
		//cache projected triangles
		trianglesToRender[i] = projectedTriangle;
	}
}

void render(void) {

	drawGrid();

	//render projected triangles
	for (int i = 0; i < N_MESH_FACES; i++) {
		triangle_t triangle = trianglesToRender[i];

		//draw vertex points
		drawRect(triangle.points[0].x, triangle.points[0].y, 4, 4, _drawColor);
		drawRect(triangle.points[1].x, triangle.points[1].y, 4, 4, _drawColor);
		drawRect(triangle.points[2].x, triangle.points[2].y, 4, 4, _drawColor);

		//draw triangles
		drawTriangle(
			trianglesToRender[i].points[0].x, trianglesToRender[i].points[0].y, 
			trianglesToRender[i].points[1].x, trianglesToRender[i].points[1].y,
			trianglesToRender[i].points[2].x, trianglesToRender[i].points[2].y
			);
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


