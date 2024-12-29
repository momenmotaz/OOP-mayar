#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

static void* get_proc(const char *namez);

#ifdef _WIN32
#define GLAD_IMPL_UTIL_SSCANF sscanf_s
#else
#define GLAD_IMPL_UTIL_SSCANF sscanf
#endif

int gladLoadGLLoader(GLADloadproc load) {
    int version = 0;
    
    if(!load) {
        return 0;
    }
    
    return 1;
}

#ifdef __cplusplus
extern "C" {
#endif

int gladLoadGL(void) {
    int status = 0;

    if(status) {
        status = gladLoadGLLoader(&get_proc);
    }

    return status;
}

static void* get_proc(const char *namez) {
#ifdef _WIN32
    void* result = (void*)wglGetProcAddress(namez);
    if(!result) {
        static HMODULE glModule;
        if(!glModule) {
            glModule = LoadLibraryA("opengl32.dll");
        }
        if(glModule) {
            result = (void*)GetProcAddress(glModule, namez);
        }
    }
    return result;
#else
    return NULL;
#endif
}

#ifdef __cplusplus
}
#endif
