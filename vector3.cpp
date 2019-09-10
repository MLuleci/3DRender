#include <utility>
#include <cmath>
#include "vector3.hpp"


Vector3::Vector3()
: x(0)
, y(0)
, z(0)
{}

Vector3::Vector3(double x, double y, double z)
: x(x)
, y(y)
, z(z)
{}

Vector3& Vector3::operator+=(const Vector3& o)
{
	x += o.x;
	y += o.y;
	z += o.z;
	return *this;
}

Vector3 Vector3::operator+(const Vector3& o) const
{
	return Vector3(x + o.x, y + o.y, z + o.z);
}

Vector3& Vector3::operator-=(const Vector3& o)
{
	x -= o.x;
	y -= o.y;
	z -= o.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3& o) const
{
	return *this + (-o);
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator*=(const Vector3& o)
{
	x *= o.x;
	y *= o.y;
	z *= o.z;
	return *this;
}

Vector3 Vector3::operator*(double f) const
{
	return Vector3(x * f, y * f, z * f);
}

Vector3& Vector3::operator/=(double f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

Vector3 Vector3::operator/(double f) const
{
	return Vector3(x / f, y / f, z / f);
}

bool Vector3::operator==(const Vector3& o)
{
	return x == o.x && y == o.y && z == o.z;
}

bool Vector3::operator!=(const Vector3& o)
{
	return !(*this == o);
}

Vector3 Vector3::cross(const Vector3& o) const
{
	return Vector3(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x);
}

double Vector3::dot(const Vector3& o) const
{
	return x * o.x + y * o.y + z * o.z;
}

Vector3 Vector3::norm() const
{
	return *this / mag();
}

double Vector3::mag() const
{
	return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
}

Vector3 Vector3::rotate(double x, double y, double z) const
{
	Vector3 v = *this;
	double c;
	double s;

	// x-axis rotation
	c = std::cos(x);
	s = std::sin(x);
	v.x = v.x;
	v.y = v.y * c + v.z * -s;
	v.z = v.y * s + v.z * c;

	// y-axis rotation
	c = std::cos(y);
	s = std::sin(y);
	v.x = v.x * c + v.z * s;
	v.y = v.y;
	v.z = v.x * -s + v.z * c;

	// z-axis rotation
	c = std::cos(z);
	s = std::sin(z);
	v.x = v.x * c + v.y * -s;
	v.y = v.x * s + v.y * c;
	v.z = v.z;

	return v;
}

double Vector3::angle(const Vector3& o) const
{
	return std::acos(this->dot(o) / (this->mag() * o.mag()));
}
