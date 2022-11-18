/*
 * PMU.cpp
 *
 *  Created on: Jul 24, 2014
 *      Author: dariuss
 */

#include "util/PMU.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>

#if PAPI == 1

PMU::PMU() :
    event_set_( PAPI_NULL ), counts_( NULL ) {
  int EventSet = PAPI_NULL;
  if ( (err_ = PAPI_library_init( PAPI_VER_CURRENT )) != PAPI_VER_CURRENT )
    test_fail( __FILE__, __LINE__, "PAPI_library_init", err_ );

  const PAPI_hw_info_t *hwinfo;
  if ( (hwinfo = PAPI_get_hardware_info()) == NULL )
    test_fail( __FILE__, __LINE__, "PAPI_get_hardware_info", PAPI_EMISC );

  printf( "Architecture %s, %d\n", hwinfo->model_string, hwinfo->model );

  /* Creating event set */
  if ( (err_ = PAPI_create_eventset( &event_set_ )) != PAPI_OK )
    test_fail( __FILE__, __LINE__, "PAPI_create_eventset", err_ );
}

PMU::~PMU() {
  if ( (err_ = PAPI_cleanup_eventset( event_set_ )) != PAPI_OK )
    test_fail( __FILE__, __LINE__, "PAPI_cleanup_eventset", err_ );

  /* Free all memory and data structures, event_set_ must be empty. */
  if ( (err_ = PAPI_destroy_eventset( &event_set_ )) != PAPI_OK )
    test_fail( __FILE__, __LINE__, "PAPI_destroy_eventset", err_ );

  /* free the resources used by PAPI */
  PAPI_shutdown();

  delete[] counts_;
}

void PMU::add_event(const char* event_name) {
  event_names_.push_back( event_name );
}

void PMU::add_cache_miss_profile() {
  event_names_.push_back( "PAPI_L1_DCM" );
  event_names_.push_back( "PAPI_L2_DCM" );
  event_names_.push_back( "PAPI_L3_TCM" );
}

void PMU::add_cache_profile_I() {
  event_names_.push_back( "PAPI_L1_DCM" );
  event_names_.push_back( "PAPI_L2_DCH" );
  event_names_.push_back( "PAPI_L3_TCM" );
//  event_names_.push_back( "PAPI_L3_DCA" );
//  event_names_.push_back( "PAPI_L3_TCA" );
}

void PMU::add_cache_profile_II() {
  event_names_.push_back( "PAPI_L1_DCM" );
  event_names_.push_back( "PAPI_L3_TCM" );
  event_names_.push_back( "PAPI_L3_DCA" );
  event_names_.push_back( "PAPI_L3_TCA" );
}

void PMU::add_predefined_events1() {
  event_names_.push_back( "PAPI_TOT_INS" ); // == INSTRUCTION_RETIRED
  event_names_.push_back( "PAPI_TOT_CYC" ); // == perf::CYCLES
//
//  event_names_.push_back( "PAPI_LD_INS" );// == "MEM_UOPS_RETIRED:ANY_LOADS"
//  event_names_.push_back( "PAPI_SR_INS" );

  event_names_.push_back( "LLC_REFERENCES" );
  event_names_.push_back( "LLC_MISSES" ); // == PAPI_L3_TCM

//  event_names_.push_back( "perf::LLC-LOADS" );
//  event_names_.push_back( "MEM_LOAD_UOPS_RETIRED" );
//  event_names_.push_back( "PAPI_LD_INS" );
//  event_names_.push_back( "PAPI_SR_INS" );
//  event_names_.push_back( "PAPI_BR_INS" );

  /* Cache Miss Events */
//  event_names_.push_back( "PAPI_L1_DCM" );
//  event_names_.push_back( "PAPI_L2_DCM" );
//  event_names_.push_back( "PAPI_L3_TCM" );
  /* Cache Hit Events */
//  event_names_.push_back( "PAPI_L1_DCM" );
//  event_names_.push_back( "PAPI_L2_DCH" );
//  event_names_.push_back( "PAPI_L3_TCA" );
//
//  event_names_.push_back( "PAPI_L3_TCM" );
//  event_names_.push_back( "PAPI_L1_DCM" ); // == "PAPI_L2_DCA" (miss @L1 -> access @L2)
//  event_names_.push_back( "PAPI_L2_DCM" );
//  event_names_.push_back( "PAPI_L2_DCH" );
//  event_names_.push_back( "PAPI_L2_TCA" );
//  event_names_.push_back( "PAPI_L3_DCA" ); // == "PAPI_L2_DCM" (miss @L2 -> access @L3)
//  event_names_.push_back( "PAPI_L3_TCM" );
//  event_names_.push_back( "PAPI_L3_TCA" );
//
//  event_names_.push_back( "MEM_LOAD_UOPS_MISC_RETIRED:LLC_MISS" );
//  event_names_.push_back( "L1-DCACHE-STORE-MISSES" );
//  event_names_.push_back( "perf::LLC-LOADS" );
//  event_names_.push_back( "perf::LLC-LOAD-MISSES" );
//
//  event_names_.push_back( "MEM_UOPS_RETIRED:ANY_LOADS" ); // both are
//  event_names_.push_back( "MEM_UOPS_RETIRED:ALL_LOADS" ); // the same
  /* These are deprecated events, use MEM_LOAD_UOPS_RETIRED:__ instead */
//  event_names_.push_back( "MEM_LOAD_RETIRED" );
//  event_names_.push_back( "MEM_LOAD_RETIRED:HIT_LFB" );
//  event_names_.push_back( "MEM_LOAD_RETIRED:L1_HIT" );
//  event_names_.push_back( "MEM_LOAD_RETIRED:L2_HIT" );
//  event_names_.push_back( "MEM_LOAD_RETIRED:L3_HIT" );
//  event_names_.push_back( "MEM_LOAD_RETIRED:L3_MISS" );
//
//  event_names_.push_back( "MEM_LOAD_UOPS_RETIRED" );// x3 times more than PAPI_LD_INS
//  event_names_.push_back( "MEM_LOAD_UOPS_RETIRED:HIT_LFB" );
//  event_names_.push_back( "MEM_LOAD_UOPS_RETIRED:L1_HIT" );
//  event_names_.push_back( "MEM_LOAD_UOPS_RETIRED:L2_HIT" );
//  event_names_.push_back( "MEM_LOAD_UOPS_RETIRED:L3_HIT" );
//  event_names_.push_back( "MEM_LOAD_UOPS_RETIRED:L3_MISS" );
}

