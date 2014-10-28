GCC_DIR =  ~/ti/gcc/bin
SUPPORT_FILE_DIRECTORY = ~/ti/gcc/include

DEVICE		 = msp430g2553
PREFIX		?= $(GCC_DIR)/msp430-elf-

CC			 = $(PREFIX)gcc

CFLAGS		 = -I ./ -I $(SUPPORT_FILE_DIRECTORY) -mmcu=$(DEVICE) -O2 -g
LDFLAGS		+= -L $(SUPPORT_FILE_DIRECTORY)

BINARY = $(DEVICE).firmware

# Source files
OBJS = main.o

# Hardware drivers
DRIVERS := uart.o spi.o usci.o
DRIVERS := $(addprefix drivers/, $(DRIVERS))

all: $(OBJS) $(DRIVERS)
	$(CC) $(CFLAGS) $(LDFLAGS) $? -o $(BINARY)

debug: all
	$(GDB) $(BINARY)

clean:
	rm -vf *.o drivers/*.o

