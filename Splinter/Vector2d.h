/*
  Vector2d.h
  
  This software is for demonstration purposes only. Use at your own
  risk.
*/


#ifndef _Vector2d_h
#define _Vector2d_h

#include "FixedPoint.h"


/** Representation of a vector */
typedef struct Vector2d {
  FixedPoint data[2];
} Vector2d;


/**
 * Sets all elements to zero.
 * @param a[out] vector to clear
 */
void Vector2dZero(Vector2d *a);


/**
 * Computes a = b + c
 * @param a[out] return value
 * @param b[in] addend
 * @param c[in] addend
 */
void Vector2dAdd(Vector2d *a, Vector2d *b, Vector2d *c);


/**
 * Computes a = b - c
 * @param a[out] return value
 * @param b[in] quantity
 * @param c[in] subtractand
 */
void Vector2dSub(Vector2d *a, Vector2d *b, Vector2d *c);


/**
 * Computes a = b * c
 * @param a[out] return value
 * @paran b[in] scalar multiplier
 * @param c[in] vector to multiply
 */
void Vector2dMul(Vector2d *a, FixedPoint *b, Vector2d *c);


/**
 * Computes a = b dot c
 * @param a[out] return value
 * @paran b[in] scalar multiplier
 * @param c[in] vector to multiply
 */
void Vector2dDot(FixedPoint *a, Vector2d *b, Vector2d *c);


/**
 * Computes a normalized vector from b
 * @param a[out] unit vector
 * @paran b[in] non-zero input vector
 */
void Vector2dNormalize(Vector2d *a, Vector2d *b);


/**
 * Computes the length of b.
 * @param a[out] length of b
 * @param b[in] input vector
 */
void Vector2dLength(FixedPoint *a, Vector2d *b);


/**
 * Computes the reflection vector of b off of n
 * @param a[out] reflection of b off of n
 * @param b[in] vector to reflect
 * @param n[in] normalized vector
 */
void Vector2dReflectionVector(Vector2d *a, Vector2d *b, Vector2d *n);


/**
 * Creates an ASCII representation of a.
 * @param buffer[out] output buffer with at least 28 bytes
 * @param a[in] source
 */
char *Vector2dToA(char *buffer, Vector2d *a);

#endif
