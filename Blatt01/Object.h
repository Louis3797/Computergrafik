#pragma once
/*
Struct to hold data for object rendering.
*/
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


class Object
{
public:
    Object();

    ~Object();


    GLuint vao;        // vertex-array-object ID

    GLuint positionBuffer; // ID of vertex-buffer: position
    GLuint colorBuffer;    // ID of vertex-buffer: color

    GLuint indexBuffer;    // ID of index-buffer

    glm::mat4x4 model; // model matrix
};