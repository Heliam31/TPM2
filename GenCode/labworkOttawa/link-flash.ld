MEMORY
{
	flash (rx) : ORIGIN = 0x00000000, LENGTH = 0x80000
	sram (rwx) : ORIGIN = 0x40000000, LENGTH = 0x80000
}

SECTIONS {
	.text 0x400 : { *(.text) } > flash
	.note.gnu.build-id : { *(.note.gnu.build-id) } > flash
	.data : { *(.data) } > sram
	.bss : { *(.bss) } > sram
}
