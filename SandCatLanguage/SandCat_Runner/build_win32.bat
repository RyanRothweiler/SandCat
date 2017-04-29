@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
cl ..\SandCat_Runner.cpp -Zi -nologo /EHsc user32.lib
cl ..\SandCat_Runner.cpp /LD -nologo /EHsc user32.lib