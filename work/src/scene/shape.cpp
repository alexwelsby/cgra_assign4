﻿
// std
#include <algorithm>
#include <utility>
#include <iostream>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// project
#include "shape.hpp"


using namespace glm;


RayIntersection AABB::intersect(const Ray &ray) {
	RayIntersection intersect;
	vec3 rel_origin = ray.origin - m_center;

	// start magic
	// x
	float rdx_inv = 1 / ray.direction.x;
	float tx1 = (-m_halfsize.x - rel_origin.x) * rdx_inv;
	float tx2 = (m_halfsize.x - rel_origin.x) * rdx_inv;

	float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);

	// y
	float rdy_inv = 1 / ray.direction.y;
	float ty1 = (-m_halfsize.y - rel_origin.y) * rdy_inv;
	float ty2 = (m_halfsize.y - rel_origin.y) * rdy_inv;

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	// z
	float rdz_inv = 1 / ray.direction.z;
	float tz1 = (-m_halfsize.z - rel_origin.z) * rdz_inv;
	float tz2 = (m_halfsize.z - rel_origin.z) * rdz_inv;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));

	if (tmax < tmin) return intersect;
	// end magic

	intersect.m_distance = tmin < 0 ? tmax : tmin;
	intersect.m_position = ray.origin + intersect.m_distance * ray.direction;
	intersect.m_valid = tmax >= 0;
	vec3 work_out_a_name_for_it_later = abs((intersect.m_position - m_center) / m_halfsize);
	float max_v = std::max(work_out_a_name_for_it_later[0], std::max(work_out_a_name_for_it_later[1], work_out_a_name_for_it_later[2]));
	intersect.m_normal = normalize(mix(intersect.m_position - m_center, vec3(0), lessThan(work_out_a_name_for_it_later, vec3(max_v))));
	intersect.m_uv_coord = (abs(intersect.m_normal.x) > 0) ?
		vec2(intersect.m_position.y, intersect.m_position.z) :
		vec2(intersect.m_position.x, intersect.m_position.y + intersect.m_position.z);
	intersect.m_shape = this;

	return intersect;
}


RayIntersection Sphere::intersect(const Ray &ray) {
	RayIntersection intersect;
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Implement the intersection method for Sphere that returns
	// a RayIntersection object with valid == false if the 
	// ray doesn't intersect and true otherwise. If true, then
	// remember to fill out each feild in the object correctly:
	// - m_valid : true if object is itersected
	// - m_distance : distance along the ray of the intersection
	// - m_position : position on the surface of the intersection
	// - m_normal : normal on the surface of the intersection
	// - m_shape : set to "this"
	// - m_uv_coord : texture coordinates (challenge only)
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	vec3 O = ray.origin;
	vec3 D = ray.direction;

	vec3 C = m_center; //the center of this sphere

	float R = m_radius; 
	//now for some quadratic nonsense
	float a = glm::dot(D, D);  //a = D DOT D
	float b = 2.0f * glm::dot(O - C, D); //b = 2[O − C] DOT D
	float c = (glm::dot(O - C, O - C)) - (R * R); //c = [O − C] DOT [O − C] − R^2
	float discriminant = (b * b) - (4 * a * c); //(b^2)-4ac
	if (discriminant < 0) {
		return intersect;
	}
	else if (discriminant >= 0) { //we have two real solutions
		float t1 = (-b + sqrt(discriminant)) / (2 * a);
		float t2 = (-b - sqrt(discriminant)) / (2 * a);
		float t = 0.0f;
		float epsilon = 1e-4f;
		if (t1 > epsilon && t2 > epsilon) {
			t = glm::min(t1, t2);
		}
		else if (t1 == t2) {
			t = t1;
		}
		else if (t1 < epsilon && t2 > epsilon) {
			t = t2;
		}
		else {
			return intersect;
		}

			
		vec3 P = O + t * D;
		vec3 N = glm::normalize(P - C);
		
		intersect.m_valid = true;
		intersect.m_distance = t;
		intersect.m_position = P;
		intersect.m_normal = N;
		intersect.m_shape = this;
		intersect.m_uv_coord = (abs(intersect.m_normal.x) > 0) ?
			vec2(intersect.m_position.y, intersect.m_position.z) :
			vec2(intersect.m_position.x, intersect.m_position.y + intersect.m_position.z);
		return intersect;
	}
	
	return intersect;
}

