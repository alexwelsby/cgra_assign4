
// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

// std
#include <random>
#include <cstdlib>
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

		vec3 norm = normalize(intersect.m_normal);
		if (glm::dot(norm, ray.direction) > 0) norm = -norm;
		vec3 fragPos = intersect.m_position;

		vec3 objectColor(0.5, 0.5, 0.5);
		vec3 result = vec3(0.0f);

		Material* mat = intersect.m_material;

		vec3 diffuse_color = mat->diffuse();

		vec3 specular_color = mat->specular();

		float visibility = 0.0f;
		int numShadowRays = 25;

		std::uniform_real_distribution<float> dist(0.0f, 1.0f);
		std::mt19937 rng((time(0)));

		for (const auto& light : m_scene->lights()) {
			//cout << i << endl;
			vec3 shadowPoint = fragPos + norm * 1e-4f;
			vec3 lightColor = light->ambience();
			vec3 lightStrength = light->irradiance(shadowPoint);
			vec3 lightDir = light->incidentDirection(shadowPoint);
			float lightDist = length(lightDir);

			for (int i = 0; i < numShadowRays; ++i) {
				
				vec3 jitter = vec3(
					(dist(rng) - 0.5f) * 0.1f,
					(dist(rng) - 0.5f) * 0.1f,
					(dist(rng) - 0.5f) * 0.1f
				);

				vec3 jitteredDirection = normalize(lightDir + jitter);

				Ray shadowRay(shadowPoint, jitteredDirection);
				RayIntersection shadowHit = m_scene->intersect(shadowRay);

				if (!shadowHit.m_valid || shadowHit.m_distance > lightDist - 1e-3f) {
					visibility += 1.0f;
				}
			}
			visibility /= numShadowRays;
			
			if (visibility > 0.0f) { //if the point of the intersect is not occluded
				
				
				float ambientStrength = 0.1;
				vec3 ambient = ambientStrength * lightColor;
				//cout << intersect.m_normal.x << "," << intersect.m_normal.y << "," << intersect.m_normal.z << endl;
				//lightDir = normalize(-lightDir);

				vec3 diffuse = mat->diffuse();

				float specularStrength = 0.0;
				vec3 reflectDir = reflect(-lightDir, norm);
				vec3 viewDir = normalize(-ray.direction);

				vec3 halfwayDir = normalize(lightDir + viewDir);
				float spec = glm::pow(glm::max(glm::dot(halfwayDir, norm), 0.0f), mat->shininess());
				vec3 specular = spec * specular_color;

				result += (ambient + diffuse + specular) * (lightStrength) * visibility;
				
			}
		}
		//result = pow(result, vec3(1.0f / 2.2f)); //gamma correction i guess
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