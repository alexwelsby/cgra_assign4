
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

	// YOUR CODE GOES HERE
	// ...
	RayIntersection intersect = m_scene->intersect(ray);

	// if ray hit something
	if (intersect.m_valid) {

		
		
		float diffuseStrength = 0.5;
		float shiniStrength = 1.0;

		vec3 fragPos = intersect.m_position;
		
		
		vec3 objectColor(0.5, 0.5, 0.5);
		vec3 ambientColor = vec3(0.1, 0.0, 0.0);
		const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
		const vec3 specColor = vec3(1.0, 1.0, 1.0);
		float shininess = 16.0;
		vec3 result = vec3(0.0f);

		vec3 norm = normalize(intersect.m_normal);
		for (const auto& light : m_scene->lights()) {
			vec3 shadowOrigin = fragPos + 1e-4f * norm;
			if (!light->occluded(m_scene, shadowOrigin)) {
				vec3 lightColor = light->ambience();
				vec3 lightPower = light->irradiance(shadowOrigin);
				vec3 lightDir = light->incidentDirection(shadowOrigin);
				
				float distance = glm::dot(lightDir, lightDir);

				float lambertian = glm::max(glm::dot(lightDir, norm), 0.0f);
				float specular = 0.0;

				if (lambertian > 0.0) {
					vec3 viewDir = normalize(-ray.direction);

					//blinn phong

					vec3 halfDir = normalize(lightDir + viewDir);
					float specAngle = glm::max(glm::dot(halfDir, norm), 0.0f);
					specular = glm::pow(specAngle, shininess);
				}

				result = ambientColor + diffuseColor * lambertian * lightColor * lightPower / distance + specColor * specular * lightColor * lightPower / distance;
				
				
				
				
				
				
				
				
				
				
				
				
				
			}
		}
		return result;
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