savedcmd_mychardev.mod := printf '%s\n'   src/mychardev.o | awk '!x[$$0]++ { print("./"$$0) }' > mychardev.mod
