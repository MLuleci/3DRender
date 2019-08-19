#include <cmath>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "camera.hpp"
#define PI 3.1415926535

/* Position starts at point P(0, 0, 0)
 * Direction starts pointing towards the X-axis D(1, 0, 0)
 * Plane width & height have D as their cross product W(0, 1, 0) & H(0, 0, 1)
 * Initial screen dimensions must be given
 * Focal length is such that the angle of view is 45*, f = w / (2 * tan(PI / 8))
*/
Camera::Camera(unsigned int w, unsigned int h)
: m_pos()
, m_dir(1, 0, 0)
, m_pW(0, 1, 0)
, m_pH(0, 0, 1)
, m_width(w)
, m_height(h)
, m_flen((double) w / (2.0 * std::tan(PI / 8)))
{}

void Camera::setPos(Vector3 p)
{
	m_pos = p;
}

Vector3 Camera::getPos() const
{
	return m_pos;
}

void Camera::setDir(Vector3 d)
{
	// Get rotation around Z-axis & use it to rotate m_pW
	double r = std::atan2(d.y, d.x) - std::atan2(m_dir.y, m_dir.x);
	m_pW = Vector3(m_pW.x * std::cos(r) - m_pW.y * std::sin(r), m_pW.x * std::sin(r) + m_pW.y * std::cos(r), m_pW.z);

	// Set new position
	m_dir = d;

	// Set m_pH to the cross product of m_dir and m_pW
	m_pH = -m_dir.cross(m_pW).norm();
}

void Camera::setDir(double yaw, double pit)
{
	double x = std::cos(yaw);
	double y = std::sin(yaw);
	double z = std::sin(pit);
	setDir(Vector3(x, y, z));
}

Vector3 Camera::getDir() const
{
	return m_dir;
}

void Camera::setAspectRatio(unsigned int w, unsigned int h)
{
	m_width = w;
	m_height = h;
}

double Camera::getAspectRatio() const
{
	return (m_height ? m_width / m_height : 0);
}

void Camera::setFocalLen(double f)
{
	m_flen = (f <= 0 ? 0 : f);
}

double Camera::getFocalLen() const
{
	return m_flen;
}

void Camera::setRoll(double r)
{
	// Rotate m_pH around m_dir (using Rodrigues' rotation formula)
	m_pH = m_pH * std::cos(r) + (-m_dir.cross(m_pH)) * std::sin(r) + -m_dir * (-m_dir.dot(m_pH)) * (1 - std::cos(r));

	// Set m_pW to the cross product of m_dir and m_pH
	m_pW = m_pH.cross(-m_dir);
}

void Camera::render(const Solid& s) const
{
	// Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// TODO: Render solid

	// Update screen buffer
	glutSwapBuffers();
}
