@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64

cd Build\WebDll
cl -DDLL=0 -DWDLL=0 T:\SandCatLanguage\SandCat_Runner\SandCat_Runner.cpp -Zi -nologo user32.lib /clr:pure -FoSandCat_Web.dll

cd ..\WindowsDll
cl -DDLL=0 T:\SandCatLanguage\SandCat_Runner\SandCat_Runner.cpp -Zi -nologo /EHsc user32.lib -FoSandCat_Windows.dll

cd ..\Exe
cl T:\SandCatLanguage\SandCat_Runner\SandCat_Runner.cpp /LD -nologo /EHsc user32.lib -FeSandCat_Exe.exe

cd ..\WindowsDll
copy /Y SandCat_Windows.dll T:\SandCatLanguage\SandCat_Unity\Assets\SandCat_Runner\Runner