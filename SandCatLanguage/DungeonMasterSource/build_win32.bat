@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" 
pushd build

cl ..\main.cpp -nologo -DEBUG=1 -Zi /EHsc /link -incremental:no -opt:ref opengl32.lib user32.lib gdi32.lib winmm.lib Xinput.lib

popd