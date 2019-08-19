#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP
#include <cstddef>
#include "vector3.hpp"

class Triangle {
public:
	// Constructors
	Triangle();
	Triangle(Vector3, Vector3, Vector3, Vector3);

	/** Get one of the triangle's vertex.
	 * @param i Index from [0-2] in counter-clockwise order
	 * @return A vertex, a default constructed one on invalid index
	*/
	Vector3 getVertex(size_t) const;

	/** Replace one of the triangle's vertex
	 * @param i Index of the vertex being replaced
	 * @param v New vertex
	*/
	void setVertex(size_t, Vector3);

	/** Test if the calculated normal matches the given normal
	 * @return True if the two normals are equal, false otherwise
	*/
	bool valid() const;

private:
	// Instance variables
	Vector3 m_vertex[3];
	Vector3 m_norm;
};

#endif
