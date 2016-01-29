#include <func_sim.h>
#include <func_memory.h>


void MIPS::run(const string& tr, unsigned int instr_to_run)
{
	mem = new FuncMemory (tr.c_str());
	PC = mem->startPC();

	for (int i = 0; i < instr_to_run; ++i) {
         uint32 instr_bytes;
         // Fetch
         instr_bytes = fetch();
         // Decode and read sources
         FuncInstr instr(instr_bytes, PC);
         read_src(instr);
         // Execute
         instr.execute();
         // Memory access
         ld_st(instr);
         // Writeback
         wb(instr);
         // Update PC
         updatePC( instr);
         // Dump
         std::cout << instr.MIPS_Dump() <<  std::endl;
    }
}

MIPS::MIPS() : hi(0), lo(0)
{
	rf = new RF;
}

MIPS::~MIPS()
{
	delete rf;
}

RF::RF()
{
//	vec.clear();
	for (int i = 0; i < MAX_REG; i++ )
		reset(static_cast< RegNum>(i));
}

RF::~RF()
{
//	vec.clear();	
}

void MIPS::read_src( FuncInstr& instr)
{
	instr.v_src1 = rf->read( static_cast<RegNum>(instr.get_src1_num_index()));
    instr.v_src2 = rf->read( static_cast<RegNum>(instr.get_src2_num_index()));
    instr.hi = hi;
    instr.lo = lo;
}

void MIPS::wb( const FuncInstr& instr)
{
	rf->write( static_cast< RegNum >(instr.get_dst_num_index()), instr.v_dst);
	lo = instr.lo;
	hi = instr.hi;
}

void MIPS::load( FuncInstr& instr)
{
    instr.v_dst = mem->read( instr.mem_addr, instr.byte_num);
}

void MIPS::store( const FuncInstr& instr)
{
	mem->write(instr.mem_addr, instr.v_dst, instr.byte_num);
}

void MIPS::ld_st( FuncInstr& instr)
{
	if(instr.load())
        load(instr);
    else if(instr.store())
        store(instr);
}