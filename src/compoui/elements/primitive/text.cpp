#include "text.hpp"
#include "../../util/shader.hpp"
#include <vector>

const char* CUI::Primitive::Text::fragSource = R"(
    #version 460 core
    in vec2 vTexCoord;
    layout(location = 2) uniform sampler2D tTex;
    layout(location = 3) uniform vec4 uTextColor;
    out vec4 outColor;

    void main() {
        outColor = uTextColor * vec4(1.0, 1.0, 1.0, texture(tTex, vTexCoord).r);
    }
)";

int CUI::Primitive::Text::program = 0;

CUI::Primitive::Text::Text(Val<std::shared_ptr<Font>> font, Val<std::string> text, Val<Color> color) :
            font(font), text(text), color(color)
{
    if(program == 0)
        program = Util::createShaderProgram(Util::genericVertexShader, fragSource);

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

    updateGeometry();      // build initial geometry
}

CUI::Primitive::Text::~Text() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
}

void CUI::Primitive::Text::draw(Rect rect) {
    Color color = this->color.get();
    auto font = this->font.get();

    glUseProgram(program);

    // Uniforms
    glUniform4f(0, rect.pos.x, rect.pos.y, 1, 1);
    glUniform2fv(1, 1, &ctx->windowSize.x);
    glUniform4fv(3, 1, &color.r);

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
    auto font = this->font.get();

    struct Vertex{Vec2 pos, uv;};
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int vertIdx = 0;

    float baseline = (font->lineHeight - (font->ascender - font->descender)) / 2.0f + font->ascender;
    Vec2 currentPos{0,baseline};

    for (char c : text.get()) {
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