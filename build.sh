#!/bin/sh

btime=`date +%Y-%m-%d`
case $1 in
     x5)
        rm -rf files
        cp ylx_files/x5/x5.config .config
        cp ylx_files/x5/files/ . -rf
        cp logo/logo_linux_clut224.ppm.5g build_dir/target-aarch64_cortex-a53_musl/linux-ipq60xx_generic/linux-5.4.164/drivers/video/logo/logo_linux_clut224.ppm
        make V=s $2
        cp bin/targets/ipq60xx/generic/x5-21.02-v1.0-ipq60xx-generic-ylx_x5-squashfs-nand-factory.ubi firm/$btime-x5.bin
        ;;
      x2)
         # rm -rf files
         cp ylx_files/x2/x5.config .config
         # cp ylx_files/x5/files/ . -rf
         make V=s $2
         ;;
     *)
        echo "please input ./build.sh x5/x2 [-j4]."
        ;;
esac
