#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "solid.hpp"
#include "vector3.hpp"

class Camera {
public:
	Camera(unsigned int, unsigned int);

	/** Change the camera's position.
	 * @param p New position
	*/
	void setPos(Vector3);

	/** Get the camera's position.
	 * @return Position vector
	*/
	Vector3 getPos() const;

	/** Change the camera's direction.
	 * @param d Direction unit vector
	*/
	void setDir(Vector3);

	/** Change the camera's angle(s).
	 * @param yaw Yaw angle in radians
	 * @param pit Pitch angle in radians
	*/
	void setDir(double, double);

	/** Get the camera's direction.
	 * @return Direction unit vector
	*/
	Vector3 getDir() const;

	/** Set the camera plane's aspect ratio.
	 * @param w Width
	 * @param h Height
	*/
	void setAspectRatio(unsigned int, unsigned int);

	/** Get the camera plane's aspect ratio.
	 * @return Ratio = width / height
	*/
	double getAspectRatio() const;

	/** Set the camera's focal length.
	 * @param f New length
	*/
	void setFocalLen(double);

	/** Get the focal length.
	 * @return Distance between camera plane and camera
	*/
	double getFocalLen() const;

	/** Set camera roll.
	 * @param r Roll angle in radians
	*/
	void setRoll(double);

	/** Render the current scene the camera sees.
	 * @param s The solid to be rendered
	*/
	void render(const Solid&) const;

private:
	Vector3 m_pos;
	Vector3 m_dir;
	Vector3 m_pW, m_pH; // Camera plane
	unsigned int m_width, m_height; // Screen dimensions (used w/ camera plane)
	double m_flen; // Focal length
};

#endif
