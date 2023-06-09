#include <stdio.h>
#include "Mesh.h"
#include "Array.h"
#include "Utils.h"

mesh_t _mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0,0,0,}
};
//_cubeVertices
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

void loadFromFile(char* filePath) {
    FILE* file;
    errno_t err;

    // Store the content of the file
    char fileContents[1024]; // each line of file

    if ((err = fopen_s(&file, filePath, "r")) != 0) {
        // File could not be opened. filepoint was set to NULL
        printf(stderr, "cannot open file '%s': %s\n", strerror(err));
        return 1;
    }
    else {
        // Read the content and store it inside myString
        if (file != NULL) {
            while (fgets(fileContents, sizeof(fileContents), file)) {
                if (startsWith(fileContents, "v ")) {
                    vec3_t vertex;
                    sscanf_s(fileContents, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z); //TODO: replace sscan_s is windows exclusive. replace with c99 version
                    array_push(_mesh.vertices, vertex);
                }
                if (startsWith(fileContents, "f ")) {
                    int vertexIndices[3];
                    int textureIndices[3];
                    int normalIndices[3];
                    sscanf_s(fileContents, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                        &vertexIndices[0], &textureIndices[0], &normalIndices[0],
                        &vertexIndices[1], &textureIndices[1], &normalIndices[1],
                        &vertexIndices[2], &textureIndices[2], &normalIndices[2]
                    );
                    face_t face = {
                        .a = vertexIndices[0],
                        .b = vertexIndices[1],
                        .c = vertexIndices[2]
                    };
                    array_push(_mesh.faces, face);
                }
            }
        }
        // Print the file content
        printf("%s", fileContents);

        // Close the file
        fclose(file);
    }
}


void loadMesh(char* fileName) {
    if (fileName == NULL || fileName[0] == '\0') {
        loadDefaultMesh();
    }
    else {
        loadFromFile(fileName);
    }
}