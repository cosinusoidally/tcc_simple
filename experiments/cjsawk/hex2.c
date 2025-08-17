/* -*- c-file-style: "linux";indent-tabs-mode:t -*- */
/* Copyright (C) 2017 Jeremiah Orians
 * Copyright (C) 2017 Jan Nieuwenhuizen <janneke@gnu.org>
 * This file is part of mescc-tools
 *
 * mescc-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mescc-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mescc-tools.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


/* Global variables */
extern FILE* output;
extern char* filename;
extern char* scratch;
extern int ALIGNED;
extern int Architecture;
extern int Base_Address;
extern int BigEndian;
extern int ByteMode;
extern int exec_enable;
extern int hold;
extern int ip;
extern int linenumber;
extern int toggle;
extern struct entry** jump_tables;

/* Function prototypes */
int Architectural_displacement(int target, int base);
int Throwaway_token(FILE* source_file);
int consume_token(FILE* source_file);
int storeLabel(FILE* source_file, int ip);
unsigned GetTarget(char* c);
void Clear_Scratch(char* s);
void line_error();
void outputPointer(int displacement, int number_of_bytes);
void pad_to_align(int write);
int hex(int c, FILE* source_file);
int octal(int c, FILE* source_file);
int binary(int c, FILE* source_file);
/* -*- c-file-style: "linux";indent-tabs-mode:t -*- */
/* Copyright (C) 2017 Jeremiah Orians
 * Copyright (C) 2017 Jan Nieuwenhuizen <janneke@gnu.org>
 * This file is part of mescc-tools
 *
 * mescc-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mescc-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mescc-tools.  If not, see <http://www.gnu.org/licenses/>.
 */

#define max_string 4096
#define TRUE 1
#define FALSE 0

#define KNIGHT 0
#define X86 0x03
#define AMD64 0x3E
#define ARMV7L 0x28
#define AARM64 0xB7
#define PPC64LE 0x15
#define RISCV32 0xF3
#define RISCV64 0x100F3 /* Because RISC-V unlike all other architectures does get a seperate e_machine when changing from 32 to 64bit */

#define HEX 16
#define OCTAL 8
#define BINARY 2


struct input_files
{
	struct input_files* next;
	char* filename;
};

struct entry
{
	struct entry* next;
	unsigned target;
	char* name;
};

/* Copyright (C) 2016 Jeremiah Orians
 * This file is part of M2-Planet.
 *
 * M2-Planet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * M2-Planet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2-Planet.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>


#define TRUE 1
#define FALSE 0


void require(int bool, char* error)
{
	if(!bool)
	{
		fputs(error, stderr);
		exit(EXIT_FAILURE);
	}
}


int match(char* a, char* b)
{
	if((NULL == a) && (NULL == b)) return TRUE;
	if(NULL == a) return FALSE;
	if(NULL == b) return FALSE;

	int i = -1;
	do
	{
		i = i + 1;
		if(a[i] != b[i])
		{
			return FALSE;
		}
	} while((0 != a[i]) && (0 !=b[i]));
	return TRUE;
}


int in_set(int c, char* s)
{
	/* NULL set is always false */
	if(NULL == s) return FALSE;

	while(0 != s[0])
	{
		if(c == s[0]) return TRUE;
		s = s + 1;
	}
	return FALSE;
}

/* INTERNAL ONLY */
int __index_number(char* s, char c)
{
	int i = 0;
	while(s[i] != c)
	{
		i = i + 1;
		if(0 == s[i]) return -1;
	}
	return i;
}

/* INTERNAL ONLY */
int __toupper(int c)
{
	if(in_set(c, "abcdefghijklmnopqrstuvwxyz")) return (c & 0xDF);
	return c;
}

/* INTERNAL ONLY */
int __set_reader(char* set, int mult, char* input)
{
	int n = 0;
	int i = 0;
	int hold;
	int negative_p = FALSE;

	if(input[0] == '-')
	{
		negative_p = TRUE;
		i = i + 1;
	}

	while(in_set(input[i], set))
	{
		n = n * mult;
		hold = __index_number(set, __toupper(input[i]));

		/* Input managed to change between in_set and index_number */
		if(-1 == hold) return 0;
		n = n + hold;
		i = i + 1;
	}

	/* loop exited before NULL and thus invalid input */
	if(0 != input[i]) return 0;

	if(negative_p)
	{
		n = 0 - n;
	}

	return n;
}

