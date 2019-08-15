#ifndef VECTOR3_HPP
#define VECTOR3_HPP

class Vector3 {
public:
	// Constructors
	Vector3();
	Vector3(float, float, float);

	// Operators
	Vector3& operator+=(const Vector3&);
	Vector3 operator+(const Vector3&) const;

	Vector3& operator-=(const Vector3&);
	Vector3 operator-(const Vector3&) const;

	Vector3& operator*=(const Vector3&);
	Vector3 operator*(float) const;

	Vector3& operator/=(float);
	Vector3 operator/(float) const;

	bool operator==(const Vector3&);
	bool operator!=(const Vector3&);


	/** Calculate the cross product of two vectors.
	 * @param a The first vector.
	 * @param b The second vector.
	 * @return Their cross product.
	*/
	Vector3 cross(const Vector3&) const;

	/** Calculate the dot product of two vectors.
	 * @param a The first vector.
	 * @param b The second vector.
	 * @return Their dot product.
	*/
	float dot(const Vector3&) const;

	/** Calculate the normalized version of the vector.
	 * @return The normalized version.
	*/
	Vector3 norm() const;

	/** Calculate the magnitude of the vector.
	 * @return Magnitude = |v|
	*/
	float mag() const;

	// Instance variables
	float x, y, z;
};

#endif
