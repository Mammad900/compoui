#include "fbo.hpp"
#include "glad/glad.h"
#include <memory>

CUI::FBO::FBO() {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,          // 8‑bit RGBA
        ctx->windowSize.x, 
        ctx->windowSize.y,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr            // no initial data
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        tex,
        0
    );

    // Check completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Layer FBO incomplete!");
    }

    // Unbind to avoid accidental rendering here
    if(ctx->currentFBO)
        glBindFramebuffer(GL_FRAMEBUFFER, ctx->currentFBO->fbo);
}

CUI::FBO::~FBO() {
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &tex);
}

std::shared_ptr<CUI::FBOLease> CUI::FBO::lease() {
    if(busy) return nullptr;

    busy = true;
    return std::make_shared<FBOLease>(shared_from_this());
}

CUI::FBOLease::~FBOLease() {
    fbo->busy = false;
}