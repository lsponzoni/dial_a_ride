
#if !defined(SORT_REQUESTS_HPP)
#define SORT_REQUESTS_HPP

#include "route.hpp"
#include "instance.hpp"


Indexes sort_dist_to_src(Indexes list, RequestIndex src, DMatrix dm);
Indexes sort_pickup_time(Indexes src_list, Requests r);


#endif // SORT_REQUESTS_HPP
