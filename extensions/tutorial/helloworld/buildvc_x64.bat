cls
call ../../../language/src/locatevc.bat x64
cl /c /DEBUG mylib.c -I"..\..\..\language\include"
link /DEBUG mylib.obj  ..\..\..\lib\ring.lib /DLL /OUT:mylib.dll
del mylib.obj