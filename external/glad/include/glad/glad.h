#ifndef GLAD_H
#define GLAD_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
    #define GLAD_API_PTR __stdcall
#else
    #define GLAD_API_PTR
#endif

typedef void* (*GLADloadproc)(const char *name);

int gladLoadGL(void);
int gladLoadGLLoader(GLADloadproc);

#include <KHR/khrplatform.h>
#include <windows.h>
#include <GL/gl.h>

#ifdef __cplusplus
}
#endif

#endif
