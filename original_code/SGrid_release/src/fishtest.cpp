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

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "util/Log.h"
#include "util/ptimer.h"
#include "2D/data_generator/FishGenerator.h"
#include "2D/data_generator/fish/unit.h"
#include "2D/point_index/RTree.h"
#include "2D/point_index/SGrid.h"
#include "2D/point_index/TPRTree.h"
#include "2D/join_index/GenericPlaneSweep2D.h"
#include "2D/join_index/SynchTraversal.h"

using namespace PSimIndex;
using namespace DBUtl;

// FishGenerator* core;

// void run(int numTicks) {
//     DBUtl::PTimer pt;

//     double queryTime = 0;
//     double updateTime = 0;
//     double indexTime = 0;

//     // first, perform the next tick
//     for(int i = 0; i < numTicks; i++) {

//         pt.start();
//         indexTime += core->query();
//         pt.finish();
//         queryTime += pt.realResult();

//         pt.start();
//         core->update();
//         pt.finish();
//         updateTime += pt.realResult();
//     }

//     printf("query: %lf, index: %lf, update: %lf\n", queryTime, indexTime, updateTime);

// }

// void init() {
//     core = new FishGenerator();
//     core->setNumGoals(2);
//     core->setNumPoints(300);
//     core->setMinX(262);
//     core->setMaxX(762);
//     core->setMinY(134);
//     core->setMaxY(634);
//     core->genFish();
// }

void setParams(FishGenerator* fg) {
  fg->setNumTicks(2);
  fg->setNumGoals(10);
  fg->setMinX(1500);
  fg->setMaxX(2500);
  fg->setMinY(1500);
  fg->setMaxY(2500);
  fg->setMinSpeed(8);
  fg->setMaxSpeed(12);
  fg->setAlpha(25);
  fg->setRho(160);
  fg->setPInformed(0.10);
  fg->setSeed(1354, 5432);
}

template<template<class, class, class, class > class JoinType>
FishGenerator* createGen(int numPoints) {
  FishGenerator* fg = FishGenerator::createFishGenerator<JoinType>();
  fg->setNumPoints(numPoints);
  setParams(fg);
  return fg;
}

template<template<class, class, class > class IndexType>
FishGenerator* createGen(int numPoints) {
  FishGenerator* fg = FishGenerator::createFishGenerator<IndexType>();
  fg->setNumPoints(numPoints);
  setParams(fg);
  return fg;
}

/*
 * The main program.
 *
 * Usage: fish_sim [-graphics | -nographics] [-units nnn] [-goals nnn]
 */
int main(int argc, char **argv) {

  Log::open("-");  //for asserts;

//  std::cout << "numPoints, RTree, StripedSweep128, SynchTraversal, TPRTree"
//      << std::endl;
  std::cout << "numPoints, RTree, SynchTraversal, TPRTree, SGrid" << std::endl;

//  for (int i = 10000; i < 100000; i += 20000) {
  for (int i = 100; i < 1000; i += 200) {
    FishGenerator* fg = createGen<RTree>(i);
    fg->genTrace();
    FishTime* ft = fg->getFishTime();
    std::cout << i << " query: " << ft->queryTime << " update: "
        << ft->updateTime << " index: " << ft->indexTime << " ";
    delete fg;

//         fg = createGen<GenericPlaneSweep2D>(i);
//         fg->genTrace();
//         ft = fg->getFishTime();

//         std::cout << "query: " << ft->queryTime << " update: " << ft->updateTime
//                   << " index: " << ft->indexTime << " ";
//         delete fg;

    fg = createGen<SynchTraversal>(i);
    fg->genTrace();
    ft = fg->getFishTime();

    std::cout << "query: " << ft->queryTime << " update: " << ft->updateTime
        << " index: " << ft->indexTime << " ";
    delete fg;

    if (i < 700000) {
      fg = createGen<TPRTreeDefault>(i);
      fg->genTrace();
      FishTime* ft = fg->getFishTime();
      std::cout << "query: " << ft->queryTime << " update: " << ft->updateTime
          << " index: " << ft->indexTime << " ";
      delete fg;
    }

    fg = createGen<SGrid>(i);
    fg->genTrace();
    ft = fg->getFishTime();
    std::cout << " query: " << ft->queryTime << " update: " << ft->updateTime
        << " index: " << ft->indexTime << " ";
    delete fg;

    std::cout << std::endl;
  }

  printf("Done!\n");
  return 0;
}

