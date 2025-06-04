
#include <iostream>

// glm
#include <glm/gtc/matrix_transform.hpp>

// project
#include "camera.hpp"
#include "opengl.hpp"


using namespace std;
using namespace glm;


void Camera::setPositionOrientation(const vec3 &pos, float yaw, float pitch) {
	m_position = pos;
	m_yaw = yaw;
	m_pitch = pitch;

	// update rotation matrix (based on yaw and pitch)
	m_rotation = rotate(mat4(1), m_yaw, vec3(0, 1, 0)) * rotate(mat4(1), m_pitch, vec3(1, 0, 0));
}


Ray Camera::generateRay(const vec2 &pixel) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Generate a ray in the scene using the camera position,
	// rotation field of view on the y axis (fovy) and the image
	// size. The pixel is given in image coordinates [0, imagesize]
	// This COULD be done by first creating the ray in ViewSpace
	// then transforming it by the position and rotation to get
	// it into worldspace.
	//-------------------------------------------------------------
	
	Ray ray;

    vec2 ndc;
    ndc.x = (pixel.x + 0.5) / m_image_size.x;
    ndc.y = (pixel.y + 0.5) / m_image_size.y;
    vec2 pixS;
    pixS.x = 2 * ndc.x - 1;
    pixS.y = 2 * ndc.y - 1;
    float aspect = m_image_size.x / m_image_size.y;
    vec2 pixC;
    pixC.x = pixS.x * aspect * tan(m_fovy / 2);
    pixC.y = pixS.y * tan(m_fovy / 2);
    vec3 camS = vec3(pixC.x, pixC.y, -1);
    vec4 whoa = vec4(camS, 0);
    whoa = m_rotation * whoa;
    ray.origin = m_position;
    vec3 rayDirection = vec3(whoa.x, whoa.y, whoa.z) - ray.origin;
    ray.direction = normalize(rayDirection);

    return ray;
}