int strtoint(char *a)
{
	int result = 0;
	/* If NULL string */
	if(0 == a[0])
	{
		result = 0;
	}
	/* Deal with binary */
	else if ('0' == a[0] && 'b' == a[1])
	{
		result = __set_reader("01", 2, a+2);
	}
	/* Deal with hex */
	else if ('0' == a[0] &&  'x' == a[1])
	{
		result = __set_reader("0123456789ABCDEFabcdef", 16, a+2);
	}
	/* Deal with octal */
	else if('0' == a[0])
	{
		result = __set_reader("01234567", 8, a+1);
	}
	/* Deal with decimal */
	else
	{
		result = __set_reader("0123456789", 10, a);
	}

	/* Deal with sign extension for 64bit hosts */
	if(0 != (0x80000000 & result)) result = (0xFFFFFFFF << 31) | result;
	return result;
}

char* int2str(int x, int base, int signed_p)
{
	require(1 < base, "int2str doesn't support a base less than 2\n");
	require(37 > base, "int2str doesn't support a base more than 36\n");
	/* Be overly conservative and save space for 32binary digits and padding null */
	char* p = calloc(34, sizeof(char));
	/* if calloc fails return null to let calling code deal with it */
	if(NULL == p) return p;

	p = p + 32;
	unsigned i;
	int sign_p = FALSE;
	char* table = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	if(signed_p && (10 == base) && (0 != (x & 0x80000000)))
	{
		/* Truncate to 31bits */
		i = -x & 0x7FFFFFFF;
		if(0 == i) return "-2147483648";
		sign_p = TRUE;
	} /* Truncate to 32bits */
	else i = x & (0x7FFFFFFF | (1 << 31));

	do
	{
		p[0] = table[i % base];
		p = p - 1;
		i = i / base;
	} while(0 < i);

	if(sign_p)
	{
		p[0] = '-';
		p = p - 1;
	}

	return p + 1;
}
/* -*- c-file-style: "linux";indent-tabs-mode:t -*- */
/* Copyright (C) 2017 Jeremiah Orians
 * Copyright (C) 2017 Jan Nieuwenhuizen <janneke@gnu.org>
 * This file is part of mescc-tools
 *
 * mescc-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mescc-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mescc-tools.  If not, see <http://www.gnu.org/licenses/>.
 */


/* Globals */
FILE* output;
struct entry** jump_tables;
int BigEndian;
int Base_Address;
int Architecture;
int ByteMode;
int exec_enable;
int ip;
char* scratch;
char* filename;
int linenumber;
int ALIGNED;

/* For processing bytes */
int hold;
int toggle;

void line_error()
{
	fputs(filename, stderr);
	fputs(":", stderr);
	fputs(int2str(linenumber, 10, FALSE), stderr);
	fputs(" :", stderr);
}

int consume_token(FILE* source_file)
{
	int i = 0;
	int c = fgetc(source_file);
	while(!in_set(c, " \t\n>"))
	{
		scratch[i] = c;
		i = i + 1;
		c = fgetc(source_file);
		require(max_string > i, "Consumed token exceeds length restriction\n");
		if(EOF == c) break;
	}

	return c;
}

int Throwaway_token(FILE* source_file)
{
	int c;
	do
	{
		c = fgetc(source_file);
		if(EOF == c) break;
	} while(!in_set(c, " \t\n>"));

	return c;
}

int length(char* s)
{
	int i = 0;
	while(0 != s[i]) i = i + 1;
	return i;
}

void Clear_Scratch(char* s)
{
	do
	{
		s[0] = 0;
		s = s + 1;
	} while(0 != s[0]);
}

void Copy_String(char* a, char* b)
{
	while(0 != a[0])
	{
		b[0] = a[0];
		a = a + 1;
		b = b + 1;
	}
}

int GetHash(char* s)
{
	int i = 5381;
	while(0 != s[0])
	{
		i = i * 31 + s[0];
		s = s + 1;
	}
	return (i & 0xFFFF);
}

unsigned GetTarget(char* c)
{
	struct entry* i;
	for(i = jump_tables[GetHash(c)]; NULL != i; i = i->next)
	{
		if(match(c, i->name))
		{
			return i->target;
		}
	}
	fputs("Target label ", stderr);
	fputs(c, stderr);
	fputs(" is not valid\n", stderr);
	exit(EXIT_FAILURE);
}

int storeLabel(FILE* source_file, int ip)
{
	struct entry* entry = calloc(1, sizeof(struct entry));
	require(NULL != entry, "failed to allocate entry\n");

	/* Ensure we have target address */
	entry->target = ip;

	/* Store string */
	int c = consume_token(source_file);
	entry->name = calloc(length(scratch) + 1, sizeof(char));
	require(NULL != entry->name, "failed to allocate entry->name\n");
	Copy_String(scratch, entry->name);
	Clear_Scratch(scratch);

	/* Prepend to list */
	int h = GetHash(entry->name);
	entry->next = jump_tables[h];
	jump_tables[h] = entry;

	return c;
}

