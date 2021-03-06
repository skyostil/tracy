# 1 "egl.h"



 




 

































# 1 "i:\\epoc32\\include\\GLES/gl.h" 1



 








 

























































typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef void GLvoid;
typedef int GLfixed;
typedef int GLclampx;

typedef int             GLintptr;
typedef int             GLsizeiptr;

 

 


 











 




 



 








 









 









 
 
 



 
 
 
 
 







 
 

 
 

 
 
 
 

 




 
 
 
 
 
 
 
 
 

 










 
 
 
 
 
 
 
 
















 








 
 



 






 



 
























































































 
 
 
 
 






 




 







 



 











 







 

















 
 

 



 
 
 

 




 
 
 
 
 

 






 



 
 




 



 
 
 
 
 
 
 
 
 

 
 




 

 





 
 
 
 
 

 


 

 

 



 


 



 
 
 





 






 
 

 



































 



 











 
 
 
 
 

 









 




















 





























 
 
 

 


 




 


















 






 


 




# 43 "egl.h" 2

# 1 "i:\\epoc32\\include\\GLES/egltypes.h" 1
 
 
 
 




 




typedef int EGLBoolean;
typedef int EGLint;
typedef int EGLDisplay;
typedef int EGLConfig;
typedef int EGLSurface;
typedef int EGLContext;

typedef int NativeDisplayType;
typedef void* NativeWindowType;
typedef void* NativePixmapType;








# 44 "egl.h" 2


 


















# 88 "egl.h"


 





 





 

















 

 


















 














 












 






 






 










 




 





 




 

 






EGLint   eglGetError (void);

EGLDisplay   eglGetDisplay (NativeDisplayType display);
EGLBoolean   eglInitialize (EGLDisplay dpy, EGLint *major, EGLint *minor);
EGLBoolean   eglTerminate (EGLDisplay dpy);
const char *   eglQueryString (EGLDisplay dpy, EGLint name);



void (*   eglGetProcAddress (const char *procname))();


EGLBoolean   eglGetConfigs (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config);
EGLBoolean   eglChooseConfig (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
EGLBoolean   eglGetConfigAttrib (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value);

EGLSurface   eglCreateWindowSurface (EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list);
EGLSurface   eglCreatePixmapSurface (EGLDisplay dpy, EGLConfig config, NativePixmapType pixmap, const EGLint *attrib_list);
EGLSurface   eglCreatePbufferSurface (EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list);
EGLBoolean   eglDestroySurface (EGLDisplay dpy, EGLSurface surface);
EGLBoolean   eglQuerySurface (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);

EGLBoolean   eglSurfaceAttrib (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
EGLBoolean   eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
EGLBoolean   eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer);

 
EGLBoolean   eglSwapInterval(EGLDisplay dpy, EGLint interval);

EGLContext   eglCreateContext (EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list);
EGLBoolean   eglDestroyContext (EGLDisplay dpy, EGLContext ctx);
EGLBoolean   eglMakeCurrent (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
EGLContext   eglGetCurrentContext (void);
EGLSurface   eglGetCurrentSurface (EGLint readdraw);
EGLDisplay   eglGetCurrentDisplay (void);
EGLBoolean   eglQueryContext (EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value);

EGLBoolean   eglWaitGL (void);
EGLBoolean   eglWaitNative (EGLint engine);
EGLBoolean   eglSwapBuffers (EGLDisplay dpy, EGLSurface draw);
EGLBoolean   eglCopyBuffers (EGLDisplay dpy, EGLSurface surface, NativePixmapType target);







