#include "triangle.hpp"

Triangle::Triangle()
: m_norm()
{}

Triangle::Triangle(Vector3 v0, Vector3 v1, Vector3 v2, Vector3 n)
: m_norm(n)
{
	m_vertex[0] = v0;
	m_vertex[1] = v1;
	m_vertex[2] = v2;
}

Vector3 Triangle::getVertex(size_t i) const
{
	if (0 <= i && i < 3) return m_vertex[i];
	return Vector3();
}

void Triangle::setVertex(size_t i, Vector3 v)
{
	if (0 <= i && i < 3) m_vertex[i] = v;
}

Vector3 Triangle::getNormal() const
{
	return m_norm;
}

bool Triangle::valid() const
{
	Vector3 a = m_vertex[1] - m_vertex[0];
	Vector3 b = m_vertex[2] - m_vertex[0];
	Vector3 n = a.cross(b).norm();
	return n == m_norm;
}
