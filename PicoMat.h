#ifndef _H_PICOMAT
#define _H_PICOMAT

#include<cstdio>
#include<cstdlib>
#include<cmath>
#include"PicoVec.h"

struct PicoMat4f
{
    PicoVec4f rows[4];

    PicoMat4f();
    PicoMat4f(PicoVec4f& row1, PicoVec4f& row2, PicoVec4f& row3, PicoVec4f& row4);

    void setToZero();
    void setToIdentity();
    void setToTransformation(PicoVec4f scale, PicoVec4f orientation, PicoVec4f translation);
    void operator=(PicoMat4f& operand);
    void operator+=(PicoMat4f& operand);
    void operator-=(PicoMat4f& operand);
    void operator*=(float& scalar);
    void operator/=(float& scalar);

    PicoMat4f operator-();
    PicoMat4f transpose();
    PicoMat4f operator+(PicoMat4f& operand);
    PicoMat4f operator-(PicoMat4f& operand);
    PicoMat4f operator*(float& scalar);
    PicoMat4f operator/(float& scalar);
    PicoVec4f matrixVectorProduct(PicoVec4f& operand);
};

#endif
