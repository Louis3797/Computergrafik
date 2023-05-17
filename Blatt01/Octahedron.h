#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "GLSLProgram.h"
#include "Object.cpp"


class Octahedron: public Object
{

public:

	Octahedron(cg::GLSLProgram& program, glm::mat4x4& view,
    glm::mat4x4& projection, int detail, float radius, glm::vec3 position, glm::vec3 color);

	~Octahedron();

	void init();

	void draw();

    // Rotate the object around the global x-axis
    void rotateXGlobal(float angle);

    // Rotate the object around the global y-axis
    void rotateYGlobal(float angle);

        // Rotate the object around the global z-axis
    void rotateZGlobal(float angle);

    // Rotate the object around the local x-axis
    void rotateXLocal(float angle);

    // Rotate the object around the local y-axis
    void rotateYLocal(float angle);

    // Rotate the object around the local z-axis
    void rotateZLocal(float angle);

    void setRadius(float radius);

    float getRadius();

    void setDetail(int detail);

    int getDetail();

private:
	int detail;
	float radius;

    std::vector <glm::vec3> vertices;
    std::vector <glm::vec3> colors;
    glm::vec3 position;
    glm::vec3 color;

    std::vector <GLushort> indices;


	cg::GLSLProgram& program;
    glm::mat4x4& view;
    glm::mat4x4& projection;

protected:
};

