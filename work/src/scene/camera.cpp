
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

	// YOUR CODE GOES HERE
	// ...
	float aspect = m_image_size.x / m_image_size.y;
	float tan_theta = tan(m_fovy / 2);
	//PR=(tanθ, 0, –1)
	vec3 Pr = vec3(tan_theta, 0, 1);
	//PL=(–tanθ, 0, –1)
	vec3 Pl = vec3(-tan_theta, 0, 1);
	//cout << "HEYYY" << m_image_size.x << "," << m_image_size.y << endl;
	//Pi = PL + ( i + 0.5 )/N × ( PR – PL )
	vec2 Pi_x = Pl + (pixel.x + 0.5f) / m_image_size.x * (Pr - Pl);

	//now for vertical>??
	Pr = vec3(0, tan_theta, 1);
	Pl = vec3(0, -tan_theta, 1);
	vec2 Pi_y = Pl + (pixel.y + 0.5f) / m_image_size.y * (Pr - Pl);

	vec3 Pi = vec3(Pi_x.x, Pi_y.y, -1.0f);

	vec3 direction_view = normalize(Pi);
	vec3 direction_world = normalize(vec3(m_rotation * vec4(direction_view, 0.0f)));

	//ray origin=camera origin
	ray.origin = m_position;
	ray.direction = direction_world;
	return ray;
}
