/* or1k-linux-as virtualKasli/jump.S -o build/jump.o */
/* 0x100: OpenRISC RESET reset vector. */
  .section .text
/*  .org 0x100 */

	/* Jump to program initialisation code at 0x10000000 */
	.global _main
	l.movhi r4, 0x0040
	l.ori r4, r4, 0x0000
	l.jr    r4
	l.nop
