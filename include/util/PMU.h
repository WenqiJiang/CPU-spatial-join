/*
 * PMU.h
 *
 *  Created on: Jul 24, 2014
 *      Author: dariuss
 *
 *  My performance monitoring unit (PMU) using PAPI.
 */

#ifndef PMU_H_
#define PMU_H_

#include <vector>
#include <string>

#include <stdint.h>

#if PAPI == 1
#include "papi.h"

#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n", retval,__FILE__,__LINE__);  exit(retval); }

class PMU {
public:
  PMU();
  virtual ~PMU();

  /* Measure predefined PAPI events (profile) */
  void add_event(const char* event_name);
  void add_cache_miss_profile();
  void add_cache_profile_I();
  void add_cache_profile_II();
  void add_predefined_events1();
  void start();
  /* Stop counting. This also reads the counters */
  void stop();
  void print();
  void print_cache_profile();

private:
  void test_fail(const char *file, int line, const char *call, int retval);

  int err_; // maintains current error code
  int event_set_;
  long long *counts_;
  std::vector<std::string> event_names_;

};

#endif

#endif /* PMU_H_ */
