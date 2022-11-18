/*
 * SGrid.h
 * Created on: Sep 4, 2012
 *
 * Darius Sidlauskas
 * Copyright (c) 2012 MADALGO, Aarhus University
 *
 *  Single-threaded variant of uniform grid index (SGrid) with modifications
 *  as described in
 *      Darius Šidlauskas and Christian S. Jensen
 *      "Spatial joins in main memory: Implementation matters!"
 *      Proceedings of the VLDB Endowment 8, no. 1 (2014): 97-100.
 *
 */

#ifndef INCLUDE_2D_POINT_INDEX_SGRID_H_
#define INCLUDE_2D_POINT_INDEX_SGRID_H_

#include <tr1/unordered_map>

#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

#include "2D/StaticSpatialIndex2D.h"
#include "util/mem_align.h"

//------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------
#define SGRID_DFLT_CPS 64 // originally was 13 (default number of cells per side)
#define SGRID_DFLT_BS 30 // originally was 4 (default bucket size)
#define DEFAULT_SPACE_SIZE 22361 // size of one space dimension
// Max side of one space dimension:
#define MAXIMUM_SPACE_SIZE 30000 // Use 224000 with medium workload
// (used with scaling experiments)

namespace PSimIndex {
template<typename U, typename K, typename A>
class SGrid: public StaticSpatialIndex2D<U, K, A> {
  //------------------------------------------------------------------------
  // Class Declarations
  //------------------------------------------------------------------------
private:
  typedef Region2D<typename K::KeyType0, typename K::KeyType1> RType;

  // sizeof(Entry) = 8
  struct Entry {
    U* data;
  };

  // sizeof(Bucket) = 24
  class Bucket {
  private:
    Entry* entries_;
    Bucket* next_bucket_;

  public:
    typedef Region2D<typename K::KeyType0, typename K::KeyType1> RType;

    int num_points_; // currently in a bucket

    explicit Bucket(const uint32_t bs);
    Bucket(Bucket* bucketNext, const uint32_t bs);
    ~Bucket() {
    }
    void enumerateOverRegion(IndexCallBack<U>* cb, RType* r);
    // add all points to the callback
    // because the grid is fully covered in the region
    void addAllPoints(IndexCallBack<U>* cb);
    // recursively delete the list.
    // if the next bucket is non-empty, recursively delete it first
    // and then delete the list.
    void deleteRec();

    void insert(U* data);
  };

  // sizeof(GridCell) = 8
  class GridCell {
  public:
    Bucket* bckt_head_;

    GridCell();
    ~GridCell() {
    }

//    void insert(U* data);
    void clear();
    void deleteRec();
    void enumerateOverRegion(IndexCallBack<U>* cb, RType* r);
    void addAllPoints(IndexCallBack<U>* cb);
  };

  //------------------------------------------------------------------------
  // SGrid's Private Members
  //------------------------------------------------------------------------
private:
  GridCell* cells_;
  const int space_side_;
  const int cps_; // cells per side
  const int bs_; // bucket size
  const double cell_size_;

  inline GridCell* CellAt(const int x, const int y) {
    return &cells_[x * cps_ + y];
  }

  //------------------------------------------------------------------------
  // Public Members
  //------------------------------------------------------------------------

public:
  static const char* getClassName() {
    return "SGrid";
  }
  const char* getInstanceName() {
    return "SGrid";
  }

  SGrid();

  SGrid(int cells_per_side, uint32_t bucket_size, int space_side);

  /*
   * Parameters:
   *    bs - bucket size
   *    cps - #cells per side on default space (22361 x 22361)
   */
  SGrid(const uint32_t cps, const uint32_t bs);

  ~SGrid();
  void startTick(std::vector<U*>* agents);
  void insert(GridCell* cell, U* data);
  void enumerateOverRegion(IndexCallBack<U>* cb, RType* r);
  void endTick(bool force = false);

