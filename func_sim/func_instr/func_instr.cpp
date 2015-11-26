#include <func_instr.h>
#include <ostream>
#include <cassert>
#include <iostream>

const Reg FuncInstr::regTable[] =
{
	{"$zero", ZERO},
	{"$at", AT},
	{"$v0", V0},
	{"$v1", V1},
	{"$a0", A0},
	{"$a1", A1},
	{"$a2", A2},
	{"$a3", A3},
	{"$t0", T0},
	{"$t1", T1},
	{"$t2", T2},
	{"$t3", T3},
	{"$t4", T4},
	{"$t5", T5},
	{"$t6", T6},
	{"$t7", T7},
	{"$s0", S0},
	{"$s1", S1},
	{"$s2", S2},
	{"$s3", S3},
	{"$s4", S4},
	{"$s5", S5},
	{"$s6", S6},
	{"$s7", S7},
	{"$t8", T8},
	{"$t9", T9},
	{"$k0", K0},
	{"$k1", K1},
	{"$gp", GP},
	{"$sp", SP},
	{"$s8", S8},
	{"$ra", RA},
};

const ISAEntry FuncInstr::isaTable[] =
{
//	name		 opcode			func		format			type			rs		rt 		rd 		shamt
	{"add ",	    0x0,  		0x20,		FORMAT_R,		ADD,			1,		1,		1,		0},
	{"addu ",	    0x0,	    0x21,		FORMAT_R,		ADD,			1,		1,		1,		0},
	{"sub ",	    0x0,		0x22,		FORMAT_R,		ADD,			1,		1,		1,		0},
	{"subu ",       0x0,		0x23,		FORMAT_R,		ADD,			1,		1,		1,		0},
	{"addi ",	    0x8,		0x0,		FORMAT_I, 		ADD,			1,		1,		0,		0},
	{"addiu ",		0x9,		0x0,		FORMAT_I,		ADD,			1,		1,		0,		0},

	{"mult ",		0x0,		0x18,		FORMAT_R,		MUL,			1,		1,		0,		0},
	{"multu ",		0x0,		0x19,		FORMAT_R,		MUL,			1,		1,		0,		0},
	{"div ",		0x0,		0X1A,		FORMAT_R,		MUL,			1,		1,		0,		0},
	{"divu ",		0x0,		0x1B,		FORMAT_R,		MUL,			1,		1,		0,		0},
	{"mfhi ",		0x0,		0x10,		FORMAT_R,		MUL,			0,		0,		1,		0},
	{"mthi ",		0x0,		0x11,		FORMAT_R,		MUL,			1,		0,		0,		0},
	{"mflo ",		0x0,		0x12,		FORMAT_R,		MUL,			0,		0,		1,		0},
	{"mtlo ",		0x0,		0x13,		FORMAT_R,		MUL,			1,		0,		0,		0},

	{"sll ",		0x0,		0x0,		FORMAT_R,		SHIFT,			1,		1,		1,		1},
	{"srl ",		0x0,		0x2,		FORMAT_R,		SHIFT,			1,		1,		1,		1},
	{"sra ",		0x0,		0x3,		FORMAT_R,		SHIFT,			1,		1,		1,		1},
	{"sllv ",		0x0,		0x4,		FORMAT_R,		SHIFT,			1,		1,		1,		0},
	{"srlv ",		0x0,		0x6,		FORMAT_R,		SHIFT,			1,		1,		1,		0},
	{"srav ",		0x0,		0x7,		FORMAT_R,		SHIFT,			1,		1,		1,		0},
	{"lui ",		0xF,		0x0,		FORMAT_I,		SHIFT,			1,		1,		0,		0},

	{"slt ",		0x0,		0x2A,		FORMAT_R,		COMPRASSION,	1,		1,		1,		0},
	{"sltu ",		0x0,		0x2B,		FORMAT_R,		COMPRASSION,	1,		1,		1,		0},
	{"slti ",		0xA,		0x0,		FORMAT_I,		COMPRASSION,	1,		1,		0,		0},
	{"sltiu ",		0xB,		0x0,		FORMAT_I,		COMPRASSION,	1,		1,		0,		0},

	{"and ",		0x0,		0x24,		FORMAT_R,		LOGIC,			1,		1,		1,		0},
	{"or ",			0x0,		0x25,		FORMAT_R,		LOGIC,			1,		1,		1,		0},
	{"xor ",		0x0,		0x26,		FORMAT_R,		LOGIC,			1,		1,		1,		0},
	{"nor ",		0x0,		0x27,		FORMAT_R,		LOGIC,			1,		1,		1,		0},
	{"andi ",		0xC,		0x0,		FORMAT_I,		LOGIC,			1,		1,		0,		0},
	{"ori ",		0xD,		0x0,		FORMAT_I,		LOGIC,			1,		1,		0,		0},
	{"xori ",		0xE,		0x0,		FORMAT_I,		LOGIC,			1,		1,		0,		0},

	{"beq ",		0x4,		0x0,		FORMAT_I,		CONDITION,		1,		1,		0,		0},
	{"bne ",		0x5,		0x0,		FORMAT_I,		CONDITION,		1,		1,		0,		0},
	{"blez ",		0x6,		0x0,		FORMAT_I,		CONDITION,		1,		0,		0,		0},
	{"bgtz ",		0x7,		0x0,		FORMAT_I,		CONDITION,		1,		0,		0,		0},

	{"j ",			0x2,		0x0,		FORMAT_J,		UNC_JUMP,		0,		0,		0,		0},
	{"jal ",		0x3,		0x0,		FORMAT_J,		UNC_JUMP,		0,		0,		0,		0},
	{"jr ",			0x0,		0x8,		FORMAT_R,		UNC_JUMP,		1,		0,		0,		0},
	{"jalr ",		0x0,		0x9,		FORMAT_R,		UNC_JUMP,		1,		0,		0,		0},

	{"lb ",			0x20,		0x0,		FORMAT_I,		LOAD,			1,		1,		0,		0},
	{"lh ",			0x21,		0x0,		FORMAT_I,		LOAD,			1,		1,		0,		0},
	{"lw ",			0x23,		0x0,		FORMAT_I,		LOAD,			1,		1,		0,		0},
	{"lbu ",		0x24,		0x0,		FORMAT_I,		LOAD,			1,		1,		0,		0},
	{"lhu ",		0x25,		0x0,		FORMAT_I,		LOAD,			1,		1,		0,		0},

	{"sb ",			0x28,		0x0,		FORMAT_I,		STORE,			1,		1,		0,		0},
	{"sh ",			0x29,		0x0,		FORMAT_I,		STORE,			1,		1,		0,		0},
	{"sw ",			0x2b,		0x0,		FORMAT_I,		STORE,			1,		1,		0,		0},

	{"syscall ",	0x0,		0xC,		FORMAT_R,		SYSCALL,		0,		0,		0,		0},
	{"break ",		0x0,		0xD,		FORMAT_R,		BREAK,			0,		0,		0,		0},
	{"trap ",		0x1A,		0x0,		FORMAT_J,		TRAP,			0,		0,		0,		0},
};

