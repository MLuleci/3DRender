#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <utility>
#include <cstdio>
#include <cmath>
#include "solid.hpp"

// Default constructor
Solid::Solid()
: m_arr(nullptr)
, m_max(0)
, m_len(0)
, m_light(false)
, m_upper()
, m_lower()
, m_index(0)
, m_vertex(nullptr)
, m_norm(nullptr)
{}

// Destructor
Solid::~Solid()
{
	delete[] m_arr;
	delete[] m_vertex;
	delete[] m_norm;
}

// Copy constructor
Solid::Solid(const Solid& o)
{
	m_max = o.m_max;
	m_len = o.m_len;
	m_light = o.m_light;
	m_index = o.m_index;

	if (o.m_arr) {
		m_arr = new Triangle[m_max];
		memcpy(m_arr, o.m_arr, sizeof(Triangle) * m_max);
	}

	if (o.m_vertex) {
		m_vertex = new GLdouble[m_max * 9];
		memcpy(m_vertex, o.m_vertex, sizeof(GLdouble) * m_max * 9);
	}

	if (o.m_norm) {
		m_norm = new GLdouble[m_max * 3];
		memcpy(m_norm, o.m_norm, sizeof(GLdouble) * m_max * 3);
	}
}

// Move constructor
Solid::Solid(Solid&& o) noexcept
: m_arr(std::move(o.m_arr))
, m_max(o.m_max)
, m_len(o.m_len)
, m_light(o.m_light)
, m_index(o.m_index)
, m_vertex(std::move(o.m_vertex))
, m_norm(std::move(o.m_norm))
{
	o.m_arr = nullptr;
	o.m_max = 0;
	o.m_len = 0;
	o.m_light = false;
	o.m_index = 0;
	o.m_vertex = nullptr;
	o.m_norm = nullptr;
}

// Copy assignment
Solid& Solid::operator=(const Solid& o)
{
	return *this = Solid(o);
}

// Move assignment
Solid& Solid::operator=(Solid&& o) noexcept
{
	m_arr = std::exchange(o.m_arr, nullptr);
	m_max = std::exchange(o.m_max, 0);
	m_len = std::exchange(o.m_len, 0);
	m_light = std::exchange(o.m_light, false);
	m_index = std::exchange(o.m_index, 0);
	m_vertex = std::exchange(o.m_vertex, nullptr);
	m_norm = std::exchange(o.m_norm, nullptr);
	return *this;
}

bool Solid::readFile(std::string f)
{
	std::string ext = f.substr(f.find_last_of('.') + 1);
	if (ext.compare("stl") && ext.compare("STL")) {
		std::cerr << "Invalid file type " << std::quoted(ext) << std::endl;
		return false;
	}

	std::ifstream is (f, std::ifstream::binary);
	if (!is) {
		std::cerr << "Couldn't open " << std::quoted(f) << std::endl;
		return false;
	}

	// Skip file header
	is.seekg(80, is.beg);

	// Get machine endianness
	bool le = endian();

	// Read number of triangles
	is.read((char *) &m_max, 4);
	if (!le) swapEndian<uint32_t>(&m_max);

	// Re-initialize variables
	m_len = 0;
	delete[] m_arr;
	m_arr = new Triangle[m_max];

	if (m_arr == nullptr) {
		std::cerr << "Not enough memory" << std::endl;
		return false;
	}

	// Read all triangles
	bool warn = false;
	while(is.good() && m_len < m_max) {
		Vector3 v[4]; // In order: norm, v0, v1, v2
		for (int i = 0; i < 4; ++i) { // 4 vectors per triangle
			float x, y, z; // 3 floats per vector

			char buf[12];
			is.read(buf, 12);

			if (is.gcount() != 12) {
				std::cerr << "Read error" << std::endl;
				return false;
			}

			x = *((float *) buf);
			y = *((float *) buf + 4);
			z = *((float *) buf + 8);

			// Do endianness swaps if necessary
			if (!le) {
				swapEndian<float>(&x);
				swapEndian<float>(&y);
				swapEndian<float>(&z);
			}

			v[i] = Vector3(x, y, z);

			m_upper.x = (m_upper.x < x ? x : m_upper.x);
			m_upper.y = (m_upper.y < y ? y : m_upper.y);
			m_upper.z = (m_upper.z < z ? z : m_upper.z);

			m_lower.x = (m_lower.x > x ? x : m_lower.x);
			m_lower.y = (m_lower.y > y ? y : m_lower.y);
			m_lower.z = (m_lower.z > z ? z : m_lower.z);
		}

		// Check if normal vector and m_max are valid
		Triangle t(v[1], v[2], v[3], v[0]);
		if (!t.valid()) warn = true;
		if (!append(t)) {
			std::cerr << "Internal failure" << std::endl;
			return false;
		}

		// Skip attribute bytes
		is.seekg(2, is.cur);
	}

	if (warn) std::cerr << "Warning: File may be corrupt (bad normals)" << std::endl;
	std::cout << "File read OK (" << m_max << " polygons)" << std::endl;
	is.close();

	genDisplayList();
	return true;
}

