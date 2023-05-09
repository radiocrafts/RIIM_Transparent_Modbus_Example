#!/bin/bash
#set -e

for f in SRC/*.c
do   
    cmake --fresh -G"Ninja" -S"./SRC" -B"./Output" -D"SOURCE_FILE=./$(basename -- $f)"
    cmake --build ./Output
    python ../../Framework/Tools/rc18xx_bootloader_utility.pyz load-image -f "./Output/$(basename -a -s .c -- $f).bin"
done

echo;
echo;
echo ----------------------------------------------
echo ALL DONE!
echo ----------------------------------------------

read -p "Press enter to continue"