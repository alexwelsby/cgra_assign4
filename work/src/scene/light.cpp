
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

	// from Abby

	vec3 reverse = -incidentDirection(point);
	Ray ray = Ray(point, reverse);
	RayIntersection intersect = scene->intersect(ray);
	return intersect.m_valid;
}


vec3 DirectionalLight::incidentDirection(const vec3 &) const {
	return m_direction;
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

	Ray shadow_ray;
	vec3 to_light = -incidentDirection(point);
	float dist_to_light = glm::length(to_light);
	const float epsilon = 1e-4f;
	shadow_ray.origin = point + epsilon * to_light;
	shadow_ray.direction = normalize(to_light);

	RayIntersection intersect = scene->intersect(shadow_ray);
		if (intersect.m_valid && intersect.m_distance < dist_to_light) {
			return true; //blocked light
		}
	return false; //no occlusion
}


vec3 PointLight::incidentDirection(const vec3 &point) const {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Return the direction of the incoming light (light to point)
	//-------------------------------------------------------------

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

	// Also referenced from Abby

	vec3 to_light = -incidentDirection(point);
	const float epsilon = 1e-4f;
	float dist_to_light = glm::length(to_light);

	if (dist_to_light > 0) {
		//we divide the total energy of the light by 4π and the squared distance to the point light.
		return m_flux / (4 * glm::pi<float>() * dist_to_light * dist_to_light);
	}
	return m_flux;
}
