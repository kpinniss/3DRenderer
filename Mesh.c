#include <stdio.h>
#include "Mesh.h"
#include "array.h"

mesh_t _mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0,0,0,}
};

vec3_t _cubeVertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1 }, // 1
    {.x = -1, .y = 1, .z = -1 }, // 2
    {.x = 1, .y = 1, .z = -1 }, // 3
    {.x = 1, .y = -1, .z = -1 }, // 4
    {.x = 1, .y = 1, .z = 1 }, // 5
    {.x = 1, .y = -1, .z = 1 }, // 6
    {.x = -1, .y = 1, .z = 1 }, // 7
    {.x = -1, .y = -1, .z = 1 }  // 8
};

face_t _cubeFaces[N_CUBE_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3 },
    {.a = 1, .b = 3, .c = 4 },
    // right
    {.a = 4, .b = 3, .c = 5 },
    {.a = 4, .b = 5, .c = 6 },
    // back
    {.a = 6, .b = 5, .c = 7 },
    {.a = 6, .b = 7, .c = 8 },
    // left
    {.a = 8, .b = 7, .c = 2 },
    {.a = 8, .b = 2, .c = 1 },
    // top
    {.a = 2, .b = 7, .c = 5 },
    {.a = 2, .b = 5, .c = 3 },
    // bottom
    {.a = 6, .b = 8, .c = 1 },
    {.a = 6, .b = 1, .c = 4 }
};

void loadDefaultMesh(void) {
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        vec3_t vertex = _cubeVertices[i];
        array_push(_mesh.vertices, vertex);
    }
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        face_t face = _cubeFaces[i];
        array_push(_mesh.faces, face);
    }
}