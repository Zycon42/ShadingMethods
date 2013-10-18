/**
 * @file Common.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef COMMON_H
#define COMMON_H

#include "Exception.h"

class ShaderException : public Exception 
{
public:
	ShaderException(const char* msg) : Exception(msg) { }
};

struct Viewport
{
	Viewport() : x(0.0f), y(0.0f), width(0.0f), height(0.0f), znear(0.0f), zfar(1.0f) {}
	Viewport(float width, float height) : x(0.0f), y(0.0f), width(width), height(height), znear(0.0f), zfar(1.0f) {}
	Viewport(float x, float y, float width, float height, float znear, float zfar)
		: x(x), y(y), width(width), height(height), znear(znear), zfar(zfar) { }

	float x;
	float y;
	float width;
	float height;
	float znear;
	float zfar;
};

enum class PrimitiveType { Points, LineList, LineStrip, TriangleList,
	TriangleStrip
};

enum class VertexElementType { Byte = 0, UnsignedByte = 1, Short = 2, UnsignedShort = 3,
	Int = 4, UsignedInt = 5, Float = 6, Double = 7
};

struct VertexElement
{
	VertexElement(int numComponents = 4, VertexElementType type = VertexElementType::Float, size_t stride = 0, size_t offset = 0)
		: numComponents(numComponents), type(type), stride(stride), offset(offset)
	{ }

	/// Number of components in this element. 4 is default.
	int numComponents;
	/// Component type.
	VertexElementType type;
	/// byte offset between this element consecutive values. If 0 is given
	/// then stride is computed in a way that elements are interleaved.
	size_t stride;
	/// byte offset of this element since the first element in vertex. If 0 is given
	/// then offset is computed in a way that elements are interleaved.
	size_t offset;
};


#endif // !COMMON_H
