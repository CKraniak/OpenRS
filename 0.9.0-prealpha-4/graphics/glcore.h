/* Copyright (C) 2016, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 */
#ifndef GLCORE_H
#define GLCORE_H

// As much as possible, try to avoid platform-specific things in GLCore
// That is what GLFunctionFinder does for us.

#include "GL/gl.h"
#include "GL/glext.h"

#include <vector>
#include <string>

struct GlPoint2d {
    GLfloat x_, y_;

    GlPoint2d(GLfloat x, GLfloat y) :
        x_(x), y_(y) {}
};

struct GlPointTriangle2d {
    GlPoint2d a_, b_, c_;

    GlPointTriangle2d(GLfloat a1, GLfloat a2,
                      GLfloat b1, GLfloat b2,
                      GLfloat c1, GLfloat c2) :
        a_(a1, a2),
        b_(b1, b2),
        c_(c1, c2) {}
};

struct GlQuad {
    GlPointTriangle2d a_, b_;

    GlQuad(GLfloat a1, GLfloat a2,
           GLfloat b1, GLfloat b2,
           GLfloat c1, GLfloat c2,
           GLfloat d1, GLfloat d2) :
        a_(a1, a2, b1, b2, c1, c2),
        b_(b1, b2, c1, c2, d1, d2) {}

    GLfloat * getPoints(GLfloat * out) {
        out[0] = a_.a_.x_;
        out[1] = a_.a_.y_;
        out[2] = a_.b_.x_;
        out[3] = a_.b_.y_;
        out[4] = a_.c_.x_;
        out[5] = a_.c_.y_;
        out[6] = b_.c_.x_;
        out[7] = b_.c_.y_;
        return out;
    }
};

GLfloat * getVertexData(GLfloat * out,
                        std::vector<GlQuad> quad_vector);



class GLCore
{
    double scale_;
    int vbuf_;

public:
    GLCore();
    GLuint prepareGLThings();
    void updateVeritces();
    void renderScene(GLuint vbuf);
    void setScale(double scale);
    double getScale() { return scale_; }

    void loadShader(std::string filename);
    void loadDefaultVertNFragShader();
};

#endif // GLCORE_H
