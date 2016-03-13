#include <perf_sim.h>
#include <sstream>


#define port_latency  1
#define port_fanout  1
#define port_bw  1

PerfMIPS::PerfMIPS(): executed_instruction(0), cycle(0), PC_valid(true), PC(0)
{
	rf = new RF();
	pfetch = new Fetch(this);
	pdecode = new Decode(this);
	pmemory = new Memory(this);
	pexecute = new Execute(this);
	pwriteback = new Writeback(this);
	
	Port<uint32>::init();
	Port<FuncInstr>::init();
	Port<bool>::init();
}


PerfMIPS::~PerfMIPS()
{
	delete rf;
	delete pfetch;
	delete pdecode;
	delete pexecute;
	delete pmemory;
	delete pwriteback;
}

void PerfMIPS::read_src(FuncInstr &instr)
{
	rf->read_src1(instr);
	rf->read_src2(instr);
}

void PerfMIPS::wb(FuncInstr &instr)
{
	rf->write_dst(instr);
}

void PerfMIPS::load(FuncInstr &instr)
{
	instr.set_v_dst( mem->read(instr.get_mem_addr(), instr.get_mem_size()));
}

void PerfMIPS::store(const FuncInstr &instr)
{
	mem->write(instr.get_mem_addr(), instr.get_v_dst(), instr.get_mem_size());
}

void PerfMIPS::ld_st(FuncInstr &instr)
{
	if(instr.is_load())
		load(instr);
	else if (instr.is_store())
		store(instr);
} 

void PerfMIPS::run( const std::string& tr, int instr_to_run, bool silent)
{

	mem = new FuncMemory( tr.c_str());
    PC = mem->startPC();

	executed_instruction = 0;
	cycle = 0;
	while(executed_instruction <= instr_to_run)
	{

		pdecode->clock();
		pfetch->clock();
		pexecute->clock();
		pmemory->clock();
		pwriteback->clock(silent);

		if(silent == false)
		{
			pfetch->_dump();
			pdecode->_dump();
			pexecute->_dump();
			pmemory->_dump();
			pwriteback->_dump();
		}
		else  pwriteback->_dump();
		++cycle;
	}

	delete mem;
}

Fetch::Fetch(PerfMIPS* ppln):
ppln(ppln), byte_instr(0), cycle(0)
{
	wp_fetch_2_decode = new WritePort<uint32>("FETCH_2_DECODE", port_bw, port_fanout);
	rp_decode_2_fetch_stall = new ReadPort<bool>("DECODE_2_FETHC_STALL", port_latency);
}

Fetch::~Fetch()
{
	delete wp_fetch_2_decode;
	delete rp_decode_2_fetch_stall;
}

void Fetch::clock()
{
	std::ostringstream oss;

	bool is_stall = false;
	cycle = ppln->get_cycle();

	oss << "fetch\tcycle " << cycle <<":\t";

	rp_decode_2_fetch_stall->read(&is_stall, cycle);
	if( is_stall )
	{
		oss << "bubble\n";
		dump = oss.str();
		return;
	}

	if(ppln->get_PC_valid() == true)
	{
//		PC = ppln.get_PC();
		byte_instr = ppln->fetch();
		wp_fetch_2_decode->write(byte_instr, cycle);
		oss << std::hex << "0x" << byte_instr << "\n";
	}
	else oss << "bubble\n";

	dump = oss.str();
}

Decode::Decode(PerfMIPS *ppln):
ppln(ppln), byte_instr(0), cycle(0), was_decode(true)
{
	rp_fetch_2_decode = new ReadPort<uint32>("FETCH_2_DECODE", port_latency);
	rp_execute_2_decode_stall = new ReadPort<bool>("EXECUTE_2_DECODE_STALL", port_latency);
	wp_decode_2_execute = new WritePort<FuncInstr>("DECODE_2_EXECUTE", port_bw, port_fanout);
	wp_decode_2_fetch_stall = new WritePort<bool>("DECODE_2_FETCH_STALL", port_bw, port_fanout);
}

Decode::~Decode()
{
	delete rp_fetch_2_decode;
	delete rp_execute_2_decode_stall;
	delete wp_decode_2_execute;
	delete wp_decode_2_fetch_stall;
}

void Decode::clock()
{	
	std::ostringstream oss;

	bool is_stall = false;
	cycle = ppln->get_cycle();

	oss << "decode\tcycle " << cycle << ":\t";

	rp_execute_2_decode_stall->read(&is_stall, cycle);
	if(is_stall)
	{
		wp_decode_2_fetch_stall->write(true, cycle);
		oss << "bubble\n";
		dump = oss.str();
		return;
	}

	uint32 PC = ppln->get_PC();

	bool is_read; 
	is_read = rp_fetch_2_decode->read(&byte_instr, cycle);
	
	if ((was_decode == true) && (is_read == false)) 
	{
		oss << "bubble\n";
		dump = oss.str();
		return;
	}
	FuncInstr instr(byte_instr, PC);

	if(instr.is_jump())
	{
		ppln->PC_invalidate();
	}


	is_stall = is_need_stall(instr);

	if(is_stall == false)
	{
		wp_decode_2_execute->write(instr, cycle);
		was_decode = true;
		if(!instr.is_jump())
			ppln->set_next_PC();
		oss << instr << "\n";
	}
	else
	{
		wp_decode_2_fetch_stall->write(true, cycle);
		was_decode = false;
		oss << "bubble\n";
	}

	dump = oss.str();
}

