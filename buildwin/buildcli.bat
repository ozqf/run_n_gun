
@set workingDir=_working

@cd..
@if not exist %workingDir% mkdir %workingDir%
@cd %workingDir%
@if not exist bin mkdir bin
@if not exist buildwin_temp mkdir buildwin_temp

@cd buildwin_temp
@set outputParam=/Fe../bin/zecli.exe

@set compilerFlags=-nologo -MT -WX -W4 -wd4100 -wd4201 -wd4189 -wd4505 /Zi
@set includes=/I../../engine
@set compIn1=../../cli/*.cpp
@set link1=user32.lib opengl32.lib Gdi32.lib shell32.lib
@set link2=../engine_temp/ze.lib

@cl %compilerFlags% %includes% %compIn1% %outputParam% /link %link1% %link2%

@cd..\..
@cd buildwin
