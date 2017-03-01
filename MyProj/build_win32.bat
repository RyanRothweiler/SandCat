@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" 
pushd build

cl ../main.cpp -nologo -DEBUG=1 -Zi /EHsc /link /LIBPATH:"T:\clingo\c-api" clingo.lib 

popd