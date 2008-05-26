#pragma once
#include <windows.h>
#include <GL/gl.h>

struct PHviewport
{
    GLint x;
    GLint y;
    GLsizei width;
    GLsizei height;
};

struct Frame
{
    GLsizei width;
    GLsizei height;
    PHviewport viewport;
    GLfloat clearColor[4];
    GLfloat modelview[16];
    GLfloat projection[16];
    GLuint texture;
    GLuint depth;
    GLuint fbo;
};

bool createFrame(Frame*, bool depth, bool fp, bool linear);
void deleteFrame(Frame*);
void checkFBO();
