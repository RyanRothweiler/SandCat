@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
cl htmlpp.cpp
cl T:\SandCatLanguage\SandCat_Runner\SandCat_Runner.cpp /LD -nologo /EHsc user32.lib

copy /Y SandCat_Runner.dll T:\SandCatLanguage\SandCat_Unity\Assets\SandCat_Runner\Runner