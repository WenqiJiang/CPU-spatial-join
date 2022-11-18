/*
 * NumSGridCellsLarge.h
 * Created on: Sep 12, 2012
 *
 * Darius Sidlauskas
 * Copyright (c) 2012 MADALGO, Aarhus University
 *
 *  Experiment for determining the optimal grid cell size for SGrid.
 */

#ifndef _NUM_SGRID_CELLS_LARGE_H_
#define _NUM_SGRID_CELLS_LARGE_H_

#include "2D/Experiment2D.h"
#include "TimingResult.h"
#include "2D/point_index/SGrid.h"
#include "2D/TestTypes2D.h"

namespace PSimIndex {

class NumSGridCellsLarge: public Experiment2D {
public:
  void init() {
    folder = "out/traces/SGrid/NumSGridCellsLarge";
    experimentName = "NumSGridCellsLarge";
    addIndexParam("GridCellsPerSide");

    const int area_size_size = 22361;

    for (int i = 4; i < 128; i += 4) {
      addIndex(getJoinMethod(new SGrid<Point2D, Key2D, SumAgg2D>(i, SGRID_DFLT_BS, area_size_size)), i);
    }

    std::vector<Workload2D*>* wc = new std::vector<Workload2D*>();
    wc->push_back(new TraceWorkload2D("traces/points/scalepoints50000trial1.dat", true));
//    wc->push_back(new TraceWorkload2D("traces/points/scalepoints50000trial2.dat", true));
//    wc->push_back(new TraceWorkload2D("traces/points/scalepoints50000trial3.dat", true));

    addWorkload(wc, "scalepoints50000", 0);

  }

  NumSGridCellsLarge() :
      Experiment2D() {
  }
};
}

#endif /* _NUM_SGRID_CELLS_LARGE_H_ */
