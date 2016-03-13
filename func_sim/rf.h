/*
 * rf.h - mips register file
 * @author Pavel Kryukov pavel.kryukov@phystech.edu
 * Copyright 2015 MIPT-MIPS 
 */

#ifndef RF_H
#define RF_H

#include <func_instr.h>


class RF
{

        struct Reg {
            uint32 value;
            bool   is_valid;
            Reg() : value(0ull), is_valid(true) { }    
        } array[REG_NUM_MAX];
    public:
        uint32 read(RegNum num)
        {
            return array[(size_t)num].value; 
        };
        bool check(RegNum num) const {return array[(size_t)num].is_valid; }
        void invalidate(RegNum num) { array[(size_t)num].is_valid = false; }
        void write(RegNum num, uint32 val)
        {

            assert(array[(size_t)num].is_valid == false);
            array[(size_t)num].is_valid = true;
            array[num].value = (num != 0) ? val : 0;
        }

        inline void read_src1( FuncInstr& instr) 
        {
           instr.set_v_src1( read(instr.get_src1_num()));
        }

        inline void read_src2( FuncInstr& instr) 
        {
           instr.set_v_src2( read(instr.get_src2_num()));
        }

        inline void write_dst( const FuncInstr& instr)
        {
            RegNum reg_num = instr.get_dst_num();
            if ( REG_NUM_ZERO == reg_num)
                return;
//            assert(array[(size_t)reg_num].is_valid == false);
            array[(size_t)reg_num].value = instr.get_v_dst();
            array[(size_t)reg_num].is_valid = true;
        }

        inline void reset( RegNum reg)
        {
            array[reg].value = 0;
            array[reg].is_valid = true;
        }
 
        RF()
        {
            for ( size_t i = 0; i < REG_NUM_MAX; ++i)
                reset((RegNum)i);
        }
};
          
#endif
 
