/*
 * Copyright (C) 2012-2015 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX6Q SabreSD board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MX6QSABRE_COMMON_CONFIG_H
#define __MX6QSABRE_COMMON_CONFIG_H

#define CONFIG_MX6

#ifdef CONFIG_MX6SOLO
#define CONFIG_MX6DL
#endif

/* uncomment for PLUGIN mode support */
/* #define CONFIG_USE_PLUGIN */

/* uncomment for SECURE mode support */
/* #define CONFIG_SECURE_BOOT */

#ifdef CONFIG_SECURE_BOOT
#ifndef CONFIG_CSF_SIZE
#define CONFIG_CSF_SIZE 0x4000
#endif
#endif

#include "mx6_common.h"
#include <linux/sizes.h>

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#include <asm/arch/imx-regs.h>
#include <asm/imx-common/gpio.h>

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG

#define CONFIG_SYS_GENERIC_BOARD

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_MXC_GPIO

#define CONFIG_MXC_UART

#define CONFIG_CMD_FUSE
#ifdef CONFIG_CMD_FUSE
#define CONFIG_MXC_OCOTP
#endif

/* MMC Configs */
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR      0

#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_BOUNCE_BUFFER
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION

#define CONFIG_SUPPORT_EMMC_BOOT /* eMMC specific */

#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_CMD_NET
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_XCV_TYPE		RGMII
#define CONFIG_ETHPRIME			"FEC"
#define CONFIG_FEC_MXC_PHYADDR		1

#define CONFIG_PHYLIB
#define CONFIG_PHY_ATHEROS

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX              1
#define CONFIG_BAUDRATE                        115200

/* Command definition */
#include <config_cmd_default.h>

#define CONFIG_CMD_BMODE
#define CONFIG_CMD_BOOTZ
#define CONFIG_CMD_SETEXPR
#undef CONFIG_CMD_IMLS

#define CONFIG_BOOTDELAY               1

#define CONFIG_LOADADDR                        0x12000000
#define CONFIG_SYS_TEXT_BASE           0x17800000
#define CONFIG_SYS_MMC_IMG_LOAD_PART	1

#ifdef CONFIG_SYS_BOOT_NAND
#define CONFIG_MFG_NAND_PARTITION "mtdparts=gpmi-nand:64m(boot),16m(kernel),16m(dtb),-(rootfs) "
#else
#define CONFIG_MFG_NAND_PARTITION ""
#endif

#define CONFIG_MFG_ENV_SETTINGS \
	"mfgtool_args=setenv bootargs console=" CONFIG_CONSOLE_DEV ",115200 " \
		"rdinit=/linuxrc " \
		"g_mass_storage.stall=0 g_mass_storage.removable=1 " \
		"g_mass_storage.idVendor=0x066F g_mass_storage.idProduct=0x37FF "\
		"g_mass_storage.iSerialNumber=\"\" "\
		"enable_wait_mode=off "\
		CONFIG_MFG_NAND_PARTITION \
		"\0" \
		"initrd_addr=0x12C00000\0" \
		"initrd_high=0xffffffff\0" \
		"bootcmd_mfg=run mfgtool_args;bootz ${loadaddr} ${initrd_addr} ${fdt_addr};\0" \

#ifdef CONFIG_SUPPORT_EMMC_BOOT
#define EMMC_ENV \
	"emmcdev=2\0" \
	"update_emmc_firmware=" \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"if ${get_cmd} ${update_sd_firmware_filename}; then " \
			"if mmc dev ${emmcdev} 1; then "	\
				"setexpr fw_sz ${filesize} / 0x200; " \
				"setexpr fw_sz ${fw_sz} + 1; "	\
				"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
			"fi; "	\
		"fi\0"
#else
#define EMMC_ENV ""
#endif

