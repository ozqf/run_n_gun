
@set workingDir=_working
@set buildDir=rng_temp

@cd..
@if not exist %workingDir% mkdir %workingDir%
@cd %workingDir%
@if not exist bin mkdir bin
@if not exist %buildDir% mkdir %buildDir%

@cd %buildDir%
@set outputParam=/Fe../bin/run_n_gun.exe

@set compilerFlags=-nologo -MT -WX -W4 -wd4100 -wd4201 -wd4189 -wd4505 /Zi
@set inc1=/I../../engine
@set inc2=/I../../rng
@set compIn1=../../rng/*.cpp
@set linkIn1=user32.lib opengl32.lib Gdi32.lib shell32.lib
@set linkIn2=../engine_temp/ze.lib ../../lib/glfw3_vc2019\glfw3_mt.lib

@cl %compilerFlags% %inc1% %inc2% %compIn1% %outputParam% /link %linkIn1% %linkIn2%

@cd..\..
@cd buildwin

