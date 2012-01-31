/*
 * OpenGL demonstration program for ICS Computer Graphics courses
 * $Id: inputModule.c,v 1.2 2002/04/03 00:36:06 mshafae Exp $
 *
 */

#include <signal.h>
//#include <unistd.h>

#include "inputModule.h"

#include "my_gl.h"

static int motionMode;
static int startX;
static int startY;
static GLfloat angle = 20;    /* in degrees */
static GLfloat angle2 = 30;   /* in degrees */
static GLfloat xdistance = 0.0;
static GLfloat ydistance = 0.0;
static GLfloat zdistance = 5.0;


void readKeyboard( unsigned char key, int x, int y ){
  switch( key ){
  case  0x1B: /* esc */
  case  'q':
//    kill( getpid(), SIGHUP );
    break;
  case ' ':

    break;
  case ',':

    break;
  case '.':

    break;
  case 'r':
    /* reset initial view parameters */
    angle = 20;
    angle2 = 30;
    xdistance = 0.0;
    ydistance = 0.0;
    zdistance = 5.0;
    break;
  default:
    break;
  }
  glutPostRedisplay( );
}

void readSpecialKeys( int key, int x, int y ){
  switch( key ){
  case GLUT_KEY_UP:

    break;
  case GLUT_KEY_DOWN:

    break;
  case GLUT_KEY_RIGHT:

    break;
  case GLUT_KEY_LEFT:

    break;
  }
  glutPostRedisplay( );
}

void mouseButtHandler(int button, int state, int x, int y)
{
  motionMode = 0;

  switch( button ){
  case GLUT_LEFT_BUTTON:
    if( state == GLUT_DOWN ){
      /* Rotate object */
      motionMode = 1;
      startX = x;
      startY = y;
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    if( state == GLUT_DOWN ){
      /* Translate object */
      motionMode = 2;
      startX = x;
      startY = y;
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if( state == GLUT_DOWN ){
      /* Zoom */
      motionMode = 3;
      startX = x;
      startY = y;
    }
    break;
  }
  glutPostRedisplay( );
}

void mouseMoveHandler( int x, int y ){
  switch(motionMode){
  case 0:
    /* No mouse button is pressed... do nothing */
    /* return; */
    break;

  case 1:
    /* Calculate the rotations */
    angle = angle + (x - startX);
    angle2 = angle2 + (y - startY);
    startX = x;
    startY = y;
    break;

  case 2:
    xdistance = xdistance - (x - startX)/100.0;
    ydistance = ydistance - (y - startY)/100.0;
    startX = x;
    startY = y;
    break;

  case 3:
    zdistance = zdistance - (y - startY)/10.0;
    startX = x;
    startY = y;
    break;
  }

  glutPostRedisplay( );
}

void setUserView( ){
  glLoadIdentity( );
  /* gluLookAt( 0.0,0.0,distance,0.0,0.0,0.0,0.0,1.0,0.0 ); */

  glTranslatef( -xdistance, ydistance, -zdistance );
  glRotatef( angle2, 1.0, 0.0, 0.0 );
  glRotatef( angle, 0.0, 1.0, 0.0 );
}

