# "Run N Gun" Source Readme

2D Run N Gun shooter written in C/C++, using opengl and box2d.

## Source Folders / Modules

| folder | module/output | build script | notes |
|--------|--------|--------------|-------|
| engine | ze.lib | buildengine.bat | "Zealous Engine" - platform layer and opengl renderer |
| run_n_gun | run_n_gun.exe | buildrng.bat | Game, links to ze.lib |
| sandbox | ze_sandbox.exe | buildsandbox | Engine Testing and experiments executable, links to ze.lib |
| cli | zecli.exe | buildcli.bat | Command line tools |
| plugins | various | none |Custom libraries shared by game and sandbox such as physics wrapper. |
| lib | various | none | third party libraries used by modules above either in source or binary. Eg glfw/glad for opengl, box2d for game physics |

