#pragma once

#include <glm/glm.hpp>
#include "GLSLProgram.h"
#include "Object.h"

class Orbit: public Object
{

public:

	Orbit(cg::GLSLProgram& program, glm::mat4x4& view,
		glm::mat4x4& projection, float radius, glm::vec3 center, unsigned int numPoints);

	~Orbit();


	void init();

	void draw();
private:

	unsigned int numPoints;
	glm::vec3 center;
	float radius;

	cg::GLSLProgram& program;
	glm::mat4x4& view;
	glm::mat4x4& projection;
protected:
};

