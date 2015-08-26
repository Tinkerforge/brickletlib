/* Linker Script for bricklets */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(entry)

SECTIONS {
	/* entry function always at address 0x0 */
	start 0x0 : {
		*(.text.entry)
	}

	.rodata : {
		*(.rodata)
	}

	.data.rel.ro : {
		*(.data.rel.ro)
	}

	.data.rel.ro.local : {
		*(.data.rel.ro.local)
	}

	.dynsym : {
		*(.dynsym)
	}

	.got.plt : {
		*(.got.plt)
	}

	.rel.dyn : {
		*(.rel.dyn)
	}

	. = 4080;

	/* device identifier always at address 4080 */
	.device_identifier : {
		*(.device_identifier)
		KEEP(*(.device_identifier))
	}

	/DISCARD/ : {
		*(.dynamic)
		*(.interp)
	}
}
