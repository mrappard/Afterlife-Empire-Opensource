//
//  VecAndBB.h
//  After Life Empire
//
//  Copyright (c) 2015 AfterlifeEmpireInc. All rights reserved.
//

#ifndef After_Life_Empire_VecAndBB_h
#define After_Life_Empire_VecAndBB_h

#include "Float.h"

#if defined(_WIN32)
#include "chipmunk.h"
#define SIZE_T_MAX ((size_t) -1)
#define _USE_MATH_DEFINES
#include <cmath> 



#else
#include "Math.h"

#include <cmath> 


typedef struct cpVect{double x,y;} cpVect;

typedef struct cpBB{
    double l, b, r ,t;
} cpBB;


static inline cpBB cpBBNew(const double l, const double b, const double r, const double t)
{
    cpBB bb = {l, b, r, t};
    return bb;
}


/// Returns true if @c a and @c b intersect.
static inline bool cpBBIntersects(const cpBB a, const cpBB b)
{
    return (a.l <= b.r && b.l <= a.r && a.b <= b.t && b.b <= a.t);
}

static inline bool cpBBContainsVect(const cpBB bb, const cpVect v)
{
    return (bb.l <= v.x && bb.r >= v.x && bb.b <= v.y && bb.t >= v.y);
}



static const cpVect cpvzero = {0.0f,0.0f};

static inline cpVect cpv(const double x, const double y)
{
    cpVect v = {x, y};
    return v;
}

/// Add two vectors
static inline cpVect cpvadd(const cpVect v1, const cpVect v2)
{
    return cpv(v1.x + v2.x, v1.y + v2.y);
}

/// Subtract two vectors.
static inline cpVect cpvsub(const cpVect v1, const cpVect v2)
{
    return cpv(v1.x - v2.x, v1.y - v2.y);
}

/// Negate a vector.
static inline cpVect cpvneg(const cpVect v)
{
    return cpv(-v.x, -v.y);
}

/// Scalar multiplication.
static inline cpVect cpvmult(const cpVect v, const double s)
{
    return cpv(v.x*s, v.y*s);
}

/// Vector dot product.
static inline double cpvdot(const cpVect v1, const cpVect v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}

/// 2D vector cross product analog.
/// The cross product of 2D vectors results in a 3D vector with only a z component.
/// This function returns the magnitude of the z value.
static inline double cpvcross(const cpVect v1, const cpVect v2)
{
    return v1.x*v2.y - v1.y*v2.x;
}

/// Returns a perpendicular vector. (90 degree rotation)
static inline cpVect cpvperp(const cpVect v)
{
    return cpv(-v.y, v.x);
}

/// Returns a perpendicular vector. (-90 degree rotation)
static inline cpVect cpvrperp(const cpVect v)
{
    return cpv(v.y, -v.x);
}

/// Returns the vector projection of v1 onto v2.
static inline cpVect cpvproject(const cpVect v1, const cpVect v2)
{
    return cpvmult(v2, cpvdot(v1, v2)/cpvdot(v2, v2));
}

/// Returns the unit length vector for the given angle (in radians).
static inline cpVect cpvforangle(const double a)
{
    return cpv(cos(a), sin(a));
}

/// Returns the angular direction v is pointing in (in radians).
static inline double cpvtoangle(const cpVect v)
{
    return atan2(v.y, v.x);
}

/// Uses complex number multiplication to rotate v1 by v2. Scaling will occur if v1 is not a unit vector.
static inline cpVect cpvrotate(const cpVect v1, const cpVect v2)
{
    return cpv(v1.x*v2.x - v1.y*v2.y, v1.x*v2.y + v1.y*v2.x);
}

/// Inverse of cpvrotate().
static inline cpVect cpvunrotate(const cpVect v1, const cpVect v2)
{
    return cpv(v1.x*v2.x + v1.y*v2.y, v1.y*v2.x - v1.x*v2.y);
}

/// Returns the squared length of v. Faster than cpvlength() when you only need to compare lengths.
static inline double cpvlengthsq(const cpVect v)
{
    return cpvdot(v, v);
}

/// Returns the length of v.
static inline double cpvlength(const cpVect v)
{
    return sqrt(cpvdot(v, v));
}

/// Linearly interpolate between v1 and v2.
static inline cpVect cpvlerp(const cpVect v1, const cpVect v2, const double t)
{
    return cpvadd(cpvmult(v1, 1.0f - t), cpvmult(v2, t));
}

/// Returns a normalized copy of v.
static inline cpVect cpvnormalize(const cpVect v)
{
    // Neat trick I saw somewhere to avoid div/0.
    return cpvmult(v, 1.0f/(cpvlength(v) + DBL_MIN));
}

/// @deprecated Just an alias for cpvnormalize() now.
static inline cpVect cpvnormalize_safe(const cpVect v)
{
    return cpvnormalize(v);
}

/// Clamp v to length len.
static inline cpVect cpvclamp(const cpVect v, const double len)
{
    return (cpvdot(v,v) > len*len) ? cpvmult(cpvnormalize(v), len) : v;
}

/// Linearly interpolate between v1 towards v2 by distance d.
static inline cpVect cpvlerpconst(cpVect v1, cpVect v2, double d)
{
    return cpvadd(v1, cpvclamp(cpvsub(v2, v1), d));
}

/// Returns the distance between v1 and v2.
static inline double cpvdist(const cpVect v1, const cpVect v2)
{
    return cpvlength(cpvsub(v1, v2));
}

/// Returns the squared distance between v1 and v2. Faster than cpvdist() when you only need to compare distances.
static inline double cpvdistsq(const cpVect v1, const cpVect v2)
{
    return cpvlengthsq(cpvsub(v1, v2));
}

/// Returns true if the distance between v1 and v2 is less than dist.
static inline bool cpvnear(const cpVect v1, const cpVect v2, const double dist)
{
    return cpvdistsq(v1, v2) < dist*dist;
}

/// Check if two vectors are equal. (Be careful when comparing floating point numbers!)
static inline bool cpveql(const cpVect v1, const cpVect v2)
{
    return (v1.x == v2.x && v1.y == v2.y);
}


#endif

#endif
