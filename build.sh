#!/bin/sh

#
rm -rf package/kernel/mac80211

btime=`date +%Y-%m-%d`
case $1 in
x5)
echo "开始下载dl文件"
#如果文件不存在，则创建文件
tempFile="dl.zip"
if [ ! -f "$tempFile" ]; then
wget http://55860.com/bak/dl.zip
unzip -q -o dl.zip
fi
echo "开始编译"

echo "开始编译qsdk 5.4 ipq6x咯"
      #   tar zxvf package/kernel/mac80211.orig.tar.gz -C package/kernel/
        tar zxvf package/kernel/mac80211.ipq60xx.tar.gz -C package/kernel/
        rm -rf files
scripts/feeds clean
scripts/feeds update -a
scripts/feeds install -a
        cp ylx_files/x5/x5.config .config
        
        make V=s $2
        echo "编译完完成"
        cp bin/targets/ipq60xx/generic/x5-21.02-v1.0-ipq60xx-generic-ylx_x5-squashfs-nand-factory.ubi firm/$btime-x5.bin
        ;;
      x2)
         # rm -rf files
         tar zxvf package/kernel/mac80211.orig.tar.gz -C package/kernel/
      #   tar zxvf package/kernel/mac80211.ipq60xx.tar.gz -C package/kernel/
         cp ylx_files/x2/x2.config .config
         # cp ylx_files/x5/files/ . -rf
         make V=s $2
         ;;
     *)
        echo "please input ./build.sh x5/x2 [-j4]."
        ;;
esac
