cmd_/home/pi/kitCLB/ioctl2true/char.mod := printf '%s\n'   char.o | awk '!x[$$0]++ { print("/home/pi/kitCLB/ioctl2true/"$$0) }' > /home/pi/kitCLB/ioctl2true/char.mod