void PMU::start() {
  const uint32_t NumEvents = event_names_.size();
  /* Get event codes and add them to EventSet */
  for (uint32_t i = 0; i < NumEvents; ++i) {
    int e_code = -1;
    char* e_name = const_cast<char*>( event_names_[i].c_str() );
    if ( (err_ = PAPI_event_name_to_code( e_name, &e_code )) != PAPI_OK )
      test_fail( __FILE__, __LINE__, "PAPI_event_name_to_code", err_ );

    if ( (err_ = PAPI_add_event( event_set_, e_code )) != PAPI_OK ) {
      printf( "Adding %-40s: %X\n", e_name, e_code );
      test_fail( __FILE__, __LINE__, "PAPI_add_event", err_ );
    }
  }

  /* Also possible to add all events at once to the eventset */
//  if ( (err_ = PAPI_add_events( event_set_, papi_event_codes, num_events_ ))
//      != PAPI_OK )
//    ERROR_RETURN( err_ );
  if ( (err_ = PAPI_start( event_set_ )) != PAPI_OK )
    test_fail( __FILE__, __LINE__, "PAPI_start", err_ );
  /* PAPI measurements started */
}

void PMU::stop() {
  counts_ = new long long[event_names_.size()];
  if ( (err_ = PAPI_stop( event_set_, counts_ )) != PAPI_OK )
    test_fail( __FILE__, __LINE__, "PAPI_stop", err_ );
}

void PMU::print() {
  const uint32_t NumEvents = event_names_.size();
  printf( "PAPI\n" );
  for (uint32_t i = 0; i < NumEvents; ++i) {
    printf( " \\-> %-40s=%12lld\n", event_names_[i].c_str(), counts_[i] );
  }
  if ( NumEvents >= 2 && event_names_[0].compare( "PAPI_TOT_INS" ) == 0
      && event_names_[1].compare( "PAPI_TOT_CYC" ) == 0 ) {
    printf( " \\-> CPI=%.2f\n", counts_[1] / (double) counts_[0] );
  }
  if ( NumEvents >= 4 && event_names_[2].compare( "LLC_REFERENCES" ) == 0
      && event_names_[3].compare( "LLC_MISSES" ) == 0 ) {
    printf( " \\-> LLC miss rate=%0.3f %%\n",
        counts_[3] / (double) counts_[2] * 100 );
  }
}

void PMU::print_cache_profile() {
  const uint32_t NumEvents = event_names_.size();
  print();
  if ( NumEvents == 3 && event_names_[0].compare( "PAPI_L1_DCM" ) == 0
      && event_names_[1].compare( "PAPI_L2_DCH" ) == 0
      && event_names_[2].compare( "PAPI_L3_TCM" ) == 0 ) {

    printf( " \\-> %-40s=%10.2lf %%\n", "L1_DCM serviced @L2",
        counts_[1] / (double) counts_[0] * 100 );
    printf( " \\-> %-40s=%10.2lf %%\n", "L1_DCM serviced @MEM",
        counts_[2] / (double) counts_[0] * 100 );
  } else if ( NumEvents == 4
      && event_names_[0].compare( "PAPI_L1_DCM" ) == 0
      && event_names_[1].compare( "PAPI_L3_TCM" ) == 0
      && event_names_[2].compare( "PAPI_L3_DCA" ) == 0
      && event_names_[3].compare( "PAPI_L3_TCA" ) == 0 ) {
    long long L3_CH = counts_[3] - counts_[1];
    printf( " \\-> %-40s=%10.2lf %%\n", "L1_DCM serviced @L3",
        L3_CH / (double) counts_[0] * 100 );
  }
}

/* Use a positive value of retval to simply print an error message */
void PMU::test_fail(const char *file, int line, const char *call, int retval) {

  int line_pad;
  char buf[128];

  line_pad = (int) (50 - strlen( file ));
  if ( line_pad < 0 )
    line_pad = 0;

  memset( buf, '\0', sizeof(buf) );

  fprintf( stdout, "%-*s FAILED\nLine # %d\n", line_pad, file, line );

  if ( retval == PAPI_ESYS ) {
    sprintf( buf, "System error in %s", call );
    perror( buf );
  } else if ( retval > 0 ) {
    fprintf( stdout, "Error: %s\n", call );
  } else if ( retval == 0 ) {
#if defined(sgi)
    fprintf( stdout, "SGI requires root permissions for this test\n" );
#else
    fprintf( stdout, "Error: %s\n", call );
#endif
  } else {
    fprintf( stdout, "Error in %s: %s\n", call, PAPI_strerror( retval ) );
  }

  fprintf( stdout, "\n" );
  exit( 1 );
}

#endif
