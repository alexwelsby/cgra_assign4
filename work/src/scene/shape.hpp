
#pragma once

// glm
#include <glm/glm.hpp>

// project
#include "ray.hpp"
#include "scene.hpp"


class Shape {
public:
	virtual RayIntersection intersect(const Ray &ray) = 0;
};


class AABB : public Shape {
private:
	glm::vec3 m_center;
	glm::vec3 m_halfsize;

public:
	AABB(const glm::vec3 &c, float hs) : m_center(c), m_halfsize(hs) { }
	AABB(const glm::vec3 &c, const glm::vec3 &hs) : m_center(c), m_halfsize(hs) { }
	virtual RayIntersection intersect(const Ray &ray) override;
};


class Sphere : public Shape {
private:
	glm::vec3 m_center;
	float m_radius;

public:
	Sphere(const glm::vec3 &c, float radius) : m_center(c), m_radius(radius) { }
	virtual RayIntersection intersect(const Ray &ray) override;
};

//-------------------------------------------------------------
// [Assignment 4] :
// Implement the following additional Shapes :
// - Plane
// - Disk
// - Triangle
// Follow the pattern shown by AABB and Sphere for implementing
// a class that subclasses Shape making sure that you implement
// the intersect method for each new Shape.
//-------------------------------------------------------------

class Triangle : public Shape {
private:
	glm::vec3 m_center;
	glm::vec3 vertices[3];

public:
	Triangle(const glm::vec3& c, glm::vec3 side_a, glm::vec3 side_b, glm::vec3 side_c) : m_center(c) {
	vertices[0] = side_a;
	vertices[1] = side_b; 
	vertices[2] = side_c;
	}
	virtual RayIntersection intersect(const Ray& ray) override;
};

class Plane : public Shape {
private:
	glm::vec3 m_center;
	float m_radius;

public:
	Plane(const glm::vec3& c, float hs) : m_center(c), m_radius(hs) {}
	virtual RayIntersection intersect(const Ray& ray) override;
};

class Disk : public Shape {
private:
	glm::vec3 m_center;
	float m_radius;

public:
	Disk(const glm::vec3& c, float radius) : m_center(c), m_radius(radius) {}
	virtual RayIntersection intersect(const Ray& ray) override;
};

// YOUR CODE GOES HERE
// ...

