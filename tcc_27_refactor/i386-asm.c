/*
 *  i386 specific functions for TCC assembler
 *
 *  Copyright (c) 2001, 2002 Fabrice Bellard
 *  Copyright (c) 2009 Frédéric Feret (x86_64 support)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#define MAX_OPERANDS 3

#define TOK_ASM_first TOK_ASM_clc
#define TOK_ASM_last TOK_ASM_emms
#define TOK_ASM_alllast TOK_ASM_subps

#define OPC_B          0x01  /* only used with OPC_WL */
#define OPC_WL         0x02  /* accepts w, l or no suffix */
#define OPC_BWL        (OPC_B | OPC_WL) /* accepts b, w, l or no suffix */
#define OPC_REG        0x04 /* register is added to opcode */
#define OPC_MODRM      0x08 /* modrm encoding */

#define OPCT_MASK      0x70
#define OPC_FWAIT      0x10 /* add fwait opcode */
#define OPC_SHIFT      0x20 /* shift opcodes */
#define OPC_ARITH      0x30 /* arithmetic opcodes */
#define OPC_FARITH     0x40 /* FPU arithmetic opcodes */
#define OPC_TEST       0x50 /* test opcodes */
#define OPCT_IS(v,i) (((v) & OPCT_MASK) == (i))

#define OPC_0F        0x100 /* Is secondary map (0x0f prefix) */
#define OPC_48        0x200 /* Always has REX prefix */
# define OPC_WLX     OPC_WL
# define OPC_BWLX    OPC_BWL

#define OPC_GROUP_SHIFT 13

/* in order to compress the operand type, we use specific operands and
   we or only with EA  */
enum {
    OPT_REG8=0, /* warning: value is hardcoded from TOK_ASM_xxx */
    OPT_REG16,  /* warning: value is hardcoded from TOK_ASM_xxx */
    OPT_REG32,  /* warning: value is hardcoded from TOK_ASM_xxx */
    OPT_MMX,    /* warning: value is hardcoded from TOK_ASM_xxx */
    OPT_SSE,    /* warning: value is hardcoded from TOK_ASM_xxx */
    OPT_CR,     /* warning: value is hardcoded from TOK_ASM_xxx */
    OPT_TR,     /* warning: value is hardcoded from TOK_ASM_xxx */
    OPT_DB,     /* warning: value is hardcoded from TOK_ASM_xxx */
    OPT_SEG,
    OPT_ST,
    OPT_IM8,
    OPT_IM8S,
    OPT_IM16,
    OPT_IM32,
    OPT_EAX,    /* %al, %ax, %eax or %rax register */
    OPT_ST0,    /* %st(0) register */
    OPT_CL,     /* %cl register */
    OPT_DX,     /* %dx register */
    OPT_ADDR,   /* OP_EA with only offset */
    OPT_INDIR,  /* *(expr) */
    /* composite types */
    OPT_COMPOSITE_FIRST,
    OPT_IM,     /* IM8 | IM16 | IM32 */
    OPT_REG,    /* REG8 | REG16 | REG32 | REG64 */
    OPT_REGW,   /* REG16 | REG32 | REG64 */
    OPT_IMW,    /* IM16 | IM32 */
    OPT_MMXSSE, /* MMX | SSE */
    OPT_DISP,   /* Like OPT_ADDR, but emitted as displacement (for jumps) */
    OPT_DISP8,  /* Like OPT_ADDR, but only 8bit (short jumps) */
    /* can be ored with any OPT_xxx */
    OPT_EA = 0x80
};

#define OP_REG8   (1 << OPT_REG8)
#define OP_REG16  (1 << OPT_REG16)
#define OP_REG32  (1 << OPT_REG32)
#define OP_MMX    (1 << OPT_MMX)
#define OP_SSE    (1 << OPT_SSE)
#define OP_CR     (1 << OPT_CR)
#define OP_TR     (1 << OPT_TR)
#define OP_DB     (1 << OPT_DB)
#define OP_SEG    (1 << OPT_SEG)
#define OP_ST     (1 << OPT_ST)
#define OP_IM8    (1 << OPT_IM8)
#define OP_IM8S   (1 << OPT_IM8S)
#define OP_IM16   (1 << OPT_IM16)
#define OP_IM32   (1 << OPT_IM32)
#define OP_EAX    (1 << OPT_EAX)
#define OP_ST0    (1 << OPT_ST0)
#define OP_CL     (1 << OPT_CL)
#define OP_DX     (1 << OPT_DX)
#define OP_ADDR   (1 << OPT_ADDR)
#define OP_INDIR  (1 << OPT_INDIR)
# define OP_REG64 0
# define OP_REG8_LOW 0
# define OP_IM64  0
# define OP_EA32  0