void range_check(int displacement, int number_of_bytes)
{
	if(4 == number_of_bytes) return;
	else if (3 == number_of_bytes)
	{
		if((8388607 < displacement) || (displacement < -8388608))
		{
			fputs("A displacement of ", stderr);
			fputs(int2str(displacement, 10, TRUE), stderr);
			fputs(" does not fit in 3 bytes\n", stderr);
			exit(EXIT_FAILURE);
		}
		return;
	}
	else if (2 == number_of_bytes)
	{
		if((32767 < displacement) || (displacement < -32768))
		{
			fputs("A displacement of ", stderr);
			fputs(int2str(displacement, 10, TRUE), stderr);
			fputs(" does not fit in 2 bytes\n", stderr);
			exit(EXIT_FAILURE);
		}
		return;
	}
	else if (1 == number_of_bytes)
	{
		if((127 < displacement) || (displacement < -128))
		{
			fputs("A displacement of ", stderr);
			fputs(int2str(displacement, 10, TRUE), stderr);
			fputs(" does not fit in 1 byte\n", stderr);
			exit(EXIT_FAILURE);
		}
		return;
	}

	fputs("Invalid number of bytes given\n", stderr);
	exit(EXIT_FAILURE);
}

void outputPointer(int displacement, int number_of_bytes)
{
	unsigned value = displacement;

	/* HALT HARD if we are going to do something BAD*/
	range_check(displacement, number_of_bytes);

	if(BigEndian)
	{ /* Deal with BigEndian */
		if(4 == number_of_bytes) fputc((value >> 24), output);
		if(3 <= number_of_bytes) fputc(((value >> 16)%256), output);
		if(2 <= number_of_bytes) fputc(((value >> 8)%256), output);
		if(1 <= number_of_bytes) fputc((value % 256), output);
	}
	else
	{ /* Deal with LittleEndian */
		unsigned byte;
		while(number_of_bytes > 0)
		{
			byte = value % 256;
			value = value / 256;
			fputc(byte, output);
			number_of_bytes = number_of_bytes - 1;
		}
	}
}

int Architectural_displacement(int target, int base)
{
	if(KNIGHT == Architecture) return (target - base);
	else if(X86 == Architecture) return (target - base);
	else if(AMD64 == Architecture) return (target - base);
	else if(ALIGNED && (ARMV7L == Architecture))
	{
		ALIGNED = FALSE;
		/* Note: Branch displacements on ARM are in number of instructions to skip, basically. */
		if (target & 3)
		{
			line_error();
			fputs("error: Unaligned branch target: ", stderr);
			fputs(scratch, stderr);
			fputs(", aborting\n", stderr);
			exit(EXIT_FAILURE);
		}
		/*
		 * The "fetch" stage already moved forward by 8 from the
		 * beginning of the instruction because it is already
		 * prefetching the next instruction.
		 * Compensate for it by subtracting the space for
		 * two instructions (including the branch instruction).
		 * and the size of the aligned immediate.
		 */
		return (((target - base + (base & 3)) >> 2) - 2);
	}
	else if(ARMV7L == Architecture)
	{
		/*
		 * The size of the offset is 8 according to the spec but that value is
		 * based on the end of the immediate, which the documentation gets wrong
		 * and needs to be adjusted to the size of the immediate.
		 * Eg 1byte immediate => -8 + 1 = -7
		 */
		return ((target - base) - 8 + (3 & base));
	}
	else if(ALIGNED && (AARM64 == Architecture))
	{
			ALIGNED = FALSE;
			return (target - (~3 & base)) >> 2;
	}
	else if (AARM64 == Architecture)
	{
		return ((target - base) - 8 + (3 & base));
	}
	else if(ALIGNED && (PPC64LE == Architecture))
	{
		ALIGNED = FALSE;
		/* set Link register with branch */
		return (target - (base & 0xFFFFFFFC )) | 1;
	}
	else if(PPC64LE == Architecture)
	{
		/* DO *NOT* set link register with branch */
		return (target - (base & 0xFFFFFFFC));
	}
	else if(RISCV32 == Architecture || RISCV64 == Architecture) return (target - base);

	fputs("Unknown Architecture, aborting before harm is done\n", stderr);
	exit(EXIT_FAILURE);
}

void Update_Pointer(char ch)
{
	/* Calculate pointer size*/
	if(in_set(ch, "%&")) ip = ip + 4; /* Deal with % and & */
	else if(in_set(ch, "@$")) ip = ip + 2; /* Deal with @ and $ */
	else if('~' == ch) ip = ip + 3; /* Deal with ~ */
	else if('!' == ch) ip = ip + 1; /* Deal with ! */
	else
	{
		line_error();
		fputs("storePointer given unknown\n", stderr);
		exit(EXIT_FAILURE);
	}
}

