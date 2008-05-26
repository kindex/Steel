#include "frame_buffer.h"
#include <math.h>
#include "../ext/libext.h"
#include "../../../common/logger.h"

bool createFrame(Frame* surface, bool depth, bool fp, bool linear)
{
    GLenum internalFormat = fp ? GL_RGBA16F_ARB : GL_RGBA;
    GLenum type = fp ? GL_HALF_FLOAT_ARB : GL_UNSIGNED_BYTE;
    GLenum filter = linear ? GL_LINEAR : GL_NEAREST;

    // create a color texture
    glGenTextures(1, &surface->texture);
    glBindTexture(GL_TEXTURE_2D, surface->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->width, surface->height, 0, GL_RGBA, type, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (glGetError() != 0)
    {
        log_msg("opengl", "Creation of the color texture for the FBO");
        return false;
    }

    // create depth renderbuffer
    if (depth)
    {
        glGenRenderbuffersEXT(1, &surface->depth);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, surface->depth);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, surface->width, surface->height);
        if (glGetError() != 0)
        {
            log_msg("opengl", "Creation of the depth renderbuffer for the FBO");
            return false;
        }
    }
    else
    {
        surface->depth = 0;
    }

    // create FBO itself
    glGenFramebuffersEXT(1, &surface->fbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surface->fbo);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, surface->texture, 0);
    if (depth)
    {
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, surface->depth);
    }
    checkFBO();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    if (glGetError() != 0)
    {
        log_msg("opengl", "Creation of the FBO itself");
        return false;
    }

    return true;
}

void deleteFrame(Frame* frame)
{
    glDeleteTextures(1, &frame->texture);
    glDeleteFramebuffersEXT(1, &frame->fbo);
    if (frame->depth != 0)
    {
        glDeleteRenderbuffersEXT(1, &frame->depth);
    }
}

void checkFBO()
{
    char enums[][20] =
    {
        "attachment",         // GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT........... All framebuffer attachment points are 'framebuffer attachment complete'.
        "missing attachment", // GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT....There is at least one image attached to the framebuffer.
        "",                   //
        "dimensions",         // GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT............All attached images have the same width and height.
        "formats",            // GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT...............All images attached to the attachment points COLOR_ATTACHMENT0_EXT through COLOR_ATTACHMENTn_EXT must have the same internal format.
        "draw buffer",        // GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT...........The value of FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT must not be NONE for any color attachment point(s) named by DRAW_BUFFERi.
        "read buffer",        // GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT...........If READ_BUFFER is not NONE, then the value of FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT must not be NONE for the color attachment point named by READ_BUFFER.
        "unsupported format"  // GL_FRAMEBUFFER_UNSUPPORTED_EXT......................The combination of internal formats of the attached images does not violate an implementation-dependent set of restrictions.
    };

    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status == GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        return;
    }

    status -= GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT;
    error("opengl", std::string("incomplete framebuffer object due to ") + enums[status]);
}

void bindFrame(const Frame* surface)
{
    if (surface != NULL)
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surface->fbo);
//        glViewport(surface->viewport.x, surface->viewport.y, surface->viewport.width, surface->viewport.height);
    }
    else
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    }
    //glMatrixMode(GL_PROJECTION);
    //glLoadMatrixf(surface->projection);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadMatrixf(surface->modelview);
}
