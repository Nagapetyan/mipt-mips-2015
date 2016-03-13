#include <iostream>
#include <perf_sim.h>
#include <string>
#include <stdlib.h>

int main(int argc, char **argv)
{
	switch(argc)
	{
		case 3:
			{
				PerfMIPS *mips = new PerfMIPS();
				mips->run( std::string( argv[1]), atoi( argv[2]), true);
				delete mips;
				break;
			} 
		case 4:
			{
				PerfMIPS *mips = new PerfMIPS();
				if (std::string( argv[3]) != "-d")
					break;
				mips->run(std::string( argv[1]), atoi( argv[2]), false);
				delete mips;
				break;
			}
		default: break;
	}
} 