#include "mempool_common.h"
#include "gm.h"
#include <time.h>
#include <stdlib.h>
#include <queue>
#include <iostream>


//Gerar unidade de tempo aleatorio
int get_random_time() {
	return rand() % 10;
}

//Gerar tamanho de bytes aleatorio
int get_random_size() {
	return rand() % 500  + 10;
}

struct T_Event {
	int _time;
	int _size;
	void* _ptr;
};


//Sobrecarga para uso do priority queue
bool operator<( const T_Event& a, const T_Event& b ) {
  return a._time < b._time;
}



int main ()  { 

	time_t t1, t2;

	srand(time(NULL));
	std::priority_queue <T_Event> pq;
	int time_limit = 10000000;
	SLPool pool(100000000);

	std::cout << "Iniciando teste de " << time_limit << " interacoes com SLPool" << std::endl;
	t1 = time(NULL);
	for (int i = 0; i < time_limit; i++) {
		while (!pq.empty()) {
			T_Event x = pq.top();
			if (x._time > i) {
				break;
			}

			pq.pop();
			pool.Free(x._ptr);
			//pool.Print_Pool();
			//std::cout << std::endl;
		}

		int memSize = get_random_size();
		int next_time = get_random_time() + i;
		void* ptr = pool.Allocate(memSize);

		T_Event y;
		y._size = memSize;
		y._time = next_time;
		y._ptr = ptr;

		pq.push(y);

		//pool.Print_Pool();
		//std::cout << std::endl;
	}
	
	t2 = time(NULL);
	std::cout << "Finalizando... resultado em segundos: "<< t2-t1 << std::endl;	



	std::cout << "Iniciando teste de " << time_limit << " interacoes com SO" << std::endl;
	t1 = time(NULL);

	for (int i = 0; i < time_limit; i++) {
		while (!pq.empty()) {
			T_Event x = pq.top();
			if (x._time > i) {
				break;
			}

			pq.pop();
			operator delete (x._ptr);
		}

		int memSize = get_random_size();
		int next_time = get_random_time() + i;
		void* ptr = operator new (memSize);

		T_Event y;
		y._size = memSize;
		y._time = next_time;
		y._ptr = ptr;

		pq.push(y);
	}	

	t2 = time(NULL);
	std::cout << "Finalizando... resultado em segundos: "<< t2-t1 << std::endl;

	return 0;
}