void storePointer(char ch, FILE* source_file)
{
	/* Get string of pointer */
	Clear_Scratch(scratch);
	Update_Pointer(ch);
	int base_sep_p = consume_token(source_file);

	/* Lookup token */
	int target = GetTarget(scratch);
	int displacement;

	int base = ip;

	/* Change relative base address to :<base> */
	if ('>' == base_sep_p)
	{
		Clear_Scratch(scratch);
		consume_token (source_file);
		base = GetTarget (scratch);

		/* Force universality of behavior */
		displacement = (target - base);
	}
	else
	{
		displacement = Architectural_displacement(target, base);
	}

	/* output calculated difference */
	if('!' == ch) outputPointer(displacement, 1); /* Deal with ! */
	else if('$' == ch) outputPointer(target, 2); /* Deal with $ */
	else if('@' == ch) outputPointer(displacement, 2); /* Deal with @ */
	else if('~' == ch) outputPointer(displacement, 3); /* Deal with ~ */
	else if('&' == ch) outputPointer(target, 4); /* Deal with & */
	else if('%' == ch) outputPointer(displacement, 4);  /* Deal with % */
	else
	{
		line_error();
		fputs("error: storePointer reached impossible case: ch=", stderr);
		fputc(ch, stderr);
		fputs("\n", stderr);
		exit(EXIT_FAILURE);
	}
}

void line_Comment(FILE* source_file)
{
	int c = fgetc(source_file);
	while(!in_set(c, "\n\r"))
	{
		if(EOF == c) break;
		c = fgetc(source_file);
	}
	linenumber = linenumber + 1;
}

int hex(int c, FILE* source_file)
{
	if (in_set(c, "0123456789")) return (c - 48);
	else if (in_set(c, "abcdef")) return (c - 87);
	else if (in_set(c, "ABCDEF")) return (c - 55);
	else if (in_set(c, "#;")) line_Comment(source_file);
	else if ('\n' == c) linenumber = linenumber + 1;
	return -1;
}

int octal(int c, FILE* source_file)
{
	if (in_set(c, "01234567")) return (c - 48);
	else if (in_set(c, "#;")) line_Comment(source_file);
	else if ('\n' == c) linenumber = linenumber + 1;
	return -1;
}

int binary(int c, FILE* source_file)
{
	if (in_set(c, "01")) return (c - 48);
	else if (in_set(c, "#;")) line_Comment(source_file);
	else if ('\n' == c) linenumber = linenumber + 1;
	return -1;
}

void process_byte(char c, FILE* source_file, int write)
{
	if(HEX == ByteMode)
	{
		if(0 <= hex(c, source_file))
		{
			if(toggle)
			{
				if(write) fputc(((hold * 16)) + hex(c, source_file), output);
				ip = ip + 1;
				hold = 0;
			}
			else
			{
				hold = hex(c, source_file);
			}
			toggle = !toggle;
		}
	}
	else if(OCTAL ==ByteMode)
	{
		if(0 <= octal(c, source_file))
		{
			if(2 == toggle)
			{
				if(write) fputc(((hold * 8)) + octal(c, source_file), output);
				ip = ip + 1;
				hold = 0;
				toggle = 0;
			}
			else if(1 == toggle)
			{
				hold = ((hold * 8) + octal(c, source_file));
				toggle = 2;
			}
			else
			{
				hold = octal(c, source_file);
				toggle = 1;
			}
		}
	}
	else if(BINARY == ByteMode)
	{
		if(0 <= binary(c, source_file))
		{
			if(7 == toggle)
			{
				if(write) fputc((hold * 2) + binary(c, source_file), output);
				ip = ip + 1;
				hold = 0;
				toggle = 0;
			}
			else
			{
				hold = ((hold * 2) + binary(c, source_file));
				toggle = toggle + 1;
			}
		}
	}
}

void pad_to_align(int write)
{
	if((ARMV7L == Architecture) || (AARM64 == Architecture) || (RISCV32 == Architecture) || (RISCV64 == Architecture))
	{
		if(1 == (ip & 0x1))
		{
			ip = ip + 1;
			if(write) fputc('\0', output);
		}
		if(2 == (ip & 0x2))
		{
			ip = ip + 2;
			if(write)
			{
				fputc('\0', output);
				fputc('\0', output);
			}
		}
	}
}

void first_pass(struct input_files* input)
{
	if(NULL == input) return;
	first_pass(input->next);
	filename = input->filename;
	linenumber = 1;
	FILE* source_file = fopen(filename, "r");

	if(NULL == source_file)
	{
		fputs("The file: ", stderr);
		fputs(input->filename, stderr);
		fputs(" can not be opened!\n", stderr);
		exit(EXIT_FAILURE);
	}

	toggle = FALSE;
	int c;
	for(c = fgetc(source_file); EOF != c; c = fgetc(source_file))
	{
		/* Check for and deal with label */
		if(':' == c)
		{
			c = storeLabel(source_file, ip);
		}

		/* check for and deal with relative/absolute pointers to labels */
		if(in_set(c, "!@$~%&"))
		{ /* deal with 1byte pointer !; 2byte pointers (@ and $); 3byte pointers ~; 4byte pointers (% and &) */
			Update_Pointer(c);
			c = Throwaway_token(source_file);
			if ('>' == c)
			{ /* deal with label>base */
				c = Throwaway_token(source_file);
			}
		}
		else if('<' == c)
		{
			pad_to_align(FALSE);
		}
		else if('^' == c)
		{
			/* Just ignore */
			continue;
		}
		else process_byte(c, source_file, FALSE);
	}
	fclose(source_file);
}

