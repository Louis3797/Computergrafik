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

glm::vec3 eye(10.0f, 4.0f, 10.0f);
glm::vec3 center(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 zoom(0.1f, 0.1f, 0.1f);

//Object axes;

SolarSystem* solarSystem;


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
