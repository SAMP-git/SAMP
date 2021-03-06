/*
The MIT License (MIT)
Copyright (c) 2016-2017 IllidanS4
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
//Documentation: http://forum.sa-mp.com/showthread.php?t=608341
//Version 1.3
/*
native GetVehicleRotationQuatFixed(vehicleid, &Float:w, &Float:x, &Float:y, &Float:z);
native GetVehicleRotation(vehicleid, &Float:x, &Float:y, &Float:z);
native VectorRelToAbsQuat(Float:q[4], Float:v1[3], Float:v2[3]);
native RotationRelToAbsQuat(Float:q[4], Float:r1[3], Float:r2[3]);
native QuaternionRelToAbsQuat(Float:q[4], Float:q1[4], Float:q2[4]);
native VectorRelToAbs(Float:r[3], Float:v1[3], Float:v2[3]);
native RotationRelToAbs(Float:r[3], Float:r1[3], Float:r2[3]);
native QuaternionRelToAbs(Float:r[3], Float:q1[4], Float:q2[4]);
native VectorAbsToRelQuat(Float:q[4], Float:v1[3], Float:v2[3]);
native RotationAbsToRelQuat(Float:q[4], Float:r1[3], Float:r2[3]);
native QuaternionAbsToRelQuat(Float:q[4], Float:q1[4], Float:q2[4]);
native VectorAbsToRel(Float:r[3], Float:v1[3], Float:v2[3]);
native RotationAbsToRel(Float:r[3], Float:r1[3], Float:r2[3]);
native QuaternionAbsToRel(Float:r[3], Float:q1[4], Float:q2[4]);
native IsValidQuaternion(Float:q[4])
native GetQuaternionAngles(Float:w, Float:x, Float:y, Float:z, &Float:xa, &Float:ya, &Float:za)
native GetRotationQuaternion(Float:x, Float:y, Float:z, &Float:qw, &Float:qx, &Float:qy, &Float:qz);
native GetQuaternionVector(Float:qw, Float:qx, Float:qy, Float:qz, &Float:x, &Float:y, &Float:z);
native GetQuaternionAngle(Float:w, &Float:a);
native RotateVectorQuat(Float:v1[3], Float:q[4], Float:v2[3]);
native GetQuatConjugate(Float:q1[4], Float:q2[4]);
native GetQuatInverse(Float:q1[4], Float:q2[4]);
native GetQuatProduct(Float:q1[4], Float:q2[4], Float:q3[4]);
*/

#include <a_samp>

#if defined GetVehicleMatrix //has to be supplied by YSF-IS4 or other plugin
static stock near_zero(Float:val)
{
    return Float:(_:val & 0x7FFFFFFF) <= Float:0x1F800000;
}

stock GetVehicleRotationQuatFixed(vehicleid, &Float:w, &Float:x, &Float:y, &Float:z)
{
    new Float:matrix[3][3];
    GetVehicleMatrix(vehicleid, matrix[0][0], matrix[0][1], matrix[0][2], matrix[1][0], matrix[1][1], matrix[1][2], matrix[2][0], matrix[2][1], matrix[2][2]);
    if(near_zero(matrix[2][0]) && near_zero(matrix[2][1]) && near_zero(matrix[2][2]))
    {
        matrix[2][0] = matrix[0][1]*matrix[1][2]-matrix[0][2]*matrix[1][1];
        matrix[2][1] = matrix[0][2]*matrix[1][0]-matrix[0][0]*matrix[1][2];
        matrix[2][2] = matrix[0][0]*matrix[1][1]-matrix[0][1]*matrix[1][0];
    }
    w = floatsqroot(1 + matrix[0][0] + matrix[1][1] + matrix[2][2])/2;
    x = (matrix[2][1] - matrix[1][2])/(4*w);
    y = (matrix[0][2] - matrix[2][0])/(4*w);
    z = (matrix[1][0] - matrix[0][1])/(4*w);
}
#else
#define GetVehicleRotationQuatFixed GetVehicleRotationQuat
#endif

//Returns a vehicle's rotation in Euler angles
stock GetVehicleRotation(vehicleid, &Float:x, &Float:y, &Float:z)
{
    new Float:qw, Float:qx, Float:qy, Float:qz;
    GetVehicleRotationQuatFixed(vehicleid, qw, qx, qy, qz);
    GetQuaternionAngles(qw, qx, qy, qz, x, y, z);
}

