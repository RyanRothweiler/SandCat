@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
cl ..\SandCat_Runner.cpp /LD -nologo /EHsc user32.lib