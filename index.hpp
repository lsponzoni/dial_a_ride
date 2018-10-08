#if !defined(INDEXES_HPP)
#define INDEXES_HPP

#include <vector>

typedef int IndexType;
typedef std::vector<IndexType> Indexes;
typedef IndexType RequestIndex;
typedef IndexType VehicleIndex;

Indexes range(IndexType from, IndexType to);

#endif // INDEXES_HPP
