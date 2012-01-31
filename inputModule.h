/*
 * OpenGL demonstration program for ICS Computer Graphics courses
 * $Id: inputModule.h,v 1.1 2002/04/03 00:00:41 mshafae Exp $
 *
 */

#ifndef __INPUTMODULE_H
#define __INPUTMODULE_H

#include <stdlib.h>
#include <GL/glut.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int screenWidth;
extern int screenHeight;

void readKeyboard( unsigned char key, int x, int y );
void readSpecialKeys( int key, int x, int y );
void mouseButtHandler( int button, int state, int x, int y );
void mouseMoveHandler( int x, int y );
void setUserView( );

#ifdef __cplusplus
}
#endif

#endif









