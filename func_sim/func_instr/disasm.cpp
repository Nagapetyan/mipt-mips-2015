#include <func_instr.h>
#include <elf_parser.h>
#include <cassert>
#include <cstring>
#include <vector>
#include <iostream>


int main(int argc, char** argv)
{
	if(argc != 3)
		assert(0);
	
	std::ostringstream oss;

	std::vector<ElfSection> sections_array;
	ElfSection::getAllElfSections( argv[1], sections_array);

	bool check = false;

	for(std::vector<ElfSection>::iterator it = sections_array.begin() ; it != sections_array.end(); ++it)
	{
		if(strcmp (it->name, argv[2]) == 0)
		{
			ElfSection  instr= *it;
			for(unsigned i = 0; (instr.size) / 4; ++i)
				{
					FuncInstr func_instr(instr.content[i]);
					std::cout << func_instr.Dump("") << std::endl;
				}
			
			check = true;
			break;
		}
	}

	if(check == false)
		assert(0);

	return 0;
}