#include "PicoMat.h"

PicoMat4f::PicoMat4f()
{
    rows[0].setToZero();
    rows[1].setToZero();
    rows[2].setToZero();
    rows[3].setToZero();
}

PicoMat4f::PicoMat4f(PicoVec4f& row1, PicoVec4f& row2, PicoVec4f& row3, PicoVec4f& row4)
{
    rows[0] = row1;
    rows[1] = row2;
    rows[2] = row3;
    rows[3] = row4;
}

void PicoMat4f::setToZero()
{
    rows[0].setToZero();
    rows[1].setToZero();
    rows[2].setToZero();
    rows[3].setToZero();
}

void PicoMat4f::setToIdentity()
{
    rows[0] = PicoVec4f(1.0,0.0,0.0,0.0);
    rows[1] = PicoVec4f(0.0,1.0,0.0,0.0);
    rows[2] = PicoVec4f(0.0,0.0,1.0,0.0);
    rows[3] = PicoVec4f(0.0,0.0,0.0,1.0);
}

void PicoMat4f::operator=(PicoMat4f& operand)
{
    rows[0] = operand.rows[0];
    rows[1] = operand.rows[1];
    rows[2] = operand.rows[2];
    rows[3] = operand.rows[3];
}

void PicoMat4f::operator+=(PicoMat4f& operand)
{
    rows[0] += operand.rows[0];
    rows[1] += operand.rows[1];
    rows[2] += operand.rows[2];
    rows[3] += operand.rows[3];
}

void PicoMat4f::operator-=(PicoMat4f& operand)
{
    rows[0] -= operand.rows[0];
    rows[1] -= operand.rows[1];
    rows[2] -= operand.rows[2];
    rows[3] -= operand.rows[3];
}

void PicoMat4f::operator*=(float& scalar)
{
    rows[0] *= scalar;
    rows[1] *= scalar;
    rows[2] *= scalar;
    rows[3] *= scalar;
}

void PicoMat4f::operator/=(float& scalar)
{
    rows[0] /= scalar;
    rows[1] /= scalar;
    rows[2] /= scalar;
    rows[3] /= scalar;
}

PicoMat4f PicoMat4f::operator-()
{
    return PicoMat4f(-rows[0],
                     -rows[1],
                     -rows[2],
                     -rows[3]);
}

PicoMat4f PicoMat4f::transpose()
{
    PicoMat4f retMat;

    retMat.rows[0].x = rows[0].x;
    retMat.rows[0].y = rows[1].x;
    retMat.rows[0].z = rows[2].x;
    retMat.rows[0].w = rows[3].x;

    retMat.rows[1].x = rows[0].y;
    retMat.rows[1].y = rows[1].y;
    retMat.rows[1].z = rows[2].y;
    retMat.rows[1].w = rows[3].y;

    retMat.rows[2].x = rows[0].z;
    retMat.rows[2].y = rows[1].z;
    retMat.rows[2].z = rows[2].z;
    retMat.rows[2].w = rows[3].z;

    retMat.rows[3].x = rows[0].w;
    retMat.rows[3].y = rows[1].w;
    retMat.rows[3].z = rows[2].w;
    retMat.rows[3].w = rows[3].w;

    return retMat;
}

PicoMat4f PicoMat4f::operator+(PicoMat4f& operand)
{
    return PicoMat4f(rows[0]+operand.rows[0],
                     rows[1]+operand.rows[1],
                     rows[2]+operand.rows[2],
                     rows[3]+operand.rows[3]);
}

PicoMat4f PicoMat4f::operator-(PicoMat4f& operand)
{
    return PicoMat4f(rows[0]-operand.rows[0],
                     rows[1]-operand.rows[1],
                     rows[2]-operand.rows[2],
                     rows[3]-operand.rows[3]);
}

PicoMat4f PicoMat4f::operator*(float& scalar)
{
    return PicoMat4f(rows[0]*scalar,
                     rows[1]*scalar,
                     rows[2]*scalar,
                     rows[3]*scalar);
}

PicoMat4f PicoMat4f::operator/(float& scalar)
{
    return PicoMat4f(rows[0]/scalar,
                     rows[1]/scalar,
                     rows[2]/scalar,
                     rows[3]/scalar);
}

PicoVec4f PicoMat4f::matrixVectorProduct(PicoVec4f& operand)
{
    return PicoVec4f(rows[0].innerProduct(operand),
                     rows[1].innerProduct(operand),
                     rows[2].innerProduct(operand),
                     rows[3].innerProduct(operand));
}

void PicoMat4f::setToTransformation(PicoVec4f scale, PicoVec4f orientation, PicoVec4f translation)
{
    (*this).setToIdentity();
    rows[0] *= scale.x;
    rows[1] *= scale.y;
    rows[2] *= scale.z;

    orientation.normalize();
    rows[0] = rows[0].quaternionRotate(orientation.conjugate());
    rows[1] = rows[1].quaternionRotate(orientation.conjugate());
    rows[2] = rows[2].quaternionRotate(orientation.conjugate());

    rows[0].w = translation.x;
    rows[1].w = translation.y;
    rows[2].w = translation.z;
}
