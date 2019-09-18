#include <cmath>
#include <cstring>
#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include "camera.hpp"
#define PI 3.1415926535
#define deg(x) (x * 180.f / PI)

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
{
	GLdouble *q = m_rotMatrix;
	memset(q, 0.0, sizeof(GLdouble) * 16);
	q[0] = 1;
	q[5] = 1;
	q[10] = 1;
	q[15] = 1;
}

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

void Camera::rotateSolid(Vector3 v, double w)
{
	Vector3 v2 = Vector3(v.x * v.x, v.y * v.y, v.z * v.z);
	double xy = v.x * v.y;
	double xz = v.x * v.z;
	double yz = v.y * v.z;
	double wx = w * v.x;
	double wy = w * v.y;
	double wz = w * v.z;

	// Convert quaternion to rotation matrix
	GLdouble q[16];
	memset(q, 0.0, sizeof(GLdouble) * 16);
	q[0] = 1 - 2 * v2.y - 2 * v2.z;
	q[1] = 2 * xy + 2 * wz;
	q[2] = 2 * xz - 2 * wy;

	q[4] = 2 * xy - 2 * wz;
	q[5] = 1 - 2 * v2.x - 2 * v2.z;
	q[6] = 2 * yz + 2 * wx;

	q[8] = 2 * xz + 2 * wy;
	q[9] = 2 * yz - 2 * wx;
	q[10] = 1 - 2 * v2.x - 2 * v2.y;

	// Multiply previous and new matrices
	GLdouble *l = m_rotMatrix;
	l[0] = l[0] * q[0] + l[1] * q[4] + l[2] * q[8];
	l[1] = l[0] * q[1] + l[1] * q[5] + l[2] * q[9];
	l[2] = l[0] * q[2] + l[1] * q[6] + l[2] * q[10];
	l[4] = l[4] * q[0] + l[5] * q[4] + l[6] * q[8];
	l[5] = l[4] * q[1] + l[5] * q[5] + l[6] * q[9];
	l[6] = l[4] * q[2] + l[5] * q[6] + l[6] * q[10];
	l[8] = l[8] * q[0] + l[9] * q[4] + l[10] * q[8];
	l[9] = l[8] * q[1] + l[9] * q[5] + l[10] * q[9];
	l[10] = l[8] * q[2] + l[9] * q[6] + l[10] * q[10];
}

void Camera::render(const Solid& s) const
{
	glPushMatrix();

	// Position lights
	GLfloat pos[] = { 0, 0, 1, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	// Viewing transformation
	gluLookAt(
		m_pos.x, m_pos.y, m_pos.z, // Camera position
		0, 0, 0, // Direction
		m_up.x, m_up.y, m_up.z // Up vector
	);

	// Model transformations
	Vector3 c = s.getCenter();
	glMultMatrixd(m_rotMatrix);
	glTranslated(-c.x, -c.y, -c.z);

	// Drawing
	glCallList(s.getList());

	glPopMatrix();
}
