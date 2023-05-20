#pragma once

#include <glm/glm.hpp>
#include "GLSLProgram.h"
#include "Object.h"

class CelestialBodyAxis: public Object
{
public:
	CelestialBodyAxis(cg::GLSLProgram& program, glm::mat4x4& view,
		glm::mat4x4& projection, float yStart, float yEnd);

	~CelestialBodyAxis();

	void init();

	void draw();

private:
	/* Program reference */
	cg::GLSLProgram& program;
	glm::mat4x4& view;
	glm::mat4x4& projection;

	float yStart;
	float yEnd;

protected:
};