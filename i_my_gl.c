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

    for (i=0; i<4; i++)
	{
		result[i*4]   =  (current_matrix->m[i*4]   * b[0]) +
					     (current_matrix->m[i*4+1] * b[4]) +
					     (current_matrix->m[i*4+2] * b[8]) +
					     (current_matrix->m[i*4+3] * b[12]) ;

		result[i*4+1] =  (current_matrix->m[i*4]   * b[1]) + 
						 (current_matrix->m[i*4+1] * b[5]) +
						 (current_matrix->m[i*4+2] * b[9]) +
						 (current_matrix->m[i*4+3] * b[13]) ;

		result[i*4+2] =  (current_matrix->m[i*4]   * b[2]) + 
					     (current_matrix->m[i*4+1] * b[6]) +
						 (current_matrix->m[i*4+2] * b[10]) +
						 (current_matrix->m[i*4+3] * b[14]) ;

		result[i*4+3] =  (current_matrix->m[i*4]   * b[3]) + 
						 (current_matrix->m[i*4+1] * b[7]) +
						 (current_matrix->m[i*4+2] * b[11]) +
						 (current_matrix->m[i*4+3] * b[15]) ;
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
    current_stack = &mode;
}

// overwrite current matrix with identity matrix
void I_my_glLoadIdentity(void)
{
    for(int i=0; i<16; i++){
        current_matrix[i] = identity[i];
    }
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
    // ...
}

void I_my_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glTranslated((GLdouble)x, (GLdouble)y, (GLdouble)z);
}

// remember to normalize vector (x, y, z), and to convert angle from degree to radius before calling sin and cos
void I_my_glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
    // ...
}

void I_my_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glRotated((GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z);
}

void I_my_glScaled(GLdouble x, GLdouble y, GLdouble z)
{
    // ...
}

void I_my_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glScaled((GLdouble)x, (GLdouble)y, (GLdouble)z);
}

// copy current matrix to m
void I_my_glGetMatrixf(GLfloat *m)
{
    // ...
}

void I_my_glGetMatrixd(GLdouble *m)
{
    // ...
}

// remember to normalize vectors
void I_my_gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
    GLdouble centerX, GLdouble centerY, GLdouble centerZ,
    GLdouble upX, GLdouble upY, GLdouble upZ)
{
    // ...
}

void I_my_glFrustum(GLdouble left, GLdouble right, GLdouble bottom,
    GLdouble top, GLdouble zNear, GLdouble zFar)
{
    // ...
}

// remember to convert fovy from degree to radius before calling tan
void I_my_gluPerspective(GLdouble fovy, GLdouble aspect,
    GLdouble zNear, GLdouble zFar)
{
    // ...
}
