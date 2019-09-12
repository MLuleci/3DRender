#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "solid.hpp"
#include "vector3.hpp"

class Camera {
public:
	Camera();

	/** Change the camera's position.
	 * @param p New position
	*/
	void setPos(Vector3);

	/** Get the camera's position.
	 * @return Position vector
	*/
	Vector3 getPos() const;

	/** Set the camera's direction as a vector instead of angles.
	 * @param d Direction vector, doesn't have to be normalized
	*/
	void setDir(Vector3);

	/** Get the camera's direction.
	 * @return Direction vector
	*/
	Vector3 getDir() const;

	/** Get the camera's up vector.
	 * @return Up vector
	*/
	Vector3 getUp() const;

	/** Set camera yaw.
	 * @param a New angle, in radians
	*/
	void setYaw(double);

	/** Get camera yaw.
	 * @return The angle m_dir makes with the z-axis
	*/
	double getYaw() const;

	/** Set camera pitch.
	 * @param a New angle, in radians
	*/
	void setPitch(double);

	/** Get camera pitch.
	 * @return The angle m_dir makes with the xz-plane
	*/
	double getPitch() const;

	/** Set camera roll.
	 * @param a New angle, in radians
	*/
	void setRoll(double);

	/** Get camera roll.
	 * @return The angle m_up makes with the y-axis
	*/
	double getRoll() const;

	/** Set camera field of view.
	 * @param a New angle in range [0, 180]
	*/
	void setFov(double);

	/** Get camera field of view.
	 * @return FoV, in degrees
	*/
	double getFov() const;

	/** Set aspect ratio.
	 * @param r Screen width / height
	*/
	void setRatio(double);

	/** Get aspect ratio.
	 * @return The current aspect ratio
	*/
	double getRatio() const;

	/** Toggle between orthographic and perspective projections.
	*/
	void toggleProj();

	/** Call to the appropriate routing to set up the projection(s).
	*/
	void setupProj() const;

	/** Set far clipping plane.
	 * @param f Far clipping plane
	*/
	void setFarClip(double);

	/** Set near clipping plane.
	 * param n Near clipping plane
	*/
	void setNearClip(double);

	/** Set projection depth clipping planes.
	 * @param f Far clipping plane
	 * @param n Near clipping plane
	*/
	void setClipping(double, double);

	/** Get far clipping plane.
	 * @return Far clip plane value
	*/
	double getFarClip() const;

	/** Get near clipping plane.
	 * @return Near clip plane value
	*/
	double getNearClip() const;

	/** Render the current scene the camera sees.
	 * @param s The solid to be rendered
	*/
	void render(const Solid&) const;

private:
	Vector3 m_pos;		// Camera position
	Vector3 m_dir; 		// Direction the camera is facing
	Vector3 m_up; 		// Vector pointing "up"
	double m_fov; 		// Field of view
	double m_ratio; 	// Aspect ratio
	double m_far;		// Far clipping plane
	double m_near;		// Near clipping plane
	bool m_persp;		// Projection type toggle
};

#endif
