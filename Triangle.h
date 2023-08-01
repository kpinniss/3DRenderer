#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Vector.h"
#include "Utils.h"

#define T_SIZE 3

typedef struct {
	int a, b, c;
	color_t color;
}face_t;

typedef struct {
	vec2_t points[T_SIZE];
	color_t color;
	float avg_depth;
} triangle_t;
#endif