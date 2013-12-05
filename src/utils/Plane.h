/**
 * @file Plane.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>
#include <glm/gtc/swizzle.hpp>

/**
 * Plane in 3D space.
 * Plane is defined via equation Ax + By + Cz + D = 0 where
 * A, B, C is plane normal vector and D is distance from origin
 */
class Plane
{
public:
	/// Creates plane with all coefficients zero
	Plane() : m_coefs(0.0f) { }
	/// Creates plane with given coefficients
	Plane(const glm::vec4& coefs) : m_coefs(coefs) { }
	Plane(float a, float b, float c, float d) : m_coefs(a, b, c, d) { }
	Plane(float* p) : m_coefs(p[0], p[1], p[2], p[3]) { }

	static Plane fromPoints(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) {
		auto v = p1 - p0;
		auto u = p2 - p0;
		auto n = glm::cross(v, u);
		n = glm::normalize(n);
		return Plane(n.x, n.y, n.z, glm::dot(-n, p0));
	}

	static Plane fromNormalAndPoint(const glm::vec3& normal, const glm::vec3& p) {
		auto n = glm::normalize(normal);
		return Plane(n.x, n.y, n.z, glm::dot(-n, p));
	}

	static Plane fromCoeficients(const glm::vec4& coefs) {
		return Plane(glm::normalize(coefs));
	}

	// Creates plane from equation coefficients and normalize it
	static Plane fromCoeficients(float a, float b, float c, float d) {
		return fromCoeficients(glm::vec4(a, b, c, d));
	}

	/// Computes distance of p from this plane. Sign of result indicates
	/// on which side of plane point is.
	float distance(const glm::vec3& p) const {
		return glm::dot(normal(), p) + m_coefs.w;
	}

	glm::vec3 normal() const {
		return glm::swizzle<glm::X, glm::Y, glm::Z>(m_coefs);
	}
private:
	glm::vec4 m_coefs;
};

#endif // PLANE_H
