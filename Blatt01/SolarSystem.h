#pragma once

#include "Octahedron.h"
#include "GLSLProgram.h"
#include "Orbit.h"
#include <glm/glm.hpp>

class SolarSystem
{

public:

	SolarSystem(cg::GLSLProgram& program, glm::mat4x4& view, glm::mat4x4& projection);

	~SolarSystem();

	void init();

	void draw();

	void increasePlanetRotationSpeed();

	void decreasePlanetRotationSpeed();

	void upY();

	void downY();

	void setStopAnimation(bool stop);

	bool getStopAnimation();


private:
protected:

	Octahedron* sun;
	Octahedron* planet;
	Orbit* planet_orbit;

	cg::GLSLProgram& program;
	glm::mat4x4& view;
	glm::mat4x4& projection;

	// planet rotation speed
	float planet_rotation_speed = 1.0f;

	bool stopAnimation = false;


	// Constants
	const float sun_radius = 3.0f;
	const unsigned int sun_detail = 4;
	const glm::vec3 sun_position = glm::vec3(0.0f, 0.0f, 0.0f);

	const float planet_radius = 1.0f;
	const unsigned int planet_detail = 4;
	const float planet_z_tilt = 45.0f;
	const glm::vec3 planet_position = glm::vec3(8.0f, 0.0f, 0.0f);


};

