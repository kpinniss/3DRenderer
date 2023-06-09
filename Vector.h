#pragma once
#ifndef  VECTOR_H
#define VECTOR_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

typedef struct {
	float x, y, z;
} vec3_t;

typedef struct {
	float x, y;
} vec2_t;

//2D vector functions
float vec2Length(vec2_t v);
vec2_t vec2Add(vec2_t a, vec2_t b);
vec2_t vec2Sub(vec2_t a, vec2_t b);
vec2_t vec2Mul(vec2_t a, float factor);
vec2_t vec2Div(vec2_t a, float factor);
float vec2Dot(vec2_t a, vec2_t b);
void vec2Normalize(vec2_t* v);

//3D vector functions
float vec3Length(vec3_t v);
vec3_t vec3Add(vec3_t a, vec3_t b);
vec3_t vec3Sub(vec3_t a, vec3_t b);
vec3_t vec3Mul(vec3_t a, float factor);
vec3_t vec3Div(vec3_t a, float factor);
vec3_t vec3Cross(vec3_t a, vec3_t b);
float vec3Dot(vec3_t a, vec3_t b);
void vec3Normalize(vec3_t* v);

vec3_t rotateX(vec3_t v, float angle);
vec3_t rotateY(vec3_t v, float angle);
vec3_t rotateZ(vec3_t v, float angle);

#endif