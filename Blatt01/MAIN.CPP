#include <iostream>
#include <vector>

#include <GL/glew.h>
//#include <GL/gl.h> // OpenGL header not necessary, included by GLEW
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "GLSLProgram.h"
#include "GLTools.h"

#include "SolarSystem.h"

using std::cout;
using std::cin;
using std::endl;

// Standard window width
const int WINDOW_WIDTH = 640;
// Standard window height
const int WINDOW_HEIGHT = 480;
// GLUT window id/handle
int glutID = 0;


cg::GLSLProgram program;

glm::mat4x4 view;
glm::mat4x4 projection;

float zNear = 0.1f;
float zFar = 100.0f;

glm::vec3 eye(8.0f, 8.0f, 8.0f);
glm::vec3 center(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 zoom(0.1f, 0.1f, 0.1f);

Object axes;

SolarSystem* solarSystem;


void initAxes()
{
    // Construct axes. These vectors can go out of scope after we have send all data to the graphics card.
    std::vector<glm::vec3> vertices = {
        { 0.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f }, // x-Axis
        { 0.0f, 0.0f, 0.0f },{ 0.0f, 10.0f, 0.0f }, // y-Axis
        { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 10.0f }  // z-Axis
    };


    const std::vector<glm::vec3> colors = {
        { 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f }, // x-Axis (red)
        { 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f }, // y-Axis (green)
        { 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f }  // z-Axis (blue)
    };

    const std::vector<GLushort> indices = {
        0,  1, // x-Axis
        2,  3, // y-Axis
        4,  5  // z-Axis
    };

    GLuint programId = program.getHandle();
    GLuint pos;

    // Step 0: Create vertex array object.
    glGenVertexArrays(1, &axes.vao);
    glBindVertexArray(axes.vao);

    // Step 1: Create vertex buffer object for position attribute and bind it to the associated "shader attribute".
    glGenBuffers(1, &axes.positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, axes.positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Bind it to position.
    pos = glGetAttribLocation(programId, "position");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Step 2: Create vertex buffer object for color attribute and bind it to...
    glGenBuffers(1, &axes.colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, axes.colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

    // Bind it to color.
    pos = glGetAttribLocation(programId, "color");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Step 3: Create vertex buffer object for indices. No binding needed here.
    glGenBuffers(1, &axes.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axes.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

    // Unbind vertex array object (back to default).
    glBindVertexArray(0);
}

void renderAxes()
{
    // Create mvp.
    glm::mat4x4 mvp = projection * view * axes.model;

    // Bind the shader program and set uniform(s).
    program.use();
    program.setUniform("mvp", mvp);

    // Bind vertex array object so we can render the triangles.
    glBindVertexArray(axes.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axes.indexBuffer);
    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_LINES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

/*
 Initialization. Should return true if everything is ok and false if something went wrong.
 */
bool init()
{
    // OpenGL: Set "background" color and enable depth testing.
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    //glEnable(GL_DEPTH_TEST);


    view = glm::lookAt(eye, center, up);


    // Create a shader program and set light direction.
    if (!program.compileShaderFromFile("shader/simple.vert", cg::GLSLShader::VERTEX)) {
        std::cerr << program.log();
        return false;
    }

    if (!program.compileShaderFromFile("shader/simple.frag", cg::GLSLShader::FRAGMENT)) {
        std::cerr << program.log();
        return false;
    }

    if (!program.link()) {
        std::cerr << program.log();
        return false;
    }

    initAxes();

    solarSystem->init();
    
    return true;
}

/*
 Rendering.
 */
void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    solarSystem->draw();
    renderAxes();

    glutSwapBuffers();
}

void glutDisplay()
{
    render();
   // glutSwapBuffers();
}

void update()
{
    glutPostRedisplay();
}


/*
 Resize callback.
 */
void glutResize(int width, int height)
{
    // Division by zero is bad...
    height = height < 1 ? 1 : height;
    glViewport(0, 0, width, height);

    // Construct projection matrix.
    projection = glm::perspective(45.0f, (float)width / height, zNear, zFar);

}

/*
 Callback for char input.
 */
void glutKeyboard(unsigned char keycode, int x, int y)
{
    switch (keycode) {
    case 27: // ESC
        glutDestroyWindow(glutID);
        return;
    case 'd':
        solarSystem->increasePlanetRotationSpeed();
        break;
    case 'f':
        solarSystem->decreasePlanetRotationSpeed();
        break;
    case 'g':
        solarSystem->setStopAnimation(!solarSystem->getStopAnimation());
        break;
    case '+':
        solarSystem->upY();
        break;
    case '-':
        solarSystem->downY();
        break;
    case 'a':
        eye = eye - zoom;
        cout << "x: " << eye.x << "y: " << eye.y << "z: " << eye.z << endl;
        view = glm::lookAt(eye, center, up);
        break;

    case 's':
        eye = eye + zoom;
        cout << "x: " << eye.x << "y: " << eye.y << "z: " << eye.z << endl;
        view = glm::lookAt(eye, center, up);
        break;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv)
{

    // GLUT: Initialize freeglut library (window toolkit).
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(40, 40);
    glutInit(&argc, argv);

    // GLUT: Create a window and opengl context (version 4.3 core profile).
    glutInitContextVersion(4, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    glutCreateWindow("Aufgabenblatt 02");
    glutID = glutGetWindow();

    // GLEW: Load opengl extensions
    //glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return -1;
    }
#if _DEBUG
    if (glDebugMessageCallback) {
        std::cout << "Register OpenGL debug callback " << std::endl;
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(cg::glErrorVerboseCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DONT_CARE,
            0,
            nullptr,
            true); // get all debug messages
    }
    else {
        std::cout << "glDebugMessageCallback not available" << std::endl;
    }
#endif


      
    // GLUT: Set callbacks for events.
    glutReshapeFunc(glutResize);
    glutDisplayFunc(glutDisplay);
    //glutIdleFunc   (glutDisplay); // redisplay when idle

    glutKeyboardFunc(glutKeyboard);
   glutIdleFunc(update);
 
   solarSystem = new SolarSystem(program, view, projection);

    // init vertex-array-objects.
    bool result = init();
    if (!result) {
        return -2;
    }

 

    // GLUT: Loop until the user closes the window
    // rendering & event handling
    glutMainLoop();

    // Cleanup in destructors:
    // Objects will be released in ~Object
    // Shader program will be released in ~GLSLProgram

    return 0;
}