void second_pass(struct input_files* input)
{
	if(NULL == input) return;
	second_pass(input->next);
	filename = input->filename;
	linenumber = 1;
	FILE* source_file = fopen(filename, "r");

	/* Something that should never happen */
	if(NULL == source_file)
	{
		fputs("The file: ", stderr);
		fputs(input->filename, stderr);
		fputs(" can not be opened!\nWTF-pass2\n", stderr);
		exit(EXIT_FAILURE);
	}

	toggle = FALSE;
	hold = 0;

	int c;
	for(c = fgetc(source_file); EOF != c; c = fgetc(source_file))
	{
		if(':' == c) c = Throwaway_token(source_file); /* Deal with : */
		else if(in_set(c, "!@$~%&")) storePointer(c, source_file);  /* Deal with !, @, $, ~, % and & */
		else if('<' == c) pad_to_align(TRUE);
		else if('^' == c) ALIGNED = TRUE;
		else process_byte(c, source_file, TRUE);
	}

	fclose(source_file);
}
/* -*- c-file-style: "linux";indent-tabs-mode:t -*- */
/* Copyright (C) 2017 Jeremiah Orians
 * Copyright (C) 2017 Jan Nieuwenhuizen <janneke@gnu.org>
 * This file is part of mescc-tools
 *
 * mescc-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mescc-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mescc-tools.  If not, see <http://www.gnu.org/licenses/>.
 */


unsigned shiftregister;
unsigned tempword;
int updates;

void outOfRange(char* s, int value)
{
	line_error();
	fputs("error: value ", stderr);
	fputs(int2str(value, 10, TRUE), stderr);
	fputs(" out of range for field type ", stderr);
	fputs(s, stderr);
	fputs("\n", stderr);
	exit(EXIT_FAILURE);
}

void UpdateShiftRegister(char ch, int value)
{
	if ('.' == ch)
	{
		unsigned swap;
		/* Assume the user knows what they are doing */
		if(!BigEndian)
		{
			/* Swap from big-endian to little endian order */
			swap = (((value >> 24) & 0xFF) |
			        ((value << 8) & 0xFF0000) |
			        ((value >> 8) & 0xFF00) |
			        ((value & 0xFF) << 24));
		}
		else
		{
			/* Big endian needs no change */
			swap = value;
		}
		/* we just take the 4 bytes after the . and shove in the shift register */
		swap = swap & ((0xFFFF << 16) | 0xFFFF);
		shiftregister = shiftregister ^ swap;
	}
	else if ('!' == ch)
	{
		/* Corresponds to RISC-V I format */
		/* Will need architecture specific logic if more architectures go this route */
		/* no range check because it needs to work with labels for lui/addi + AUIPC combos */
		/* !label is used in the second instruction of AUIPC combo but we want an offset from */
		/* the first instruction */
		value = value + 4;
		tempword = (value & 0xFFF) << 20;
		/* Update shift register */
		tempword = tempword & ((0xFFFF << 16) | 0xFFFF);
		shiftregister = shiftregister ^ tempword;
	}
	else if ('@' == ch)
	{
		/* Corresponds to RISC-V B format (formerly known as SB) */
		/* Will need architecture specific logic if more architectures go this route */
		if ((value < -0x1000 || value > 0xFFF) || (value & 1)) outOfRange("B", value);

		/* Prepare the immediate's word */
		tempword = ((value & 0x1E) << 7)
			| ((value & 0x7E0) << (31 - 11))
			| ((value & 0x800) >> 4)
			| ((value & 0x1000) << (31 - 12));
		tempword = tempword & ((0xFFFF << 16) | 0xFFFF);
		/* Update shift register */
		shiftregister = shiftregister ^ tempword;
	}
	else if ('$' == ch)
	{
		/* Corresponds with RISC-V J format (formerly known as UJ) */
		/* Will need architecture specific logic if more architectures go this route */
		if ((value < -0x100000 || value > 0xFFFFF) || (value & 1)) outOfRange("J", value);

		tempword = ((value & 0x7FE) << (30 - 10))
			| ((value & 0x800) << (20 - 11))
			| ((value & 0xFF000))
			| ((value & 0x100000) << (31 - 20));
		tempword = tempword & ((0xFFFF << 16) | 0xFFFF);
		shiftregister = shiftregister ^ tempword;
	}
	else if ('~' == ch)
	{
		/* Corresponds with RISC-V U format */
		/* Will need architecture specific logic if more architectures go this route */
		if ((value & 0xFFF) < 0x800) tempword = value & (0xFFFFF << 12);
		else tempword = (value & (0xFFFFF << 12)) + 0x1000;
		tempword = tempword & ((0xFFFF << 16) | 0xFFFF);
		shiftregister = shiftregister ^ tempword;
	}
	else
	{
		line_error();
		fputs("error: UpdateShiftRegister reached impossible case: ch=", stderr);
		fputc(ch, stderr);
		fputs("\n", stderr);
		exit(EXIT_FAILURE);
	}
}

