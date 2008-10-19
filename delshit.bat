del temp\*.* /q /s
del demo\sources\demo.ncb /q
del demo\sources\demo.suo /q
del steel\sources\steel.ncb /q
del steel\sources\steel.suo /q
del demo\bin\*.* /q /s
del ..\steel.res\sdk\steel_lib\*.* /q /s

cd demo
delete_logs.bat
de ..

del Labyrinth\sources\Labyrinth.ncb /q
del Labyrinth\sources\Labyrinth.suo /q
del steel\sources\steel.ncb /q
del steel\sources\steel.suo /q
del Labyrinth\bin\*.* /q /s
del ..\steel.res\sdk\steel_lib\*.* /q /s

cd Labyrinth
delete_logs.bat
de ..