#define OP_EA     0x40000000
#define OP_REG    (OP_REG8 | OP_REG16 | OP_REG32 | OP_REG64)

# define TREG_XAX   TREG_EAX
# define TREG_XCX   TREG_ECX
# define TREG_XDX   TREG_EDX

typedef struct ASMInstr {
    uint16_t sym;
    uint16_t opcode;
    uint16_t instr_type;
    uint8_t nb_ops;
    uint8_t op_type[MAX_OPERANDS]; /* see OP_xxx */
} ASMInstr;

typedef struct Operand {
    uint32_t type;
    int8_t  reg; /* register, -1 if none */
    int8_t  reg2; /* second register, -1 if none */
    uint8_t shift;
    ExprValue e;
} Operand;

static const uint8_t reg_to_size[9] = {
/*
    [OP_REG8] = 0,
    [OP_REG16] = 1,
    [OP_REG32] = 2,
*/
    0, 0, 1, 0, 2, 0, 0, 0, 3
};

#define NB_TEST_OPCODES 30

static const uint8_t test_bits[NB_TEST_OPCODES] = {
 0x00, /* o */
 0x01, /* no */
 0x02, /* b */
 0x02, /* c */
 0x02, /* nae */
 0x03, /* nb */
 0x03, /* nc */
 0x03, /* ae */
 0x04, /* e */
 0x04, /* z */
 0x05, /* ne */
 0x05, /* nz */
 0x06, /* be */
 0x06, /* na */
 0x07, /* nbe */
 0x07, /* a */
 0x08, /* s */
 0x09, /* ns */
 0x0a, /* p */
 0x0a, /* pe */
 0x0b, /* np */
 0x0b, /* po */
 0x0c, /* l */
 0x0c, /* nge */
 0x0d, /* nl */
 0x0d, /* ge */
 0x0e, /* le */
 0x0e, /* ng */
 0x0f, /* nle */
 0x0f, /* g */
};

static const uint8_t segment_prefixes[] = {
 0x26, /* es */
 0x2e, /* cs */
 0x36, /* ss */
 0x3e, /* ds */
 0x64, /* fs */
 0x65  /* gs */
};

static const ASMInstr asm_instrs[] = {
#define ALT(x) x
/* This removes a 0x0f in the second byte */
#define O(o) ((uint64_t) ((((o) & 0xff00) == 0x0f00) ? ((((o) >> 8) & ~0xff) | ((o) & 0xff)) : (o)))
/* This constructs instr_type from opcode, type and group.  */
#define T(o,i,g) ((i) | ((g) << OPC_GROUP_SHIFT) | ((((o) & 0xff00) == 0x0f00) ? OPC_0F : 0))
#define DEF_ASM_OP0(name, opcode)
#define DEF_ASM_OP0L(name, opcode, group, instr_type) { TOK_ASM_ ## name, O(opcode), T(opcode, instr_type, group), 0, { 0 } },
#define DEF_ASM_OP1(name, opcode, group, instr_type, op0) { TOK_ASM_ ## name, O(opcode), T(opcode, instr_type, group), 1, { op0 }},
#define DEF_ASM_OP2(name, opcode, group, instr_type, op0, op1) { TOK_ASM_ ## name, O(opcode), T(opcode, instr_type, group), 2, { op0, op1 }},
#define DEF_ASM_OP3(name, opcode, group, instr_type, op0, op1, op2) { TOK_ASM_ ## name, O(opcode), T(opcode, instr_type, group), 3, { op0, op1, op2 }},

# include "i386-asm.h"

    /* last operation */
    { 0, },
};

static const uint16_t op0_codes[] = {
#define ALT(x)
#define DEF_ASM_OP0(x, opcode) opcode,
#define DEF_ASM_OP0L(name, opcode, group, instr_type)
#define DEF_ASM_OP1(name, opcode, group, instr_type, op0)
#define DEF_ASM_OP2(name, opcode, group, instr_type, op0, op1)
#define DEF_ASM_OP3(name, opcode, group, instr_type, op0, op1, op2)
# include "i386-asm.h"
};

static int asm_parse_reg(unsigned int *type)
{
    int reg = 0;
    *type = 0;
    next();
    reg = tok - TOK_ASM_eax;
    *type = OP_REG32;
    next();
    return reg;
}

