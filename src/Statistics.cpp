#include "Statistics.h"

namespace OledDashboard {

Statistics::Statistics(uint16_t capacity)
    : capacity_(capacity), count_(0), head_(0), min_(0), max_(0), sum_(0) {
    if (capacity_ == 0 || capacity_ > kMaxCapacity) {
        capacity_ = kMaxCapacity;
    }
}

void Statistics::add(int16_t sample) {
    if (count_ < capacity_) {
        buf_[head_] = sample;
        ++count_;
    } else {
        // Buffer is full: the slot at head_ holds the oldest sample.
        const int16_t evicted = buf_[head_];
        sum_ -= evicted;
        buf_[head_] = sample;  // overwrite before any rescan
        if (evicted == min_ || evicted == max_) {
            // The removed sample may have been the running min/max; rescan
            // the (already updated) buffer.
            min_ = 32767;
            max_ = -32768;
            for (uint16_t i = 0; i < count_; ++i) {
                const int16_t v = buf_[i];
                if (v < min_) min_ = v;
                if (v > max_) max_ = v;
            }
        }
    }

    sum_ += sample;
    if (count_ == 1) {
        min_ = sample;
        max_ = sample;
    } else {
        if (sample < min_) min_ = sample;
        if (sample > max_) max_ = sample;
    }

    head_ = (head_ + 1) % capacity_;
}

void Statistics::clear() {
    count_ = 0;
    head_ = 0;
    min_ = 0;
    max_ = 0;
    sum_ = 0;
}

void Statistics::setCapacity(uint16_t capacity) {
    if (capacity == 0 || capacity > kMaxCapacity) capacity = kMaxCapacity;
    capacity_ = capacity;
    clear();
}

int16_t Statistics::at(uint16_t idx) const {
    // idx 0 == oldest. Ring slot = head - count + idx (mod capacity).
    if (idx >= count_) return 0;
    return buf_[(head_ + capacity_ + idx - count_) % capacity_];
}

int32_t Statistics::averageX10() const {
    if (count_ == 0) return 0;
    return (sum_ * 10) / count_;
}

}  // namespace OledDashboard
