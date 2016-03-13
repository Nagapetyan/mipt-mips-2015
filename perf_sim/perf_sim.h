#ifndef PERF_SIM_H
#define PERF_SIM_H

#include <iostream>
#include <func_memory.h>
#include <func_memory.h>
#include <rf.h>
#include <ports.h>


class PerfMIPS;
class Fetch;
class Decode; 
class Execute;
class Memory;
class Writeback;


class PerfMIPS
{
	public:
		PerfMIPS();
		void run( const string&, int, bool);
		~PerfMIPS();

		uint32 get_cycle() const { return cycle; }

		uint32 get_PC() const { return PC; }		
		void PC_validate() { PC_valid = true; }
		void PC_invalidate() { PC_valid = false; }
		bool get_PC_valid() { return PC_valid; }
		void set_next_PC() { PC += 4; }

		void update_exec_instr() { executed_instruction++; }

		void reg_invalidate(RegNum num) { rf->invalidate(num); }
		bool check(RegNum num) { rf->check(num); }

		uint32 fetch() { return mem->read(PC);}

		void read_src( FuncInstr& instr);
		void wb( FuncInstr& instr);

		void load( FuncInstr& instr);
		void store( const FuncInstr& instr);
		void ld_st( FuncInstr& instr);

		void updatePC( const FuncInstr& instr) { PC = instr.get_new_PC(); }

	private:
		RF *rf;
		FuncMemory *mem;
		uint32 executed_instruction;
		uint32 cycle;
		
		uint32 PC;
		bool PC_valid;

		Fetch *pfetch;
		Decode *pdecode;
		Execute *pexecute;
		Memory *pmemory;
		Writeback *pwriteback;
};



class Fetch
{
		public:
			Fetch(PerfMIPS* ppln);
			~Fetch();
			void clock();
			void _dump(){ std::cout << dump; }
		private:
			ReadPort<bool>* rp_decode_2_fetch_stall;
			WritePort<uint32>* wp_fetch_2_decode;
			
			uint32 byte_instr;
			PerfMIPS *ppln;
			uint32 cycle;

			std::string dump;
};


class Decode
{
	public:
		Decode(PerfMIPS* ppln);
		~Decode();
		void clock();
		void _dump(){ std::cout << dump; }
	private:
		ReadPort<bool>* rp_execute_2_decode_stall;
		WritePort<bool>* wp_decode_2_fetch_stall;
		ReadPort<uint32>* rp_fetch_2_decode;
		WritePort<FuncInstr>* wp_decode_2_execute;
		
		uint32 byte_instr;
		PerfMIPS* ppln;
		uint32 cycle;
		bool was_decode;

		std::string dump;

		bool is_need_stall(FuncInstr&);
};

class Execute
{
	public:
		Execute(PerfMIPS* ppln);
		~Execute();
		void clock();
		void _dump(){ std::cout << dump; }
	private:
		ReadPort<bool>* rp_memory_2_execute_stall;
		WritePort<bool>* wp_execute_2_decode_stall;
		ReadPort<FuncInstr>* rp_decode_2_execute;
		WritePort<FuncInstr>* wp_execute_2_memory;

		PerfMIPS* ppln;
		uint32 cycle;

		std::string dump;
};

class Memory
{
	public:
		Memory(PerfMIPS* ppln);
		~Memory();
		void clock();
		void _dump(){ std::cout << dump; }
	private:
		ReadPort<bool>* rp_writeback_2_memory_stall;
		WritePort<bool>* wp_memory_2_execute_stall;
		ReadPort<FuncInstr>* rp_execute_2_memory;
		WritePort<FuncInstr>* wp_memory_2_writeback;

		PerfMIPS* ppln;
		uint32 cycle;

		std::string dump;
};

class Writeback
{
	public:
		Writeback(PerfMIPS* ppln);
		~Writeback();
		void clock(bool silent);
		void _dump(){ std::cout << dump; }
	private:
		WritePort<bool>* wp_writeback_2_memory_stall;
		ReadPort<FuncInstr>* rp_memory_2_writeback;

		PerfMIPS* ppln;
		uint32 cycle;

		std::string dump;
};

#endif  //PERF_SIM_H