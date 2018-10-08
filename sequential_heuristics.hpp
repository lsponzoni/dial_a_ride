#if !defined(SEQUENTIAL_HEURISTICS_HPP)
#define SEQUENTIAL_HEURISTICS_HPP


#include "instance.hpp"
#include "solution.hpp"



namespace Heuristic{
	namespace Sequential{
		Solution cons_time(Instance &i);
		Solution cons_distance(Instance &i);
	}
}

#endif // SEQUENTIAL_HEURISTICS_HPP
