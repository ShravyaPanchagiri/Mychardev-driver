savedcmd_mychardev.o := ld -m elf_x86_64 -z noexecstack --no-warn-rwx-segments   -r -o mychardev.o @mychardev.mod 