void WordStorePointer(char ch, FILE* source_file)
{
	/* Get string of pointer */
	ip = ip + 4;
	Clear_Scratch(scratch);
	int base_sep_p = consume_token(source_file);

	/* Lookup token */
	int target = GetTarget(scratch);
	int displacement;

	int base = ip;

	/* Change relative base address to :<base> */
	if ('>' == base_sep_p)
	{
		Clear_Scratch(scratch);
		consume_token (source_file);
		base = GetTarget (scratch);

		/* Force universality of behavior */
		displacement = (target - base);
	}
	else
	{
		displacement = Architectural_displacement(target, base);
	}

	/* output calculated difference */
	if('&' == ch) outputPointer(target, 4); /* Deal with & */
	else if('%' == ch) outputPointer(displacement, 4);  /* Deal with % */
	else
	{
		line_error();
		fputs("error: WordStorePointer reached impossible case: ch=", stderr);
		fputc(ch, stderr);
		fputs("\n", stderr);
		exit(EXIT_FAILURE);
	}
}

unsigned sr_nextb()
{
	unsigned rv = shiftregister & 0xff;
	shiftregister = shiftregister >> 8;
	return rv;
}

void DoByte(char c, FILE* source_file, int write, int update)
{
	if(HEX == ByteMode)
	{
		if(0 <= hex(c, source_file))
		{
			if(toggle)
			{
				if(write) fputc(((hold * 16)) + hex(c, source_file) ^ sr_nextb(), output);
				ip = ip + 1;
				if(update)
				{
					hold = (hold * 16) + hex(c, source_file);
					tempword = (tempword << 8) ^ hold;
					updates = updates + 1;
				}
				hold = 0;
			}
			else
			{
				hold = hex(c, source_file);
			}
			toggle = !toggle;
		}
	}
	else if(OCTAL ==ByteMode)
	{
		if(0 <= octal(c, source_file))
		{
			if(2 == toggle)
			{
				if(write) fputc(((hold * 8)) + octal(c, source_file) ^ sr_nextb(), output);
				ip = ip + 1;
				if(update)
				{
					hold = ((hold * 8) + octal(c, source_file));
					tempword = (tempword << 8) ^ hold;
					updates = updates + 1;
				}
				hold = 0;
				toggle = 0;
			}
			else if(1 == toggle)
			{
				hold = ((hold * 8) + octal(c, source_file));
				toggle = 2;
			}
			else
			{
				hold = octal(c, source_file);
				toggle = 1;
			}
		}
	}
	else if(BINARY == ByteMode)
	{
		if(0 <= binary(c, source_file))
		{
			if(7 == toggle)
			{
				if(write) fputc((hold * 2) + binary(c, source_file) ^ sr_nextb(), output);
				ip = ip + 1;
				if(update)
				{
					hold = ((hold * 2) + binary(c, source_file));
					tempword = (tempword << 8) ^ hold;
					updates = updates + 1;
				}
				hold = 0;
				toggle = 0;
			}
			else
			{
				hold = ((hold * 2) + binary(c, source_file));
				toggle = toggle + 1;
			}
		}
	}
}

void WordFirstPass(struct input_files* input)
{
	if(NULL == input) return;
	WordFirstPass(input->next);
	filename = input->filename;
	linenumber = 1;
	FILE* source_file = fopen(filename, "r");

	if(NULL == source_file)
	{
		fputs("The file: ", stderr);
		fputs(input->filename, stderr);
		fputs(" can not be opened!\n", stderr);
		exit(EXIT_FAILURE);
	}

	toggle = FALSE;
	int c;
	for(c = fgetc(source_file); EOF != c; c = fgetc(source_file))
	{
		/* Check for and deal with label */
		if(':' == c)
		{
			c = storeLabel(source_file, ip);
		}

		/* check for and deal with relative/absolute pointers to labels */
		if('.' == c)
		{
			/* Read architecture specific number of bytes for what is defined as a word */
			/* 4bytes in RISC-V's case */
			updates = 0;
			tempword = 0;
			while (updates < 4)
			{
				c = fgetc(source_file);
				DoByte(c, source_file, FALSE, TRUE);
			}
			ip = ip - 4;
		}
		else if(in_set(c, "!@$~"))
		{
			/* Don't update IP */
			c = Throwaway_token(source_file);
		}
		else if(in_set(c, "%&"))
		{
			ip = ip + 4;
			c = Throwaway_token(source_file);
			if ('>' == c)
			{ /* deal with label>base */
				c = Throwaway_token(source_file);
			}
		}
		else if('<' == c)
		{
			pad_to_align(FALSE);
		}
		else if('^' == c)
		{
			/* Just ignore */
			continue;
		}
		else DoByte(c, source_file, FALSE, FALSE);
	}
	fclose(source_file);
}

