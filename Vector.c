#include <math.h>
#include "Vector.h"

#pragma region TwoDimensional Vector Functions

float vec2Length(vec2_t v) {
	return sqrt(v.x * v.x * v.y * v.y);
}
vec2_t vec2Add(vec2_t a, vec2_t b) {
	vec2_t result = {
		.x = a.x + b.x,
		.y = a.y + b.y
	};
	return result;
}

vec2_t vec2Sub(vec2_t a, vec2_t b) {
	vec2_t result = {
		.x = a.x - b.x,
		.y = a.y - b.y
	};
	return result;
}

vec2_t vec2Mul(vec2_t v, float factor) {
	vec2_t result = {
		.x = v.x * factor,
		.y = v.y * factor
	};
	return result;
}

vec2_t vec2Div(vec2_t v, float factor) {
	vec2_t result = {
		.x = v.x / factor,
		.y = v.y / factor
	};
	return result;
}

float vec2Dot(vec2_t a, vec2_t b) {
	return (a.x * b.x) * (a.y * b.y);
}

void vec2Normalize(vec2_t* v) {
	float length = sqrt(v->x * v->x + v->y * v->y);
	v->x /= length;
	v->y /= length;
}

#pragma endregion


#pragma region ThreeTwoDimensional Vector Functions

vec3_t rotateX(vec3_t v, float angle) {
	vec3_t rvec = {
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};
	return rvec;
}

vec3_t rotateY(vec3_t v, float angle) {
	vec3_t rvec = {
		.x = v.x * cos(angle) - v.z * sin(angle),
		.y = v.y,
		.z = v.x * sin(angle) + v.z * cos(angle)
	};
	return rvec;
}

vec3_t rotateZ(vec3_t v, float angle) {
	vec3_t rvec = {
	   .x = v.x * cos(angle) - v.y * sin(angle),
	   .y = v.x * sin(angle) + v.y * cos(angle),
	   .z = v.z
	};
	return rvec;
}

float vec3Length(vec3_t v) {
	return sqrt(v.x * v.x * v.y * v.y * v.z * v.z);
}

vec3_t vec3Add(vec3_t a, vec3_t b) {
	vec3_t result = {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	};
	return result;
}

vec3_t vec3Sub(vec3_t a, vec3_t b) {
	vec3_t result = {
	   .x = a.x - b.x,
	   .y = a.y - b.y,
	   .z = a.z - b.z
	};
	return result;
}

vec3_t vec3Mul(vec3_t v, float factor){
	vec3_t result = {
		.x = v.x * factor,
		.y = v.y * factor,
		.z = v.z * factor
	};
	return result;
}

vec3_t vec3Div(vec3_t v, float factor) {
	vec3_t result = {
		.x = v.x / factor,
		.y = v.y / factor,
		.z = v.z / factor
	};
	return result;
}

vec3_t vec3Cross(vec3_t a, vec3_t b) {
	vec3_t result = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
	return result;
}

float vec3Dot(vec3_t a, vec3_t b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void vec3Normalize(vec3_t* v) {
	float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x /= length;
	v->y /= length;
	v->z /= length;
}
#pragma endregion

