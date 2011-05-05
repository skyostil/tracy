all: win32player.exe

LIBS=kernel32.lib user32.lib gdi32.lib shlwapi.lib comdlg32.lib winmm.lib libGLES_CM.lib glut32.lib
CC=cl
CFLAGS=/c /Zi /I.
LD=link
LDFLAGS=/nologo /DEBUG /LIBPATH:. /LIBPATH:..\..\lib
RM=del

win32player.exe: win32player.obj
	$(LD) $(LDFLAGS) win32player.obj /out:win32player.exe $(LIBS)

win32player.obj: win32player.c *.inl
	@$(CC) $(CFLAGS) win32player.c

clean:
	@$(RM) win32player.obj win32player.exe
