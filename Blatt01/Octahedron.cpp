#include "Octahedron.h"
#include <GL/glew.h>
//#include <GL/gl.h> // OpenGL header not necessary, included by GLEW
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "GLSLProgram.h"
#include "GLTools.h"


Octahedron::Octahedron(cg::GLSLProgram& program, glm::mat4x4& view,
    glm::mat4x4& projection, int detail, float radius, glm::vec3 position, glm::vec3 color) : program(program), view(view), projection(projection), detail(detail), radius(radius), position(position), color(color) {
   init();
}

Octahedron::~Octahedron() {
    this->indices.clear();
    this->vertices.clear();
    this->colors.clear();
}


void Octahedron::init() {

    // Construct triangle. These vectors can go out of scope after we have send all data to the graphics card.
    this->vertices = {
            glm::vec3(0.0f, 0.0f, radius), // top
            glm::vec3(radius, 0.0f, 0.0f), // right
            glm::vec3(0.0f, radius, 0.0f), // front
            glm::vec3(-radius, 0.0f, 0.0f), // left
            glm::vec3(0.0f, -radius, 0.0f), // back
            glm::vec3(0.0f, 0.0f, -radius) // bottom
    };



    this->indices = {
            0, 1, 2, // top-front-right
            0, 2, 3, // top-front-left
            0, 3, 4, // top-back-left
            0, 4, 1, // top-back-right
            5, 2, 1, // bottom-front-right
            5, 3, 2, // bottom-front-left
            5, 4, 3, // bottom-back-left
            5, 1, 4  // bottom-back-right
    };

    // Subdivide the octahedron
    for (int i = 0; i < this->detail; i++) {
        std::vector<GLushort> newIndices;
        for (unsigned int j = 0; j < this->indices.size(); j += 3) {
            unsigned int v1 = this->indices[j];
            unsigned int v2 = this->indices[j + 1];
            unsigned int v3 = this->indices[j + 2];

            glm::vec3 p1 = this->vertices[v1];
            glm::vec3 p2 = this->vertices[v2];
            glm::vec3 p3 = this->vertices[v3];

            glm::vec3 p12 = glm::normalize(p1 + p2) * this->radius;
            glm::vec3 p23 = glm::normalize(p2 + p3) * this->radius;
            glm::vec3 p31 = glm::normalize(p3 + p1) * this->radius;

            unsigned int v12 = this->vertices.size();
            this->vertices.push_back(p12);

            unsigned int v23 = this->vertices.size();
            this->vertices.push_back(p23);

            unsigned int v31 = this->vertices.size();
            this->vertices.push_back(p31);

            newIndices.push_back(v1);
            newIndices.push_back(v12);
            newIndices.push_back(v31);

            newIndices.push_back(v2);
            newIndices.push_back(v23);
            newIndices.push_back(v12);

            newIndices.push_back(v3);
            newIndices.push_back(v31);
            newIndices.push_back(v23);

            newIndices.push_back(v12);
            newIndices.push_back(v23);
            newIndices.push_back(v31);
        }

        this->indices = newIndices;
    }

    for (unsigned int i = 0; i < this->indices.size(); i++) 
        this->colors.push_back(this->color);
    


    GLuint programId = this->program.getHandle();
    GLuint pos;

    // Step 0: Create vertex array object.
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    // Step 1: Create vertex buffer object for position attribute and bind it to the associated "shader attribute".
    glGenBuffers(1, &this->positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), this->vertices.data(), GL_STATIC_DRAW);

    // Bind it to position.
    pos = glGetAttribLocation(programId, "position");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Step 2: Create vertex buffer object for color attribute and bind it to...
    glGenBuffers(1, &this->colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(glm::vec3), this->colors.data(), GL_STATIC_DRAW);

    // Bind it to color.
    pos = glGetAttribLocation(programId, "color");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Step 3: Create vertex buffer object for indices. No binding needed here.
    glGenBuffers(1, &this->indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLushort), this->indices.data(),
        GL_STATIC_DRAW);

    // Unbind vertex array object (back to default).
    glBindVertexArray(0);

    // Modify model matrix.
    this->model = glm::translate(glm::mat4(1.0f), this->position);


}

void Octahedron::draw() {
        // Create mvp.
        glm::mat4x4 mvp = this->projection * this->view * this->model;

        // Bind the shader program and set uniform(s).
        program.use();
        program.setUniform("mvp", mvp);

        // Bind vertex array object so we can render the 2 triangles.
        

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Octahedron::rotateXGlobal(float angle) {
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

    // Apply the rotation matrix to the model matrix
    this->model = rotationMatrix * this->model;
}
 

void Octahedron::rotateYGlobal(float angle) {
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

    // Apply the rotation matrix to the model matrix
    this->model = rotationMatrix * this->model;
}


void Octahedron::rotateZGlobal(float angle) {
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

    // Apply the rotation matrix to the model matrix
    this->model = rotationMatrix * this->model;
}

// Rotate the object around the local x-axis
void Octahedron::rotateXLocal(float angle) {
    this->model = glm::rotate(this->model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

// Rotate the object around the local y-axis
void Octahedron::rotateYLocal(float angle) {
    this->model = glm::rotate(this->model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

// Rotate the object around the local z-axis
void Octahedron::rotateZLocal(float angle) {
    this->model = glm::rotate (this->model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Octahedron::setRadius(float radius) {
    this->radius = radius;
}

float Octahedron::getRadius() {
    return this->radius;
}

void Octahedron::setDetail(int detail) {
    this->detail = detail;
}

int Octahedron::getDetail() {
    return this->detail;
}