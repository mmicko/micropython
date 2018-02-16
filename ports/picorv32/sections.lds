SECTIONS {
	.memory : {
		. = 00000000;
		heap = .;
		. = 0x100000;
		start*(.text);
		*(.text);
		*(*);
		. = ALIGN(4);
	}
}
