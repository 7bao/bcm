EXEC = Abcm_manage_vlan_test

#5728 toolchain
CROSS_COMPILE_PATH = /home/bqq/am5728/gcc/bin

#svn dir
SVN_ROOT = /home/bqq/code/cloud/CRRC/bcm/bcm_project

#CFLAG = -g -Wall -ansi

#raspberry pi gcc
CC_PI = arm-linux-gnueabihf-gcc

#smartfusion2 gcc
#CC = $(CROSS_COMPILE_PATH)/arm-linux-gnueabihf-gcc

CC = arm-xilinx-linux-gnueabi-gcc

all: $(EXEC)

$(EXEC): main.o vlan/vlan.o chip/chip.o port/port.o phy/phy.o
	$(CC) -o $(EXEC) $^

main.o : chip/chip.h port/port.h vlan/vlan.h

vlan.o : vlan.h chip/chip.h phy/phy.h

chip.o : chip.h phy/phy.h

port.o : port.h phy/phy.h

phy.o : phy.h

clean:
	@find . -type f -name "*.o" | xargs -i rm {}
	@find . -type f -name "*~" | xargs -i rm {}
	@find . -type f -name "*.gch" | xargs -i rm {}
	@find . -type f -name "*.gdb" | xargs -i rm {}

cp:
	cp $(EXEC) $(SVN_ROOT)
