#ifndef MESH_H
#define MESH_H

#include "Vector.h"
#include "Triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6*2) //6 cube faces, 2 triangles per face
extern vec3_t _cubeVertices[N_CUBE_VERTICES];
extern face_t _cubeFaces[N_CUBE_FACES];

//Defines struct for dynamically sized meshes
typedef struct {
	vec3_t* vertices; //dynamic array of vertices
	face_t* faces; //dynamic array of faces
	vec3_t rotation; //store rotation x,y,z
} mesh_t;

void loadMesh(char* fileName);
extern mesh_t _mesh;
#endif