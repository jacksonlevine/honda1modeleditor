#pragma once
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Model {
public:
    std::vector<GLfloat> verts;
    std::vector<GLfloat> cols;
    std::vector<GLfloat> uvs;
};
