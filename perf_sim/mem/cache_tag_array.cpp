#include <cache_tag_array.h>
#include <cmath>
#include <iostream>
#include <cassert>
#include <deque>


CacheTagArray::CacheTagArray(unsigned int size_in_bytes,
                   			unsigned int ways,
                   			unsigned short block_size_in_bytes, 
                   			unsigned short addr_size_in_bits):
    size_in_bytes(size_in_bytes), ways(ways), block_size_in_bytes(block_size_in_bytes),
    addr_size_in_bits(addr_size_in_bits), time(0)
{
	if(ways == 0)
	{
		way_size_in_bytes = size_in_bytes;
		lset = 0;
		loffset = log2(block_size_in_bytes);
		ltag = addr_size_in_bits - loffset;

		num_of_sets = way_size_in_bytes / block_size_in_bytes;
		assert(num_of_sets);

		tag_array_type2 = std::deque<uint64>(way_size_in_bytes / 4, 0);
	}

	else
	{		
		assert(size_in_bytes);
		assert(block_size_in_bytes);
		assert(addr_size_in_bits);

		way_size_in_bytes = size_in_bytes / ways;

		assert(way_size_in_bytes);
		assert(ways);

		num_of_sets = way_size_in_bytes / block_size_in_bytes;
		assert(num_of_sets);

		loffset = log2(block_size_in_bytes);
		lset = log2(num_of_sets);
		ltag = addr_size_in_bits - lset - loffset;

		assert(loffset);

		tag_array = new  map*[num_of_sets];
		assert(tag_array);

		for(unsigned int set = 0; set < num_of_sets; set++)
		{
			tag_array[set] = new  map[ways];
		}
		for(unsigned int set = 0; set < num_of_sets; set++)
		{
			for(unsigned short way = 0; way < ways; way++)
			{
				tag_array[set][way].time = 0;
				tag_array[set][way].tag = 0;;
			}
		}
	}	
}

CacheTagArray::~CacheTagArray()
{
	if(ways != 0)
	{
		for(unsigned int i = 0; i < num_of_sets; ++i)
			delete[] tag_array[i];
		delete[] tag_array;
	}
}


bool CacheTagArray::read(uint64 addr)
{
	uint64 tag = getTag(addr);
	uint64 set = getSet(addr);


	if(ways == 0)
	{
		for(auto it = tag_array_type2.begin(); it != tag_array_type2.end(); ++it)
		{
			if(*it == tag)
			{
				uint64 tmp = *it;
				tag_array_type2.erase(it);
				tag_array_type2.push_back(tmp);
				return true;
			} 
		}
		write(addr);
		return false;
	}
	else
	{
		assert(ways);
		for(unsigned int way = 0; way < ways; way++)
		{
			if((tag_array[set][way].tag == tag))
			{
				this->time++;
				tag_array[set][way].time = time;
				return true;
			}
		}
	}
		write(addr);
		return false;
}

void CacheTagArray::write(uint64 addr)
{
	uint64 tag = getTag(addr);
	uint64 set = getSet(addr);

	if(ways == 0)
	{
		tag_array_type2.pop_front();
		tag_array_type2.push_back(tag);
	}
	else
	{
		assert(ways);
		uint64 min = time;

		for(unsigned way = 0; way < ways;  way++)
		{
			min = tag_array[set][way].time < time ? tag_array[set][way].time : min;
		}

		for(unsigned int way = 0; way < ways; way++)
		{
			if(tag_array[set][way].time == min)
			{
				tag_array[set][way].tag = tag;
				tag_array[set][way].time = time;
				break;	
			}
		}
		time++;
	}
}