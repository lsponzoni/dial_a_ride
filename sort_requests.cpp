#include "sort_requests.hpp"

#include <algorithm>

Indexes sort_dist_to_src(Indexes list, RequestIndex src, DMatrix dm){	
	Indexes requests(list);
	std::sort(requests.begin(), requests.end(), [src, dm]
		(RequestIndex &x, RequestIndex &y){
    		return dm.dist_points(x, src) < dm.dist_points(y, src);
		}
	);
	return requests;
}

Indexes sort_pickup_time(Indexes src_list, Requests r){
	Indexes requests(src_list);
	std::sort(requests.begin(), requests.end(), [r]
		(RequestIndex &r1, RequestIndex &r2) { 
    		return r.early(r1) < r.early(r2); 
		});
	return requests;
}
