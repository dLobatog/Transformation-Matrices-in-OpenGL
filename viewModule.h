/*
 * OpenGL demonstration program for ICS Computer Graphics courses
 * $Id: viewModule.h,v 1.1 2002/04/03 00:00:51 mshafae Exp $
 *
 */

#ifndef  __STEREOMODULE_H
#define  __STEREOMODULE_H

#ifdef __cplusplus
extern "C" {
#endif

#define IMAGE_WIDTH  512
#define IMAGE_HEIGHT 512

typedef struct{
        float fieldOfView;
        float aspect;
        float nearPlane;
        float farPlane;
}perspectiveData;


#ifdef __cplusplus
}
#endif

#endif
