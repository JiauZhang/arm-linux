SECTIONS
{
    . = 0x0;
    . = ALIGN(4);
	__code_start__ = .;
    .text :
    {
      *(.text)
    }

    . = ALIGN(4);
    .rodata : 
	{ 
		*(.rodata) 
	}
	__code_end__ = .;
	
    . = ALIGN(4);
    .data 0x30000000 : AT(__code_end__) 
	{ 
		data_load_addr = LOADADDR(.data);
		data_start = .;
		*(.data) 
		data_end = .;
	}
	
    . = ALIGN(4);
    bss_start = .;
    .bss : 
	{ 
		*(.bss) *(.COMMON) 
	}
    bss_end = .;
}