static void parse_operand(TCCState *s1, Operand *op)
{
    ExprValue e;
    int reg, indir;
    const char *p;

    indir = 0;

    if (tok == '%') {
        next();
        reg = tok - TOK_ASM_al;
        op->type = 1 << (reg >> 3); /* WARNING: do not change constant order */
        op->reg = reg & 7;
        if ((op->type & OP_REG) && op->reg == TREG_XAX)
            op->type |= OP_EAX;
        next();
    no_skip: ;
    } else if (tok == '$') {
        /* constant value */
        next();
        asm_expr(s1, &e);
        op->type = OP_IM32;
        op->e = e;
        if (!op->e.sym) {
            if (op->e.v == (uint8_t)op->e.v)
                op->type |= OP_IM8;
            if (op->e.v == (int8_t)op->e.v)
                op->type |= OP_IM8S;
            if (op->e.v == (uint16_t)op->e.v)
                op->type |= OP_IM16;
        }
    } else {
        /* address(reg,reg2,shift) with all variants */
        op->type = OP_EA;
        op->reg = -1;
        op->reg2 = -1;
        op->shift = 0;
        if (tok != '(') {
            asm_expr(s1, &e);
            op->e = e;
        } else {
            next();
            if (tok == '%') {
                unget_tok('(');
                op->e.v = 0;
                op->e.sym = NULL;
            }
	    op->e.pcrel = 0;
        }
        if (tok == '(') {
	    unsigned int type = 0;
            next();
            if (tok != ',') {
                op->reg = asm_parse_reg(&type);
            }
	    if (type & OP_REG32)
	        op->type |= OP_EA32;
            skip(')');
        }
        if (op->reg == -1 && op->reg2 == -1)
            op->type |= OP_ADDR;
    }
    op->type |= indir;
}

/* XXX: unify with C code output ? */
ST_FUNC void gen_expr32(ExprValue *pe)
{
    gen_addr32(pe->sym ? VT_SYM : 0, pe->sym, pe->v);
}

/* XXX: unify with C code output ? */
static void gen_disp32(ExprValue *pe)
{
    Sym *sym = pe->sym;
    gen_addrpc32(VT_SYM, sym, pe->v);
}

/* generate the modrm operand */
static inline int asm_modrm(int reg, Operand *op)
{
    int mod, reg1, reg2, sib_reg1;

    if (op->type & (OP_REG | OP_MMX | OP_SSE)) {
        g(0xc0 + (reg << 3) + op->reg);
    } else {
        sib_reg1 = op->reg;
        /* fist compute displacement encoding */
        if (op->e.v == 0 && !op->e.sym && op->reg != 5) {
            mod = 0x00;
        } else if (op->e.v == (int8_t)op->e.v && !op->e.sym) {
            mod = 0x40;
        }
        /* compute if sib byte needed */
        reg1 = op->reg;
        g(mod + (reg << 3) + reg1);

        /* add offset */
        if (mod == 0x40) {
            g(op->e.v);
        }
    }
    return 0;
}

