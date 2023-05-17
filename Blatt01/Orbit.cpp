#include "Orbit.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <iostream>
#include <cmath>
#include <vector>

#include "GLSLProgram.h"
#include "GLTools.h"

const double PI = 3.14159265358979323846;

Orbit::Orbit(cg::GLSLProgram& program, glm::mat4x4& view,
	glm::mat4x4& projection, float radius) : program(program), view(view), projection(projection), radius(radius) {
}

Orbit::~Orbit() {
	this->positions.clear();
}

// Function to create the orbit geometry
void Orbit::init()
{
    int numSegments = 360;
    float angleIncrement = 2.0f * PI / numSegments;

    // Calculate the positions of the vertices
    std::vector<float> positions;
    positions.reserve(numSegments * 2);

    for (int i = 0; i < numSegments; ++i) {
        float angle = i * angleIncrement;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        positions.push_back(x);
        positions.push_back(y);
    }

	GLuint programId = this->program.getHandle();
	GLuint pos;

	// Step 0: Create vertex array object.
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	// Step 1: Create vertex buffer object for position attribute and bind it to the associated "shader attribute".
	glGenBuffers(1, &this->positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->positions.size() * sizeof(float), this->positions.data(), GL_STATIC_DRAW);

	// Bind it to position.
	pos = glGetAttribLocation(programId, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);


	// Bind it to color.
	pos = glGetAttribLocation(programId, "color");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Step 3: Create vertex buffer object for indices. No binding needed here.
	glGenBuffers(1, &this->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->positions.size() * sizeof(GLushort), this->positions.data(),
		GL_STATIC_DRAW);

	// Unbind vertex array object (back to default).
	glBindVertexArray(0);
}

// Function to draw the orbit
void Orbit::draw()
{		
    glBindVertexArray(this->vao);
    glDrawArrays(GL_LINE_LOOP, 0, 360);
    glBindVertexArray(0);
}



