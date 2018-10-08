#include <iostream>
#include "mempool_common.h"
#include "gm.h"

int main () {
	
	std::cout << std::endl;	

	std::cout << "*** Demonstracao do Gerenciador de memória SLPool em funcionamento ***" << std::endl;
	std::cout << "*** A exibicao eh feita por conjuntos de blocos; ***" << std::endl;
	std::cout << "*** Para cada conjunto de blocos, eh exibido seu tamanho de blocos e se esta ocupado ou livre, p.e  [21.Ocupado] ***" << std::endl;

	std::cout << std::endl;

	std::cout << "-> Criando um Pool com 1024 bytes de memoria, organizado em blocos de 16 bytes" << std::endl;
		SLPool x(1024);			//Criação de um GM Pool com 500 bytes
		x.Print_Pool();
	std::cout << std::endl;

	std::cout << "-> Requisitando um espaco 'A' de 180 bytes (12 blocos) no Pool" << std::endl;
		void* a = x.Allocate(180);
		x.Print_Pool();
	std::cout << std::endl;

	std::cout << "-> Requisitando um espaco 'B' de 32 bytes (2 blocos) no Pool" << std::endl;
		void* b = x.Allocate(32);
		x.Print_Pool();
	std::cout << std::endl;

	std::cout << "-> Requisitando um espaco 'C' de 210 bytes (14 blocos) no Pool" << std::endl;
		void* c = x.Allocate(210);
		x.Print_Pool();
	std::cout << std::endl;

	std::cout << "-> Devolvendo o espaco 'B' ao Pool" << std::endl;
		x.Free(b);
		x.Print_Pool();
	std::cout << std::endl;

	std::cout << "-> Devolvendo o espaco 'C' ao Pool" << std::endl;
		x.Free(c);
		x.Print_Pool();
	std::cout << std::endl;

	std::cout << "-> Devolvendo o espaco 'A' ao Pool" << std::endl;
		x.Free(a);
		x.Print_Pool();
	std::cout << std::endl;
	

	return 0;
}