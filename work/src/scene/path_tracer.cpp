
// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

// std
#include <random>

// project
#include "scene.hpp"
#include "shape.hpp"
#include "light.hpp"
#include "material.hpp"
#include "path_tracer.hpp"


using namespace std;
using namespace glm;


vec3 SimplePathTracer::sampleRay(const Ray &ray, int) {
	// intersect ray with the scene
	RayIntersection intersect = m_scene->intersect(ray);

	// if ray hit something
	if (intersect.m_valid) {

		// simple grey shape shading
		float f = abs(dot(-ray.direction, intersect.m_normal));
		vec3 grey(0.5, 0.5, 0.5);
		return mix(grey / 2.0f, grey, f);
	}

	// no intersection - return background color
	return { 0.3f, 0.3f, 0.4f };
}



vec3 CorePathTracer::sampleRay(const Ray &ray, int) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Implement a PathTracer that calculates the ambient, diffuse
	// and specular, for the given ray in the scene, using the 
	// Phong lighting model. Give special consideration to objects
	// that occluded from direct lighting (shadow rays). You do
	// not need to use the depth argument for this implementation.
	//-------------------------------------------------------------

	// Abby again... I had trouble with this section
	//tried to compare line by line to see where i went wrong so it's slightly different
	//original borked code here https://github.com/alexwelsby/cgra_assign4/blob/59f6c3b421a8cbe1e82dfa125c3c64588353d6bd/work/src/scene/path_tracer.cpp#L40
	RayIntersection intersect = m_scene->intersect(ray);
	if (intersect.m_valid) {
		vec3 colour = vec3(0);
		for (const auto& light : m_scene->lights()) {
			vec3 irradiance = light->irradiance(intersect.m_position);
			vec3 ambience = light->ambience();
			vec3 incidentDir = light->incidentDirection(intersect.m_position);

			vec3 i_diffuse = intersect.m_material->diffuse();
			vec3 i_normal = intersect.m_normal;
			vec3 i_spec = intersect.m_material->specular();
			float i_shininess = intersect.m_material->shininess();

			vec3 diffuseReflec = vec3(0);
			vec3 specReflec = vec3(0);

			vec3 diffuseElem = light->ambience() * i_diffuse;

			if (!light->occluded(m_scene, intersect.m_position + (i_normal * 0.0001f))) {

				float angle = dot(i_normal, normalize(-incidentDir));
				if (angle >= 0) {
					diffuseReflec = irradiance * angle * i_diffuse;
				}

				vec3 reflec = reflect(incidentDir, i_normal);
				angle = dot(normalize(reflec), normalize(-ray.direction));
				if (angle >= 0) {
					specReflec = irradiance * pow(angle, i_shininess) * i_spec;
				}
			}

			colour += diffuseElem + diffuseReflec + specReflec;
		}
		return colour;
	}
	// no intersection - return background color
	return { 0.3f, 0.3f, 0.4f };
}



vec3 CompletionPathTracer::sampleRay(const Ray &ray, int depth) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Using the same requirements for the CorePathTracer add in 
	// a recursive element to calculate perfect specular reflection.
	// That is compute the reflection ray off your intersection and
	// sample a ray in that direction, using the result to additionally
	// light your object. To make this more realistic you may weight
	// the incoming light by the (1 - (1/shininess)).
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...
	float epsilon = 1e05f;
	RayIntersection intersect = m_scene->intersect(ray);
	if (intersect.m_valid) {
		vec3 color = vec3(0);

		vec3 i_diffuse = intersect.m_material->diffuse();
		vec3 i_normal = intersect.m_normal;
		vec3 i_spec = intersect.m_material->specular();
		float i_shininess = intersect.m_material->shininess();

		for (const auto& light : m_scene->lights()) {
			vec3 irradiance = light->irradiance(intersect.m_position);
			vec3 ambience = light->ambience();
			vec3 incidentDir = light->incidentDirection(intersect.m_position);

			vec3 diffuseReflec = vec3(0);
			vec3 specReflec = vec3(0);

			vec3 diffuseElem = light->ambience() * i_diffuse;

			if (!light->occluded(m_scene, intersect.m_position + (i_normal * 0.0001f))) {

				float angle = dot(i_normal, normalize(-incidentDir));
				if (angle >= 0) {
					diffuseReflec = irradiance * angle * i_diffuse;
				}
				//call the reflect function with the surface’s normal and the original ray’s direction
				vec3 reflec = reflect(i_normal, ray.direction);
				Ray reflectRay;
				//The ray’s origin is of course the point where the primary ray hit the surface.
				reflectRay.origin = reflec + 0.00001f;
				reflectRay.direction = reflec;
				float reflectivity = 1.0f - (1.0f / i_shininess);
				//cout << reflectivity << endl;
				if (depth > 0) {
					specReflec += (sampleRay(reflectRay, depth - 1) * reflectivity);
				}
				else {
					if (angle >= 0) {
						specReflec += irradiance * pow(angle, i_shininess) * i_spec;
					}
					
				}
			}
			
			color += diffuseElem + diffuseReflec + specReflec;
		}
		//now for recursive reflections...
		
		//vec3 reflectVec = reflect(ray.direction, i_normal);
		

		return color;
	}
	// no intersection - return background color
	return { 0.3f, 0.3f, 0.4f };
}



vec3 ChallengePathTracer::sampleRay(const Ray &ray, int depth) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Implement a PathTracer that calculates the diffuse and 
	// specular, for the given ray in the scene, using the 
	// Phong lighting model. Give special consideration to objects
	// that occluded from direct lighting (shadow rays).
	// Implement support for textured materials (using a texture
	// for the diffuse portion of the material).
	//
	// EXTRA FOR EXPERTS :
	// Additionally implement indirect diffuse and specular instead
	// of using the ambient lighting term.
	// The diffuse is sampled from the surface hemisphere and the
	// specular is sampled from a cone of the phong lobe (which
	// gives a glossy look). For best results you need to normalize
	// the lighting (see http://www.thetenthplanet.de/archives/255)
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	// no intersection - return background color
	return { 0.3f, 0.3f, 0.4f };
}