#if defined(CONFIG_SYS_BOOT_NAND)
	/*
	 * The dts also enables the WEIN NOR which is mtd0.
	 * So the partions' layout for NAND is:
	 *     mtd1: 16M      (uboot)
	 *     mtd2: 16M      (kernel)
	 *     mtd3: 16M      (dtb)
	 *     mtd4: left     (rootfs)
	 */
#define CONFIG_EXTRA_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS \
	"fdt_addr=0x18000000\0" \
	"fdt_high=0xffffffff\0"	  \
	"bootargs=console=" CONFIG_CONSOLE_DEV ",115200 ubi.mtd=4 "  \
		"root=ubi0:rootfs rootfstype=ubifs "		     \
		"mtdparts=gpmi-nand:64m(boot),16m(kernel),16m(dtb),-(rootfs)\0"\
	"bootcmd=nand read ${loadaddr} 0x4000000 0x800000;"\
		"nand read ${fdt_addr} 0x5000000 0x100000;"\
		"bootz ${loadaddr} - ${fdt_addr}\0"

#elif defined(CONFIG_SYS_BOOT_SATA)

#define CONFIG_EXTRA_ENV_SETTINGS \
		CONFIG_MFG_ENV_SETTINGS \
		"fdt_addr=0x18000000\0" \
		"fdt_high=0xffffffff\0"   \
		"bootargs=console=" CONFIG_CONSOLE_DEV ",115200 \0"\
		"bootargs_sata=setenv bootargs ${bootargs} " \
			"root=/dev/sda1 rootwait rw \0" \
		"bootcmd_sata=run bootargs_sata; sata init; " \
			"sata read ${loadaddr} 0x800  0x4000; " \
			"sata read ${fdt_addr} 0x8000 0x800; " \
			"bootz ${loadaddr} - ${fdt_addr} \0" \
		"bootcmd=run bootcmd_sata \0"

