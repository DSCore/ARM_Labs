# Where to find the CodeSourcery tools
BINDIR = /opt/CodeSourcery/bin/

# Which assembler and linker to use
AS = $(BINDIR)/arm-none-eabi-as
LD = $(BINDIR)/arm-none-eabi-ld

# Eclipse calls "make all" when a project build is requested
all: lab1	

#
# This is a "brute force" Makefile - as the labs progress we'll use more
# of make's capabilities to simplify Makefile maintenance
#
lab1: lab1.S USART2.S
	# -c              : create intermediate object file only
	# -g              : generate debugging information in object file
	# -mthumb         : generate thumb code (vs. ARM code - not supported on M4)
	# -mcpu=cortex-m4 : target the Cortex-M4 processor
	# -o <outfile>    : name of resulting object file
	$(AS) -c -g -mthumb -mcpu=cortex-m4 -o lab1.o lab1.S
	$(AS) -c -g -mthumb -mcpu=cortex-m4 -o USART2.o USART2.S
	
	# -T<script>      : Use the linker script <script> - for memory layout
	# -nostdlib       : do not link in the standard libraries
	# -nostartfiles   : do not link in CodeSourcery initialization code
	# -g              : generate debugging information in output file
	# -o <outfile>    : name of resulting executable image
	$(LD) -TSTM32F407VG.ld -nostdlib -nostartfiles -g -o lab1.elf lab1.o USART2.o

# Eclipse calls "make clean" when a project clean is requested	
clean:
	rm -f *.o *.elf