void Solid::toggleLight()
{
	m_light = !m_light;
	genDisplayList();
}

GLuint Solid::getList() const
{
	return m_index;
}

double Solid::getRadius() const
{
	return (getCenter() - m_lower).mag();
}

Vector3 Solid::getCenter() const
{
	return (m_upper + m_lower) / 2.0;
}

bool Solid::append(const Triangle& t)
{
	if (m_len < m_max && m_arr) {
		m_arr[m_len++] = t;
		return true;
	}
	return false;
}

bool Solid::endian() const
{
	int x = 1;
	char *p = (char *) &x;
	return *p == 1;
}

template<typename T>
void Solid::swapEndian(T *p) const
{
	T temp = 0;
	char *bp = (char *) &temp;
	for (size_t i = 0; i < sizeof(T); ++i) {
		char b = *((char *) p + i);
		bp[sizeof(T) - i - 1] = b;
	}
	memcpy(p, &temp, sizeof(T));
}

void Solid::genDisplayList()
{
	// Don't create new arrays if no triangles in solid
	if (m_max <= 0) {
		glDeleteLists(m_index, 1);
		m_index = 0;
		return;
	}

	// Construct new vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	m_vertex = new GLdouble[m_max * 9];
	for (uint32_t i = 0; i < m_max; ++i) { // Each triangle
		Triangle &t = m_arr[i];
		for (uint32_t j = 0; j < 3; ++j) { // Each vertex
			Vector3 v = t.getVertex(j);
			uint32_t p = i * 9 + j * 3;
			m_vertex[p] = v.x;
			m_vertex[p + 1] = v.y;
			m_vertex[p + 2] = v.z;
		}
	}
	glVertexPointer(3, GL_DOUBLE, 0, m_vertex);

	// Construct new normal array
	if (m_light) {
		glEnableClientState(GL_NORMAL_ARRAY);
		m_norm = new GLdouble[m_max * 9];
		for (uint32_t i = 0; i < m_max; ++i) { // Each triangle
			Vector3 v = m_arr[i].getNormal();
			for (uint32_t j = 0; j < 3; ++j) { // Each vertex
				uint32_t p = i * 9 + j * 3;
				m_norm[p] = v.x;
				m_norm[p + 1] = v.y;
				m_norm[p + 2] = v.z;
			}
		}
		glNormalPointer(GL_DOUBLE, 0, m_norm);
	} else {
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	// Delete old list if exists
	glDeleteLists(m_index, 1);

	// Create new list
	m_index = glGenLists(1);
	glNewList(m_index, GL_COMPILE);
		glColor3f(1.f, 1.f, 1.f); // TODO: Add option to change default color
		glDrawArrays(GL_TRIANGLES, 0, m_max * 3);
	glEndList();

	// Disable & clear arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	if (m_light) glDisableClientState(GL_NORMAL_ARRAY);

	delete[] m_vertex; // OpenGL stores vertex data, we can freely delete these
	delete[] m_norm;
	m_vertex = m_norm  = nullptr;
}