#else
#define CONFIG_EXTRA_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS \
	"epdc_waveform=epdc_splash.bin\0" \
	"script=boot.scr\0" \
	"image=zImage\0" \
	"load_image_addr=0x12000000\0" \
	"load_dtb_addr=0x18000000\0" \
	"load_rootfs_addr=0x20000000\0" \
	"load_ver_usb_addr=0x12000000\0" \
	"load_ver_mmc_addr=0x12400000\0" \
	"load_update_flg_addr=0x12000000\0" \
	"boot_kernel_file=boot/zImage\0" \
	"boot_fdt_file=boot/imx6dl-sabresd.dtb\0" \
	"rootfs_image_file=boot/ramdisk.image.gz.uboot\0" \
	"update_force_flg=update/update_force.flag\0" \
	"mmc_version_file=version.txt\0" \
	"usb_version_file=update/version.txt\0" \
	"version_file_len=8\0" \
	"usb1updateflag=fatload usb 0:1 ${load_update_flg_addr} ${update_force_flg}\0" \
	"usb2updateflag=fatload usb 0:2 ${load_update_flg_addr} ${update_force_flg}\0" \
	"usb3updateflag=fatload usb 0:3 ${load_update_flg_addr} ${update_force_flg}\0" \
	"usb4updateflag=fatload usb 0:4 ${load_update_flg_addr} ${update_force_flg}\0" \
	"fdt_file=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"fdt_addr=0x18000000\0" \
	"boot_fdt=try\0" \
	"ip_dyn=yes\0" \
	"console=" CONFIG_CONSOLE_DEV "\0" \
	"fdt_high=0xffffffff\0"	  \
	"initrd_high=0xffffffff\0" \
	"mmcdev=" __stringify(CONFIG_SYS_MMC_ENV_DEV) "\0" \
	"mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"mmcroot=" CONFIG_MMCROOT " rootwait rw\0" \
	"mmcautodetect=yes\0" \
	"update_sd_firmware=" \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"if mmc dev ${mmcdev}; then "	\
			"if ${get_cmd} ${update_sd_firmware_filename}; then " \
				"setexpr fw_sz ${filesize} / 0x200; " \
				"setexpr fw_sz ${fw_sz} + 1; "	\
				"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
			"fi; "	\
		"fi\0" \
	EMMC_ENV	  \
	"smp=" CONFIG_SYS_NOSMP "\0"\
	"loadupdateflag=if run usb1updateflag;then echo updateflag in usb part 1;else if run usb2updateflag;then echo updateflag in usb part 2;else if run usb3updateflag;then echo updateflag in usb part 3;else if run usb4updateflag;then echo updateflag in usb part 4;fi;fi;fi;fi;\0" \
	"usb1image=fatload usb 0:1 ${load_image_addr} ${boot_kernel_file};\0" \
	"usb2image=fatload usb 0:2 ${load_image_addr} ${boot_kernel_file};\0" \
	"usb3image=fatload usb 0:3 ${load_image_addr} ${boot_kernel_file};\0" \
	"usb4image=fatload usb 0:4 ${load_image_addr} ${boot_kernel_file};\0" \
	"loadzimage=if run usb1image;then echo image in usb part 1;else if run usb2image;then echo image in usb part 2;else if run usb3image;then echo image in usb part 3;else if run usb4image;then echo image in usb part 4;fi;fi;fi;fi;\0" \
	"usb1dtb=fatload usb 0:1 ${load_dtb_addr} ${boot_fdt_file};\0" \
	"usb2dtb=fatload usb 0:2 ${load_dtb_addr} ${boot_fdt_file};\0" \
	"usb3dtb=fatload usb 0:3 ${load_dtb_addr} ${boot_fdt_file};\0" \
	"usb4dtb=fatload usb 0:4 ${load_dtb_addr} ${boot_fdt_file};\0" \
	"loaddtb=if run usb1dtb;then echo dtb in usb part 1;else if run usb2dtb;then echo dtb in usb part 2;else if run usb3dtb;then echo dtb in usb part 3;else if run usb4dtb;then echo dtb in usb part 4;fi;fi;fi;fi;\0" \
	"usb1rootfs=fatload usb 0:1 ${load_rootfs_addr} ${rootfs_image_file};\0" \
	"usb2rootfs=fatload usb 0:2 ${load_rootfs_addr} ${rootfs_image_file};\0" \
	"usb3rootfs=fatload usb 0:3 ${load_rootfs_addr} ${rootfs_image_file};\0" \
	"usb4rootfs=fatload usb 0:4 ${load_rootfs_addr} ${rootfs_image_file};\0" \
	"loadrootfs=if run usb1rootfs;then echo rootfs in usb part 1;else if run usb2rootfs;then echo rootfs in usb part 2;else if run usb3rootfs;then echo rootfs in usb part 3;else if run usb4rootfs;then echo rootfs in usb part 4;fi;fi;fi;fi;\0" \
	"usbboot=echo Booting from usb;run loadzimage;setenv bootargs console=ttymxc0,115200 root=/dev/ram rw init=/linuxrc initrd=0x20000040,40M;run loaddtb;run loadrootfs;bootz ${load_image_addr} ${load_rootfs_addr} ${load_dtb_addr};\0" \
	"usb1version=fatload usb 0:1 ${load_ver_usb_addr} ${usb_version_file};\0" \
	"usb2version=fatload usb 0:2 ${load_ver_usb_addr} ${usb_version_file};\0" \
	"usb3version=fatload usb 0:3 ${load_ver_usb_addr} ${usb_version_file};\0" \
	"usb4version=fatload usb 0:4 ${load_ver_usb_addr} ${usb_version_file};\0" \
	"loadusbversion=if run usb1version;then echo version file in usb part 1;else if run usb2version;then echo version file in usb part 2;else if run usb3version;then echo version file in usb part 3;else if run usb4version;then echo version file in usb part 4;fi;fi;fi;fi;\0" \
	"loadmmcversion=fatload mmc ${mmcdev}:${mmcpart} ${load_ver_mmc_addr} ${mmc_version_file};\0" \
	"mmcboot=echo Booting from mmc;fatload mmc ${mmcdev}:${mmcpart} ${load_image_addr} ${image};run mmcargs;if test ${boot_fdt} = yes || test ${boot_fdt} = try; then if run loadfdt;then bootz ${loadaddr} - ${fdt_addr}; else if test ${boot_fdt} = try; then bootz; else echo WARN: Cannot load the DT;fi;fi;else bootz;fi;\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} ${smp} " \
		"root=${mmcroot}\0" \
	"loadbootscript=" \
		"fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"netargs=setenv bootargs console=${console},${baudrate} ${smp} " \
		"root=/dev/nfs " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${image}; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0"


