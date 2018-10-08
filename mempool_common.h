#ifndef mempool_commom_H_
#define mempool_commom_H_

#include <cstdlib> 


typedef long unsigned int size_t;

class StoragePool {			//Possui as funcoes básicas Allocate e Free	a serem implementadas por um Gerenciador de Memória;
	public:
		StoragePool() {};
		~StoragePool() {};	
		void* Allocate (size_t bytes) {};
		void Free (void* ptr) {};
};

struct Tag {				//Definicao de Tag; consiste em um ponteiro para algum StoragePool
	StoragePool* pool;	
};

/*
void* operator new (size_t bytes, StoragePool &p);
void* operator new (size_t bytes);
void operator delete (void* ptr);

*/


#endif 