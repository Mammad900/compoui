#include "cui.hpp"
#include "glad/glad.h"
#include "util/shader.hpp"

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

const char *fragmentSource = R"(
    #version 460 core

    in vec2 globalPixel;
    in vec2 localPixel;
    in vec2 globalUV;
    in vec2 localUV;

    out vec4 color;

    void main() {
        color = vec4(mod(globalUV, 1.0), 1.0, 1.0);
    }
)";

CUI::Context::Context(uint w, uint h, std::shared_ptr<Element> root): windowSize(w, h), root(root) {

    // A unit quad. x/y/w/h are applied through the model matrix.
    constexpr float vertices[] = {0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, 1.0f, 1.0f, 1.0f};

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindVertexArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CUI::Context::resize(uint w, uint h) {
    windowSize = Vec2(w, h);
    glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
}

void CUI::Context::draw() {
    root->draw(shared_from_this(), {{0,0}, windowSize});
}