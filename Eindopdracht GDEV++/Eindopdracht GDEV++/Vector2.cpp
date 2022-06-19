#include "Vector2.h"
#include <cmath>

// use modulus to restrict x to the range [0, y)
float Clampf(float x, float y)
{
	float result = fmod(x, y);
	if (result < 0) {
		return result + y;
	}
	return result;
}

Vector2::Vector2(float _x, float _y)
{
	x = _x;
	y = _y;
}

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2 Vector2::operator+(const Vector2& vec)
{
	return Vector2(x + vec.x, y + vec.y);
}

Vector2 Vector2::operator-(const Vector2& vec)
{
	return Vector2(x - vec.x, y - vec.y);
}

Vector2 Vector2::operator-()
{
	return Vector2(-x, -y);
}

void Vector2::operator+=(const Vector2& vec)
{
	x += vec.x;
	y += vec.y;
}

void Vector2::operator-=(const Vector2& vec)
{
	x += vec.x;
	y += vec.y;
}

float Vector2::magnitude()
{
	return sqrt(x * x + y * y);
}

float Vector2::dotProduct(const Vector2& vec)
{
	return  x * vec.x + y * vec.y;
}


Vector2 Vector2::operator*(const float s)
{
	return Vector2(x * s, y * s);
}

Vector2 Vector2::operator/(const float s)
{
	return Vector2(x / s, y / s);
}

Vector2 Vector2::Clamp(const Vector2& v)
{
	return Vector2(Clampf(x,v.x), Clampf(y, v.y));
}

Vector2 Vector2::normalize()
{
	return Vector2(x, y) / this->magnitude();
}
