#include <iostream>
#include "gm.h"
#include <new>

typedef long unsigned int size_t;


SLPool::SLPool (size_t bytes) {
	if ((bytes % BlockSize) != 0) {
		bytes = ((bytes / BlockSize) + 1) * BlockSize;		//Arrendonda bytes para multiplo de blocksize
	}

	bytes = bytes + BlockSize;								//Aumenta um block (Sentinela);
	Number_blocks = bytes/BlockSize;						//Quantidade final de blocos

	mp_Pool = new Block[Number_blocks];						//Aloca pool 

	mr_sentinel.Header.Length = 0;							
	mr_sentinel.next = mp_Pool;								//sentila next aponta para mp_Pool  / inicio de areas livres

	mp_Pool->Header.Length = Number_blocks-1;					//Tamanho do primeiro espaço é igual ao total de blocos;
}


SLPool::~SLPool () { 
	delete mp_Pool;
}

void* SLPool::Allocate (size_t bytes) {
	unsigned int NBlocks;											//Numero de blocos para satisfazer requisicao de memoria
	if (((bytes+sizeof(T_Header)) % BlockSize) != 0) {				//Calcula numero minimo de blocos
		NBlocks = ((bytes+sizeof(T_Header))/BlockSize) + 1;			//Se nao for multiplo, arredonda para cima
	}

	else															//Se for multiplo...
		NBlocks = ((bytes+sizeof(T_Header))/BlockSize);

	Block* aux_pre = &mr_sentinel;									//aux_pre aponta para o bloco antes de aux
	Block* aux = mr_sentinel.next;									//aux aponta para inicio das areas livres

	//Procura area com tamanho maior ou igual ao requisitado
	while (aux->Header.Length < NBlocks) {							//Enquanto Tamanho da area for menor ou igual a n de blocks
		if (aux->next == NULL) {
			throw std::bad_alloc();										//Erro - Não tem área com tamanho suficiente disponivel;
			break;
		}
		aux = aux->next;											//Vai para a proxima area
		aux_pre = aux_pre->next;
	}

	if (aux->Header.Length > NBlocks) {								//Se a area for maior que numero de blocos pedidos; divide em area ocupada e livre
		Block* aux_post = aux + NBlocks*BlockSize;					//Ponteiro para inicio da area livre
		aux_post->Header.Length = aux->Header.Length - NBlocks;		//Tamanho da area livre
	
		aux->Header.Length = NBlocks;								//Tamanho da area ocupada

		aux_pre->next = aux_post;

		return aux;													//Retorna ponteiro para a memoria reservada
	}

	else if (aux->Header.Length == NBlocks) {						//Se a area for exatamente o numero de blocos pedidos;
		Block* aux_post = aux->next;
		Block* busca = &mr_sentinel;								//Começa a busca nas areas livres	
		
		aux_pre->next = aux_post;		

		return aux;
	}
}

void SLPool::Free (void* ptr) {
	Block* busca = reinterpret_cast <Block*> (ptr);
	Block* busca_pre = &mr_sentinel;
	Block* busca_post = mr_sentinel.next;

	while (busca_post < ptr && busca_post != NULL) {				//Para quando busca_post for a proxima area livre dpois de ptr		
		busca_pre = busca_pre->next;								//busca_pre sera a area livre anterior;
		busca_post = busca_post->next; 								//busca_post sera a area livre posterior;
	}


	//Se a posterior for livre
	if (busca + (busca->Header.Length)*BlockSize == busca_post) {	//Proxima area depois da que vai ser liberada é igual a busca_post ? 
		busca->Header.Length = busca->Header.Length + busca_post->Header.Length; //Junta as duas (Tamanho da primeira agora é o tamanho das duas)
		busca->next = busca_post->next;								//aponta para a proxima area
		busca_pre->next = busca;									//E a area anterior aponta para a nova junção	
		busca_post = busca_post->next;

	}

	//Se a anterior for livre
	if ((busca_pre + (busca_pre->Header.Length)*BlockSize) == busca) {
		busca_pre->Header.Length = busca_pre->Header.Length + busca->Header.Length;	 //Tamanho da area anterior agora é igual ao tamanho das duas
		busca_pre->next = busca_post;
	}

	//Se a anterior e posterior forem reservadas, nao é necessario juntar areas
	else if ((busca_pre + (busca_pre->Header.Length)*BlockSize) != busca && busca + (busca->Header.Length)*BlockSize != busca_post) {
		busca_pre->next = busca;			//Area livre anterior passa a apontar para a area liberada
		busca->next = busca_post;			//Area liberada passa a apontar para a proxima area livre;
	}
}




//Imprime o Memory Pool
void SLPool::Print_Pool () {
	Block* busca = mp_Pool;					//Busca começa no inicio do memory pool
	Block* busca_aux = mr_sentinel.next;	//Busca_aux começa no inicio das áreas livres
	int i = 0;	

	while (i < Number_blocks-1) {				//Contagem para passar por todos os blocos
		if (busca != busca_aux) {			//Se busca for diferente de busca_aux, quer dizer que o bloco que busca aponta é Reservado
			std::cout << " [" << busca->Header.Length << ".Ocupado] **";		//Imprime " Quantidade de Blocos . Ocupado "
			i = i + busca->Header.Length;	//incrementa i com a quantidade de blocos que ja foi checado
			busca = busca + (busca->Header.Length * BlockSize);	//busca vai para a proxima area;
		}

		if (busca == busca_aux) {			//Se busca for igual de busca_aux, quer dizer que o bloco que busca aponta é Livre
			std::cout << " [" << busca->Header.Length << ".Livre] **";
			i = i + busca->Header.Length;	//incrementa i com a quantidade de blocos que ja foi checado
			busca = busca + (busca->Header.Length * BlockSize); //Busca vai para a proxima area
			busca_aux = busca_aux->next;						//busca_aux vai para a proxima área vazia
		}
	}

		std::cout << std::endl;
}