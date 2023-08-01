#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Display.h"
#include "Vector.h"
#include "Mesh.h"
#include "Array.h"

#pragma region Fields

int _zOffset = 5;
float _fovFactor = 700;
triangle_t* trianglesToRender = NULL;
vec3_t _cameraPos = { 0, 0, 0 };
int vertPointSize = 2;

bool _isRunning = false;
int _prevFrameTime = 0;

#pragma endregion

#pragma region Modules

void setup(void) {

	//set defualt render and cull methods
	_renderMethod = RENDER_WIRE;
	_cullMethod = CULL_BACKFACE;

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

	//loads mesh data for default cube shape
	char* fileName = "./assets/f22.obj";
	fileName = NULL;
	loadMesh(fileName);
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
		if (event.key.keysym.sym == SDLK_1) {
			_renderMethod = RENDER_WIRE_VERTEX;
		}
		if (event.key.keysym.sym == SDLK_2) {
			_renderMethod = RENDER_WIRE;
		}
		if (event.key.keysym.sym == SDLK_3) {
			_renderMethod = RENDER_FILL_TRIANGLE;
		}
		if (event.key.keysym.sym == SDLK_4) {
			_renderMethod = RENDER_FILL_TRIANGLE_WIRE;
		}
		if (event.key.keysym.sym == SDLK_c) {
			_cullMethod = CULL_BACKFACE;
		}
		if (event.key.keysym.sym == SDLK_d) {
			_cullMethod = CULL_NONE;
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

	//re-init array of triangles
	trianglesToRender = NULL;

	_mesh.rotation.x += 0.01;
	_mesh.rotation.y += 0.01;
	_mesh.rotation.z += 0.01;

	//iterate over triangle faces
	int numFaces = array_length(_mesh.faces);
	for (int i = 0; i < numFaces; i++) {
		face_t currentMeshFace = _mesh.faces[i];

		vec3_t faceVertices[T_SIZE];
		faceVertices[0] = _mesh.vertices[(currentMeshFace.a - 1)];
		faceVertices[1] = _mesh.vertices[(currentMeshFace.b - 1)];
		faceVertices[2] = _mesh.vertices[(currentMeshFace.c - 1)];

		vec3_t transformedVertices[T_SIZE];

		// apply transform to vertices
		for (int j = 0; j < T_SIZE; j++) {
			vec3_t transformedVertex = faceVertices[j];

			//transform
			transformedVertex = rotateX(transformedVertex, _mesh.rotation.x);
			transformedVertex = rotateY(transformedVertex, _mesh.rotation.y);
			transformedVertex = rotateZ(transformedVertex, _mesh.rotation.z);

			//translate the vertex away from camera
			transformedVertex.z += _zOffset;

			//cache transformed vertex
			transformedVertices[j] = transformedVertex;
		}

		//Check backface-culling//
		if (_cullMethod == CULL_BACKFACE) {
			
			vec3_t vecA = transformedVertices[0];	 /*	 A   */
			vec3_t vecB = transformedVertices[1];	/*  / \  */
			vec3_t vecC = transformedVertices[2];  /*  C---B */

			//Get Vector Subtraction of b-a and c-a
			vec3_t vecAB = vec3Sub(vecB, vecA);
			vec3_t vecAC = vec3Sub(vecC, vecA);
			vec3Normalize(&vecAB);
			vec3Normalize(&vecAC);

			//Find face-normal between vectors (use cross product to find perpindicular)
			vec3_t normal = vec3Cross(vecAB, vecAC);
			vec3Normalize(&normal);

			//Find vector between point on the triangle and the camera origin  (camera --RAY--> vector)
			vec3_t cameraRay = vec3Sub(_cameraPos, vecA);

			//Check alignment of normal and cameraRay
			float product = vec3Dot(normal, cameraRay);

			//skip faces that are facing away from camera
			if (product < 0) {
				continue;
			}
		}

		vec2_t projectedPoints[3];

		//project vertices
		for (int k = 0; k < T_SIZE; k++) {

			//project current vertex
			projectedPoints[k] = project(transformedVertices[k]);

			//scale and translate to center screen
			projectedPoints[k].x += (_windowWidth / 2);
			projectedPoints[k].y += (_windowHeight / 2);
		}

		// Calculate the average depth for each face based on the vertices after transformation
		float avgDepth = (transformedVertices[0].z + transformedVertices[1].z + transformedVertices[2].z) / 3.0;

		//init triangle with projected points and color
		triangle_t projectedTriangle = { 
			.points = { 
				{projectedPoints[0].x, projectedPoints[0].y},
				{projectedPoints[1].x, projectedPoints[1].y},
				{projectedPoints[2].x, projectedPoints[2].y},
			},
			.color = currentMeshFace.color,
			.avg_depth = avgDepth
		};

		//cache projected triangles
		array_push(trianglesToRender, projectedTriangle);
	}

	// Sort the triangles to render by their avg_depth
	int num_triangles = array_length(trianglesToRender);
	for (int i = 0; i < num_triangles; i++) {
		for (int j = i; j < num_triangles; j++) {
			if (trianglesToRender[i].avg_depth < trianglesToRender[j].avg_depth) {
				// Swap the triangles positions in the array
				triangle_t temp = trianglesToRender[i];
				trianglesToRender[i] = trianglesToRender[j];
				trianglesToRender[j] = temp;
			}
		}
	}
}

void render(void) {

	drawGrid();

	int numTriangles = array_length(trianglesToRender);

	//render projected triangles
	for (int i = 0; i < numTriangles; i++) {
		triangle_t triangle = trianglesToRender[i];

		//draw vertex points
		/*drawRect(triangle.points[0].x, triangle.points[0].y, vertPointSize, vertPointSize, _drawColor);
		drawRect(triangle.points[1].x, triangle.points[1].y, vertPointSize, vertPointSize, _drawColor);
		drawRect(triangle.points[2].x, triangle.points[2].y, vertPointSize, vertPointSize, _drawColor);*/

		//draw triangles
		if (_renderMethod == RENDER_FILL_TRIANGLE || _renderMethod == RENDER_FILL_TRIANGLE_WIRE) {
			drawFilledTriangle(
				triangle.points[0].x, triangle.points[0].y, // vertex A
				triangle.points[1].x, triangle.points[1].y,	// vertex B
				triangle.points[2].x, triangle.points[2].y,	// vertex C
				triangle.color
			);
		}
		if (_renderMethod == RENDER_WIRE || _renderMethod == RENDER_WIRE_VERTEX || _renderMethod == RENDER_FILL_TRIANGLE_WIRE) {
			drawTriangle(
				triangle.points[0].x, triangle.points[0].y, // vertex A
				triangle.points[1].x, triangle.points[1].y,	// vertex B
				triangle.points[2].x, triangle.points[2].y,	// vertex C
				BLACK
			);
		}
		if (_renderMethod == RENDER_WIRE_VERTEX) {
			drawRect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, BLACK);
			drawRect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, BLACK);
			drawRect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, BLACK);
		}
	}
	//drawFilledTriangle(300, 100, 50, 400, 500, 700);

	//Clear array to render every frame
	array_free(trianglesToRender);
	renderColorBuffer();
	clearBuffer(_gridBackGroundColor);
	SDL_RenderPresent(_renderer);
}

void disposeResources(void) {
	free(_colorBuffer);
	array_free(_mesh.faces);
	array_free(_mesh.vertices);
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
	destroyWindow();
	disposeResources();

	return 0;
}

#pragma endregion


