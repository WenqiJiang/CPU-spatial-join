/*
 * Copyright 2012 Cornell Database Group
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _STATIC_INDEX_SCALE_QUERIES_H_
#define _STATIC_INDEX_SCALE_QUERIES_H_

#include "2D/Experiment2D.h"
#include "2D/point_index/BinarySearch2D.h"
#include "2D/point_index/RTree.h"
#include "2D/point_index/CRTree.h"
#include "2D/point_index/SimpleGrid.h"
#include "2D/point_index/SGrid.h"
#include "2D/point_index/LinearizedKDTrie.h"
#include "2D/workload/TraceWorkload2D.h"
#include "2D/TestTypes2D.h"

namespace PSimIndex {
class StaticIndexScaleQueries: public Experiment2D {
public:
  void init() {

    folder = "out/traces/StaticIndex/StaticIndexScaleQueries";
    experimentName = "StaticIndexScaleQueries";
    addWorkloadParam("QueryFrac");

    int size = 22361;
    int numAgents = 50000;

    addIndex(getJoinMethod(new BinarySearch2D<Point2D, Key2D, SumAgg2D>()));
    addIndex(getJoinMethod(new RTree<Point2D, Key2D, SumAgg2D>()));
    addIndex(getJoinMethod(new CRTree<Point2D, Key2D, SumAgg2D>()));
    addIndex(getJoinMethod(new LinearizedKDTrie<Point2D, Key2D, SumAgg2D>()));
    addIndex(getJoinMethod(new SimpleGrid<Point2D, Key2D, SumAgg2D>(GRID_DFLT_CPS, size, GRID_DFLT_BS, numAgents)));

    // drs: new index added
    addIndex(getJoinMethod(new SGrid<Point2D, Key2D, SumAgg2D>(SGRID_DFLT_CPS, SGRID_DFLT_BS, size)));

    // addWorkload(new TraceWorkload2D("traces/query_update/query0.1.update0.5trial1.dat", true), 0, 0.1);
    // addWorkload(new TraceWorkload2D("traces/query_update/query0.3.update0.5trial1.dat", true), 0, 0.3);
    // addWorkload(new TraceWorkload2D("traces/query_update/query0.5.update0.5trial1.dat", true), 0, 0.5);
    // addWorkload(new TraceWorkload2D("traces/query_update/query0.7.update0.5trial1.dat", true), 0, 0.7);
    // addWorkload(new TraceWorkload2D("traces/query_update/query0.9.update0.5trial1.dat", true), 0, 0.9);

    // drs: one trail is enough
    std::vector<Workload2D*>* workloads = new std::vector<Workload2D*>();
    workloads->push_back(new TraceWorkload2D("traces/query_update/query0.1.update0.5trial1.dat", true));
//    workloads->push_back(
//        new TraceWorkload2D("traces/query_update/query0.1.update0.5trial2.dat",
//            true));
//    workloads->push_back(
//        new TraceWorkload2D("traces/query_update/query0.1.update0.5trial3.dat",
//            true));
    addWorkload(workloads, "scalequeries0.1", 0, 0.1);

    workloads = new std::vector<Workload2D*>();
    workloads->push_back(new TraceWorkload2D("traces/query_update/query0.3.update0.5trial1.dat", true));
//    workloads->push_back(
//        new TraceWorkload2D("traces/query_update/query0.3.update0.5trial2.dat",
//            true));
//    workloads->push_back(
//        new TraceWorkload2D("traces/query_update/query0.3.update0.5trial3.dat",
//            true));
    addWorkload(workloads, "scalequeries0.3", 0, 0.3);

    workloads = new std::vector<Workload2D*>();
    workloads->push_back(new TraceWorkload2D("traces/query_update/query0.5.update0.5trial1.dat", true));
//    workloads->push_back(
//        new TraceWorkload2D("traces/query_update/query0.5.update0.5trial2.dat",
//            true));
//    workloads->push_back(
//        new TraceWorkload2D("traces/query_update/query0.5.update0.5trial3.dat",
//            true));
    addWorkload(workloads, "scalequeries0.5", 0, 0.5);

    workloads = new std::vector<Workload2D*>();
    workloads->push_back(new TraceWorkload2D("traces/query_update/query0.7.update0.5trial1.dat", true));
//    workloads->push_back(
//        new TraceWorkload2D("traces/query_update/query0.7.update0.5trial2.dat",
//            true));
//    workloads->push_back(
//        new TraceWorkload2D("traces/query_update/query0.7.update0.5trial3.dat",
//            true));
    addWorkload(workloads, "scalequeries0.7", 0, 0.7);

    workloads = new std::vector<Workload2D*>();
    workloads->push_back(new TraceWorkload2D("traces/query_update/query0.9.update0.5trial1.dat", true));
//    workloads->push_back(
//        new TraceWorkload2D("traces/query_update/query0.9.update0.5trial2.dat",
//            true));
//    workloads->push_back(
//        new TraceWorkload2D("traces/query_update/query0.9.update0.5trial3.dat",
//            true));
    addWorkload(workloads, "scalequeries0.9", 0, 0.9);

  }

  StaticIndexScaleQueries() :
      Experiment2D() {
  }
};
}

#endif /* _STATIC_INDEX_SCALE_QUERIES_H_ */
