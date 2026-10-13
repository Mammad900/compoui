#pragma once
#include "cui.hpp"
#include <memory>

namespace CUI {
    class FBOLease {
    public:
        std::shared_ptr<FBO> fbo;
        FBOLease(std::shared_ptr<FBO> fbo) : fbo(fbo) {}
        ~FBOLease();
    };
    class FBO : public std::enable_shared_from_this<FBO> {
    public:
        unsigned int fbo, tex;
        bool busy = false;

        FBO(ContextPtr ctx);

        ~FBO();
        
        std::shared_ptr<FBOLease> lease();
    };
}