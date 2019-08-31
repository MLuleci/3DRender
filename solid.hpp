#ifndef SOLID_HPP
#define SOLID_HPP
#include <string>
#include <cstdint>
#include <GL/glew.h>
#include "triangle.hpp"

class Solid {
public:
	Solid(); 							// Default constructor
	~Solid(); 							// Destructor
	Solid(const Solid&); 				// Copy constructor
	Solid(Solid&&) noexcept; 			// Move constructor

	Solid& operator=(const Solid&); 	// Copy assignment
	Solid& operator=(Solid&&) noexcept; // Move assignment

	/** Construct a new solid from a given `.stl` file.
	 * @param Filename
	 * @return True on success, false otherwise
	*/
	bool readFile(std::string);

	/** Toggle lighting on and off. This determines wether or not
	 * normal vectors are included in the display list.
	 * After a call to this method, getList() must follow.
	*/
	void toggleLight();

	/** Get the display list index.
	 * @return GLuint display list index
	*/
	const GLuint getList() const;

	/** Get the radius of a rough spehere that bounds the solid.
	 * @return Distance between minimum and maximum points
	*/
	double getRadius() const;

	/** Get the rough center of the solid.
	 * @return Center of the bounding sphere
	*/
	Vector3 getCenter() const;

private:
	/** Append a triangle to the solid.
	 * @param Triangle to append
	 * @bool True on success, false otherwise
	*/
	bool append(const Triangle&);

	/** Get machine endianness.
	 * @return True if little-endian, false if big-endian
	*/
	bool endian() const;

	/** Swap memory endianess.
	 * @param p Pointer to the memory to be swapped
	*/
	template<typename T>
	void swapEndian(T *) const;

	/** Create a display list using the current parameters.
	 * This is called when a file is read or when lighting is toggled.
	*/
	void genDisplayList();

	// Instance variables
	Triangle *m_arr;
	uint32_t m_max;
	uint32_t m_len;
	bool m_light;
	Vector3 m_upper;
	Vector3 m_lower;
	GLuint m_index;
	GLfloat *m_vertex;
	GLfloat *m_norm;
};

#endif
