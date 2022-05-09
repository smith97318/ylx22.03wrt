define Device/FitImage
  KERNEL_SUFFIX := -fit-uImage.itb
  KERNEL = kernel-bin | gzip | fit gzip $$(DEVICE_DTS_DIR)/$$(DEVICE_DTS).dtb
  KERNEL_NAME := Image
endef

define Device/FitImageLzma
  KERNEL_SUFFIX := -fit-uImage.itb
  KERNEL = kernel-bin | lzma | fit lzma $$(DEVICE_DTS_DIR)/$$(DEVICE_DTS).dtb
  KERNEL_NAME := Image
endef

define Device/UbiFit
  KERNEL_IN_UBI := 1
  IMAGES := nand-factory.ubi nand-sysupgrade.bin
  IMAGE/nand-factory.ubi := append-ubi
  IMAGE/nand-sysupgrade.bin := sysupgrade-tar | append-metadata
endef

define Device/ylx_y6000
  $(call Device/FitImage)
  $(call Device/UbiFit)
  DEVICE_VENDOR := YLX
  DEVICE_MODEL := Y6000
  DEVICE_DTS_CONFIG := config@cp03-c1
  SOC := ipq6018
  BLOCKSIZE := 128k
  PAGESIZE := 2048
endef
TARGET_DEVICES += ylx_y6000

define Device/ylx_x5
  $(call Device/FitImage)
  $(call Device/UbiFit)
  DEVICE_VENDOR := YLX
  DEVICE_MODEL := X5
  DEVICE_DTS_CONFIG := config@cp01-c3
  SOC := ipq6018
  BLOCKSIZE := 128k
  PAGESIZE := 2048
endef
TARGET_DEVICES += ylx_x5

define Device/ylx_x511
  $(call Device/FitImage)
  $(call Device/UbiFit)
  DEVICE_VENDOR := YLX
  DEVICE_MODEL := X511
  DEVICE_DTS_CONFIG := config@cp03-c1
  SOC := ipq6018
  BLOCKSIZE := 128k
  PAGESIZE := 2048
endef
TARGET_DEVICES += ylx_x511
