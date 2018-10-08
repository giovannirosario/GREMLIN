#include "mempool_common.h"
#include <cstdlib> 

typedef long unsigned int size_t;

/** Overloading of operators **

void* operator new (size_t bytes, StoragePool &p) {
	Tag* tag = reinterpret_cast <Tag*> (p.Allocate (bytes + sizeof(Tag))); //Cria um espaço de x bytes + Tag.   tag aponta para esse bloco de espaço
	tag->pool = &p;										//pool (inicio do bloco de espaço) recebe o endereço de quem o bloco pertence
	return tag + sizeof(Tag);							//Retorna o endereço de onde começa o espaço livre de memória
}

void* operator new (size_t bytes) {
	Tag* tag = reinterpret_cast <Tag*> (std::malloc (bytes + sizeof(Tag)));
	tag->pool = 0;
	return ( reinterpret_cast <void *> (tag + sizeof(Tag)));
}

void operator delete (void* ptr) {
	Tag* ptr1 = reinterpret_cast <Tag*> (ptr);
	Tag* tag = reinterpret_cast <Tag*> (ptr1 - sizeof(Tag));
	if (tag->pool != 0)					//Se o bloco pertence a algum StoragePool
		tag->pool->Free(tag);
	else
		std::free (tag);			
}

*/

