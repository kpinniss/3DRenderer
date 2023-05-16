#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Vector.h"

#define T_SIZE 3

typedef struct {
	int a, b, c;
}face_t;

typedef struct {
	vec2_t points[T_SIZE];
} triangle_t;
#endif