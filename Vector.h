#pragma once
#ifndef  VECTOR_H
#define VECTOR_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

typedef struct {
	float x;
	float y;
	float z;
} vec3_t;

typedef struct {
	float x;
	float y;
} vec2_t;

vec3_t rotateX(vec3_t v, float angle);
vec3_t rotateY(vec3_t v, float angle);
vec3_t rotateZ(vec3_t v, float angle);
//TODO: add functions to manipulate vectors 2D and 3D

#endif