#include "CelestialBodyAxis.h"

CelestialBodyAxis::CelestialBodyAxis(cg::GLSLProgram& program, glm::mat4x4& view,
	glm::mat4x4& projection, float yStart, float yEnd) : program(program), view(view), projection(projection), yStart(yStart), yEnd(yEnd) {}


CelestialBodyAxis::~CelestialBodyAxis()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &positionBuffer);
}

void CelestialBodyAxis::init()
{
	// Construct axis. These vectors can go out of scope after we have send all data to the graphics card.
	std::vector<glm::vec3> vertices = { {0.0f, yStart, 0.0f}, { 0.0f, yEnd, 0.0f } };
	std::vector<glm::vec3> colors = { { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } };
	std::vector<GLushort> indices = { 0, 1 };

	GLuint programId = this->program.getHandle();
	GLuint pos;

	// Step 0: Create vertex array object.
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	// Step 1: Create vertex buffer object for position attribute and bind it to the associated "shader attribute".
	glGenBuffers(1, &this->positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	// Bind it to position.
	pos = glGetAttribLocation(programId, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Step 2: Create vertex buffer object for color attribute and bind it to...
	glGenBuffers(1, &this->colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

	// Bind it to color.
	pos = glGetAttribLocation(programId, "color");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Step 3: Create vertex buffer object for indices. No binding needed here.
	glGenBuffers(1, &this->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

	// Unbind vertex array object (back to default).
	glBindVertexArray(0);
}

void CelestialBodyAxis::draw()
{
	glm::mat4x4 mvp = this->projection * this->view * this->model;
	// Bind the shader program and set uniform(s).
	this->program.use();
	this->program.setUniform("mvp", mvp);

	// Bind vertex array object so we can render the triangles.
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_LINES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}