#include "index.hpp"

Indexes range(IndexType from, IndexType to){
	Indexes k(to);
	for(int i = 0; i < to; i++){
		k[i] = from + i;
	}
	return k;
}