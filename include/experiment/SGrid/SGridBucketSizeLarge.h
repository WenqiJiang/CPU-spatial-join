/*
 * SGridBucketSizeLarge.h
 * Created on: Sep 12, 2012
 *
 * Darius Sidlauskas
 * Copyright (c) 2012 MADALGO, Aarhus University
 *
 *  Experiment for determining the optimal bucket size for SGrid.
 */

#ifndef _SGRID_BUCKET_SIZE_LARGE_H_
#define _SGRID_BUCKET_SIZE_LARGE_H_

#include "2D/Experiment2D.h"
#include "TimingResult.h"
#include "2D/point_index/SGrid.h"
#include "2D/TestTypes2D.h"

namespace PSimIndex {
class SGridBucketSizeLarge: public Experiment2D {
public:

  void init() {
    folder = "out/traces/SGrid/SGridBucketSizeLarge";
    experimentName = "SGridBucketSizeLarge";
    addIndexParam("bucketSize");

    const int size = 22361;

    for (int bs = 2; bs <= 64; bs += 2) {
      SGrid<Point2D, Key2D, SumAgg2D>* grid;
      grid = new SGrid<Point2D, Key2D, SumAgg2D>(SGRID_DFLT_CPS, bs, size);
      addIndex(getJoinMethod(grid), bs);
    }

    std::vector<Workload2D*>* wc = new std::vector<Workload2D*>();
    wc->push_back(new TraceWorkload2D("traces/points/scalepoints50000trial1.dat", true));
//    wc->push_back(new TraceWorkload2D("traces/points/scalepoints50000trial2.dat", true));
//    wc->push_back(new TraceWorkload2D("traces/points/scalepoints50000trial3.dat", true));

    addWorkload(wc, "scalepoints50000", 0);

    //addWorkload(new TraceWorkload2D("traces/scalepoints90000.dat", true), 0);
  }

  SGridBucketSizeLarge() {
  }
};
}

#endif /* _SGRID_BUCKET_SIZE_LARGE_H_ */
