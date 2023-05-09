rem For all source files in the SRC directory, compile and upload

for %%i IN (SRC/*.c) DO (
   cmake --fresh -G"Ninja" -S"./SRC" -B"./Output" -D"SOURCE_FILE=./%%i"
   cmake --build ./Output
   python ..\..\Framework\Tools\rc18xx_bootloader_utility.pyz load-image -f "./Output/%%~ni.bin"
)


@echo;
@echo;
@echo **********************************************
@echo ALL DONE!
@echo **********************************************

pause