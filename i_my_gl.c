#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

// define maximum stack capacity
#define STACK_CAP 16
// define pi for converting angles
#define PI 3.14159265359

// structure for the matrix stack, top specifies current top position on the stack, initially zero (which means one matrix in the stack)
struct matrix_stack
{
    GLdouble m[STACK_CAP][16];
    int top;
};

// define a macro for retrieving current matrix from top of current stack
#define current_matrix (current_stack->m[current_stack->top])

// identity matrix constant
const GLdouble identity[16] =
{1, 0, 0, 0,
 0, 1, 0, 0,
 0, 0, 1, 0,
 0, 0, 0, 1};

// the model view matrix stack
struct matrix_stack model_view = {{{0}}, 0};
// the projection matrix stack
struct matrix_stack projection = {{{0}}, 0};
// the current stack pointer that specifies the matrix mode
struct matrix_stack *current_stack = &model_view;

// multiply current matrix with matrix b, put the result in current matrix
// current = current * b
void matrix_multiply(const GLdouble *b)
{
    GLdouble result[16];

    for (int i=0; i<4; i++)
	{
		result[i*4]   =  (current_matrix[i*4]   * b[0]) +
					     (current_matrix[i*4+1] * b[4]) +
					     (current_matrix[i*4+2] * b[8]) +
					     (current_matrix[i*4+3] * b[12]) ;

		result[i*4+1] =  (current_matrix[i*4]   * b[1]) +
						 (current_matrix[i*4+1] * b[5]) +
						 (current_matrix[i*4+2] * b[9]) +
						 (current_matrix[i*4+3] * b[13]) ;

		result[i*4+2] =  (current_matrix[i*4]   * b[2]) +
					     (current_matrix[i*4+1] * b[6]) +
						 (current_matrix[i*4+2] * b[10]) +
						 (current_matrix[i*4+3] * b[14]) ;

		result[i*4+3] =  (current_matrix[i*4]   * b[3]) +
						 (current_matrix[i*4+1] * b[7]) +
						 (current_matrix[i*4+2] * b[11]) +
						 (current_matrix[i*4+3] * b[15]) ;
	}

    for(int i=0; i<16; i++){
        current_matrix[i] =  result[i];
    }
}

// calculating cross product of b and c, put the result in a
// a = b x c
void cross_product(GLdouble *ax, GLdouble *ay, GLdouble *az,
    GLdouble bx, GLdouble by, GLdouble bz,
    GLdouble cx, GLdouble cy, GLdouble cz)
{
    *ax = by * cz - cy * bz;
    *ay = bz * cx - cz * bx;
    *az = bx * cy - cx * by;
}

// normaliz vector (x, y, z)
void normalize(GLdouble *x, GLdouble *y, GLdouble *z)
{
    // Calculate absolute value of vector length
    double length = abs(sqrt((*x * *x) + (*y * *y) + (*z * *z)));
    // Divide each of the components by length
    *x = *x/length;
    *y = *y/length;
    *z = *z/length;
}

// switch matrix mode by changing the current stack pointer
void I_my_glMatrixMode(GLenum mode)
{
    if (mode == GL_MODELVIEW){
        current_stack = &model_view;
    } else if(mode == GL_PROJECTION){
        current_stack = &projection;
    }
}

// overwrite current matrix with identity matrix
void I_my_glLoadIdentity(void)
{
    memset(current_matrix, 0x0, sizeof(GLdouble[16]));
    current_matrix[0]  = 1.0f;
    current_matrix[5]  = 1.0f;
    current_matrix[10] = 1.0f;
    current_matrix[15] = 1.0f;
}

// push current matrix onto current stack, report error if the stack is already full
void I_my_glPushMatrix(void)
{
    int holder = current_stack->top;
    if (holder+1 >= 16){
        //cout << "Stack is full";
    } else{
        for(int i = 0; i < 16; i++){
            current_stack->m[holder+1][i] = current_matrix[i];
        }
        current_stack->top += 1;
    }
}

// pop current matrix from current stack, report error if the stack has only one matrix left
void I_my_glPopMatrix(void)
{
    int holder = current_stack->top;
    if (holder-1 <= 0){
        //cout << "Stack is empty";
    } else{
        current_stack->top -= 1;  // Memory is not freed because that space will be taken on next push
    }
}

// overwrite currentmatrix with m
void I_my_glLoadMatrixf(const GLfloat *m)
{
    for(int i=0; i<16; i++){
        current_matrix[i] = (GLdouble) m[i];
    }
}

void I_my_glLoadMatrixd(const GLdouble *m)
{
    for(int i=0; i<16; i++){
        current_matrix[i] = m[i];
    }
}

void I_my_glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
    current_matrix[12] += (current_matrix[0] * x + current_matrix[4] * y + current_matrix[8]  * z);
    current_matrix[13] += (current_matrix[1] * x + current_matrix[5] * y + current_matrix[9]  * z);
    current_matrix[14] += (current_matrix[2] * x + current_matrix[6] * y + current_matrix[10] * z);
    current_matrix[15] += (current_matrix[3] * x + current_matrix[7] * y + current_matrix[11] * z);
}

void I_my_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glTranslated((GLdouble)x, (GLdouble)y, (GLdouble)z);
}

