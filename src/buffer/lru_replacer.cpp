//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) : size_{num_pages} {}

LRUReplacer::~LRUReplacer() = default;

auto LRUReplacer::Victim(frame_id_t *frame_id) -> bool {
  if (this->size_ <= 0) {
    return false;
  }
  *frame_id = lru_list_.back();
  lru_list_.pop_back();
  lru_map_.erase(*frame_id);
  return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
  auto iter = lru_map_.find(frame_id);

  if (iter != lru_map_.end()) {
    lru_list_.erase(lru_map_[frame_id]);
    lru_map_.erase(frame_id);
  }
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  auto iter = lru_map_.find(frame_id);
  if (iter == lru_map_.end()) {
    lru_list_.push_front(frame_id);
    lru_map_[frame_id] = lru_list_.begin();
    if (lru_map_.size() > this->size_) {
      frame_id_t ret = lru_list_.back();
      lru_list_.pop_back();
      lru_map_.erase(ret);
    }
  }
}

auto LRUReplacer::Size() -> size_t { return lru_map_.size(); }

}  // namespace bustub
