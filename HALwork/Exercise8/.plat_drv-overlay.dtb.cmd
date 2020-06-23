cmd_/home/stud/exer82/Exercise6/plat_drv-overlay.dtb := mkdir -p /home/stud/exer82/Exercise6/ ; arm-poky-linux-gnueabi-gcc -E -Wp,-MD,/home/stud/exer82/Exercise6/.plat_drv-overlay.dtb.d.pre.tmp -nostdinc -I./scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o /home/stud/exer82/Exercise6/.plat_drv-overlay.dtb.dts.tmp /home/stud/exer82/Exercise6/plat_drv-overlay.dts ; ./scripts/dtc/dtc -O dtb -o /home/stud/exer82/Exercise6/plat_drv-overlay.dtb -b 0 -i/home/stud/exer82/Exercise6/ -i./scripts/dtc/include-prefixes -Wno-unit_address_vs_reg -Wno-simple_bus_reg -Wno-unit_address_format -Wno-pci_bridge -Wno-pci_device_bus_num -Wno-pci_device_reg  -d /home/stud/exer82/Exercise6/.plat_drv-overlay.dtb.d.dtc.tmp /home/stud/exer82/Exercise6/.plat_drv-overlay.dtb.dts.tmp ; cat /home/stud/exer82/Exercise6/.plat_drv-overlay.dtb.d.pre.tmp /home/stud/exer82/Exercise6/.plat_drv-overlay.dtb.d.dtc.tmp > /home/stud/exer82/Exercise6/.plat_drv-overlay.dtb.d

source_/home/stud/exer82/Exercise6/plat_drv-overlay.dtb := /home/stud/exer82/Exercise6/plat_drv-overlay.dts

deps_/home/stud/exer82/Exercise6/plat_drv-overlay.dtb := \

/home/stud/exer82/Exercise6/plat_drv-overlay.dtb: $(deps_/home/stud/exer82/Exercise6/plat_drv-overlay.dtb)

$(deps_/home/stud/exer82/Exercise6/plat_drv-overlay.dtb):