void WordSecondPass(struct input_files* input)
{
	shiftregister = 0;
	tempword = 0;

	if(NULL == input) return;
	WordSecondPass(input->next);
	filename = input->filename;
	linenumber = 1;
	FILE* source_file = fopen(filename, "r");

	/* Something that should never happen */
	if(NULL == source_file)
	{
		fputs("The file: ", stderr);
		fputs(input->filename, stderr);
		fputs(" can not be opened!\nWTF-pass2\n", stderr);
		exit(EXIT_FAILURE);
	}

	toggle = FALSE;
	hold = 0;

	int c;
	for(c = fgetc(source_file); EOF != c; c = fgetc(source_file))
	{
		if(':' == c) c = Throwaway_token(source_file); /* Deal with : */
		else if('.' == c)
		{
			/* Read architecture specific number of bytes for what is defined as a word */
			/* 4bytes in RISC-V's case */
			updates = 0;
			tempword = 0;
			while (updates < 4)
			{
				c = fgetc(source_file);
				DoByte(c, source_file, FALSE, TRUE);
			}
			UpdateShiftRegister('.', tempword);
			ip = ip - 4;
		}
		else if(in_set(c, "%&")) WordStorePointer(c, source_file);  /* Deal with % and & */
		else if(in_set(c, "!@$~"))
		{
			Clear_Scratch(scratch);
			consume_token(source_file);
			UpdateShiftRegister(c, Architectural_displacement(GetTarget(scratch), ip)); /* Play with shift register */
		}
		else if('<' == c) pad_to_align(TRUE);
		else if('^' == c) ALIGNED = TRUE;
		else DoByte(c, source_file, TRUE, FALSE);
	}

	fclose(source_file);
}
/* -*- c-file-style: "linux";indent-tabs-mode:t -*- */
/* Copyright (C) 2017 Jeremiah Orians
 * Copyright (C) 2017 Jan Nieuwenhuizen <janneke@gnu.org>
 * This file is part of mescc-tools
 *
 * mescc-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mescc-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mescc-tools.  If not, see <http://www.gnu.org/licenses/>.
 */


/* The essential functions */
void first_pass(struct input_files* input);
void second_pass(struct input_files* input);
void WordFirstPass(struct input_files* input);
void WordSecondPass(struct input_files* input);

