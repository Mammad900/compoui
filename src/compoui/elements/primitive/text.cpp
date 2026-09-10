#include "text.hpp"
#include "../../util/shader.hpp"
#include <vector>

const char* CUI::Primitive::Text::vertSource = R"(
    #version 460 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aTexCoord;

    uniform vec4 rect;
    uniform vec2 viewportSize;

    out vec2 vTexCoord;

    void main() {
        vec2 globalPixel = round(rect.xy + aPos + vec2(0.01, 0.01));
        vec2 globalUV = globalPixel / viewportSize;
        vec2 pos_normalized = globalUV * vec2(2.0, -2.0) + vec2(-1.0, 1.0);
        gl_Position = vec4(pos_normalized, 0.0, 1.0);
        vTexCoord = aTexCoord;
    }
)";

const char* CUI::Primitive::Text::fragSource = R"(
    #version 460 core
    in vec2 vTexCoord;
    uniform sampler2D uTex;
    uniform vec4 textColor;
    out vec4 FragColor;

    void main() {
        FragColor = textColor * vec4(1.0, 1.0, 1.0, texture(uTex, vTexCoord).r);
    }
)";

int CUI::Primitive::Text::program = 0;

CUI::Primitive::Text::Text(std::shared_ptr<Font> font, std::string text, Color color) :
            font(font), text(text), color(color)
{
    if(program == 0)
        program = Util::createShaderProgram(vertSource, fragSource);

    /* ---- Create VAO/VBO/EBO --------------------------------- */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);          // interleaved pos+uv
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr,
                    GL_DYNAMIC_DRAW);   // will be filled in updateGeometry()

    glGenBuffers(1, &ebo);          // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    /* Vertex attributes */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                            sizeof(Vec2) * 2, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                            sizeof(Vec2) * 2,
                            (void*)(sizeof(Vec2)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);   // unbind VAO

    glUseProgram(program);
    rectLoc = glGetUniformLocation(program, "rect");
    vpSizeLoc = glGetUniformLocation(program, "viewportSize");
    textColorLoc = glGetUniformLocation(program, "textColor");

    updateGeometry();      // build initial geometry
}

CUI::Primitive::Text::~Text() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
}

void CUI::Primitive::Text::draw(ContextPtr ctx, Rect rect) {
    glUseProgram(program);

    // Uniforms
    glUniform4fv(rectLoc, 1, &rect.pos.x);
    glUniform2fv(vpSizeLoc, 1, &ctx->windowSize.x);
    glUniform4fv(textColorLoc, 1, &color.r);

    /* Bind font atlas texture */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->atlas());

    /* Draw the batch */
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,
                    static_cast<GLsizei>(indexCount),
                    GL_UNSIGNED_INT,
                    0);
    glBindVertexArray(0);

    glUseProgram(0);   // unbind program (optional)
}

void CUI::Primitive::Text::updateGeometry() {
    struct Vertex{Vec2 pos, uv;};
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int vertIdx = 0;

    float baseline = (font->lineHeight - (font->ascender - font->descender)) / 2.0f + font->ascender;
    Vec2 currentPos{0,baseline};

    for (char c : text) {
        Font::CharInfo glyph = font->get(c);
        Rect uv = glyph.uv;
        Rect pos = {
            currentPos + Vec2{(float)glyph.bearing.x, -(float)glyph.bearing.y}, 
            {glyph.size}
        };
        currentPos.x += glyph.advance;

        /* Four vertices per glyph – positions in pixel space */
        vertices.push_back({pos.pos,            uv.pos});
        vertices.push_back({{pos.max().x, pos.pos.y},
                            {uv.max().x, uv.pos.y}});
        vertices.push_back({{pos.pos.x, pos.max().y},
                            {uv.pos.x, uv.max().y}});
        vertices.push_back({pos.max(),          uv.max()});

        /* Two triangles (6 indices) */
        indices.insert(indices.end(),
                        {vertIdx, vertIdx + 1, vertIdx + 2,
                        vertIdx + 2, vertIdx + 1, vertIdx + 3});
        vertIdx += 4;
    }

    totalSize = {currentPos.x, font->lineHeight};

    /* Upload vertex buffer */
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                    vertices.size() * sizeof(Vertex),
                    vertices.data(),
                    GL_DYNAMIC_DRAW);

    /* Upload index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    indices.size() * sizeof(unsigned int),
                    indices.data(),
                    GL_DYNAMIC_DRAW);

    indexCount = static_cast<unsigned int>(indices.size());
}