/* Minimal linker script for the STM32F407VG */

/* The STM32F processors mirror flash at 0x08000000 to address 0x00000000 */
/* This puts the vector table stored at 0x08000000 at the expected        */
/* Vector Table location of 0x00000000 on power-on/reset                  */
MEMORY
{
  flash (rx) : ORIGIN = 0x08000000, LENGTH = 1024k
  ram (rwx)  : ORIGIN = 0x20000000, LENGTH = 192k
}

SECTIONS
{
	.text :
	{
		*(vectors)
		*(.text)
	} >flash
}