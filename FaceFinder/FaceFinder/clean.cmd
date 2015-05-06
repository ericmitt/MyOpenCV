REM for /f "delims=" %%f in (list.txt) do del /q "%%f"

rd /q /s Debug
rd /q /s ipch
rd /q /s "FaceFinder\\Debug"
rd /q /s "FaceFinder\\Generated Files"
rd /q /s "winrtfacedetect\\Debug"
rd /q /s "winrtfacedetect\\Generated Files"

del /q *.sdf