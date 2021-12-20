set PATH_TO_MAKE="C:\Program Files (x86)\GnuWin32\bin\make.exe"

for %%i IN (SRC/*.c) DO (
    make SOURCE_FILE="SRC\%%i"
)

@rem Remove unused files for clarity
@del Output\*.o Output\*.map Output\*.elf Output\*.hex


@echo;
@echo;
@echo **********************************************
@echo ALL DONE!
@echo **********************************************

pause