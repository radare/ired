
#include <stdarg.h>
static const char *regs16[8] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"} ; 
static const char *regs8[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"} ;
static const char *segreg[4] = {"es", "cs", "ss", "ds"} ;

// global output
static char *go[64];

static const char *rm16_sreg(const unsigned char *buffer, int *j, int *error);
static char *moffs16(char *buffer, int *j, int *err);
char* read_file(char *name, long *num) ; 
char *rm8_r8(char *buffer, int *j, int *err) ;
static const char *rm(const unsigned char *buffer, int *j, char type, int *error) ;
static const char *rm16_r16(const unsigned char *buffer, int *j, int *err) ;
char *r8_rm8(char *buffer, int *j, int *err) ;
char *r16_rm16(char *buffer, int *j, int *err) ;
char *imm8(char *buffer, int *j, int *err) ;
char *imm16(char *buffer, int *j, int *err) ;
char *rel8(char *buufer, int *j, int *err) ;
char *rm8_imm8(char *buffer, int *j, int *err) ;
char *rm16_imm16(char *buffer, int *j, int *err) ;
static const char *rm16_imm8(const unsigned char *buffer, int *j, int *err) ;
static const char *sreg_rm16(const unsigned char *buffer, int *j, int *error) ;
static const char *m16(const unsigned char *buffer, int *j, int *err) ;
static const char *call_inter(const unsigned char *buffer, int *j, int *err) ;
char *rm8(char *buffer, int *j, int *err) ;
char *rm16(char *buffer, int *j, int *err) ;
char *rel16(char *buffer, int *j, int *err) ;
int parse(char *s, char*(*func)(const unsigned char*, int*, int*), const unsigned char *buffer, int *j) ;
static int parse_noop(char *s, const unsigned char *buffer, int *j) ;
static int get_bytes(int k, int j) ;
enum segment_registers {ES=0, CS, SS, DS} ; 

signed char segment_override = -1 ;
static int bytes = 0 ;
static int num_bytes = 0 ;
static int rm_segment_override = -1 ; 

static void goprintf(const char *fmt, void *arg) {
	if (!arg) {
		strcat ((char*)go, fmt);
		return;
	}
	sprintf ((char*)go + strlen (go), fmt, arg);
}

// int parse(char *s, char*(*func)(char*, int*, int *), const unsigned char *buffer, int *j) {
int parse(char *s, char*(*func)(const unsigned char*, int*, int*), const unsigned char *buffer, int *j) 
{
	//int temp_j = *j ; 
	int e = 0 ;
	char *result = func(buffer, j, &e) ;
	bytes = 1;
return 0;
#if 0
	if (error)
	{
		char tmp_buffer[20] ; 
		memset(tmp_buffer, '\0', 20) ;
		unsigned char tmp_char = buffer[*j] ; 
		sprintf(tmp_buffer, "db 0x%X", tmp_char) ;
		parse_noop(tmp_buffer, buffer, j) ;
		return 0 ;
	}
	int i=0;
	int k = 16 ;
	int t = 0 ;
	if (segment_override == -1 && rm_segment_override >= 0)
	{
		t = 1; 
		k = k - 2 ;
		switch (rm_segment_override)
		{
			case ES: goprintf("%02X", 0x26) ;	 break ;
			case CS: goprintf("%02X", 0x2E) ;	 break ;
			case SS: goprintf("%02X", 0x36) ;	 break ;
			case DS: goprintf("%02X", 0x3E) ;	 break ;
		} 
		rm_segment_override = -1 ; 
		segment_override = -1 ;
	}
	char segment[20] ;

	if (t == 1)
	{
			memset(segment, '\0', 20) ;
			switch (segment_override)
			{
				case ES: sprintf(segment, "es") ; break ;
				case CS: sprintf(segment, "cs") ; break ;
				case SS: sprintf(segment, "ss") ; break ;
				case DS: sprintf(segment, "ds") ; break ;
			}
	}
	if (segment_override >= 0 )
	{
		k = k - 2 ;
		switch (segment_override)
		{
			case ES: goprintf("%02X", 0x26) ;	 break ;
			case CS: goprintf("%02X", 0x2E) ;	 break ;
			case SS: goprintf("%02X", 0x36) ;	 break ;
			case DS: goprintf("%02X", 0x3E) ;	 break ;
		}
		segment_override = -1 ;
		rm_segment_override = -1 ; 
	}
	for (i=0; i < bytes; i++)
	{
		unsigned char byte = buffer[temp_j+i]  ; 
		goprintf("%02X", byte) ;	
	}
	k = (k - (bytes*2))  ; 
	for (i=0; i < k; i++) goprintf(" ") ;
	if (t == 1)
	{
		char tmp_string[255] ; 
		char tmp_string2[255] ; 
		memset(tmp_string, '\0', 255) ;
		memset(tmp_string2, '\0', 255) ;
		sprintf(tmp_string, s, result) ; 
		sprintf(tmp_string2, "%s %s", segment, tmp_string) ; 
		goprintf("%s", tmp_string2) ;
	} else goprintf(s, result) ; 
#endif
}

static inline int get_bytes(int k, int j) {
	return (k + j < num_bytes)? 0: 1;
}

static int parse_noop(char *s, const unsigned char *buffer, int *j)
{
	int k = 16 ; 
#if 0
	if (segment_override >= 0) 
	{
			switch (segment_override)
			{
				case ES: goprintf("%02X", 0x26) ;	 break ;
				case CS: goprintf("%02X", 0x2E) ;	 break ;
				case SS: goprintf("%02X", 0x36) ;	 break ;
				case DS: goprintf("%02X", 0x3E) ;	 break ;
			}
			k = k - 2 ; 
	}
	unsigned char tmp_char = buffer[*j] ; 
	goprintf("%02x", tmp_char) ;	
	int i= 0 ;
	k = k - 1*2 ; 
	for (i=0; i < k; i++) goprintf(" ") ;
	if (segment_override >= 0)
	{
		char segment[20] ;
		memset(segment, '\0', 20) ;
		switch (segment_override)
		{
			case ES: sprintf(segment, "es") ; break ;
			case CS: sprintf(segment, "cs") ; break ;
			case SS: sprintf(segment, "ss") ; break ;
			case DS: sprintf(segment, "ds") ; break ;
		}
		goprintf("%s %s", segment, s ) ; 
		segment_override = -1 ;
		rm_segment_override = -1 ; 
	} else {
		goprintf("%s", s) ; 
	}
#else
	goprintf("%s", s) ; 
#endif
}

int disasm(unsigned char *buffer, long num, long addr, char *output) {
	int j = 0; 
	int z = 0;
	*go = 0;
	*output = 0;
	while (j < num)
	{
		unsigned int addr = j ;
		// if (segment_override == -1)
			// goprintf("%08X  ", addr) ;
		z = buffer[j] ;
		switch (buffer[j])
		{
			case 0x00: parse("add %s", rm8_r8, buffer, &j) ; break ;
			case 0x01: parse("add %s", rm16_r16,buffer, &j) ; break ;
			case 0x02: parse("add %s", r8_rm8, buffer, &j) ; break ;
			case 0x03: parse("add %s", r16_rm16, buffer, &j) ; break ;
			case 0x04: parse("add al,%s", imm8, buffer, &j) ; break ; 
			case 0x05: parse("add ax,%s", imm16, buffer, &j) ; break ;
			case 0x06: parse_noop("push es", buffer, &j); break ; 
			case 0x07: parse_noop("pop es", buffer, &j); break ;
			case 0x08: parse("or %s", rm8_r8, buffer,&j) ; break ;
			case 0x09: parse("or %s", rm16_r16, buffer, &j) ; break ;
			case 0x0A: parse("or %s", r8_rm8, buffer, &j) ; break ;
			case 0x0B: parse("or %s", r16_rm16, buffer, &j) ; break ;
			case 0x0C: parse("or al,%s", imm8, buffer, &j) ; break ; 
			case 0x0D: parse("or ax,%s", imm16, buffer, &j) ; break ;
			case 0x0E: parse_noop("push cs", buffer, &j) ; break ;
			case 0x10: parse("adc %s", rm8_r8, buffer,&j) ; break ;
			case 0x11: parse("adc %s", rm16_r16, buffer, &j) ; break ;
			case 0x12: parse("adc %s", r8_rm8, buffer, &j) ; break ;
			case 0x13: parse("adc %s", r16_rm16, buffer, &j) ; break ;
			case 0x14: parse("adc al,%s", imm8, buffer, &j) ; break ; 
			case 0x15: parse("adc ax,%s", imm16, buffer, &j) ; break ;
			case 0x16: parse_noop("push ss", buffer, &j) ; break ;
			case 0x17: parse_noop("pop ss", buffer, &j) ; break ;
			case 0x18: parse("sbb %s", rm8_r8, buffer,&j) ; break ;
			case 0x19: parse("sbb %s", rm16_r16, buffer, &j) ; break ;
			case 0x1A: parse("sbb %s", r8_rm8, buffer, &j) ; break ;
			case 0x1B: parse("sbb %s", r16_rm16, buffer, &j) ; break ;
			case 0x1C: parse("sbb al,%s", imm8, buffer, &j) ; break ; 
			case 0x1D: parse("sbb ax,%s", imm16, buffer, &j) ; break ;
			case 0x1E: parse_noop("push ds", buffer, &j) ; break ;
			case 0x1F: parse_noop("pop ds", buffer, &j) ; break ;
			case 0x20: parse("and %s", rm8_r8, buffer, &j) ; break ;
			case 0x21: parse("and %s", rm16_r16, buffer, &j) ; break ;
			case 0x22: parse("and %s", r8_rm8, buffer, &j) ; break ;
			case 0x23: parse("and %s", r16_rm16, buffer, &j) ; break ;
			case 0x24: parse("and al,%s", imm8, buffer, &j) ; break ; 
			case 0x25: parse("and ax,%s", imm16, buffer, &j) ; break ;
			case 0x26: 
			{
				segment_override = ES ; 
				rm_segment_override = ES;
			}
			break ;
			case 0x27: parse_noop("daa", buffer, &j) ; break ;
			case 0x28: parse("sub %s", rm8_r8, buffer, &j) ; break ;
			case 0x29: parse("sub %s", rm16_r16, buffer, &j) ; break ;
			case 0x2A: parse("sub %s", r8_rm8, buffer, &j) ; break ;
			case 0x2B: parse("sub %s", r16_rm16, buffer, &j) ; break ;
			case 0x2C: parse("sub al,%s", imm8, buffer, &j) ; break ; 
			case 0x2D: parse("sub ax,%s", imm16, buffer, &j) ; break ;
			case 0x2E: 
			{
				segment_override = CS ; 
				rm_segment_override = CS ;
			} break ;
			case 0x2F: parse_noop("das", buffer, &j) ; break ;
			case 0x30: parse("xor %s", rm8_r8, buffer,&j) ; break ;
			case 0x31: parse("xor %s", rm16_r16, buffer, &j) ; break ;
			case 0x32: parse("xor %s", r8_rm8, buffer, &j) ; break ;
			case 0x33: parse("xor %s", r16_rm16, buffer, &j) ; break ;
			case 0x34: parse("xor al,%s", imm8, buffer, &j) ; break ; 
			case 0x35: parse("xor ax,%s", imm16, buffer, &j) ; break ;
			case 0x36: 
			{
				segment_override = SS ; 
				rm_segment_override = SS ; 
			} break ;
			case 0x37: parse_noop("aaa", buffer, &j) ; break ;
			case 0x38: parse("cmp %s", rm8_r8, buffer,&j) ; break ;
			case 0x39: parse("cmp %s", rm16_r16, buffer, &j) ; break ;
			case 0x3A: parse("cmp %s", r8_rm8, buffer, &j) ; break ;
			case 0x3B: parse("cmp %s", r16_rm16, buffer, &j) ; break ;
			case 0x3C: parse("cmp al,%s", imm8, buffer, &j) ; break ; 
			case 0x3D: parse("cmp ax,%s", imm16, buffer, &j) ; break ;
			case 0x3E: 
			{
				segment_override = DS ; 
				rm_segment_override = DS ; 
			}
			break ;
			case 0x3F: parse_noop("ass", buffer, &j) ; break ;
			case 0x40: parse_noop("inc ax", buffer, &j) ; break ;
			case 0x41: parse_noop("inc cx", buffer, &j) ; break ;
			case 0x42: parse_noop("inc dx", buffer, &j) ; break ;
			case 0x43: parse_noop("inc bx", buffer, &j) ; break ;
			case 0x44: parse_noop("inc sp", buffer, &j) ; break ;
			case 0x45: parse_noop("inc bp", buffer, &j) ; break ;
			case 0x46: parse_noop("inc si", buffer, &j) ; break ;
			case 0x47: parse_noop("inc di", buffer, &j) ; break ;
			case 0x48: parse_noop("dec ax", buffer, &j) ; break ;
			case 0x49: parse_noop("dec cx", buffer, &j) ; break ;
			case 0x4A: parse_noop("dec dx", buffer, &j) ; break ;
			case 0x4B: parse_noop("dec bx", buffer, &j) ; break ;
			case 0x4C: parse_noop("dec sp", buffer, &j) ; break ;
			case 0x4D: parse_noop("dec bp", buffer, &j) ; break ;
			case 0x4E: parse_noop("dec si", buffer, &j) ; break ;
			case 0x4F: parse_noop("dec di", buffer, &j) ; break ;
			case 0x50: parse_noop("push ax", buffer, &j) ; break ;
			case 0x51: parse_noop("push cx", buffer, &j) ; break ;
			case 0x52: parse_noop("push dx", buffer, &j) ; break ;
			case 0x53: parse_noop("push bx", buffer, &j) ; break ;
			case 0x54: parse_noop("push sp", buffer, &j) ; break ;
			case 0x55: parse_noop("push bp", buffer, &j) ; break ;
			case 0x56: parse_noop("push si", buffer, &j) ; break ;
			case 0x57: parse_noop("push di", buffer, &j) ; break ;
			case 0x58: parse_noop("pop ax", buffer, &j) ; break ;
			case 0x59: parse_noop("pop cx", buffer, &j) ; break ;
			case 0x5A: parse_noop("pop dx", buffer, &j) ; break ;
			case 0x5B: parse_noop("pop bx", buffer, &j) ; break ;
			case 0x5C: parse_noop("pop sp", buffer, &j) ; break ;
			case 0x5D: parse_noop("pop bp", buffer, &j) ; break ;
			case 0x5E: parse_noop("pop si", buffer, &j) ; break ;
			case 0x5F: parse_noop("pop di", buffer, &j) ; break ;
			case 0x70: parse("jo %s", rel8, buffer, &j) ; break ;
			case 0x71: parse("jno %s", rel8, buffer, &j) ; break ;
			case 0x72: parse("jc %s", rel8, buffer, &j) ; break ;
			case 0x73: parse("jnc %s", rel8, buffer, &j) ; break ;
			case 0x74: parse("jz %s", rel8, buffer, &j) ; break ;
			case 0x75: parse("jnz %s", rel8, buffer, &j) ; break ;
			case 0x76: parse("jna %s", rel8, buffer, &j) ; break ;
			case 0x77: parse("ja %s", rel8, buffer, &j) ; break ;
			case 0x78: parse("js %s", rel8, buffer, &j) ; break ;
			case 0x79: parse("jns %s", rel8, buffer, &j) ; break ;
			case 0x7A: parse("jpe %s", rel8, buffer, &j) ; break ;
			case 0x7B: parse("jpo %s", rel8, buffer, &j) ; break ;
			case 0x7C: parse("jl %s", rel8, buffer, &j) ; break ;
			case 0x7D: parse("jnl %s", rel8, buffer, &j) ; break ;
			case 0x7E: parse("jng %s", rel8, buffer, &j) ; break ;
			case 0x7F: parse("jg %s", rel8, buffer, &j) ; break ;
			case 0x80:
			{
				unsigned char opcode = ((buffer[++j] & 0x38) >> 3 ); 
				unsigned char t = 0 ;
				j-- ; 
				switch (opcode)
				{
					case 0x00: parse("add %s", rm8_imm8, buffer, &j) ; break ;
					case 0x01: parse("or %s", rm8_imm8, buffer, &j) ; break ;
					case 0x02: parse("adc %s", rm8_imm8, buffer, &j) ; break ;
					case 0x03: parse("sbb %s", rm8_imm8, buffer, &j) ; break ;
					case 0x04: parse("and %s", rm8_imm8, buffer, &j) ; break ;
					case 0x05: parse("sub %s", rm8_imm8, buffer, &j) ; break ;
					case 0x06: parse("xor %s", rm8_imm8, buffer, &j) ; break ;
					case 0x07: parse("cmp %s", rm8_imm8, buffer, &j) ; break ;
					default: t = 1; break ;
				}
				if (t) goto print_symbol ;
			} break ;
			case 0x81:
			{
				unsigned char opcode = ((buffer[++j] & 0x38) >> 3 ); 
				unsigned char t = 0 ;
				j-- ; 
				switch (opcode)
				{
					case 0x00: parse("add %s", rm16_imm16, buffer, &j) ; break ;
					case 0x01: parse("or %s", rm16_imm16, buffer, &j) ; break ;
					case 0x02: parse("adc %s", rm16_imm16, buffer, &j) ; break ;
					case 0x03: parse("sbb %s", rm16_imm16, buffer, &j) ; break ;
					case 0x04: parse("and %s", rm16_imm16, buffer, &j) ; break ;
					case 0x05: parse("sub %s", rm16_imm16, buffer, &j) ; break ;
					case 0x06: parse("xor %s", rm16_imm16, buffer, &j) ; break ;
					case 0x07: parse("cmp %s", rm16_imm16, buffer, &j) ; break ;
					default: t = 1; break ;
				}
				if (t) goto print_symbol ;
			} break ;
			case 0x83:
			{
				unsigned char opcode = ((buffer[++j] & 0x38) >> 3 ); 
				unsigned char t = 0 ;
				j-- ;
				switch (opcode)
				{
					case 0x00: parse("add %s", rm16_imm8, buffer, &j) ; break ;
					case 0x02: parse("adc %s", rm16_imm8, buffer, &j) ; break ;
					case 0x03: parse("sbb %s", rm16_imm8, buffer, &j) ; break ;
					case 0x05: parse("sub %s", rm16_imm8, buffer, &j) ; break ;
					case 0x07: parse("cmp %s", rm16_imm8, buffer, &j) ; break ;
					default: t = 1; break ;
				} 
				if (t) goto print_symbol ;
			} break ;
			case 0x84: parse("test %s", rm8_r8, buffer, &j) ; break ;
			case 0x85: parse("test %s", rm16_r16, buffer, &j) ; break ;
			case 0x86: parse("xchg %s", rm8_r8, buffer, &j) ; break ;
			case 0x87: parse("xchg %s", rm16_r16, buffer, &j) ; break ;
			case 0x88: parse("mov %s", rm8_r8, buffer, &j) ; break ;
			case 0x89: parse("mov %s", rm16_r16, buffer, &j) ; break ;
			case 0x8A: parse("mov %s", r8_rm8, buffer, &j) ; break ;
			case 0x8B: parse("mov %s", r16_rm16, buffer, &j) ; break ;
			case 0x8C: 
			{ 
				parse("mov %s", rm16_sreg, buffer, &j) ; break ;
			}
			case 0x8D: parse("lea %s", r16_rm16, buffer, &j) ; break ;
			case 0x8E: 
			{
					parse("mov %s", sreg_rm16, buffer, &j) ; break ;
			}
			case 0x8F:
			{
			  	unsigned char opcode = ((buffer[++j] & 0x38) >> 3 ); 
				unsigned char t = 0 ;
				j-- ; 
				switch (opcode)
				{
					case 0x00: parse("pop word %s", m16, buffer, &j) ; break ;
					default: t = 1; break ;
				}	
				if (t) goto print_symbol ;
			} break ;
			case 0x90: parse_noop("xchg ax,ax", buffer, &j) ; break ;
			case 0x91: parse_noop("xchg cx,ax", buffer, &j) ; break ;
			case 0x92: parse_noop("xchg dx,ax", buffer, &j) ; break ;
			case 0x93: parse_noop("xchg bx,ax", buffer, &j) ; break ;
			case 0x94: parse_noop("xchg sp,ax", buffer, &j) ; break ;
			case 0x95: parse_noop("xchg bp,ax", buffer, &j) ; break ;
			case 0x96: parse_noop("xchg si,ax", buffer, &j) ; break ;
			case 0x97: parse_noop("xchg di,ax", buffer, &j) ; break ;
			case 0x98: parse_noop("cbw", buffer, &j) ; break ;
			case 0x99: parse_noop("cwd", buffer, &j) ; break ;
			case 0x9A: parse("call %s", call_inter, buffer, &j) ; break ; 
			case 0x9B: parse_noop("wait", buffer, &j) ; break ;
			case 0x9C: parse_noop("pushf", buffer, &j) ; break ;
			case 0x9D: parse_noop("popf", buffer, &j) ; break ;
			case 0x9E: parse_noop("sahf", buffer, &j) ; break ;
			case 0x9F: parse_noop("lahf", buffer, &j) ; break ;
			case 0xA0: parse("mov al,%s", moffs16, buffer, &j) ; break ;
			case 0xA1: parse("mov ax,%s", moffs16, buffer, &j) ; break ;
			case 0xA2: parse("mov %s,al", moffs16, buffer, &j) ; break ;
			case 0xA3: parse("mov %s,ax", moffs16, buffer, &j) ; break ;
			case 0xA4: parse_noop("movsb", buffer, &j) ; break ;
			case 0xA5: parse_noop("movsw", buffer, &j) ; break ;
			case 0xA6: parse_noop("cmpsb", buffer, &j) ; break ;
			case 0xA7: parse_noop("cmpsw", buffer, &j) ; break ;
			case 0xA8: parse("test al, %s", imm8, buffer, &j) ; break ;
			case 0xA9: parse("test ax, %s", imm16, buffer, &j) ; break ;
			case 0xAA: parse_noop("stosb", buffer, &j) ; break ;
			case 0xAB: parse_noop("stosw", buffer, &j) ; break ;
			case 0xAC: parse_noop("lodsb", buffer, &j) ; break ;
			case 0xAD: parse_noop("lodsw", buffer, &j) ; break ;
			case 0xAE: parse_noop("scasb", buffer, &j) ; break ;
			case 0xAF: parse_noop("scasw", buffer, &j) ; break ;
			case 0xB0: parse("mov al,%s",imm8, buffer, &j); break;
			case 0xB1: parse("mov cl,%s",imm8, buffer, &j); break;
			case 0xB2: parse("mov dl,%s",imm8, buffer, &j); break;
			case 0xB3: parse("mov bl,%s",imm8, buffer, &j); break;
			case 0xB4: parse("mov ah,%s",imm8, buffer, &j); break;
			case 0xB5: parse("mov ch,%s",imm8, buffer, &j); break;
			case 0xB6: parse("mov dh,%s",imm8, buffer, &j); break;
			case 0xB7: parse("mov bh,%s",imm8, buffer, &j); break;
			case 0xB8: parse("mov ax,%s",imm16, buffer, &j); break;
			case 0xB9: parse("mov cx,%s",imm16, buffer, &j); break;
			case 0xBA: parse("mov dx,%s",imm16, buffer, &j); break;
			case 0xBB: parse("mov bx,%s",imm16, buffer, &j); break;
			case 0xBC: parse("mov sp,%s",imm16, buffer, &j); break;
			case 0xBD: parse("mov bp,%s",imm16, buffer, &j); break;
			case 0xBE: parse("mov si,%s",imm16, buffer, &j); break;
			case 0xBF: parse("mov di,%s",imm16, buffer, &j); break;
			case 0xC2: parse("ret %s", imm16, buffer, &j) ; break ; 
			case 0xC3: parse_noop("ret", buffer, &j) ; break ;
			case 0xC4: parse("les %s", r16_rm16, buffer, &j) ; break ;
			case 0xC5: parse("lds %s", r16_rm16, buffer, &j) ; break ;
			case 0xC6: parse("mov %s", rm16_imm8, buffer, &j) ; break ;
			case 0xC7: parse("mov %s", rm16_imm16, buffer, &j) ; break ;
			case 0xCA: parse("retf %s", imm16, buffer, &j) ; break ;
			case 0xCB: parse_noop("retf", buffer, &j) ; break ;
			case 0xCC: parse_noop("int3", buffer, &j) ; break ;
			case 0xCD: parse("int %s", imm8, buffer, &j) ; break ;
			case 0xCE: parse_noop("into", buffer, &j) ; break ;
			case 0xCF: parse_noop("iret", buffer, &j) ; break ;
			case 0xD0:
			{
				unsigned char opcode = ((buffer[++j] & 0x38) >> 3 ); 
				unsigned char t = 0 ;
				j-- ;
				switch (opcode)
				{
					case 0x00: parse("rol %s,1", rm8, buffer, &j) ; break ;
					case 0x01: parse("ror %s,1", rm8, buffer, &j) ; break ;
					case 0x02: parse("rcl %s,1", rm8, buffer, &j) ; break ;
					case 0x03: parse("rcr %s,1", rm8, buffer, &j) ; break ;
					case 0x04: parse("shl %s,1", rm8, buffer, &j) ; break ;
					case 0x05: parse("shr %s,1", rm8, buffer, &j) ; break ;
					case 0x07: parse("sar %s,1", rm8, buffer, &j) ; break ;
					default: t = 1; break ;
				}
				if (t) goto print_symbol ;
			} break ;
			case 0xD1:
			{
				unsigned char opcode = ((buffer[++j] & 0x38) >> 3 ); 
				unsigned char t = 0 ;
				j-- ;	
				switch (opcode)
				{
					case 0x00: parse("rol %s,1", rm16, buffer, &j) ; break ;
					case 0x01: parse("ror %s,1", rm16, buffer, &j) ; break ;
					case 0x02: parse("rcl %s,1", rm16, buffer, &j) ; break ;
					case 0x03: parse("rcr %s,1", rm16, buffer, &j) ; break ;
					case 0x04: parse("shl %s,1", rm16, buffer, &j) ; break ;
					case 0x05: parse("shr %s,1", rm16, buffer, &j) ; break ;
					case 0x07: parse("sar %s,1", rm16, buffer, &j) ; break ;
					default: t = 1; break ;
				}
				if (t) goto print_symbol ;
			} break ;
			case 0xD2:
			{
				unsigned char opcode = ((buffer[++j] & 0x38) >> 3 ); 
				unsigned char t = 0 ;
				j-- ;	
				switch (opcode)
				{
					case 0x00: parse("rol %s,cl", rm8, buffer, &j) ; break ;
					case 0x01: parse("ror %s,cl", rm8, buffer, &j) ; break ;
					case 0x02: parse("rcl %s,cl", rm8, buffer, &j) ; break ;
					case 0x03: parse("rcr %s,cl", rm8, buffer, &j) ; break ;
					case 0x04: parse("shl %s,cl", rm8, buffer, &j) ; break ;
					case 0x05: parse("shr %s,cl", rm8, buffer, &j) ; break ;
					case 0x07: parse("sar %s,cl", rm8, buffer, &j) ; break ;
					default: t = 1; break ;
				}
				if (t) goto print_symbol ;
			} break ;
			case 0xD3:
			{
				unsigned char opcode = ((buffer[++j] & 0x38) >> 3 ); 
				unsigned char t = 0 ;
				j-- ;	
				switch (opcode)
				{
					case 0x00: parse("rol %s,cl", rm16, buffer, &j) ; break ;
					case 0x01: parse("ror %s,cl", rm16, buffer, &j) ; break ;
					case 0x02: parse("rcl %s,cl", rm16, buffer, &j) ; break ;
					case 0x03: parse("rcr %s,cl", rm16, buffer, &j) ; break ;
					case 0x04: parse("shl %s,cl", rm16, buffer, &j) ; break ;
					case 0x05: parse("shr %s,cl", rm16, buffer, &j) ; break ;
					case 0x07: parse("sar %s,cl", rm16, buffer, &j) ; break ;
					default: t = 1; break ;
				}
				if (t) goto print_symbol ;
			} break ;
			case 0xD4: parse_noop("aam", buffer, &j) ; break ;
			case 0xD5: parse_noop("aad", buffer, &j) ; break ;
			case 0xD7: parse_noop("xlatb", buffer, &j) ; break ;
			/*D8-DF => ESC0-7*/
			case 0xE0: parse("loopne %s", rel8, buffer, &j) ; break ;
			case 0xE1: parse("loope %s", rel8, buffer, &j) ; break ;
			case 0xE2: parse("loop %s", rel8, buffer, &j) ; break ;
			case 0xE3: parse("jcxz %s", rel8, buffer, &j) ; break ;
			case 0xE4: parse("in al,%s", imm8, buffer, &j) ; break ;
			case 0xE5: parse("in ax,%s", imm8, buffer, &j) ; break ;
			case 0xE6: parse("out %s,al", imm8, buffer, &j) ; break ;
			case 0xE7: parse("out %s,ax", imm8, buffer, &j) ; break ;
			case 0xE8: parse("call %s", rel16, buffer, &j) ; break ;
			case 0xE9: parse("jmp %s", rel16, buffer, &j) ; break ;
			case 0xEA: parse("jmp %s", call_inter, buffer, &j) ; break ;
			case 0xEB: parse("jmp short %s", rel8, buffer, &j) ; break ;
			case 0xEC: parse_noop("in al,dx", buffer, &j) ; break ;
			case 0xED: parse_noop("in ax,dx", buffer, &j) ; break ;
			case 0xEE: parse_noop("out dx,al", buffer, &j) ; break ;
			case 0xEF: parse_noop("out dx,ax", buffer, &j) ; break ;
			case 0xF0: goprintf("lock ", NULL) ; break ;
			case 0xF2: goprintf("repne ", NULL) ; break ;
			case 0xF3: goprintf("rep ", NULL) ; break ;
			case 0xF4: parse_noop("hlt", buffer, &j) ; break ;
			case 0xF5: parse_noop("cmc", buffer, &j) ; break ;
			case 0xF6:
			{
				unsigned char opcode = ((buffer[++j] & 0x38) >> 3 ); 
				unsigned char t = 0 ;
				j-- ;	
				switch (opcode)
				{
					case 0x00: parse("test %s", rm8_imm8, buffer, &j) ; break ;
					case 0x02: parse("not %s", rm8, buffer, &j) ; break ;
					case 0x03: parse("neg %s", rm8, buffer, &j) ; break ;
					case 0x04: parse("mul %s", rm8, buffer, &j) ; break ;
					case 0x05: parse("imul %s", rm8, buffer, &j) ; break ;
					case 0x06: parse("div %s", rm8, buffer, &j) ; break ;
					case 0x07: parse("idiv %s", rm8, buffer, &j) ; break ;
					default: t = 1; break ;
				}
				if (t) goto print_symbol ;
			} break ;
			case 0xF7:
			{
				unsigned char opcode = ((buffer[++j] & 0x38) >> 3 ); 
				unsigned char t = 0 ;
				j-- ;	
				switch (opcode)
				{
					case 0x00: parse("test %s", rm16_imm16, buffer, &j) ; break ;
					case 0x02: parse("not %s", rm16, buffer, &j) ; break ;
					case 0x03: parse("neg %s", rm16, buffer, &j) ; break ;
					case 0x04: parse("mul %s", rm16, buffer, &j) ; break ;
					case 0x05: parse("imul %s", rm16, buffer, &j) ; break ;
					case 0x06: parse("div %s", rm16, buffer, &j) ; break ;
					case 0x07: parse("idiv %s", rm16, buffer, &j) ; break ;
					default: t = 1; break ;
				}
				if (t) goto print_symbol ;
			} break ;
			case 0xF8: parse_noop("clc", buffer, &j) ; break ;
			case 0xF9: parse_noop("stc", buffer, &j) ; break ;
			case 0xFA: parse_noop("cli", buffer, &j) ; break ;
			case 0xFB: parse_noop("sti", buffer, &j) ; break ;
			case 0xFC: parse_noop("cld", buffer, &j) ; break ;
			case 0xFD: parse_noop("std", buffer, &j) ; break ;
			case 0xFE:
			{
				unsigned char opcode = ((buffer[++j] & 0x38) >> 3 ); 
				unsigned char t = 0 ;	
				j-- ;
				switch (opcode)
				{
					case 0x00: parse("inc %s", rm8, buffer,&j) ; break ;
					case 0x01: parse("dec %s", rm8, buffer,&j) ; break ;
					default: t = 1; break ;
				}
				if (t) goto print_symbol ;
			} break ;
			case 0xFF:
			{
				unsigned char opcode = ((buffer[++j] & 0x38) >> 3 ); 
				unsigned char t = 0 ;
				j-- ;	
				switch (opcode)
				{
					case 0x00: parse("inc %s", rm16, buffer,&j) ; break ;
					case 0x01: parse("dec %s", rm16, buffer,&j) ; break ;
					case 0x02: parse("call near %s", rm16, buffer,&j) ; break ;
					case 0x03: parse("call far %s", rm16, buffer,&j) ; break ;
					case 0x04: parse("jmp near %s", rm16, buffer,&j) ; break ;
					case 0x05: parse("jmp far %s", rm16, buffer,&j) ; break ;
					case 0x06: parse("push %s", rm16, buffer,&j) ; break ;
					default: t = 1; break ;
				}
				if (t) goto print_symbol ;
			} break ;
			print_symbol:
			default: 
			{
				char tmp_buffer[20] ; 
				memset(tmp_buffer, '\0', 20) ;
				sprintf(tmp_buffer, "db 0x%X", buffer[j]) ;
				parse_noop(tmp_buffer, buffer, &j) ;
				break ;
			}
		}
		j++ ;
		break;
	}
	strcpy ((char*)output, (const char *)go);
	return j;
}

static char str[255] ; 

static char *moffs16(char *buffer, int *j, int *err) {
	unsigned char low, high;
	unsigned short imm16;
	char segment[10] ;
	memset(str, '\0', 255) ;
	memset(segment, '\0', 10) ;
	if (segment_override >= 0) {
		switch (segment_override) {
			case ES: sprintf(segment, "es:") ; break ;
			case CS: sprintf(segment, "cs:") ; break ;
			case SS: sprintf(segment, "ss:") ; break ;
			case DS: sprintf(segment, "ds:") ; break ;
		}
		segment_override = -1 ;
	}
	if (get_bytes(2, *j)) {
		*err = 1 ; 
		return str ; 
	}
	(*j)++ ; 
	bytes++ ;
	low = buffer[*j] ; 
	(*j)++ ;
	bytes++ ; 
	high = buffer[*j] ; 
	imm16 = ((high << 8) + low) ;
	sprintf(str, "[%s0x%x]", segment, imm16) ;
	return str ;
}

char *rm8(char *buffer, int *j, int *err) {
	int error = 0 ;
	const char *s;
	memset(str, '\0', 255) ;
	s =  rm(buffer, j, 8, &error)  ;
	if (error) {
		*err = 1 ;
		return str ;
	}
	sprintf(str, "%s", s) ; 
	return str ;
}

char *rm16(char *buffer, int *j, int *err)
{
	int error = 0; 
	const char*s;
	memset(str, '\0', 255) ;
	s =  rm(buffer, j, 16, &error)  ;
	if (error)
	{
		*err = error ;
		return str; 
	}
	sprintf(str, "%s", s) ; 
	return str ;
}

static const char *call_inter(const unsigned char *buffer, int *j, int *err)
{
	unsigned char seg_high;
	unsigned short offset, seg;
	unsigned char offset_low, offset_high, seg_low;
	memset(str, '\0', 255) ;
	if (get_bytes(4, *j))
	{
		*err = 1 ; 
		return str ;
	}
	(*j)++ ;
	bytes++ ; 
	offset_low = buffer[*j] ; 
	(*j)++ ;
	bytes++ ;
	offset_high = buffer[*j] ; 
	(*j)++ ;
	bytes++ ;
	seg_low = buffer[*j] ; 
	(*j)++ ;
	bytes++ ;

	seg_high = buffer[*j] ; 
	offset = ((offset_high << 8) + offset_low) ; 
	seg = ((seg_high << 8) + seg_low) ;
	sprintf(str,"0x%x:0x%x", seg, offset) ;
	return str ;	
}
static const char *m16(const unsigned char *buffer, int *j, int *err)
{
	int error = 0; 
	const char *s =  rm(buffer, j, 16, &error)  ;
	memset(str, '\0', 255) ;
	if (error)
	{
		*err = 1 ;
		return str ;
	}
	sprintf(str,"%s", s) ;
	return str ;
}

static const char *sreg_rm16(const unsigned char *buffer, int *j, int *error) {
	unsigned char reg;
	const char *s;
	int err = 0 ;
	memset(str, '\0', 255) ;
	if (get_bytes(1, *j))
	{
		*error = 1 ; 
		return str ;
	}
	reg = ((buffer[++(*j)] & 0x38) >> 3) ;
	(*j)-- ;
	s =  rm(buffer, j, 16, &err)  ;
	if (err)
	{
		*error = 1 ;
		return str ;
	}
	if (reg < 4)
	{
		const char *sreg = segreg[reg] ;
		sprintf(str,"%s,%s", sreg, s) ;
		*error = 0 ;
	} else *error = 1 ; 
	return str ;
}

static const char *rm16_sreg(const unsigned char *buffer, int *j, int *error)
{
	int err = 0; 
	unsigned char reg;
	const char *s;
	memset(str, '\0', 255) ;
	if (get_bytes(1, *j))
	{
		*error = 1 ; 
		return str ;
	}
	reg = ((buffer[++(*j)] & 0x38) >> 3) ;
	(*j)-- ; 
	s =  rm(buffer, j, 16, &err)  ;
	if (err)
	{
		*error = 1 ;
		return str ;
	}
	if (reg < 4)
	{
		const char *sreg = segreg[reg] ;
		sprintf(str,"%s,%s", s, sreg) ;
		*error = 0 ;
	} else *error = 1 ; 
	return str ;
}

static const char *rm16_imm8(const unsigned char *buffer, int *j, int *err)
{
	int error = 0 ;
	const char *s;
	memset(str, '\0', 255) ;
	s = rm(buffer, j, 16, &error) ;
	if (error)
	{
		*err = 1 ; 
		return str ;
	}
	(*j)++ ;
	if (get_bytes(1, *j))
	{
		(*j)--;
		*err = 1 ; 
		return str ;
	}
	(*j)--;
	(*j)++; 
	bytes++ ;
	{
	signed char imm8 = buffer[*j] ; 
	char sign = '+' ;
	if (imm8 < 0) 
	{
		sign = '-' ;
		imm8 = -imm8 ;
	}
	sprintf(str, "%s,byte %c0x%x", s, sign, imm8) ;
	}
	return str ;
}
char *rm16_imm16(char *buffer, int *j, int *err)
{
	int error = 0; 
const char*s;
	memset(str, '\0', 255) ;
	s = rm(buffer, j, 16, &error) ;
	if (error)
	{
		*err = 1 ; 
		return str ;
	}
	(*j)++ ;
	if (get_bytes(2, *j))
	{
		(*j)-- ;
		*err = 1 ;
		return str ;
	}
	(*j)-- ;
	(*j)++;
	bytes++ ;
	{
	unsigned char low = buffer[*j] ; 
	unsigned char high;
	unsigned short imm16;

	(*j)++ ;
	bytes++ ;
	high = buffer[*j] ; 
	imm16 = ((high << 8) + low) ; 
	sprintf(str, "%s,0x%x", s, imm16) ;
	}
	return str ;
}
char *rm8_imm8(char *buffer, int *j, int *err)
{
	int error = 0;
	const char *s;
	memset(str, '\0', 255) ;
	s = rm(buffer, j, 8, &error) ;
	if (error)
	{
		*err = 1 ; 
		return str ;
	}
	(*j)++ ;
	if (get_bytes(1, *j))
	{
		(*j)-- ;
		*err = 1 ; 
		return str ;
	} 
	(*j)-- ; 
	(*j)++;
	bytes++ ;
	{
	unsigned char imm8 = buffer[*j] ; 
	sprintf(str, "%s,0x%x", s, imm8) ;
	}
	return str ;  
}

char *rel16(char *buffer, int *j, int *err)
{
	memset(str, '\0', 255) ;
	if (get_bytes(2, *j))
	{
		*err = 1 ;
		return str ;
	}
	(*j)++ ;
	bytes++ ;
{
	unsigned char rel_low, rel_high;
	rel_low = buffer[*j] ; 
	(*j)++ ;
	bytes++ ; 
	rel_high = buffer[*j] ; 
{
	signed short rel = ((rel_high << 8) + rel_low) ;
	unsigned short result = *j + rel + 1 ;
	sprintf(str, "0x%x", result) ; 
}
	}
	return str ;
}


char *rel8(char *buffer, int *j, int *err)
{
	memset(str, '\0', 255) ;
	if (get_bytes(1, *j))
	{
		*err = 1 ;
		return str ;
	}
	(*j)++ ;
	bytes++ ;
{
	signed char rel = buffer[*j] ; 
	unsigned short result = *j + rel + 1 ;
	sprintf(str, "0x%x", result) ; 
}
	return str ;
}

char *imm8(char *buffer, int *j, int *err)
{
	memset(str, '\0', 255) ;
	if (get_bytes(1, *j))
	{
		*err = 1 ;
		return str ;
	}
	(*j)++ ;
	bytes++ ;
{
	unsigned char imm8 = buffer[*j] ; 
	sprintf(str, "0x%x", imm8) ; 
}
	return str ; 
}

char *imm16(char *buffer, int *j, int *err)
{
	memset(str, '\0', 255) ;
	if (get_bytes(2, *j))
	{
		*err = 1 ;
		return str ;
	}
	(*j)++ ; 
	bytes++ ;
{
	unsigned char low = buffer[*j] ; 
	(*j)++ ;
	bytes++ ;
{
	unsigned char high = buffer[*j] ; 
	unsigned short imm16 = ((high << 8) + low) ;
	sprintf(str, "0x%x", imm16); 
}
}
	return str ;
}

char *r16_rm16(char *buffer, int *j, int *err)
{
	int error = 0 ;
	memset(str, '\0', 255) ;
{
	unsigned char reg = ((buffer[++(*j)] & 0x38) >> 3 ); 
	(*j)-- ; 
{
	const char *s = rm(buffer, j, 16, &error) ;
	if (error)
	{
		*err = 1 ; 
		return str ;
	}
	sprintf(str, "%s,%s", regs16[reg], s) ;
}}
	return str ;
}

char *rm8_r8(char *buffer, int *j, int *err) {
	int error = 0; 
	memset(str, '\0', 255) ;
	{
		unsigned char reg = ((buffer[++(*j)] & 0x38) >> 3 ); 
		(*j)-- ; 
		{
			const char *s = rm(buffer, j, 8, &error) ;
			if (error)
			{
				*err = 1 ;
				return str ;
			}
			{
				const char *reg8 = regs8[reg] ; 
				sprintf(str, "%s,%s", s, reg8) ;
			}
		}
	}
	return str;
}

char *r8_rm8(char *buffer, int *j, int *err)
{
	int error = 0 ;
	memset(str, '\0', 255) ;
{
	unsigned char reg = ((buffer[++(*j)] & 0x38) >> 3 ); 
	(*j)-- ; 
{
	const char *s = rm(buffer, j, 8, &error) ;
	if (error)
	{
		*err = 1 ;
		return str ;
	}
	sprintf(str, "%s,%s", regs8[reg], s) ; 
}}
	return str ;
}

static const char *rm16_r16(const unsigned char *buffer, int *j, int *err)
{
	int error = 0;
	memset(str, '\0', 255) ; 
{
	unsigned char reg = ((buffer[++(*j)] & 0x38) >> 3 ); 
	(*j)-- ;
{
	const char *s = rm(buffer, j, 16, &error) ;
	if (error)
	{
		*err = 1 ;
		return str ;
	}
{
	const char *reg16 = regs16[reg] ; 
	sprintf(str, "%s,%s", s, reg16) ;
}}}
	return str ;
}

static char rm_str[255] ; 

static const char *rm(const unsigned char *buffer, int *j, char type, int *error) {
	char disp_str[255] ; 
	char segment[10] ;
	unsigned char rm_byte, mod, rm8;
	memset(rm_str, '\0', 255) ; 
	bytes++ ;
	if (get_bytes(1, *j))
	{
		bytes = 1 ;
		*error = 1; 
		return rm_str ;  
	}
	rm_byte = buffer[++(*j)] ; 
	mod = (rm_byte >> 6) ; 
	rm8 = (rm_byte & 7) ; 
	memset(rm_str, '\0', 255) ; 
	memset(disp_str, '\0', 255) ;
	memset(segment, '\0', 10) ;
	if (segment_override >= 0)
	{
		switch (segment_override)
		{
			case ES: sprintf(segment, "es:") ; break ;
			case CS: sprintf(segment, "cs:") ; break ;
			case SS: sprintf(segment, "ss:") ; break ;
			case DS: sprintf(segment, "ds:") ; break ;
		}
	}
	switch (mod)
	{
		case 0x0:
		{
		if (rm8 == 0x06)
		{
			if (get_bytes(2, *j))
			{
				*error = 1 ;
				(*j)-- ; 
				return rm_str ;  
			}
			(*j)++ ; 
			bytes++ ;
{
			unsigned char low = buffer[*j] ;
			(*j)++ ; 
			bytes++ ;
{
			unsigned char high = buffer[*j] ; 
			unsigned short disp = ((high << 8) + low) ; 
			char sign = '+' ;
			sprintf(disp_str, "%c0x%x", sign, disp) ;
}}
		}
		else sprintf(disp_str, "") ;
		} break ; 
		case 0x01:
		{
			if (get_bytes(1, *j))
			{
				*error = 1;
				(*j)-- ;
				return rm_str ;  
			}
{
			signed char disp_low = buffer[++(*j)] ; 
			signed short disp = disp_low ; 
			char sign = '+' ; 
			bytes++ ;
			if (disp < 0) 
			{
				sign = '-' ;
				disp = ~disp ;
				disp++ ;
			}
			sprintf(disp_str, "%c0x%x", sign, disp) ; 
}
		} break ;
		case 0x02:
		{
			if (get_bytes(2, *j))
			{
				*error = 1;
				(*j)-- ; 
				return rm_str ;  
			}
			(*j)++ ; 
			bytes++ ;
{
			unsigned char low = buffer[*j] ;
			(*j)++ ; 
			bytes++ ;
{
			unsigned char high = buffer[*j] ;
			unsigned short disp = ((high << 8) + low) ; 
			char sign = '+' ;
			sprintf(disp_str, "%c0x%x", sign, disp) ;
}}
		} break ;
		case 0x03:
		{
			if (type == 8)
			{
 				return regs8[rm8] ;
			}
			if (type == 16)
			{
				return regs16[rm8]; 
			}
		} break ;
	}
	switch (rm8)
	{
		case 0x00: sprintf(rm_str, "[%sbx+si%s]", segment, disp_str) ; break ;
		case 0x01: sprintf(rm_str, "[%sbx+di%s]", segment, disp_str) ; break ;
		case 0x02: sprintf(rm_str, "[%sbp+si%s]", segment, disp_str) ; break ;
		case 0x03: sprintf(rm_str, "[%sbp+di%s]", segment, disp_str) ; break ;
		case 0x04: sprintf(rm_str, "[%ssi%s]", segment, disp_str) ; break ;
		case 0x05: sprintf(rm_str, "[%sdi%s]", segment, disp_str) ; break ;
		case 0x06: sprintf(rm_str, "[%sbp%s]", segment, disp_str) ; break ; 
		case 0x07: sprintf(rm_str, "[%sbx%s]", segment, disp_str) ; break ;
	}
	return rm_str ; 
}
