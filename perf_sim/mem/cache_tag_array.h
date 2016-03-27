#ifndef CACHE_TAG_ARRAY_H
#define CACHE_TAG_ARRAY_H

#include <types.h>
#include <iostream>
#include <cassert>
#include <stdlib.h>
#include <deque>


struct map
	{
		uint64 tag;
	//	time counter which shows
	//	how many "read" operation didn't use it
		unsigned time;	
	};

class CacheTagArray
{
public:
	CacheTagArray(unsigned int size_in_bytes,
                   unsigned int ways,
                   unsigned short block_size_in_bytes , 
                   unsigned short addr_size_in_bits );
	~CacheTagArray();
    bool read( uint64 addr);
    void write( uint64 addr);
private:
	map** tag_array;

	std::deque<uint64> tag_array_type2;

	unsigned int size_in_bytes;
	unsigned int  ways;
	unsigned int block_size_in_bytes;
	unsigned int addr_size_in_bits;
	unsigned int way_size_in_bytes;
	unsigned int num_of_sets;

	uint64 time;

	unsigned short loffset;
	unsigned short lset;
	unsigned short ltag;

	uint64 getTag(uint64 addr) const{ return addr >> lset >> loffset; }
	uint64 getSet(uint64 addr) const{ return  (addr >> loffset) & (num_of_sets- 1); }
};

#endif //CACHE_TAG_ARRAY_H