/* Standard C main program */
int main(int argc, char **argv)
{
	int InsaneArchitecture = FALSE;
	ALIGNED = FALSE;
	BigEndian = FALSE;
	jump_tables = calloc(65537, sizeof(struct entry*));
	require(NULL != jump_tables, "Failed to allocate our jump_tables\n");

	Architecture = X86;
	Base_Address = 0x8048000;
	struct input_files* input = NULL;
	output = stdout;
	char* output_file = "";
	exec_enable = TRUE;
	ByteMode = HEX;
	scratch = calloc(max_string + 1, sizeof(char));
	require(NULL != scratch, "failed to allocate our scratch buffer\n");
	char* arch;
	struct input_files* temp;

	int option_index = 1;
	temp = calloc(1, sizeof(struct input_files));
	require(NULL != temp, "failed to allocate file for processing\n");
	temp->filename = argv[option_index];
	temp->next = input;
	input = temp;
	option_index = option_index + 1;
	while(option_index <= argc)
	{
		if(NULL == argv[option_index])
		{
			option_index = option_index + 1;
		}
		else if(match(argv[option_index], "--big-endian"))
		{
			BigEndian = TRUE;
			option_index = option_index + 1;
		}
		else if(match(argv[option_index], "--little-endian"))
		{
			BigEndian = FALSE;
			option_index = option_index + 1;
		}
		else if(match(argv[option_index], "--non-executable"))
		{
			exec_enable = FALSE;
			option_index = option_index + 1;
		}
		else if(match(argv[option_index], "-A") || match(argv[option_index], "--architecture"))
		{
			arch = argv[option_index + 1];
			if(match("knight-native", arch) || match("knight-posix", arch)) Architecture = KNIGHT;
			else if(match("x86", arch)) Architecture = X86;
			else if(match("amd64", arch)) Architecture = AMD64;
			else if(match("armv7l", arch)) Architecture = ARMV7L;
			else if(match("aarch64", arch)) Architecture = AARM64;
			else if(match("ppc64le", arch)) Architecture = PPC64LE;
			else if(match("riscv32", arch)) Architecture = RISCV32;
			else if(match("riscv64", arch)) Architecture = RISCV64;
			else
			{
				fputs("Unknown architecture: ", stderr);
				fputs(arch, stderr);
				fputs(" know values are: knight-native, knight-posix, x86, amd64, armv7l, riscv32 and riscv64", stderr);
			}
			option_index = option_index + 2;
		}
		else if(match(argv[option_index], "-b") || match(argv[option_index], "--binary"))
		{
			ByteMode = BINARY;
			option_index = option_index + 1;
		}
		else if(match(argv[option_index], "-B") || match(argv[option_index], "--base-address"))
		{
			Base_Address = strtoint(argv[option_index + 1]);
			option_index = option_index + 2;
		}
		else if(match(argv[option_index], "-h") || match(argv[option_index], "--help"))
		{
			fputs("Usage: ", stderr);
			fputs(argv[0], stderr);
			fputs(" --file FILENAME1 {-f FILENAME2} (--big-endian|--little-endian)", stderr);
			fputs(" [--base-address 0x12345] [--architecture name]\nArchitecture:", stderr);
			fputs(" knight-native, knight-posix, x86, amd64, armv7l, aarch64, riscv32 and riscv64\n", stderr);
			fputs("To leverage octal or binary input: --octal, --binary\n", stderr);
			exit(EXIT_SUCCESS);
		}
		else if(match(argv[option_index], "-f") || match(argv[option_index], "--file"))
		{
			temp = calloc(1, sizeof(struct input_files));
			require(NULL != temp, "failed to allocate file for processing\n");
			temp->filename = argv[option_index + 1];
			temp->next = input;
			input = temp;
			option_index = option_index + 2;
		}
		else if(match(argv[option_index], "-o") || match(argv[option_index], "--output"))
		{
			output_file = argv[option_index + 1];
			output = fopen(output_file, "w");

			if(NULL == output)
			{
				fputs("The file: ", stderr);
				fputs(argv[option_index + 1], stderr);
				fputs(" can not be opened!\n", stderr);
				exit(EXIT_FAILURE);
			}
			option_index = option_index + 2;
		}
		else if(match(argv[option_index], "-O") || match(argv[option_index], "--octal"))
		{
			ByteMode = OCTAL;
			option_index = option_index + 1;
		}
		else if(match(argv[option_index], "-V") || match(argv[option_index], "--version"))
		{
			fputs("hex2 1.4.0\n", stdout);
			exit(EXIT_SUCCESS);
		}
		else
		{
			fputs("Unknown option\n", stderr);
			exit(EXIT_FAILURE);
		}
	}

	if((Architecture == RISCV32) || (Architecture == RISCV64))
	{
		/* Forcing me to use words instead of just byting into the problem */
		InsaneArchitecture = TRUE;
	}

	/* Catch a common mistake */
	if((KNIGHT != Architecture) && (0 == Base_Address))
	{
		fputs(">> WARNING <<\n>> WARNING <<\n>> WARNING <<\n", stderr);
		fputs("If you are not generating a ROM image this binary will likely not work\n", stderr);
	}

	/* Catch implicitly false assumptions */
	if(BigEndian && ((X86 == Architecture) || ( AMD64 == Architecture) || (ARMV7L == Architecture) || (AARM64 == Architecture) || (RISCV32 == Architecture) || (RISCV64 == Architecture)))
	{
		fputs(">> WARNING <<\n>> WARNING <<\n>> WARNING <<\n", stderr);
		fputs("You have specified big endian output on likely a little endian processor\n", stderr);
		fputs("if this is a mistake please pass --little-endian next time\n", stderr);
	}

	/* Make sure we have a program tape to run */
	if (NULL == input)
	{
		return EXIT_FAILURE;
	}

	/* Get all of the labels */
	ip = Base_Address;
	if(InsaneArchitecture) WordFirstPass(input);
	else first_pass(input);

	/* Fix all the references*/
	ip = Base_Address;
	if(InsaneArchitecture) WordSecondPass(input);
	else second_pass(input);

	/* flush all writes */
	fflush(output);

	/* Set file as executable */
	if(exec_enable && (output != stdout))
	{
		/* Close output file */
		fclose(output);

		if(0 != chmod(output_file, 0750))
		{
			fputs("Unable to change permissions\n", stderr);
			exit(EXIT_FAILURE);
		}
	}

	return EXIT_SUCCESS;
}