FuncInstr::FuncInstr(uint32 byte):
	format(FORMAT_INV)
//	bytes(byte)
{
	bytes.raw = byte;
	initFormat(byte);
    switch (format)
    {
        case FORMAT_R:
            parserR(byte);
            break;
        case FORMAT_I:
            parserI(byte);
            break;
        case FORMAT_J:
            parserJ(byte);
            break;
        default:
        	std::cerr << "ERROR: ivalid format" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }
}

void FuncInstr::initFormat(uint32 byte)
{
	uint32 op = byte >> 26;
	if(op == 0)
		format = FORMAT_R;
	else
	{
		for(uint32 i = 0; i < 51; i++)
		{
			if(isaTable[i].opcode == op)
			{
				format = isaTable[i].format;
				break;
			}
		}
	}
}


void FuncInstr::parserR(uint32 byte)
{
	std::ostringstream oss;
//	uint32 _func = 0;
	for(uint32 i = 0; i < 51; i++)
	{
		uint32 _func = byte - ((byte >> 6) << 6);
		if(_func == isaTable[i].func)
		{
			oss << isaTable[i].name;
			switch(isaTable[i].type)
			{
				case ADD : 
					oss << DST_SequenceR(i);
					break;
				case MUL :
					oss << DST_SequenceR(i);
					break;
				case SYSCALL:
					break;
				case BREAK:
					break;
				default  :
					oss << DTS_SequenceR(i);
					break;
			}
			break;
		}
	}
	str = oss.str();
}

