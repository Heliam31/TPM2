cmd_/home/pi/kitCLB/hello/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/pi/kitCLB/hello/"$$0) }' > /home/pi/kitCLB/hello/hello.mod
