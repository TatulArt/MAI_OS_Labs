#ifndef METRICS_H
#define METRICS_H

#include "./geometry.h"


double Acceleration(double t_s,
                    double t_p);

double Efficiency(double s,
                  uint64_t p);

double Measure(Points points,
               uint64_t threads_count,
               Triangle expected);

#endif