
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
@set inc3=/I../../lib/box2d
@set inc4=/I../../plugins
@set compIn1=../../rng/*.cpp
@set compIn2=../../plugins/box2d_wrapper/*.cpp ../../plugins/map2d/*.cpp
@set linkIn1=user32.lib opengl32.lib Gdi32.lib shell32.lib
@set linkIn2=../engine_temp/ze.lib ../../lib/glfw3_vc2019\glfw3_mt.lib ../../lib/box2d/box2d.lib

@cl %compilerFlags% %inc1% %inc2% %inc3% %inc4% %compIn1% %compIn2% %outputParam% /link %linkIn1% %linkIn2%

@cd..\..
@cd buildwin

