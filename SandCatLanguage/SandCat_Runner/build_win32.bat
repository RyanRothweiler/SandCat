call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64

rem Windows dll
cd Build\WindowsDll
cl -DDLL=0 /LD T:\SandCatLanguage\SandCat_Runner\SandCat_Runner.cpp -Zi -nologo /EHsc user32.lib
copy /Y SandCat_Runner.dll T:\SandCatLanguage\SandCat_Unity\Assets\SandCat_Runner\Runner

rem Windows exe
cd ..\Exe
cl T:\SandCatLanguage\SandCat_Runner\SandCat_Runner.cpp /LD -nologo /EHsc user32.lib -FeSandCat_Exe.exe

rem Copy sandcat source to the unity proj
copy /Y T:\SandCatLanguage\SandCat_Runner\SandCat_Runner.cpp T:\SandCatLanguage\SandCat_Unity\Assets\Plugins