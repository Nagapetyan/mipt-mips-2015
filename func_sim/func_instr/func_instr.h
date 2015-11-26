#ifndef FUNC_INSTR_H
#define FUNC_INSTR_H

#include <types.h>
#include <string>
#include <sstream>


enum Type
	{
		ADD = 0,
		MUL,
		SHIFT,
		COMPRASSION,
		LOGIC,
		CONDITION,
		UNC_JUMP,
		LOAD,
		STORE,
		SYSCALL,
		BREAK,
		TRAP
	};

enum Format
	{
		FORMAT_R,
		FORMAT_I,
		FORMAT_J,
		FORMAT_INV
	};

enum Register
	{
		ZERO = 0,
		AT,
		V0, V1,
		A0, A1, A2, A3, 
		T0, T1, T2, T3, T4, T5, T6, T7,
		S0, S1, S2, S3, S4, S5, S6, S7,
		T8, T9,
		K0, K1, 
		GP, 
		SP,
		S8,
		RA
	};

struct ISAEntry
    {
        const char* name;

        uint8 opcode;
        uint8 func;

        Format format;
        Type type;

        bool rs;
        bool rt;
        bool rd;
        bool shamt;
    };

    struct Reg
    {
    	const char* name;
    	Register reg;
    };

class FuncInstr
{
public:
	FuncInstr(uint32 byte);
	virtual ~FuncInstr(){};
	std::string Dump( std::string indent) const;

private:
	Format format;
	Type type;

	std::string str;

	union 
	{
		struct
		{
			unsigned imm:16;
            unsigned rt:5;
            unsigned rs:5;
            unsigned opcode:6;
		} asI;
		struct 
		{
			unsigned func:6;
			unsigned shamt:5;
			unsigned rd:5;
			unsigned rt:5;
			unsigned rs:5;
			unsigned opcode:6;
		} asR;
		struct 
		{
			unsigned offset:26;
			unsigned opcode:6;
		} asJ;
		uint32 raw;
	} bytes;

    static const ISAEntry isaTable[];
    static const Reg regTable[];

    void initFormat(uint32 byte);

    void parserR(uint32 byte);
    void parserI(uint32 byte);
    void parserJ(uint32 byte);

    std::string DST_SequenceR(uint32 i);
    std::string DTS_SequenceR(uint32 i);
    std::string ST_SequenceI(uint32 i);
    std::string TS_SequenceI(uint32 i);
    std::string Load_Store(uint32 i);
};

std::ostream& operator<<( std::ostream& out, const FuncInstr& instr);

#endif 