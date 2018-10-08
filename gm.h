#ifndef _GM_H_
#define _GM_H_

#include "mempool_common.h"
typedef long unsigned int size_t;

class SLPool : public StoragePool{
	private:
		struct T_Header {
			unsigned int Length;
		};	

		enum { BlockSize = 16};    //Block has 16 bytes

		struct Block {
			T_Header Header;
			union {
				Block* next;
				char RawArea [BlockSize - sizeof(Header)];
			};
		};

		unsigned int Number_blocks;

		Block* mp_Pool;  	// Head of list
		Block mr_sentinel; 	//End of list

	public:
		SLPool(size_t bytes);
		~SLPool();
		void* Allocate (size_t bytes);
		void Free (void* ptr);
		void Print_Pool ();
};

#endif