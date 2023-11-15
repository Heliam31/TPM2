cmd_/home/pi/kitCLB/char/char.mod := printf '%s\n'   char.o | awk '!x[$$0]++ { print("/home/pi/kitCLB/char/"$$0) }' > /home/pi/kitCLB/char/char.mod
