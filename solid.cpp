#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <utility>
#include <cstdio>
#include "solid.hpp"

// Default constructor
Solid::Solid()
: m_arr(nullptr)
, m_max(0)
, m_len(0)
{}

// Destructor
Solid::~Solid()
{
	delete[] m_arr;
}

// Copy constructor
Solid::Solid(const Solid& o)
{
	m_max = o.m_max;
	m_len = o.m_len;
	if (o.m_arr) {
		m_arr = new Triangle[m_max];
		memcpy(m_arr, o.m_arr, m_max);
	}
}

// Move constructor
Solid::Solid(Solid&& o) noexcept
: m_arr(std::move(o.m_arr))
, m_max(o.m_max)
, m_len(o.m_len)
{
	o.m_arr = nullptr;
	o.m_max = 0;
	o.m_len = 0;
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

	if (warn) std::cerr << "Warning: File may be corrupt" << std::endl;
	std::cout << "File read OK" << std::endl;
	is.close();
	return true;
}

uint32_t Solid::length() const
{
	return m_len;
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
