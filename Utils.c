#include "Triangle.h"
#include "Vector.h"
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include "Array.h"
#include "Mesh.h"

bool startsWith(const char* a, const char* b){
	if (strncmp(a, b, strlen(b)) == 0) return 1;
	return 0;
}
