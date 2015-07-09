#ifndef CUBES_H
#define CUBES_H

#include "vectorial\vectorial.h"

struct CubeInstance
{
	float r, g, b, a;
	vectorial::mat4f model;
	vectorial::mat4f modelView;
	vectorial::mat4f modelViewProjection;
};

#endif