call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64

rem Web .net dll
cd Build\WebDll
cl -DDLL=0 -DWDLL=0 /LD T:\SandCatLanguage\SandCat_Runner\SandCat_Runner.cpp -Zi -nologo user32.lib /clr:pure
copy /Y SandCat_Runner.dll SandCat_Runner_Web.dll 
copy /Y SandCat_Runner_Web.dll T:\SandCatLanguage\SandCat_Unity\Assets\SandCat_Runner\Runner

rem Windows dll
cd ..\WindowsDll
cl -DDLL=0 /LD T:\SandCatLanguage\SandCat_Runner\SandCat_Runner.cpp -Zi -nologo /EHsc user32.lib
copy /Y SandCat_Runner.dll SandCat_Runner_Windows.dll 
copy /Y SandCat_Runner_Windows.dll T:\SandCatLanguage\SandCat_Unity\Assets\SandCat_Runner\Runner

rem Windows exe
cd ..\Exe
cl T:\SandCatLanguage\SandCat_Runner\SandCat_Runner.cpp /LD -nologo /EHsc user32.lib -FeSandCat_Exe.exe