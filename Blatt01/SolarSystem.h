#pragma once

#include "Octahedron.h"
#include "GLSLProgram.h"
#include "Orbit.h"
#include "CelestialBodyAxis.h"
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
	Octahedron* moon;

	Orbit* planet_orbit;
	Orbit* moon_orbit;

	CelestialBodyAxis* sun_axis;
	CelestialBodyAxis* planet_axis;


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

	const float moon_radius = 0.3f;
	const unsigned int moon_detail = 4;
	const glm::vec3 moon_position = glm::vec3(planet_position.x + 1.5f, planet_position.y, planet_position.z);
	float rotation_angle = 0;

};