bool Decode::is_need_stall(FuncInstr &instr)
{
	if((ppln->check(instr.get_src1_num()) == false) || (ppln->check(instr.get_src2_num()) == false))
		return true;
	else 
	{
		ppln->reg_invalidate(instr.get_dst_num());
		return false;
	}
}

Execute::Execute(PerfMIPS* ppln):
ppln(ppln), cycle(0)
{
	rp_decode_2_execute = new ReadPort<FuncInstr>("DECODE_2_EXECUTE", port_latency);
	wp_execute_2_decode_stall = new WritePort<bool>("EXECUTE_2_DECODE_STALL", port_bw, port_fanout);
	wp_execute_2_memory = new WritePort<FuncInstr>("EXECUTE_2_MEMORY", port_bw, port_fanout);
	rp_memory_2_execute_stall = new ReadPort<bool>("MEMORY_2_EXECUTE_STALL", port_latency);
}

Execute::~Execute()
{
	delete rp_decode_2_execute;
	delete rp_memory_2_execute_stall;
	delete wp_execute_2_memory;
	delete wp_execute_2_decode_stall;
}

void Execute::clock()
{
	std::ostringstream oss;

	bool is_stall = false;
	cycle = ppln->get_cycle();
	
	oss << "execute\tcycle " << cycle << ":\t";

	rp_memory_2_execute_stall->read(&is_stall, cycle);
	if(is_stall)
	{
		wp_execute_2_decode_stall->write(true, cycle);
		oss << "bubble\n";
		dump = oss.str();
		return;
	}

	bool is_read;
	FuncInstr instr(0);
	is_read = rp_decode_2_execute->read(&instr, cycle);

 	if(is_read == false)
 	{
 		oss << "bubble\n";
 		dump = oss.str();
 		return;
 	}

 	instr.execute();
 	wp_execute_2_memory->write(instr, cycle);
 	oss << instr << "\n";
 	dump = oss.str();
}


Memory::Memory(PerfMIPS* ppln):
ppln(ppln), cycle(0)
{
	rp_writeback_2_memory_stall = new ReadPort<bool>("WRITEBACK_2_MEMORY_STALL", port_latency);
	wp_memory_2_execute_stall = new WritePort<bool>("MEMORY_2_EXECUTE_STALL", port_bw, port_fanout);
	rp_execute_2_memory = new ReadPort<FuncInstr>("EXECUTE_2_MEMORY", port_latency);
	wp_memory_2_writeback = new WritePort<FuncInstr>("MEMORY_2_WRITEBACK", port_bw, port_fanout);
}

Memory::~Memory()
{
	delete rp_writeback_2_memory_stall;
	delete wp_memory_2_execute_stall;
	delete rp_execute_2_memory;
	delete wp_memory_2_writeback;
}


void Memory::clock()
{
	std::ostringstream oss;

	bool is_stall = false;
	cycle = ppln->get_cycle();

	oss << "memory\tcycle " << cycle << ":\t";
	rp_writeback_2_memory_stall->read(&is_stall, cycle); 

	if(is_stall == true)
	{
		wp_memory_2_execute_stall->write(true, cycle);
		oss << "bubble\n";
		dump = oss.str();
		return;
	}

	FuncInstr instr(0);
	bool is_read;
	is_read = rp_execute_2_memory->read(&instr, cycle);

	if(is_read == false)
	{
		oss << "bubble\n";
		dump = oss.str();
		return;
	}
	else
	{
		ppln->ld_st(instr);
		wp_memory_2_writeback->write(instr, cycle);
		oss << instr << "\n";
		dump = oss.str();
	}
}

Writeback::Writeback(PerfMIPS* ppln):
ppln(ppln), cycle(0)
{
	wp_writeback_2_memory_stall = new WritePort<bool>("WRITEBACK_2_MEMORY_STALL", port_bw, port_fanout);
	rp_memory_2_writeback = new ReadPort<FuncInstr>("MEMORY_2_WRITEBACK", port_latency);
}

Writeback::~Writeback()
{
	delete wp_writeback_2_memory_stall;
	delete rp_memory_2_writeback;
}

void Writeback::clock(bool silent)
{
	std::ostringstream oss;

	cycle = ppln->get_cycle();
	FuncInstr instr(0);

	if(silent == false )
		oss << "writeback\tcycle " << cycle << ":\t";

	if(rp_memory_2_writeback->read(&instr, cycle))
	{
		if(silent == false )
			oss << "bubble\n";
		dump = oss.str();
		return;
	}

	if(instr.is_jump())
	{
		ppln->PC_validate();
		ppln->updatePC(instr);
	}

	ppln->wb(instr);
	ppln->update_exec_instr();

	oss << instr << "\n";
	dump = oss.str();
}