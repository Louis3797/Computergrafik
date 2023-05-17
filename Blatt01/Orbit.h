

#include <vector>
#include <glm/glm.hpp>
#include "GLSLProgram.h"
#include "Object.cpp"

class Orbit : public Object 
{

public:

    Orbit(cg::GLSLProgram& program, glm::mat4x4& view,
        glm::mat4x4& projection, float radius);

    ~Orbit();

    void draw();

    void init();

private:
    float radius;
    std::vector<float> positions;

    cg::GLSLProgram& program;
    glm::mat4x4& view;
    glm::mat4x4& projection;

protected:

};