ST_FUNC void asm_opcode(TCCState *s1, int opcode)
{
    const ASMInstr *pa;
    int i, modrm_index, modreg_index, reg, v, op1, seg_prefix, pc;
    int nb_ops, s;
    Operand ops[MAX_OPERANDS], *pop;
    int op_type[3]; /* decoded op type */
    int alltypes;   /* OR of all operand types */
    int autosize;
    int p66;

    /* get operands */
    pop = ops;
    nb_ops = 0;
    seg_prefix = 0;
    alltypes = 0;
    for(;;) {
        if (tok == ';' || tok == TOK_LINEFEED)
            break;
        parse_operand(s1, pop);
        pop++;
        nb_ops++;
        if (tok != ',')
            break;
        next();
    }

    s = 0; /* avoid warning */

again:
    /* optimize matching by using a lookup table (no hashing is needed
       !) */
    for(pa = asm_instrs; pa->sym != 0; pa++) {
	int it = pa->instr_type & OPCT_MASK;
        s = 0;
        if (it == OPC_FARITH) {
            v = opcode - pa->sym;
            if (!((unsigned)v < 8 * 6 && (v % 6) == 0))
                continue;
        } else if (it == OPC_ARITH) {
            if (!(opcode >= pa->sym && opcode < pa->sym + 8*NBWLX))
                continue;
            s = (opcode - pa->sym) % NBWLX;
        } else if (it == OPC_SHIFT) {
            if (!(opcode >= pa->sym && opcode < pa->sym + 7*NBWLX))
                continue;
            s = (opcode - pa->sym) % NBWLX;
        } else if (it == OPC_TEST) {
	    /* cmovxx is a test opcode but accepts multiple sizes.
	       The suffixes aren't encoded in the table, instead we
	       simply force size autodetection always and deal with suffixed
	       variants below when we don't find e.g. "cmovzl".  */
	    if (pa->instr_type & OPC_WLX)
	        s = NBWLX - 1;
        } else if (pa->instr_type & OPC_B) {
            if (!(opcode >= pa->sym && opcode < pa->sym + NBWLX))
                continue;
            s = opcode - pa->sym;
        } else if (pa->instr_type & OPC_WLX) {
            if (!(opcode >= pa->sym && opcode < pa->sym + NBWLX-1))
                continue;
            s = opcode - pa->sym + 1;
        } else {
            if (pa->sym != opcode)
                continue;
        }
        if (pa->nb_ops != nb_ops)
            continue;

        /* now decode and check each operand */
	alltypes = 0;
        for(i = 0; i < nb_ops; i++) {
            int op1, op2;
            op1 = pa->op_type[i];
            op2 = op1 & 0x1f;
            switch(op2) {
            case OPT_IM:
                v = OP_IM8 | OP_IM16 | OP_IM32;
                break;
            case OPT_REG:
                v = OP_REG8 | OP_REG16 | OP_REG32 | OP_REG64;
                break;
            case OPT_REGW:
                v = OP_REG16 | OP_REG32 | OP_REG64;
                break;
	    case OPT_DISP:
	    case OPT_DISP8:
		v = OP_ADDR;
		break;
            default:
                v = 1 << op2;
                break;
            }
            if (op1 & OPT_EA)
                v |= OP_EA;
	    op_type[i] = v;
            if ((ops[i].type & v) == 0)
                goto next;
	    alltypes |= ops[i].type;
        }
        /* all is matching ! */
        break;
    next: ;
    }
    if (pa->sym == 0) {
        int b;
        b = op0_codes[opcode - TOK_ASM_first];
        g(b);
        return;
    }
    /* if the size is unknown, then evaluate it (OPC_B or OPC_WL case) */
    autosize = NBWLX-1;

    v = pa->opcode;
    if (pa->instr_type & OPC_0F)
        v = ((v & ~0xff) << 8) | 0x0f00 | (v & 0xff);
    if (v <= 0x05) {
        /* arith case */
        v += ((opcode - TOK_ASM_addb) / NBWLX) << 3;
    }

    /* search which operand will be used for modrm */
    modrm_index = -1;
    modreg_index = -1;
    if (pa->instr_type & OPC_MODRM) {
        /* first look for an ea operand */
        for(i = 0;i < nb_ops; i++) {
            if (op_type[i] & OP_EA)
                break;
        }
        modrm_index = i;
        /* if a register is used in another operand then it is
           used instead of group */
        for(i = 0;i < nb_ops; i++) {
            int t = op_type[i];
            if (i != modrm_index &&
                (t & (OP_REG | OP_MMX | OP_SSE | OP_CR | OP_TR | OP_DB | OP_SEG))) {
                modreg_index = i;
                break;
            }
        }
    }

    if (pa->instr_type & OPC_REG) {
        /* mov $im, %reg case */
        if (v == 0xb0 && s >= 1)
            v += 7;
        for(i = 0; i < nb_ops; i++) {
            if (op_type[i] & (OP_REG | OP_ST)) {
                v += ops[i].reg;
                break;
            }
        }
    }
    if (pa->instr_type & OPC_B)
        v += s >= 1;
    if (OPCT_IS(pa->instr_type, OPC_TEST))
        v += test_bits[opcode - pa->sym];
    op1 = v >> 16;
    op1 = (v >> 8) & 0xff;
    if (op1)
        g(op1);
    g(v);

    if (OPCT_IS(pa->instr_type, OPC_SHIFT)) {
        reg = (opcode - pa->sym) / NBWLX;
    } else if (OPCT_IS(pa->instr_type, OPC_ARITH)) {
        reg = (opcode - pa->sym) / NBWLX;
    } else {
        reg = (pa->instr_type >> OPC_GROUP_SHIFT) & 7;
    }

    pc = 0;
    if (pa->instr_type & OPC_MODRM) {
        /* if a register is used in another operand then it is
           used instead of group */
	if (modreg_index >= 0)
	    reg = ops[modreg_index].reg;
        pc = asm_modrm(reg, &ops[modrm_index]);
    }

    for(i = 0;i < nb_ops; i++) {
        v = op_type[i];
        if (v & (OP_IM8 | OP_IM16 | OP_IM32 | OP_IM64 | OP_IM8S | OP_ADDR)) {
            /* if multiple sizes are given it means we must look
               at the op size */
            if ((v | OP_IM8 | OP_IM64) == (OP_IM8 | OP_IM16 | OP_IM32 | OP_IM64)) {
                if (s == 2 || (v & OP_IM64) == 0)
                    v = OP_IM32;
                else
                    v = OP_IM64;
            }

            if (v & (OP_IM8 | OP_IM8S)) {
                g(ops[i].e.v);
	    } else if (pa->op_type[i] == OPT_DISP || pa->op_type[i] == OPT_DISP8) {
                gen_disp32(&ops[i].e);
            } else {
                gen_expr32(&ops[i].e);
            }
        }
    }
}