//Converts a vector in coordinates relative to a body with rotation specified by a quaternion to a vector in absolute coordinates
stock VectorRelToAbsQuat(Float:q[4], Float:v1[3], Float:v2[3])
{
    new Float:q2[4];
    GetQuatConjugate(q, q2);
    RotateVectorQuat(v1, q2, v2);
}

//Converts an Euler rotation relative to a body with rotation specified by a quaternion to an absolute rotation
stock RotationRelToAbsQuat(Float:q[4], Float:r1[3], Float:r2[3])
{
    new Float:rq[4];
    GetRotationQuaternion(r1[0], r1[1], r1[2], rq[0], rq[1], rq[2], rq[3]);
    QuaternionRelToAbsQuat(q, rq, rq);
    GetQuaternionAngles(rq[0], rq[1], rq[2], rq[3], r2[0], r2[1], r2[2]);
}

//Converts a quaternion rotation relative to a body with rotation specified by a quaternion to an absolute rotation
stock QuaternionRelToAbsQuat(Float:q[4], Float:q1[4], Float:q2[4])
{
    GetQuatProduct(q1, q, q2);
}

//Converts a vector in coordinates relative to a body with rotation specified by Euler angles to a vector in absolute coordinates
stock VectorRelToAbs(Float:r[3], Float:v1[3], Float:v2[3])
{
    new Float:q[4];
    GetRotationQuaternion(r[0], r[1], r[2], q[0], q[1], q[2], q[3]);
    VectorRelToAbsQuat(q, v1, v2);
}

//Converts an Euler rotation relative to a body with rotation specified by Euler angles to an absolute rotation
stock RotationRelToAbs(Float:r[3], Float:r1[3], Float:r2[3])
{
    new Float:q[4];
    GetRotationQuaternion(r[0], r[1], r[2], q[0], q[1], q[2], q[3]);
    RotationRelToAbsQuat(q, r1, r2);
}

//Converts a quaternion rotation relative to a body with rotation specified by Euler angles to an absolute rotation
stock QuaternionRelToAbs(Float:r[3], Float:q1[4], Float:q2[4])
{
    new Float:q[4];
    GetRotationQuaternion(r[0], r[1], r[2], q[0], q[1], q[2], q[3]);
    QuaternionRelToAbsQuat(q, q1, q2);
}

//Converts a vector in absolute coordinates to a vector in coordinates relative to a body with rotation specified by a quaternion
stock VectorAbsToRelQuat(Float:q[4], Float:v1[3], Float:v2[3])
{
    RotateVectorQuat(v1, q, v2);
}

//Converts an Euler rotation to a rotation relative to a body with rotation specified by a quaternion
stock RotationAbsToRelQuat(Float:q[4], Float:r1[3], Float:r2[3])
{
    new Float:rq[4];
    GetRotationQuaternion(r1[0], r1[1], r1[2], rq[0], rq[1], rq[2], rq[3]);
    QuaternionAbsToRelQuat(q, rq, rq);
    GetQuaternionAngles(rq[0], rq[1], rq[2], rq[3], r2[0], r2[1], r2[2]);
}

//Converts an absolute quaternion rotation to a rotation relative to a body with rotation specified by Euler angles
stock QuaternionAbsToRelQuat(Float:q[4], Float:q1[4], Float:q2[4])
{
    new Float:qi[4];
    GetQuatConjugate(q, qi);
    GetQuatProduct(q1, qi, q2);
}

//Converts a vector in absolute coordinates to a vector in coordinates relative to a body with rotation specified by Euler angles 
stock VectorAbsToRel(Float:r[3], Float:v1[3], Float:v2[3])
{
    new Float:q[4];
    GetRotationQuaternion(r[0], r[1], r[2], q[0], q[1], q[2], q[3]);
    VectorAbsToRelQuat(q, v1, v2);
}

//Converts an Euler rotation to a rotation relative to a body with rotation specified by Euler angles 
stock RotationAbsToRel(Float:r[3], Float:r1[3], Float:r2[3])
{
    new Float:q[4];
    GetRotationQuaternion(r[0], r[1], r[2], q[0], q[1], q[2], q[3]);
    RotationAbsToRelQuat(q, r1, r2);
}

//Converts an absolute quaternion rotation to a rotation relative to a body with rotation specified by Euler angles
stock QuaternionAbsToRel(Float:r[3], Float:q1[4], Float:q2[4])
{
    new Float:q[4];
    GetRotationQuaternion(r[0], r[1], r[2], q[0], q[1], q[2], q[3]);
    QuaternionAbsToRelQuat(q, q1, q2);
}

