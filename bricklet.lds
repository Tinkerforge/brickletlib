/* Linker Script for bricklets */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(entry)
  
SECTIONS {
	start 0x0 : {     /* entry function always at address 0x0 */
		*(.text.entry)
	}
		
/*	/DISCARD/ : {     /* writable ram can not be used in bricklet  
		*(.bss)
		*(.data)
	}*/
}