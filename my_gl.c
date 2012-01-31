
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

// include declaration ofI_my_gl functions
#include "i_my_gl.h"

// the followings are wrapper functions

// synchronize OpenGL's current matrix with i_my_gl's current matrix
void sync_matrix()
{
    GLdouble m[16];
    I_my_glGetMatrixd(m);
    glLoadMatrixd(m);
}

// switching matrix mode for both OpenGL and i_my_gl
void my_glMatrixMode(GLenum mode)
{
    I_my_glMatrixMode(mode);
    glMatrixMode(mode);
    sync_matrix();
}

// all following functions first call corresponding i_my_gl functions and then synchronize current matrix with OpenGL

void my_glLoadIdentity(void)
{
    I_my_glLoadIdentity();
    sync_matrix();
}

void my_glPushMatrix(void)
{
    I_my_glPushMatrix();
    sync_matrix();
}

void my_glPopMatrix(void)
{
    I_my_glPopMatrix();
    sync_matrix();
}

void my_glLoadMatrixf(const GLfloat *m)
{
    I_my_glLoadMatrixf(m);
    sync_matrix();
}

void my_glLoadMatrixd(const GLdouble *m)
{
    I_my_glLoadMatrixd(m);
    sync_matrix();    
}

void my_glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
    I_my_glTranslated(x, y, z);
    sync_matrix();
}

void my_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glTranslatef(x, y, z);
    sync_matrix();
}

void my_glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
    I_my_glRotated(angle, x, y, z);
    sync_matrix();
}

void my_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glRotatef(angle, x, y, z);
    sync_matrix();
}

void my_glScaled(GLdouble x, GLdouble y, GLdouble z)
{
    I_my_glScaled(x, y, z);
    sync_matrix();
}

void my_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glScalef(x, y, z);
    sync_matrix();
}

void my_gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
    GLdouble centerX, GLdouble centerY, GLdouble centerZ,
    GLdouble upX, GLdouble upY, GLdouble upZ)
{
    I_my_gluLookAt(eyeX, eyeY, eyeZ, 
        centerX, centerY, centerZ,
        upX, upY, upZ);
    sync_matrix();
}

void my_glFrustum(GLdouble left, GLdouble right, 
    GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
    I_my_glFrustum(left, right, bottom, top, zNear, zFar);
    sync_matrix();
}

void my_gluPerspective(GLdouble fovy, GLdouble aspect,
    GLdouble zNear, GLdouble zFar)
{
    I_my_gluPerspective(fovy, aspect, zNear, zFar);
    sync_matrix();
}

//*/



