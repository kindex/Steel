del temp\*.* /q /s
del demo\project\demo.ncb /q
del steel\project\steel.ncb /q
del demo\bin\*.* /q /s
del ..\steel.res\sdk\steel_lib\*.* /q /s

cd demo
delete_logs.bat
de ..

