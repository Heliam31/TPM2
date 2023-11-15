cmd_/home/pi/kitCLB/char/Module.symvers :=  sed 's/ko$$/o/'  /home/pi/kitCLB/char/modules.order | scripts/mod/modpost -m -a    -o /home/pi/kitCLB/char/Module.symvers -e -i Module.symvers -T - 
