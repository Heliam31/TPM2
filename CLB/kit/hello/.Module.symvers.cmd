cmd_/home/pi/kitCLB/hello/Module.symvers :=  sed 's/ko$$/o/'  /home/pi/kitCLB/hello/modules.order | scripts/mod/modpost -m -a    -o /home/pi/kitCLB/hello/Module.symvers -e -i Module.symvers -T - 
