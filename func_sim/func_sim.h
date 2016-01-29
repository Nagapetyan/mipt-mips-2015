#ifndef FUCK_SIM_H
#define FUNC_SIM_H

#include <func_instr.h>
#include <func_memory.h>
#include <vector>
#include <types.h>


enum RegNum
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
    FP,
    RA,
    MAX_REG = 32
};


class RF
{
public:
	RF();
//	uint32 read( RegNum index) const { return vec[static_cast < int > (index)]; };
//    void write( RegNum index, uint32 data) { vec[static_cast< int >(index)] = ((static_cast< int>(index)) != 0) ? data : 0; };
//    void reset( RegNum index) { vec[index] = 0; };
	uint32 read(RegNum index) const { return array[index]; }
	void write (RegNum index, uint32 data) { array[index] = (index != 0) ? data : 0; }
	void reset (RegNum index) { array[index] = 0; }
	~RF();

private:
//	std::vector<uint32> vec(static_cast< int >(MAX_REG));
	uint32 array[MAX_REG];
};

class MIPS
{
public:
	MIPS();
	void run( const string&, unsigned int instr_to_run);
private:
	~MIPS();
	RF *rf;
	uint32 PC;
	uint32 hi;
	uint32 lo;
	FuncMemory *mem;

	uint32 fetch() { return mem->read(PC);}

	void read_src( FuncInstr& instr);
	void wb( const FuncInstr& instr);

	void load( FuncInstr& instr);
	void store( const FuncInstr& instr);
	void ld_st( FuncInstr& instr);

	void updatePC( const FuncInstr& instr) { PC = instr.new_PC; }
};



#endif 