// remember to normalize vector (x, y, z), and to convert angle from degree to radius before calling sin and cos
void I_my_glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
    GLdouble sin_angle, cos_angle;
    GLdouble mag = sqrtf(x * x + y * y + z * z);

    sin_angle = sinf ( angle * PI / 180.0 );
    cos_angle = cosf ( angle * PI / 180.0 );
    if ( mag > 0.0 )
    {
        GLdouble xx, yy, zz, xy, yz, zx, xs, ys, zs;
        GLdouble one_minus_cos;
        GLdouble rotation_matrix[16];

        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sin_angle;
        ys = y * sin_angle;
        zs = z * sin_angle;
        one_minus_cos = 1.0 - cos_angle;

        rotation_matrix[0] = (one_minus_cos * xx) + cos_angle;
        rotation_matrix[1] = (one_minus_cos * xy) - zs;
        rotation_matrix[2] = (one_minus_cos * zx) + ys;
        rotation_matrix[3] = 0.0;

        rotation_matrix[4] = (one_minus_cos * xy) + zs;
        rotation_matrix[5] = (one_minus_cos * yy) + cos_angle;
        rotation_matrix[6] = (one_minus_cos * yz) - xs;
        rotation_matrix[7] = 0.0;

        rotation_matrix[8] = (one_minus_cos * zx) - ys;
        rotation_matrix[9] = (one_minus_cos * yz) + xs;
        rotation_matrix[10] = (one_minus_cos * zz) + cos_angle;
        rotation_matrix[11] = 0.0;

        rotation_matrix[12] = 0.0;
        rotation_matrix[13] = 0.0;
        rotation_matrix[14] = 0.0;
        rotation_matrix[15] = 1.0;

        // It is necessary to save the current state of current_matrix to do Rotation*Current
        // since matrix multiplication is not conmutative
        GLdouble backup_current_matrix[16];

        for(int i=0; i<16; i++){
            backup_current_matrix[i] = current_matrix[i];
        }

        I_my_glLoadMatrixd(rotation_matrix);
        matrix_multiply(backup_current_matrix);
    }
}

void I_my_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glRotated((GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z);
}

void I_my_glScaled(GLdouble x, GLdouble y, GLdouble z)
{
    current_matrix[0]  *= x;
    current_matrix[1]  *= x;
    current_matrix[2]  *= x;
    current_matrix[3]  *= x;

    current_matrix[4]  *= y;
    current_matrix[5]  *= y;
    current_matrix[6]  *= y;
    current_matrix[7]  *= y;

    current_matrix[8]  *= z;
    current_matrix[9]  *= z;
    current_matrix[10] *= z;
    current_matrix[11] *= z;
}

void I_my_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glScaled((GLdouble)x, (GLdouble)y, (GLdouble)z);
}

// copy current matrix to m
void I_my_glGetMatrixf(GLfloat *m)
{
    for(int i=0; i<16; i++){
        m[i] = (GLfloat) current_matrix[i];
    }
}

void I_my_glGetMatrixd(GLdouble *m)
{
    for(int i=0; i<16; i++){
        m[i] = current_matrix[i];
    }
}

// remember to normalize vectors
void I_my_gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
    GLdouble centerX, GLdouble centerY, GLdouble centerZ,
    GLdouble upX, GLdouble upY, GLdouble upZ)
{
    GLdouble m[16];
    GLdouble x[3], y[3], z[3];
    GLdouble mag;

    /* Make rotation matrix */

    /* Z vector */
    z[0] = eyeX - centerX;
    z[1] = eyeY - centerY;
    z[2] = eyeZ - centerZ;
    mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {          /* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }

    /* Y vector */
    y[0] = upX;
    y[1] = upY;
    y[2] = upZ;

    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];

    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];

    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */

    mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }

    mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }

#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    matrix_multiply(m);

    /* Translate Eye to Origin */
    I_my_glTranslatef(-eyeX, -eyeY, -eyeZ);
}

void I_my_glFrustum(GLdouble left, GLdouble right, GLdouble bottom,
        GLdouble top, GLdouble zNear, GLdouble zFar)
{
    float       deltaX = right - left;
    float       deltaY = top - bottom;
    float       deltaZ = zFar - zNear;
    GLdouble    frust[16];

    if ( (zNear <= 0.0) || (zFar <= 0.0) ||
            (deltaX <= 0.0) || (deltaY <= 0.0) || (deltaZ <= 0.0) )
        return;

    frust[0] = 2.0 * zNear / deltaX;
    frust[1] = frust[2] = frust[3] = 0.0;

    frust[5] = 2.0 * zNear / deltaY;
    frust[4] = frust[6] = frust[7] = 0.0;

    frust[8] = (right + left) / deltaX;
    frust[9] = (top + bottom) / deltaY;
    frust[10] = -(zNear + zFar) / deltaZ;
    frust[11] = -1.0;

    frust[14] = -2.0 * zNear * zFar / deltaZ;
    frust[12] = frust[13] = frust[15] = 0.0;

    matrix_multiply(frust);
}

// remember to convert fovy from degree to radius before calling tan
void I_my_gluPerspective(GLdouble fovy, GLdouble aspect,
    GLdouble zNear, GLdouble zFar)
{
    GLdouble frustumW, frustumH;

    frustumH = tanf( fovy / 360.0 * PI ) * zNear;
    frustumW = frustumH * aspect;

    I_my_glFrustum(-frustumW, frustumW, -frustumH, frustumH, zNear, zFar);
}
