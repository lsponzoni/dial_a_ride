#if !defined(FEASIBILITY_HPP)
#define FEASIBILITY_HPP

#include "route.hpp"
#include "index.hpp"
#include "instance.hpp"


/* Corrected implementation of 
 * Efficient feasibility Testing Hunsaker Savelsberg 2002
 * A note on Efficient feasibility Testing, Tang
 */ 
typedef std::vector<double> Schedule;

bool test_feasibility(Route &r, VehicleIndex vehicle, Instance &g);
bool test_capacity(Route &r, VehicleIndex vehicle, Instance &g);
bool feasible_schedule(Route &r, Instance &g, Schedule &D, Schedule &L, Schedule &A, Schedule &W);

double ride_time_delta(int pickup, Schedule &departure, Schedule &arrival, Requests &gr);


void printScheduleConstraints(Route r, Instance &g);

#endif // FEASIBILITY_HPP
