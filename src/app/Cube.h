/**
 * @file Cube.h
 *
 * @author Jan Dusek <jan.dusek90@gmail.com>
 * @date 2013
 */

#ifndef CUBE_H
#define CUBE_H

#include <cstdint>

const static size_t cubeVerticesCount = 8;
const static size_t cubeIndicesCount = 36;

struct CubeVertex
{
	float pos[3];
	float normal[3];
};

static const CubeVertex cubeVertices[cubeVerticesCount] = {
	{{1.0f, 1.0f, -1.0f}, {0.333333f, 0.666667f, -0.666667f}},
	{{-1.0f, 1.0f, -1.0f}, {-0.816497f, 0.408248f, -0.408248f}},
	{{-1.0f, 1.0f, 1.0f}, {-0.333333f, 0.666667f, 0.666667f}},
	{{1.0f, 1.0f, 1.0f}, {0.816497f, 0.408248f, 0.408248f}},
	{{1.0f, -1.0f, -1.0f}, {0.666667f, -0.666667f, -0.333333f}},
	{{-1.0f, -1.0f, -1.0f}, {-0.408248f, -0.408248f, -0.816497f}},
	{{-1.0f, -1.0f, 1.0f}, {-0.666667f, -0.666667f, 0.333333f}},
	{{1.0f, -1.0f, 1.0f}, {0.408248f, -0.408248f, 0.816497f}}
};

static const uint32_t cubeIndices[cubeIndicesCount] = {
	0, 1, 2,
	0, 2, 3,
	0, 4, 5,
	0, 5, 1,
	1, 5, 6,
	1, 6, 2,
	2, 6, 7,
	2, 7, 3,
	3, 7, 4,
	3, 4, 0,
	4, 7, 6,
	4, 6, 5
};

#endif