//Checks if a quaternion is a valid rotation quaternion
stock IsValidQuaternion(Float:q[4])
{
    for(new i = 0; i < sizeof q; i++)
    {
        if(!(-1.0 <= q[i] <= 1.0)) return false;
    }
    return true;
}

static stock Float:asin_limit(Float:value)
{
    if(value > 1.0) value = 1.0;
    else if(value < -1.0) value = -1.0;
    return asin(value);
}

static stock Float:acos_limit(Float:value)
{
    if(value > 1.0) value = 1.0;
    else if(value < -1.0) value = -1.0;
    return acos(value);
}

static stock Float:atan_limit(Float:value)
{
    if(value > 1.0) value = 1.0;
    else if(value < -1.0) value = -1.0;
    return atan(value);
}

static stock Float:atan2_limit(Float:x, Float:y)
{
    if(x > 1.0) x = 1.0;
    else if(x < -1.0) x = -1.0;
    if(y > 1.0) y = 1.0;
    else if(y < -1.0) y = -1.0;
    return atan2(x, y);
}

//Returns a set of Euler angles from a quaternion
stock GetQuaternionAngles(Float:w, Float:x, Float:y, Float:z, &Float:xa, &Float:ya, &Float:za)
{
    xa = asin_limit(2*y*z - 2*x*w);
    ya = -atan2_limit(x*z + y*w, 0.5 - x*x - y*y);
    za = -atan2_limit(x*y + z*w, 0.5 - x*x - z*z);
}

//Creates a quaternion from Euler angles
stock GetRotationQuaternion(Float:x, Float:y, Float:z, &Float:qw, &Float:qx, &Float:qy, &Float:qz)
{
    new Float:cx = floatcos(-0.5*x, degrees);
    new Float:sx = floatsin(-0.5*x, degrees);
    new Float:cy = floatcos(-0.5*y, degrees);
    new Float:sy = floatsin(-0.5*y, degrees);
    new Float:cz = floatcos(-0.5*z, degrees);
    new Float:sz = floatsin(-0.5*z, degrees);
    qw = cx * cy * cz + sx * sy * sz;
    qx = cx * sy * sz + sx * cy * cz;
    qy = cx * sy * cz - sx * cy * sz;
    qz = cx * cy * sz - sx * sy * cz;
}

//Returns the vector component of a quaternion
stock GetQuaternionVector(Float:qw, Float:qx, Float:qy, Float:qz, &Float:x, &Float:y, &Float:z)
{
    new Float:a;
    GetQuaternionAngle(qw, a);
    new Float:s = floatsin(a/2.0, degrees);
    x = qx/s;
    y = qy/s;
    z = qz/s;
}

//Returns the angle component of a quaternion
stock GetQuaternionAngle(Float:w, &Float:a)
{
    a = 2.0*acos_limit(w);
}

//Rotates a vector with a specified quaternion performing a conjugation v2 = q v1 q*
stock RotateVectorQuat(Float:v1[3], Float:q[4], Float:v2[3])
{
    new Float:q1[4], Float:q2[4];
    q1 = q;
    q2[1] = v1[0], q2[2] = v1[1], q2[3] = v1[2];
    GetQuatProduct(q1, q2, q2);
    GetQuatConjugate(q1, q1);
    GetQuatProduct(q2, q1, q2);
    v2[0] = q2[1], v2[1] = q2[2], v2[2] = q2[3];
}

//Returns a conjugate of a quaternion, with all non-real components inversed
stock GetQuatConjugate(Float:q1[4], Float:q2[4])
{
    q2[0] =  q1[0];
    q2[1] = -q1[1];
    q2[2] = -q1[2];
    q2[3] = -q1[3];
}

//Returns a Hamilton product of two quaternions
stock GetQuatProduct(Float:q1[4], Float:q2[4], Float:q3[4])
{
    new Float:w = q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2] - q1[3]*q2[3];
    new Float:x = q1[0]*q2[1] + q1[1]*q2[0] + q1[2]*q2[3] - q1[3]*q2[2];
    new Float:y = q1[0]*q2[2] - q1[1]*q2[3] + q1[2]*q2[0] + q1[3]*q2[1];
    new Float:z = q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1] + q1[3]*q2[0];
    q3[0] = w, q3[1] = x, q3[2] = y, q3[3] = z;
}