#pragma once

/*
 * Hacker Disassembler Engine 64 (extended)
 *
 * Lightweight x86-64 length disassembler derived from the Patkov / MinHook
 * lineage.  Supports legacy prefixes, REX, VEX (C4/C5), EVEX (62), XOP (8F),
 * and 3-byte opcode escapes (0F 38 / 0F 3A).
 */

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HDE64_VERSION 0x0200u

	typedef struct {
		uint8_t  len;           /* Instruction length (1-15) */

		uint8_t  p_rep;         /* REP/REPE/REPNE prefix */
		uint8_t  p_lock;        /* LOCK prefix */
		uint8_t  p_seg;         /* Segment override */
		uint8_t  p_66;          /* Operand-size override */
		uint8_t  p_67;          /* Address-size override */
		uint8_t  rex;           /* REX prefix byte */
		uint8_t  rex_w;         /* REX.W */
		uint8_t  rex_r;         /* REX.R */
		uint8_t  rex_x;         /* REX.X */
		uint8_t  rex_b;         /* REX.B */

		/* VEX prefix (Intel SDM Vol.2 §2.6.1): C5 = 2-byte, C4 = 3-byte */
		uint8_t  vex;           /* 0 = none, 1 = 2-byte (C5), 2 = 3-byte (C4) */
		uint8_t  vex_m;         /* m-mmmm opcode-map select (3-byte VEX only) */
		uint8_t  vex_vvvv;     /* vvvv (stored as encoded, inverted in insn) */
		uint8_t  vex_rxb;      /* R'/X'/B' or R' (2-byte); inverted ext bits */
		uint8_t  vex_pp;       /* pp: implied 66/F3/F2 when non-zero */
		uint8_t  vex_l;        /* L bit (128/256-bit vector length) */
		uint8_t  vex_w;        /* W bit (3-byte VEX only) */

		/* EVEX prefix (Intel SDM Vol.2 §2.6.2): 62, four bytes total */
		uint8_t  evex;         /* 1 when EVEX prefix present */
		uint8_t  evex_mmm;     /* mmm opcode-map select */
		uint8_t  evex_w;       /* W bit */
		uint8_t  evex_vvvv;    /* vvvv (stored as encoded, inverted in insn) */
		uint8_t  evex_pp;      /* pp: implied 66/F3/F2 */
		uint8_t  evex_z;       /* z (merging/zeroing) */
		uint8_t  evex_ll;      /* L'L vector length / rounding control */
		uint8_t  evex_b;       /* b (broadcast/rounding) */
		uint8_t  evex_v;       /* V' (inverted, extra opcode bit) */
		uint8_t  evex_aaa;     /* aaa opmask register k1-k7 */
		uint8_t  evex_rxb;     /* R' X' B' R ext bits from byte 0 (inverted) */

		/* XOP prefix (AMD APM Vol.3 §A.5): 8F, three bytes when m >= 8 */
		uint8_t  xop;          /* 1 when XOP prefix present */
		uint8_t  xop_m;        /* m-mmmm (valid XOP maps: 0x08-0x1F) */
		uint8_t  xop_vvvv;
		uint8_t  xop_rxb;
		uint8_t  xop_pp;
		uint8_t  xop_w;

		uint8_t  opcode;        /* Primary or final opcode byte */
		uint8_t  opcode2;       /* 0F escape secondary (38/3A/other) */

		uint8_t  modrm;
		uint8_t  modrm_mod;
		uint8_t  modrm_reg;
		uint8_t  modrm_rm;

		uint8_t  sib;
		uint8_t  sib_scale;
		uint8_t  sib_index;
		uint8_t  sib_base;

		union {
			uint8_t  imm8;
			uint16_t imm16;
			uint32_t imm32;
			uint64_t imm64;
		} imm;

		union {
			uint8_t  disp8;
			uint16_t disp16;
			uint32_t disp32;
		} disp;

		uint32_t flags;
	} hde64s;

	/* Operand flags */
#define F_MODRM         0x00000001u
#define F_SIB           0x00000002u
#define F_IMM8          0x00000004u
#define F_IMM16         0x00000008u
#define F_IMM32         0x00000010u
#define F_IMM64         0x00000020u
#define F_DISP8         0x00000040u
#define F_DISP16        0x00000080u
#define F_DISP32        0x00000100u
#define F_RELATIVE      0x00000200u

	/* Extension / escape flags */
#define F_VEX           0x00020000u
#define F_EVEX          0x00040000u
#define F_XOP           0x00080000u
#define F_3BYTE_38      0x00100000u
#define F_3BYTE_3A      0x00200000u

	/* Error flags */
#define F_ERROR         0x00001000u
#define F_ERROR_OPCODE  0x00002000u
#define F_ERROR_LENGTH  0x00004000u
#define F_ERROR_LOCK    0x00008000u
#define F_ERROR_OPERAND 0x00010000u

	unsigned int hde64_disasm(const void* code, hde64s* hs);

#ifdef __cplusplus
}
#endif