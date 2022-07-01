@echo off

"C:\Program Files\CMake\bin\cmake.EXE" --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -Sc:/Users/kamwi/Development/Projects/TextractorSender -Bc:/Users/kamwi/Development/Projects/TextractorSender/build -G "Visual Studio 16 2019" -T host=x86 -A win32
"C:\Program Files\CMake\bin\cmake.EXE" --build c:/Users/kamwi/Development/Projects/TextractorSender/build --config Debug --target ALL_BUILD -j 18 --
IF EXIST "C:\Users\kamwi\Development\Applications\Textractor\x86\TextractorSender.dll" DEL "C:\Users\kamwi\Development\Applications\Textractor\x86\TextractorSender.dll"
IF EXIST "C:\Users\kamwi\Development\Applications\Textractor\x86\TextractorSender.xdll" DEL "C:\Users\kamwi\Development\Applications\Textractor\x86\TextractorSender.xdll"
echo C:\Users\kamwi\Development\Projects\TextractorSender\build\Debug\ | clip
