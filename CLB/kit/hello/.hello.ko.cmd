cmd_/home/pi/kitCLB/hello/hello.ko := ld -r -EL  -maarch64elf -z noexecstack --build-id=sha1  -T scripts/module.lds -o /home/pi/kitCLB/hello/hello.ko /home/pi/kitCLB/hello/hello.o /home/pi/kitCLB/hello/hello.mod.o;  true