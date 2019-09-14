#include <cmath>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include "camera.hpp"
#define PI 3.1415926535
#define deg(x) (x * 180.f / PI)

/**
 * Position at origin
 * Direction points at negative z-axis
 * Up vector points at positive y-axis
 * FoV & ratio are 0, need to be initialized
 * Far & near clips are 1 & 0, need to be initialized
 * Initial FoV & height of ortho rectangle are 0, need to be initialized
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
, m_ifov(0)
, m_h(0)
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
		double a = m_fov / m_ifov;
		double h = (m_h * a) / 2.f;
		double w = h * m_ratio;
		glOrtho(-w, w, -h, h, m_near, m_far);
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

void Camera::setupOrtho(double h, double f)
{
	m_ifov = (0 <= f && f <= 180 ? f : m_ifov);
	m_h = (h >= 0 ? h : m_h);
}

void Camera::rotateSolid(double x, double y)
{
	m_rotx = std::fmod(m_rotx + x, 2.f * PI);
	m_roty = std::fmod(m_roty + y, 2.f * PI);
}

void Camera::render(const Solid& s) const
{
	glPushMatrix();

	// Viewing transformation
	gluLookAt(
		m_pos.x, m_pos.y, m_pos.z, // Camera position
		0, 0, 0, // Direction
		m_up.x, m_up.y, m_up.z // Up vector
	);

	// Model transformations
	Vector3 c = s.getCenter();
	Vector3 kx = Vector3(0, 1, 0);
	Vector3 ky = m_dir.cross(m_up).norm();
	ky = ky * std::cos(m_rotx) + kx.cross(ky) * std::sin(m_rotx) + kx * kx.dot(ky) * (1.f - std::cos(m_rotx));

	glRotated(deg(m_rotx), kx.x, kx.y, kx.z);
	glRotated(deg(m_roty), ky.x, ky.y, ky.z);
	glTranslated(-c.x, -c.y, -c.z);

	// Drawing
	glCallList(s.getList());

	glPopMatrix();
}
