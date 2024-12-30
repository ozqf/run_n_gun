
@set workingDir=_working
@set buildDir=engine_temp

@cd..
@if not exist %workingDir% mkdir %workingDir%
@cd %workingDir%
@if not exist bin mkdir bin
@if not exist %buildDir% mkdir %buildDir%

@cd %buildDir%
@set outputParam=/Fe../bin/run_n_gun.exe

@set compilerFlags=-nologo -MT -WX -W4 -wd4100 -wd4201 -wd4189 -wd4505 /Zi
@set inc1=/I../../engine
@set inc2=/I../../lib/glad
@set inc3=/I../../lib/glfw3_vc2015
@set compIn1=../../engine/*.cpp
@set compIn2=../../lib/glad/*.c
@rem @set linkInputA=user32.lib opengl32.lib Gdi32.lib shell32.lib

@cl %compilerFlags% %inc1% %inc2% %inc3% %compIn1% %compIn2% %outputParam% /c
@lib -nologo /out:ze.lib *.obj

@cd..\..
@cd buildwin
