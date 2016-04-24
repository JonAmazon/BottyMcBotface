#include "PicoVec.h"

PicoVec4f::PicoVec4f()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

PicoVec4f::PicoVec4f(float xin,float yin,float zin,float win)
{
    x = xin;
    y = yin;
    z = zin;
    w = win;
}

float PicoVec4f::lengthSqrd()
{
    return (x*x+y*y+z*z+w*w);
}

float PicoVec4f::length()
{
    return sqrt(x*x+y*y+z*z+w*w);
}

void PicoVec4f::setToZero()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

void PicoVec4f::normalize()
{
	float r = length();
	if(r > 0.0)
	{
		x /= r;
		y /= r;
		z /= r;
		w /= r;
	}
}

float PicoVec4f::normalizeAndGetLength()
{
	float r = (*this).length();
	if(r > 0.0)
	{
		x /= r;
		y /= r;
		z /= r;
		w /= r;
	}
    return r;
}

void PicoVec4f::operator=(PicoVec4f& operand)
{
    x = operand.x;
    y = operand.y;
    z = operand.z;
    w = operand.w;
}

void PicoVec4f::operator+=(PicoVec4f& operand)
{
    x += operand.x;
    y += operand.y;
    z += operand.z;
    w += operand.w;
}

void PicoVec4f::operator-=(PicoVec4f& operand)
{
    x -= operand.x;
    y -= operand.y;
    z -= operand.z;
    w -= operand.w;
}

void PicoVec4f::operator*=(const float& scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
}

void PicoVec4f::operator/=(const float& scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
}

PicoVec4f PicoVec4f::operator-()
{
    return PicoVec4f(-x,-y,-z,-w);
}

PicoVec4f PicoVec4f::conjugate()
{
	return PicoVec4f(-x,-y,-z,w);
}

PicoVec4f PicoVec4f::operator+(PicoVec4f& operand)
{
    return PicoVec4f(x+operand.x,
                     y+operand.y,
                     z+operand.z,
                     w+operand.w);
}

PicoVec4f PicoVec4f::operator-(PicoVec4f& operand)
{
    return PicoVec4f(x-operand.x,
                     y-operand.y,
                     z-operand.z,
                     w-operand.w);
}

PicoVec4f PicoVec4f::operator*(const float& scalar)
{
    return PicoVec4f(x*scalar,
                     y*scalar,
                     z*scalar,
                     w*scalar);
}

PicoVec4f PicoVec4f::operator/(const float& scalar)
{
    return PicoVec4f(x/scalar,
                     y/scalar,
                     z/scalar,
                     w/scalar);
}

float PicoVec4f::innerProduct(PicoVec4f& operand)
{
    return (x*operand.x+
            y*operand.y+
            z*operand.z+
            w*operand.w);
}

PicoVec4f PicoVec4f::crossProduct(PicoVec4f& operand)
{
    return PicoVec4f(y*operand.z - z*operand.y,
                     z*operand.x - x*operand.z,
                     x*operand.y - y*operand.x,
                     0.0);
}

PicoVec4f PicoVec4f::directProduct(PicoVec4f& operand)
{
    return PicoVec4f(x*operand.x,
                     y*operand.y,
                     z*operand.z,
                     w*operand.w);
}

PicoVec4f PicoVec4f::quaternionProduct(PicoVec4f& quaternion)
{
	return PicoVec4f(x*quaternion.w + y*quaternion.z - z*quaternion.y + w*quaternion.x,
					-x*quaternion.z + y*quaternion.w + z*quaternion.x + w*quaternion.y,
					 x*quaternion.y - y*quaternion.x + z*quaternion.w + w*quaternion.z,
					 -x*quaternion.x - y*quaternion.y - z*quaternion.z + w*quaternion.w);
}

PicoVec4f PicoVec4f::quaternionRotate(PicoVec4f& quaternion)
{
	return quaternion.quaternionProduct((*this).quaternionProduct(quaternion.conjugate()));
}