/* return the constraint priority (we allocate first the lowest
   numbered constraints) */
static inline int constraint_priority(const char *str)
{
    int priority, c, pr;

    /* we take the lowest priority */
    priority = 0;
    for(;;) {
        c = *str;
        if (c == '\0')
            break;
        str++;
        switch(c) {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'S':
        case 'D':
            pr = 1;
            break;
        case 'r':
	case 'R':
	case 'p':
            pr = 3;
            break;
        case 'N':
        case 'M':
        case 'I':
	case 'e':
        case 'i':
        case 'm':
        case 'g':
            pr = 4;
            break;
        }
        if (pr > priority)
            priority = pr;
    }
    return priority;
}

static const char *skip_constraint_modifiers(const char *p)
{
    while (*p == '=' || *p == '&' || *p == '+' || *p == '%')
        p++;
    return p;
}

#define REG_OUT_MASK 0x01
#define REG_IN_MASK  0x02

#define is_reg_allocated(reg) (regs_allocated[reg] & reg_mask)

ST_FUNC void asm_compute_constraints(ASMOperand *operands,
                                    int nb_operands, int nb_outputs,
                                    const uint8_t *clobber_regs,
                                    int *pout_reg)
{
    ASMOperand *op;
    int sorted_op[MAX_ASM_OPERANDS];
    int i, j, k, p1, p2, tmp, reg, c, reg_mask;
    const char *str;
    uint8_t regs_allocated[NB_ASM_REGS];

    /* init fields */
    for(i=0;i<nb_operands;i++) {
        op = &operands[i];
        op->input_index = -1;
        op->ref_index = -1;
        op->reg = -1;
        op->is_memory = 0;
        op->is_rw = 0;
    }
    /* compute constraint priority and evaluate references to output
       constraints if input constraints */
    for(i=0;i<nb_operands;i++) {
        op = &operands[i];
        str = op->constraint;
        str = skip_constraint_modifiers(str);
        if (isnum(*str) || *str == '[') {
            /* this is a reference to another constraint */
            k = find_constraint(operands, nb_operands, str, NULL);
            op->ref_index = k;
            operands[k].input_index = i;
            op->priority = 5;
        } else {
            op->priority = constraint_priority(str);
        }
    }

    /* sort operands according to their priority */
    for(i=0;i<nb_operands;i++)
        sorted_op[i] = i;
    for(i=0;i<nb_operands - 1;i++) {
        for(j=i+1;j<nb_operands;j++) {
            p1 = operands[sorted_op[i]].priority;
            p2 = operands[sorted_op[j]].priority;
            if (p2 < p1) {
                tmp = sorted_op[i];
                sorted_op[i] = sorted_op[j];
                sorted_op[j] = tmp;
            }
        }
    }

    for(i = 0;i < NB_ASM_REGS; i++) {
        if (clobber_regs[i])
            regs_allocated[i] = REG_IN_MASK | REG_OUT_MASK;
        else
            regs_allocated[i] = 0;
    }
    /* esp cannot be used */
    regs_allocated[4] = REG_IN_MASK | REG_OUT_MASK;
    /* ebp cannot be used yet */
    regs_allocated[5] = REG_IN_MASK | REG_OUT_MASK;

    /* allocate registers and generate corresponding asm moves */
    for(i=0;i<nb_operands;i++) {
        j = sorted_op[i];
        op = &operands[j];
        str = op->constraint;
        /* no need to allocate references */
        if (op->ref_index >= 0)
            continue;
        /* select if register is used for output, input or both */
        if (op->input_index >= 0) {
            reg_mask = REG_IN_MASK | REG_OUT_MASK;
        } else if (j < nb_outputs) {
            reg_mask = REG_OUT_MASK;
        } else {
            reg_mask = REG_IN_MASK;
        }
    try_next:
        c = *str++;
        switch(c) {
        case '=':
            goto try_next;
        case '&':
            reg_mask = REG_IN_MASK | REG_OUT_MASK;
            goto try_next;
        case 'a':
            reg = TREG_XAX;
            goto alloc_reg;
        case 'd':
            reg = TREG_XDX;
            goto alloc_reg;
        alloc_reg:
            goto reg_found;
        case 'r':
	case 'R':
	case 'p': /* A general address, for x86(64) any register is acceptable*/
            /* any general register */
            for(reg = 0; reg < 8; reg++) {
                if (!is_reg_allocated(reg))
                    goto reg_found;
            }
            goto try_next;
        reg_found:
            /* now we can reload in the register */
            op->is_llong = 0;
            op->reg = reg;
            regs_allocated[reg] |= reg_mask;
            break;
        default:
            break;
        }
        /* if a reference is present for that operand, we assign it too */
        if (op->input_index >= 0) {
            operands[op->input_index].reg = op->reg;
            operands[op->input_index].is_llong = op->is_llong;
        }
    }

}

