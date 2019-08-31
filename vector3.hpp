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

	/** Calculate the cross product of two vectors.
	 * @param a First vector
	 * @param b Second vector
	 * @return Cross product
	*/
	Vector3 cross(const Vector3&) const;

	/** Calculate the dot product of two vectors.
	 * @param a First vector
	 * @param b Second vector
	 * @return Dot product
	*/
	double dot(const Vector3&) const;

	/** Calculate the normalized version of the vector.
	 * @return Normalize (unit) vector
	*/
	Vector3 norm() const;

	/** Calculate the magnitude of the vector.
	 * @return Magnitude = |v|
	*/
	double mag() const;

	/** Rotate a vector around multiple axis.
	 * @param x Angle around x-axis
	 * @param y Angle around y-axis
	 * @param z Angle around z-axis
	 * @return A vector with all the rotations applied
	*/
	Vector3 rotate(double, double, double) const;

	/** Find the angle between two vectors.
	 * @param o The other vector
	*/
	double angle(const Vector3&) const;

	// Instance variables
	double x, y, z;
};

#endif