#define CONFIG_BOOTCOMMAND \
	"usb start;" \
	"mmc dev ${mmcdev};" \
	"if run loadupdateflag; then " \
		"run usbboot; " \
	"else " \
		"if run loadusbversion; then " \
			"if run loadmmcversion; then " \
				"if cmp.b ${load_ver_usb_addr} ${load_ver_mmc_addr} ${version_file_len}; then " \
					"run mmcboot; " \
				"else run usbboot; " \
				"fi; " \
			"else run usbboot; " \
			"fi; " \
		"else run mmcboot; " \
		"fi; " \
	"fi; "
#endif

#define CONFIG_ARP_TIMEOUT     200UL

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2     "> "
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE              1024

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS             256
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE

#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START       0x10000000
#define CONFIG_SYS_MEMTEST_END         0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH     0x10800000

#define CONFIG_SYS_LOAD_ADDR           CONFIG_LOADADDR

#define CONFIG_CMDLINE_EDITING
#define CONFIG_STACKSIZE               (128 * 1024)

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS           1
#define PHYS_SDRAM                     MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE          PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR       IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE       IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_SIZE			(8 * 1024)

#ifndef CONFIG_SYS_NOSMP
#define CONFIG_SYS_NOSMP
#endif

#if defined CONFIG_SYS_BOOT_SPINOR
#define CONFIG_SYS_USE_SPINOR
#define CONFIG_ENV_IS_IN_SPI_FLASH
#elif defined CONFIG_SYS_BOOT_EIMNOR
#define CONFIG_SYS_USE_EIMNOR
#define CONFIG_ENV_IS_IN_FLASH
#elif defined CONFIG_SYS_BOOT_NAND
#define CONFIG_SYS_USE_NAND
#define CONFIG_ENV_IS_IN_NAND
#elif defined CONFIG_SYS_BOOT_SATA
#define CONFIG_ENV_IS_IN_SATA
#define CONFIG_CMD_SATA
#else
#define CONFIG_ENV_IS_IN_MMC
#endif

#ifdef CONFIG_CMD_SATA
#define CONFIG_DWC_AHSATA
#define CONFIG_SYS_SATA_MAX_DEVICE	1
#define CONFIG_DWC_AHSATA_PORT_ID	0
#define CONFIG_DWC_AHSATA_BASE_ADDR	SATA_ARB_BASE_ADDR
#define CONFIG_LBA48
#define CONFIG_LIBATA
#endif

#ifdef CONFIG_SYS_USE_SPINOR
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_MXC_SPI
#define CONFIG_SF_DEFAULT_BUS  0
#define CONFIG_SF_DEFAULT_SPEED 20000000
#define CONFIG_SF_DEFAULT_MODE (SPI_MODE_0)
#endif

#ifdef CONFIG_SYS_USE_EIMNOR
#undef CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_FLASH_BASE           WEIM_ARB_BASE_ADDR
#define CONFIG_SYS_FLASH_SECT_SIZE     (128 * 1024)
#define CONFIG_SYS_MAX_FLASH_BANKS 1    /* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT 256   /* max number of sectors on one chip */
#define CONFIG_SYS_FLASH_CFI            /* Flash memory is CFI compliant */
#define CONFIG_FLASH_CFI_DRIVER         /* Use drivers/cfi_flash.c */
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE /* Use buffered writes*/
#define CONFIG_SYS_FLASH_EMPTY_INFO
#endif

