OUTPUT_FORMAT(elf64-x86-64)
ENTRY(start)

SECTIONS
{
	. = 1M; /* Load the kernel at 1M */
	phys = .;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.bootstrap)
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
		*(.data.*)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		bss_start = .;
		*(COMMON)
		*(.bss)
		*(.stack)
	}

	end = .;

	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame)
		*(.note.gnu.build-id)
	}
}
