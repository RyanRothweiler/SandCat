@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" 
pushd build

cl ..\main.cpp /LD -nologo -DEBUG=1 -Zi /EHsc /link -incremental:no -opt:ref user32.lib

popd