#ifdef CONFIG_SYS_USE_NAND
#define CONFIG_CMD_NAND
#define CONFIG_CMD_NAND_TRIMFFS

/* NAND stuff */
#define CONFIG_NAND_MXS
#define CONFIG_SYS_MAX_NAND_DEVICE     1
#define CONFIG_SYS_NAND_BASE           0x40000000
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_ONFI_DETECTION

/* DMA stuff, needed for GPMI/MXS NAND support */
#define CONFIG_APBH_DMA
#define CONFIG_APBH_DMA_BURST
#define CONFIG_APBH_DMA_BURST8
#endif

#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_ENV_OFFSET		(8 * 64 * 1024)
#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_OFFSET              (768 * 1024)
#define CONFIG_ENV_SECT_SIZE           (64 * 1024)
#define CONFIG_ENV_SPI_BUS             CONFIG_SF_DEFAULT_BUS
#define CONFIG_ENV_SPI_CS              CONFIG_SF_DEFAULT_CS
#define CONFIG_ENV_SPI_MODE            CONFIG_SF_DEFAULT_MODE
#define CONFIG_ENV_SPI_MAX_HZ          CONFIG_SF_DEFAULT_SPEED
#elif defined(CONFIG_ENV_IS_IN_FLASH)
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE                        CONFIG_SYS_FLASH_SECT_SIZE
#define CONFIG_ENV_SECT_SIZE           CONFIG_SYS_FLASH_SECT_SIZE
#define CONFIG_ENV_OFFSET              (4 * CONFIG_SYS_FLASH_SECT_SIZE)
#elif defined(CONFIG_ENV_IS_IN_NAND)
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_OFFSET              (8 << 20)
#define CONFIG_ENV_SECT_SIZE           (128 << 10)
#define CONFIG_ENV_SIZE                        CONFIG_ENV_SECT_SIZE
#elif defined(CONFIG_ENV_IS_IN_SATA)
#define CONFIG_ENV_OFFSET		(768 * 1024)
#define CONFIG_SATA_ENV_DEV		0
#define CONFIG_SYS_DCACHE_OFF /* remove when sata driver support cache */
#endif

#define CONFIG_OF_LIBFDT

#ifndef CONFIG_SYS_DCACHE_OFF
#define CONFIG_CMD_CACHE
#endif

/* I2C Configs */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_SPEED		  100000

/* Framebuffer */
#define CONFIG_VIDEO
#define CONFIG_VIDEO_IPUV3
#define CONFIG_CFB_CONSOLE
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_BMP_LOGO
#ifdef CONFIG_MX6DL                                                             
#define CONFIG_IPUV3_CLK 198000000
#else
#define CONFIG_IPUV3_CLK 264000000
#endif
#define CONFIG_IMX_HDMI
#define CONFIG_IMX_VIDEO_SKIP

#ifndef CONFIG_SPL
/*
 * USE our legacy UDC driver, but not CI_UDC
 * USE our legacy fastboot way, but not gnl
 */
#if 0
#define CONFIG_CI_UDC
#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_DUALSPEED

#define CONFIG_USB_GADGET
#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_GADGET_MASS_STORAGE
#define CONFIG_USBDOWNLOAD_GADGET
#define CONFIG_USB_GADGET_VBUS_DRAW	2

#define CONFIG_G_DNL_VENDOR_NUM		0x0525
#define CONFIG_G_DNL_PRODUCT_NUM	0xa4a5
#define CONFIG_G_DNL_MANUFACTURER	"FSL"
#endif
#endif

#if defined(CONFIG_ANDROID_SUPPORT)
#include "mx6sabreandroid_common.h"
#endif
#endif                         /* __MX6QSABRE_COMMON_CONFIG_H */
