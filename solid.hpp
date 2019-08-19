#ifndef SOLID_HPP
#define SOLID_HPP
#include <string>
#include <cstdint>
#include "triangle.hpp"

class Solid {
public:
	Solid(); 							// Default constructor
	~Solid(); 							// Destructor
	Solid(const Solid&); 				// Copy constructor
	Solid(Solid&&) noexcept; 			// Move constructor

	Solid& operator=(const Solid&); 	// Copy assignment
	Solid& operator=(Solid&&) noexcept; // Move assignment

	/** Construct a new solid from a given `.stl` file
	 * @param Filename
	 * @return True on success, false otherwise
	*/
	bool readFile(std::string);

	/** Get the number of triangles that make up the solid.
	 * @return Triangle count
	*/
	uint32_t length() const;

private:
	/** Append a triangle to the solid
	 * @param Triangle to append
	 * @bool True on success, false otherwise
	*/
	bool append(const Triangle&);

	/** Get machine endianness
	 * @return True if little-endian, false if big-endian
	*/
	bool endian() const;

	/** Swap memory endianess.
	 * @param p Pointer to the memory to be swapped
	*/
	template<typename T>
	void swapEndian(T *) const;

	// Instance variables
	Triangle *m_arr;
	uint32_t m_max;
	uint32_t m_len;
};

#endif
