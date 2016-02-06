#include "glcore.h"
#include "glfunctionfinder.h"

GLfloat *getVertexData(GLfloat *out, std::vector<GlQuad> quad_vector) {
    GLfloat tmp[8];
    for(int i = 0; i < quad_vector.size(); ++i) {
        memset(tmp, 0, sizeof(GLfloat) * 8);
        quad_vector[i].getPoints(tmp);

        out[ 0] = tmp[0];
        out[ 1] = tmp[1];
        out[ 2] =   0.0f;

        out[ 3] = tmp[2];
        out[ 4] = tmp[3];
        out[ 5] =   0.0f;

        out[ 6] = tmp[4];
        out[ 7] = tmp[5];
        out[ 8] =   0.0f;

        out[ 9] = tmp[6];
        out[10] = tmp[7];
        out[11] =   0.0f;
    }
    return out;
}

GLCore::GLCore() : scale_(1.0)
{

}

GLuint GLCore::prepareGLThings() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLfloat val = 1.0 * scale_;

    static const GLfloat g_vertex_buffer_data[] = {
        -val, -val,  0.0f,
        val,  -val,  0.0f,
        0.0f,  val,  0.0f,
    };

    GLuint vbuf;
    glGenBuffers(1, &vbuf);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(g_vertex_buffer_data),
                 g_vertex_buffer_data,
                 GL_STATIC_DRAW);

    vbuf_ = vbuf;
    return vbuf;
}

void GLCore::updateVeritces() {
    GLfloat val = 1.0 * scale_;

    GLfloat g_vertex_buffer_data[] = {
        -val, -val,  0.0f,
        val,  -val,  0.0f,
        0.0f,  val,  0.0f,
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbuf_);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(g_vertex_buffer_data),
                 g_vertex_buffer_data,
                 GL_STATIC_DRAW);
}

void GLCore::renderScene(GLuint vbuf) {
    updateVeritces();

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void*)0
                );
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

void GLCore::setScale(double scale)
{
    scale_ = scale;
    if(scale_ < 0.10) {
        scale_ = 0.10;
    }
}

void GLCore::loadShader(std::string filename)
{
    if(filename.size() == 0) {
        loadDefaultVertNFragShader();
        return;
    }
}

void GLCore::loadDefaultVertNFragShader()
{

}

