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

#ifndef _SIMPLE_GRID_BUCKET_SIZE_LARGE_H_
#define _SIMPLE_GRID_BUCKET_SIZE_LARGE_H_

#include "2D/Experiment2D.h"
#include "TimingResult.h"
#include "2D/point_index/SimpleGrid.h"
#include "2D/TestTypes2D.h"

namespace PSimIndex {
    class SimpleGridBucketSizeLarge : public Experiment2D {
    public:

        void init() {
            folder = "out/traces/SimpleGrid/SimpleGridBucketSizeLarge";
            experimentName = "SimpleGridBucketSizeLarge";
            addIndexParam("bucketSize");

            int size = 22361;
            int numAgents = 50000;

            for(int bs = 2; bs <= 32; bs += 2) {
                SimpleGrid<Point2D,Key2D,SumAgg2D>* grid;
                grid = new SimpleGrid<Point2D,Key2D,SumAgg2D>(GRID_DFLT_CPS, size, bs, numAgents);
                addIndex(getJoinMethod(grid), bs);
            }

            std::vector<Workload2D*>* wc = new std::vector<Workload2D*>();
            wc->push_back(new TraceWorkload2D("traces/points/scalepoints50000trial1.dat", true));
//            wc->push_back(new TraceWorkload2D("traces/points/scalepoints50000trial2.dat", true));
//            wc->push_back(new TraceWorkload2D("traces/points/scalepoints50000trial3.dat", true));

            addWorkload(wc, "scalepoints50000", 0);

            //addWorkload(new TraceWorkload2D("traces/scalepoints90000.dat", true), 0);
        }

        SimpleGridBucketSizeLarge() {}
    };
}

#endif /* _SIMPLE_GRID_BUCKET_SIZE_LARGE_H_ */
