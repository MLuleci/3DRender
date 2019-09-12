#include <cmath>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include "camera.hpp"
#define PI 3.1415926535

/**
 * Position at origin
 * Direction points at negative z-axis
 * Up vector points at positive y-axis
 * FoV & ratio are 0, need to be initialized
 * Far & near clips are 1 & 0, need to be initialized
 * Initial projection is perspective
*/
Camera::Camera()
: m_pos()
, m_dir(0, 0, -1)
, m_up(0, 1, 0)
, m_fov(0)
, m_ratio(0)
, m_far(1)
, m_near(0)
, m_persp(true)
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
	m_dir = d.norm();
}

Vector3 Camera::getDir() const
{
	return m_dir;
}

Vector3 Camera::getUp() const
{
	return m_up;
}

void Camera::setYaw(double a)
{
	m_dir = m_dir.rotate(0, a, 0);
}

double Camera::getYaw() const
{
	return std::asin(m_dir.x / std::sqrt(std::pow(m_dir.x, 2) + std::pow(m_dir.z, 2)));
}

void Camera::setPitch(double a)
{
	m_dir = m_dir.rotate(a, 0, 0);
}

double Camera::getPitch() const
{
	Vector3 y(0, 1, 0);
	return (PI / 2.0) - m_dir.angle(y);
}

void Camera::setRoll(double a)
{
	double c = std::cos(a);
	double s = std::sin(a);
	m_up = m_up * c + (m_dir.cross(m_up)) * s + m_dir * (m_dir.dot(m_up)) * (1.0 - c);
}

double Camera::getRoll() const
{
	Vector3 y(0, 1, 0);
	return m_up.angle(y);
}

void Camera::setFov(double a)
{
	if (0.0 <= a && a <= 180.0) {
		m_fov = a;
		setupProj();
	}
}

double Camera::getFov() const
{
	return m_fov;
}

void Camera::setRatio(double r)
{
	m_ratio = r;
	setupProj();
}

double Camera::getRatio() const
{
	return m_ratio;
}

void Camera::toggleProj()
{
	m_persp = !m_persp;
	setupProj();
}

void Camera::setupProj() const
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (m_persp) {
		gluPerspective(m_fov, m_ratio, m_near, m_far);
	} else {
		double h = glutGet(GLUT_WINDOW_HEIGHT);
		double w = glutGet(GLUT_WINDOW_WIDTH);
		glOrtho(-w/2, w/2, -h/2, h/2, m_near, m_far);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Camera::setFarClip(double f)
{
	setClipping(f, m_near);
}

void Camera::setNearClip(double n)
{
	setClipping(m_far, n);
}

void Camera::setClipping(double f, double n)
{
	m_far = (f >= 0 ? f : m_far);
	m_near = (n >= 0 ? n : m_near);
	setupProj();
}

double Camera::getFarClip() const
{
	return m_far;
}

double Camera::getNearClip() const
{
	return m_near;
}

void Camera::render(const Solid& s) const
{
	glPushMatrix();

	// Viewing transformation
	gluLookAt(
		m_pos.x, m_pos.y, m_pos.z, // Camera position
		m_dir.x, m_dir.y, m_dir.z, // Direction
		m_up.x, m_up.y, m_up.z // Up vector
	);

	// Drawing
	glCallList(s.getList());

	glPopMatrix();
}
