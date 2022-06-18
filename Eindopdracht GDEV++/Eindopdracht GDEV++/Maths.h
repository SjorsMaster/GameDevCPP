#pragma once
#include <iostream>
#include <list>

class Vector2 //Convert to namespace maths instead of this
{
public:
	float x = 0;
	float y = 0;

	Vector2(float _x, float _y);

	Vector2();

	// Overload add (+) operator
	Vector2 operator+(const Vector2& vec);

	// Overload subtract (-) operator
	Vector2 operator-(const Vector2& vec);

	// Overload add equals (+=) operator
	void operator+=(const Vector2& vec);

	// Overload subtract equals (-=) operator
	void operator-=(const Vector2& vec);

	// Overload multiply (*) operator (scalar multiplication)
	// x = x * s, y = y * s
	Vector2 operator*(const float s);

	// Overload divide (/) operator (scalar division)
	// x = x / s, y = y / s
	Vector2 operator/(const float s);

	// Magnitude (vector length)
	// Use Pythagorian theorem
	float magnitude();

	// Dot-product
	// v.x * w.x + v.x * w.x
	float dotProduct(const Vector2& vec);

	// Normalize
	// Create vector of same direction of length/magnitude 1
	Vector2 normalize();
};