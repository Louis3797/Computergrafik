#include "Object.h"

Object::Object()
    : vao(0),
    positionBuffer(0),
    colorBuffer(0),
    indexBuffer(0)
{}

Object::~Object() { // GL context must exist on destruction
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->indexBuffer);
    glDeleteBuffers(1, &this->colorBuffer);
    glDeleteBuffers(1, &this->positionBuffer);
}