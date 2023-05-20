#include "Orbit.h"

Orbit::Orbit(cg::GLSLProgram& program, glm::mat4x4& view,
	glm::mat4x4& projection, float radius, glm::vec3 center, unsigned int numPoints) : program(program), view(view), projection(projection), radius(radius), center(center), numPoints(numPoints) {
	init();
}

Orbit::~Orbit() {
}

// Function to create the orbit geometry
void Orbit::init()
{
	std::vector<glm::vec3> orbitPoints;
	for (int i = 0; i < numPoints; ++i) {
		float angle = glm::radians(360.0f * i / numPoints);
		float x = this->center.x + this->radius * std::cos(angle);
		float z = this->center.z + this->radius * std::sin(angle);
		orbitPoints.emplace_back(x, this->center.y, z);
	}

	GLuint programId = this->program.getHandle();
	GLuint pos;

	// Step 0: Create vertex array object.
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	// Step 1: Create vertex buffer object for position attribute and bind it to the associated "shader attribute".
	glGenBuffers(1, &this->positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);


	glBufferData(GL_ARRAY_BUFFER, orbitPoints.size() * sizeof(glm::vec3), orbitPoints.data(), GL_STATIC_DRAW);

	// Bind it to position.
	pos = glGetAttribLocation(programId, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Unbind the vertex array object (VAO)
	glBindVertexArray(0);
}

// Function to draw the orbit
void Orbit::draw()
{
	glBindVertexArray(this->vao);
	glDrawArrays(GL_LINE_LOOP, 0, 360);
	glBindVertexArray(0);
}