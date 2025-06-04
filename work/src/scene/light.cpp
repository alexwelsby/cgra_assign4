
// glm
#include <glm/gtc/constants.hpp>

// project
#include "light.hpp"

#include "scene_object.hpp"

using namespace glm;


bool DirectionalLight::occluded(Scene *scene, const vec3 &point) const {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Determine whether the given point is being occluded from
	// this directional light by an object in the scene.
	// Remember that directional lights are "infinitely" far away
	// so any object in the way would cause an occlusion.
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	vec3 direc = -incidentDirection(point);
	float len = length(direc);
	if (len > 0) direc = normalize(direc);
	Ray ray = Ray(point, direc);
	RayIntersection intersect = scene->intersect(ray);
	return intersect.m_valid && intersect.m_distance < len;
}


vec3 DirectionalLight::incidentDirection(const vec3 &) const {
	return -glm::normalize(m_direction);
}


vec3 DirectionalLight::irradiance(const vec3 &) const {
	return m_irradiance;
}


bool PointLight::occluded(Scene *scene, const vec3 &point) const {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Determine whether the given point is being occluded from
	// this directional light by an object in the scene.
	// Remember that point lights are somewhere in the scene and
	// an occulsion has to occur somewhere between the light and 
	// the given point.
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	vec3 direc = -incidentDirection(point);
	float len = length(direc);
	if (len > 0) direc = normalize(direc);
	Ray ray = Ray(point, direc);
	RayIntersection intersect = scene->intersect(ray);
	return intersect.m_valid && intersect.m_distance < len;
}


vec3 PointLight::incidentDirection(const vec3 &point) const {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Return the direction of the incoming light (light to point)
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	return point - m_position;
}


vec3 PointLight::irradiance(const vec3 &point) const {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Return the total irradiance on the given point.
	// This requires you to convert the flux of the light into
	// irradiance by dividing it by the surface of the sphere
	// it illuminates. Remember that the surface area increases
	// as the sphere gets bigger, ie. the point is further away.
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	vec3 direc = -incidentDirection(point);
	float len = length(direc);
	if (len > 0) {
		return m_flux / (4.0f * pi<float>() * powf(len, 2));
	}
	return m_flux;
}
