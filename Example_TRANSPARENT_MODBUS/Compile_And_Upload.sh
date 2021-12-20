#!/bin/bash
set -e

for f in SRC/*.c
do
    make SOURCE_FILE='$f'
done

# Remove unused files for clarity
rm -f Output/*.o Output/*.map Output/*.elf Output/*.hex


echo;
echo;
echo ----------------------------------------------
echo ALL DONE!
echo ----------------------------------------------

read -p "Press enter to continue"