  // Unimplemented Methods.
  void aggregateOverRegion(A* aggs, RType* r) {
  }
  void nearestK(IndexCallBack<U>* cb, U* u, int k) {
  }
  void afterUpdateAgg(U* u) {
  }
  void beforeDelete(U* u) {
  }
  void afterInsert(U* u) {
  }
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// Implementation Starts Here
//------------------------------------------------------------------------
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Bucket Methods
//------------------------------------------------------------------------

//template<typename U, typename K, typename A>
//uint32_t SGrid<U, K, A>::Bucket::_bucket_size = DEFAULT_BUCKET_SIZE;

template<typename U, typename K, typename A>
SGrid<U, K, A>::Bucket::Bucket(const uint32_t bs) {
  next_bucket_ = NULL;
  num_points_ = 0;
  entries_ = reinterpret_cast<Entry*>(aligned_calloc(sizeof(Entry) * bs));
}

template<typename U, typename K, typename A>
SGrid<U, K, A>::Bucket::Bucket(Bucket* bucketNext, const uint32_t bs) {
  next_bucket_ = bucketNext;
  num_points_ = 0;
  entries_ = reinterpret_cast<Entry*>(aligned_calloc(sizeof(Entry) * bs));
}

template<typename U, typename K, typename A>
void SGrid<U, K, A>::Bucket::enumerateOverRegion(IndexCallBack<U>* cb,
    RType* r) {
  Entry* entry = entries_;

  // drs: version 1.1
  for (int i = 0; i < num_points_; ++i) {
    if (r->template containsPoint<U, K>((entry + i)->data)) {
      LOG(DEBUG) << "[SGrid enumOverRegion] reporting: " << *((entry + i)->data)
          << std::endl;
      cb->reportPoint((entry + i)->data);
    }
  }

  if (next_bucket_ != NULL) {
    next_bucket_->enumerateOverRegion(cb, r);
  }
}

template<typename U, typename K, typename A>
void SGrid<U, K, A>::Bucket::addAllPoints(IndexCallBack<U>* cb) {
  for (int i = 0; i < num_points_; ++i) {
    cb->reportPoint((entries_ + i)->data);
  }

  if (next_bucket_ != NULL) {
    next_bucket_->addAllPoints(cb);
  }
}

template<typename U, typename K, typename A>
void SGrid<U, K, A>::Bucket::deleteRec() {
  if (next_bucket_ != NULL) {
    next_bucket_->deleteRec();
    delete next_bucket_;
    next_bucket_ = NULL;
  }

  free(entries_);
  entries_ = NULL;
}

template<typename U, typename K, typename A>
void SGrid<U, K, A>::Bucket::insert(U* data) {
  (entries_ + num_points_++)->data = data;
}

//------------------------------------------------------------------------
// GridCell Methods
//------------------------------------------------------------------------

template<typename U, typename K, typename A>
SGrid<U, K, A>::GridCell::GridCell() {
  bckt_head_ = NULL;
}

template<typename U, typename K, typename A>
void SGrid<U, K, A>::GridCell::clear() {
  deleteRec();
  bckt_head_ = NULL;
}

template<typename U, typename K, typename A>
void SGrid<U, K, A>::GridCell::deleteRec() {
  if (bckt_head_ != NULL) {
    bckt_head_->deleteRec();
    delete bckt_head_;
    bckt_head_ = NULL;
  }
}

template<typename U, typename K, typename A>
void SGrid<U, K, A>::GridCell::enumerateOverRegion(IndexCallBack<U>* cb,
    RType* r) {
  if (bckt_head_ != NULL) {
    bckt_head_->enumerateOverRegion(cb, r);
  }
}

template<typename U, typename K, typename A>
void SGrid<U, K, A>::GridCell::addAllPoints(IndexCallBack<U>* cb) {
  if (bckt_head_ != NULL) {
    bckt_head_->addAllPoints(cb);
  }
}

//------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------

template<typename U, typename K, typename A>
SGrid<U, K, A>::SGrid() :
    space_side_(DEFAULT_SPACE_SIZE), cps_(SGRID_DFLT_CPS), bs_(SGRID_DFLT_BS), cell_size_(
        space_side_ * 1.0 / cps_) {

  cells_ = reinterpret_cast<GridCell*>(AlignedMem::allocateClear(
      (sizeof(GridCell) * this->cps_ * this->cps_), 64));

  for (int i = 0; i < this->cps_; i++) {
    for (int j = 0; j < this->cps_; j++) {
      CellAt(i, j)->clear();
    }
  }
}

template<typename U, typename K, typename A>
SGrid<U, K, A>::SGrid(const uint32_t cps, const uint32_t bs) :
    space_side_(MAXIMUM_SPACE_SIZE), cps_(cps), bs_(bs), cell_size_(
        space_side_ * 1.0 / cps_) {

  cells_ = reinterpret_cast<GridCell*>(AlignedMem::allocateClear(
      (sizeof(GridCell) * cps_ * cps_), 64));

//  for (int i = 0; i < this->cps_; i++) {
//    for (int j = 0; j < this->cps_; j++) {
//      CellAt(i, j)->clear();
//    }
//  }

//  printf("sizeof(GridCell)=%lu\n", sizeof(GridCell));
//  printf("sizeof(BucketSize)=%lu\n", sizeof(Bucket));
//  printf("sizeof(Entry)=%lu\n", sizeof(Entry));
//  printf("sizeof(Data)=%lu\n", sizeof(U));
//  printf("Grid: %dx%d, bucket_size=%d, cell_size=%0.2f\n", cps_, cps_, bs_,
//      cell_size_);
}

template<typename U, typename K, typename A>
SGrid<U, K, A>::SGrid(int cells_per_side, uint32_t bucket_size, int space_side) :
    bs_(bucket_size), space_side_(space_side), cps_(cells_per_side), cell_size_(
        space_side_ * 1.0 / cps_) {

  cells_ = reinterpret_cast<GridCell*>(AlignedMem::allocateClear(
      (sizeof(GridCell) * this->cps_ * this->cps_), 64));

  for (int i = 0; i < cps_; i++) {
    for (int j = 0; j < cps_; j++) {
      CellAt(i, j)->clear();
    }
  }
}

template<typename U, typename K, typename A>
SGrid<U, K, A>::~SGrid() {
  for (int i = 0; i < cps_; i++) {
    for (int j = 0; j < cps_; j++) {
      CellAt(i, j)->deleteRec();
    }
  }
  AlignedMem::deallocate(cells_);
}

template<typename U, typename K, typename A>
void SGrid<U, K, A>::startTick(std::vector<U*>* agents) {
  typename std::vector<U*>::iterator it;

  for (int i = 0; i < agents->size(); ++i) {
    int idx_x = static_cast<int>(K::getKey0((*agents)[i]) / cell_size_);
    int idx_y = static_cast<int>(K::getKey1((*agents)[i]) / cell_size_);

    assert(0 <= idx_x && idx_x < cps_);
    assert(0 <= idx_y && idx_y < cps_);
//    getCell(idx_x, idx_y)->insert(agents->at(i));
    insert( CellAt(idx_x, idx_y), agents->data()[i] );
  }
}

template<typename U, typename K, typename A>
void SGrid<U, K, A>::insert(GridCell* cell, U* data) {
  if (cell->bckt_head_ == NULL || cell->bckt_head_->num_points_ == bs_) {
    Bucket* newHead = new Bucket(cell->bckt_head_, bs_);
    cell->bckt_head_ = newHead;
    assert( cell->bckt_head_->num_points_ == 0);
  }
  cell->bckt_head_->insert(data);
}

template<typename U, typename K, typename A>
void SGrid<U, K, A>::enumerateOverRegion(IndexCallBack<U>* cb, RType* r) {
  const int cellxmin = std::max(static_cast<int>(r->getLow0() / cell_size_), 0);
  const int cellymin = std::max(static_cast<int>(r->getLow1() / cell_size_), 0);
  const int cellxmax = std::min(static_cast<int>(r->getHigh0() / cell_size_), cps_ - 1);
  const int cellymax = std::min(static_cast<int>(r->getHigh1() / cell_size_), cps_ - 1);

  // (1) Cycle through all not fully covered grid cells_
  // (1a) go left on cellymax
  for (int i = cellxmin; i <= cellxmax; i++)
    CellAt(i, cellymax)->enumerateOverRegion(cb, r);
  // (1b) go left on cellymin if they are different
  if ( cellymax != cellymin )
    for (int i = cellxmin; i <= cellxmax; i++)
      CellAt(i, cellymin)->enumerateOverRegion(cb, r);

  // (1c) go up on cellxmax
  for (int j = cellymin + 1; j < cellymax; j++)
    CellAt(cellxmax, j)->enumerateOverRegion(cb, r);
  // (1d) go up on cellxmin they are different
  if ( cellxmax != cellxmin )
    for (int j = cellymin + 1; j < cellymax; j++)
      CellAt(cellxmin, j)->enumerateOverRegion(cb, r);

  // (2) Cycle through all fully covered grid cells
  for (int i = cellxmin + 1; i < cellxmax; i++) {
    for (int j = cellymin + 1; j < cellymax; j++) {
      CellAt(i, j)->addAllPoints(cb);
    }
  }
}

/*
 * Original Querying
 */
//template<typename U, typename K, typename A>
//void SGrid<U, K, A>::enumerateOverRegion(IndexCallBack<U>* cb, RType* r) {
//  for (int i = 0; i < cps_; ++i) {
//    for (int j = 0; j < cps_; ++j) {
//
//      //TODO: Avoid allocation here.
//      RType* rect = new RType(cell_size_ * i, cell_size_ * (i + 1),
//          cell_size_ * j, cell_size_ * (j + 1));
//
//      if (r->contains(rect)) {
//        // fully covered
//        CellAt(i, j)->addAllPoints(cb);
//      } else if (r->intersects(rect)) {
//        CellAt(i, j)->enumerateOverRegion(cb, r);
//      }
//      delete rect;
//      DBUTL_ASSERT(CellAt(i, j) != NULL);
//    }
//  }
//}
template<typename U, typename K, typename A>
void SGrid<U, K, A>::endTick(bool force) {
  for (int i = 0; i < cps_; i++) {
    for (int j = 0; j < cps_; j++) {
      CellAt(i, j)->clear();
    }
  }
}
}

#endif  // INCLUDE_2D_POINT_INDEX_SGRID_H_
