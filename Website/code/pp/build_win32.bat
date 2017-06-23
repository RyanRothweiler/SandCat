@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
cl htmlpp.cpp -nologo /EHsc -Zi