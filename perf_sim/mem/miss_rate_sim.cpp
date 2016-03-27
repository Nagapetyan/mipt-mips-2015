#include <fstream>
#include <iostream>
#include <cache_tag_array.h>
#include <cassert>

#define BLOCK_SIZE_IN_BYTES 4
#define ADDR_SIZE_IN_BYTES  32

int main(int argc, char** argv)
{
	assert(argc == 3);

	std::fstream write;
	write.open(argv[2], std::fstream::out);


	write << "Associative/Size, ";
	for (unsigned int size = 1; size <= 1024; size <<= 1 )
	{
		write << size << "KB" ;
		if(size != 1024)
			write << ", ";
	}


	write << std::endl;

	write << "Full ";

	for(unsigned long size = 1024; size <= 1024*1024; size <<= 1 )
	{
		CacheTagArray cache_tag_array( size, 0, BLOCK_SIZE_IN_BYTES, ADDR_SIZE_IN_BYTES);
		unsigned  miss = 0;
		unsigned  total = 0;

		std::fstream read;
		read.open(argv[1], std::fstream::in);


		while(!read.eof())
		{
			uint64 addr;
			read >> std::hex >> addr;
			if(cache_tag_array.read(addr) == false)
				miss++;
			total++;
		}
		write << (1. * miss / total)ж
		if(size != 1024*1024)
			write << ", ";
		read.close();
		std::cout<< "close size = " << size << std::endl;
	}
	write << std::endl;



	for(unsigned int way = 1; way <= 16; way <<= 1)
	{
		write << way << "way(s) ";
		for(unsigned long size = 1024; size <= 1024*1024 ; size <<= 1 )
		{
			CacheTagArray cache_tag_array( size, way, BLOCK_SIZE_IN_BYTES, ADDR_SIZE_IN_BYTES);
			unsigned  miss = 0;
			unsigned  total = 0;

			std::fstream read;
			read.open(argv[1], std::fstream::in);


			while(!read.eof())
			{
				uint64 addr;
				read >> std::hex >> addr;
				if(cache_tag_array.read(addr) == false)
					miss++;
				total++;
			}
			write << (1. * miss / total);
			if(size != 1024*1024)
				write <<", ";
			read.close();
		}
		write << std::endl;
	}

	write.close();

	return 0;
}