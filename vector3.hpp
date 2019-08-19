#ifndef VECTOR3_HPP
#define VECTOR3_HPP

class Vector3 {
public:
	// Constructors
	Vector3();
	Vector3(double, double, double);

	// Operators
	Vector3& operator+=(const Vector3&);
	Vector3 operator+(const Vector3&) const;

	Vector3& operator-=(const Vector3&);
	Vector3 operator-(const Vector3&) const;
	Vector3 operator-() const;

	Vector3& operator*=(const Vector3&);
	Vector3 operator*(double) const;

	Vector3& operator/=(double);
	Vector3 operator/(double) const;

	bool operator==(const Vector3&);
	bool operator!=(const Vector3&);


	/** Calculate the cross product of two Vector3s.
	 * @param a First vector
	 * @param b Second vector
	 * @return Cross product
	*/
	Vector3 cross(const Vector3&) const;

	/** Calculate the dot product of two Vector3s.
	 * @param a First vector
	 * @param b Second vector
	 * @return Dot product
	*/
	double dot(const Vector3&) const;

	/** Calculate the normalized version of the Vector3.
	 * @return Normalize (unit) vector
	*/
	Vector3 norm() const;

	/** Calculate the magnitude of the Vector3.
	 * @return Magnitude = |v|
	*/
	double mag() const;

	// Instance variables
	double x, y, z;
};

#endif
