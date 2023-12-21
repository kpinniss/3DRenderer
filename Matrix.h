#ifndef MATRIX_H
#define MATIX_H
#include "Vector.h"

typedef struct {
	float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_makeScale(float sx, float sy, float sz);
mat4_t mat4_makeTranslation(float tx, float ty, float tz);
vec4_t mat4_mul_vec4(mat4_t matrix, vec4_t v);
mat4_t mat4_makeRotationX(float angle);
mat4_t mat4_makeRotationY(float angle);
mat4_t mat4_makeRotationZ(float angle);

#endif