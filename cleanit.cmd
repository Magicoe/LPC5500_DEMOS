@echo off
echo starting remove

del /f /s /q *.o
del /f /s /q *.bak
del /f /s /q *.__i
del /f /s /q *.crf
del /f /s /q *.iex
del /f /s /q *.lnp
del /f /s /q *.tra
del /f /s /q *.d

del /f /s /q *.bak
del /f /s /q *.lst
del /f /s /q *.uvguix.*
del /f /s /q *.uvgui.*
del /f /s /q *uvprojx.bak
del /f /s /q *.dep
del /f /s /q *.TMP
del /f /s /q *.axf
del /f /s /q *.map
del /f /s /q *.hex
del /f /s /q *.lst
del /f /s /q *.pbi
del /f /s /q *.pbi.cout
del /f /s /q *.pbd
del /f /s /q *.pbd.browse


del /f /s /q *.out

@echo off
echo %cd%


echo Deleted successful
pause


