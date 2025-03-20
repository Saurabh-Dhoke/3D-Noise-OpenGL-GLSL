cls
cl.exe /c /EHsc /I "C:\\glew-2.1.0\\include" OGL.cpp Noise.cpp
rc.exe OGL.rc
link.exe  OGL.obj  Noise.obj OGL.res User32.lib GDI32.lib  winmm.lib /LIBPATH:"C:\\glew-2.1.0\\lib\\Release\\x64" /SUBSYSTEM:WINDOWS
OGL.exe

