
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
@set linkInputA=user32.lib opengl32.lib Gdi32.lib shell32.lib

@cl %compilerFlags% %includes% %compIn1% %outputParam% /link %linkInputA%

@cd..\..
@cd buildwin