ST_FUNC void subst_asm_operand(CString *add_str,
                              SValue *sv, int modifier)
{
    int r, reg, size, val;
    char buf[64];

    r = sv->r;
    if ((r & VT_VALMASK) == VT_LOCAL) {
        snprintf(buf, sizeof(buf), "%d(%%ebp)", (int)sv->c.i);
        cstr_cat(add_str, buf, -1);
    } else if (r & VT_LVAL) {
        reg = r & VT_VALMASK;
        snprintf(buf, sizeof(buf), "(%%%s)",
                 get_tok_str(TOK_ASM_eax + reg, NULL)
		 );
        cstr_cat(add_str, buf, -1);
    } else {
        /* register case */
        reg = r & VT_VALMASK;

        reg = TOK_ASM_eax + reg;
        snprintf(buf, sizeof(buf), "%%%s", get_tok_str(reg, NULL));
        cstr_cat(add_str, buf, -1);
    }
}

/* generate prolog and epilog code for asm statement */
ST_FUNC void asm_gen_code(ASMOperand *operands, int nb_operands,
                         int nb_outputs, int is_output,
                         uint8_t *clobber_regs,
                         int out_reg)
{
    uint8_t regs_allocated[NB_ASM_REGS];
    ASMOperand *op;
    int i, reg;

    /* Strictly speaking %Xbp and %Xsp should be included in the
       call-preserved registers, but currently it doesn't matter.  */
    static uint8_t reg_saved[] = { 3, 6, 7 };

    /* mark all used registers */
    memcpy(regs_allocated, clobber_regs, sizeof(regs_allocated));
    for(i = 0; i < nb_operands;i++) {
        op = &operands[i];
        if (op->reg >= 0)
            regs_allocated[op->reg] = 1;
    }
    if (!is_output) {
        /* generate reg save code */
        for(i = 0; i < sizeof(reg_saved)/sizeof(reg_saved[0]); i++) {
            reg = reg_saved[i];
            if (regs_allocated[reg]) {
                g(0x50 + reg);
            }
        }

        /* generate load code */
        for(i = 0; i < nb_operands; i++) {
            op = &operands[i];
            if (op->reg >= 0) {
                if (i >= nb_outputs || op->is_rw) {
                    /* load value in register */
                    load(op->reg, op->vt);
                }
            }
        }
    } else {
        /* generate save code */
        for(i = 0 ; i < nb_outputs; i++) {
            op = &operands[i];
            if (op->reg >= 0) {
                    store(op->reg, op->vt);
            }
        }
        /* generate reg restore code */
        for(i = sizeof(reg_saved)/sizeof(reg_saved[0]) - 1; i >= 0; i--) {
            reg = reg_saved[i];
            if (regs_allocated[reg]) {
                g(0x58 + reg);
            }
        }
    }
}

ST_FUNC void asm_clobber(uint8_t *clobber_regs, const char *str)
{
    int reg;
    TokenSym *ts;
    ts = tok_alloc(str, strlen(str));
    reg = ts->tok;
    if (reg >= TOK_ASM_eax && reg <= TOK_ASM_edi) {
        reg -= TOK_ASM_eax;
    }
    clobber_regs[reg] = 1;
}