std::string FuncInstr::DST_SequenceR(uint32 i)
{
	std::ostringstream oss;
	if(isaTable[i].rd != 0)
		oss << regTable[bytes.asR.rd].name << ", ";
	if(isaTable[i].rs != 0)
		oss << regTable[bytes.asR.rs].name << ", ";
	if(isaTable[i].rt != 0)
		oss << regTable[bytes.asR.rt].name << ", ";
	std::string s = oss.str();
	s.pop_back();
	s.pop_back();
	return s;
}

std::string FuncInstr::DTS_SequenceR(uint32 i)
{
	std::ostringstream oss;

	if(isaTable[i].rd != 0)
		oss << regTable[bytes.asR.rd].name << ", ";
	if(isaTable[i].rt != 0)
		oss << regTable[bytes.asR.rt].name << ", ";
	if(isaTable[i].rs != 0)
		oss << regTable[bytes.asR.rs].name << ", ";
	if(isaTable[i].shamt != 0)
		oss << std::hex << bytes.asR.shamt << ", ";

	std::string s = oss.str();
	s.pop_back();
	s.pop_back();
	return s;
}

void FuncInstr::parserI(uint32 byte)
{
	std::ostringstream oss;

	uint32 op = byte >> 26;

	for(uint32 i = 0; i < 51; i++)
	{		
		if(op == isaTable[i].opcode) 
		{
			oss << isaTable[i].name;
			switch(isaTable[i].type)
			{
				case ADD :
					oss << TS_SequenceI(i);
					break;
				case LOAD:
					oss << Load_Store(i);
					break;
				case STORE:
					oss << Load_Store(i);
					break;
				default:
					oss << ST_SequenceI(i);
					break;
			}
		}
	}
	str = oss.str();
}


std::string FuncInstr::TS_SequenceI(uint32 i)
{
	std::ostringstream oss;

	oss << regTable[bytes.asI.rt].name << ", " << regTable[bytes.asI.rs].name << ", "
		<< "0x" << std::hex << bytes.asI.imm;
	return oss.str(); 
}

std::string FuncInstr::Load_Store(uint32 i)
{
	std::ostringstream oss;

	oss << regTable[bytes.asI.rt].name << ", " 
		<< "0x" << std::hex << bytes.asI.imm << "(" 
		<< regTable[bytes.asI.rs].name << ")";
	return oss.str(); 
}


std::string FuncInstr::ST_SequenceI(uint32 i)
{
	std::ostringstream oss;

	if(isaTable[i].rs != 0)
		oss << regTable[bytes.asI.rs].name << ", ";
	if(isaTable[i].rt != 0)
		oss << regTable[bytes.asI.rt].name << ", ";
	oss << "0x" << std::hex << bytes.asI.imm;

	return oss.str();
}


void FuncInstr::parserJ(uint32 byte)
{
	std::ostringstream oss;
	if(bytes.asJ.opcode == 0x2)
		oss << "j " <<  "0x" << std::hex << bytes.asJ.offset;
	if(bytes.asJ.opcode == 0x3)
		oss << "jal " <<  "0x" << std::hex << bytes.asJ.offset;
	if(bytes.asJ.opcode == 0x1A)
		oss << "trap " << "0x" << std::hex << bytes.asJ.offset;
	str = oss.str();
}


std::string FuncInstr::Dump(std::string indent) const
{
	std::ostringstream oss;
	oss << indent << str;
	return oss.str();
}

std::ostream& operator<<( std::ostream& out, const FuncInstr& instr)
{
	out << instr.Dump(" ");
	return out;
}