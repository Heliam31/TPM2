cmd_/home/pi/kitCLB/ioctl2true/Module.symvers :=  sed 's/ko$$/o/'  /home/pi/kitCLB/ioctl2true/modules.order | scripts/mod/modpost -m -a    -o /home/pi/kitCLB/ioctl2true/Module.symvers -e -i Module.symvers -T - 