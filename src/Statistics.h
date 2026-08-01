/**
 * @file Statistics.h
 * @brief Fixed-capacity ring buffer of int16_t samples with running statistics.
 *
 * Historical data is never stored as float. Values are scaled by ten
 * (e.g. 23.6 deg C is stored as 236) and kept as int16_t, which cuts RAM and
 * Flash dramatically while keeping 0.1 resolution for display.
 *
 * The class tracks minimum, maximum and sum incrementally so statistics are
 * O(1) per sample; only a full buffer ever triggers an O(n) min/max rescan.
 */

#ifndef OLED_DASHBOARD_STATISTICS_H
#define OLED_DASHBOARD_STATISTICS_H

#include <stdint.h>

#include "Layout.h"

namespace OledDashboard {

/**
 * @brief A compact, allocation-free sample history with running statistics.
 */
class Statistics {
public:
    /// Hard upper bound of the ring buffer (compile-time, in Flash only).
    static constexpr uint16_t kMaxCapacity = Layout::kDefaultHistory;  // 128

    /**
     * @brief Construct a Statistics buffer.
     * @param capacity  Number of samples to retain (1..kMaxCapacity).
     */
    explicit Statistics(uint16_t capacity = Layout::kDefaultHistory);

    /// Append one sample, evicting the oldest when full.
    void add(int16_t sample);

    /// Forget all samples.
    void clear();

    /// Resize the retained history length (1..kMaxCapacity); clears data.
    void setCapacity(uint16_t capacity);

    /// Maximum number of retained samples.
    uint16_t capacity() const { return capacity_; }

    /// Number of samples currently retained (0..capacity()).
    uint16_t count() const { return count_; }

    /// True once the buffer holds @ref capacity() samples.
    bool isFull() const { return count_ == capacity_; }

    /// True when no samples have been added yet.
    bool isEmpty() const { return count_ == 0; }

    /**
     * @brief Sample by age.
     * @param idx  0 is the oldest retained sample,
     *             count()-1 is the newest.
     */
    int16_t at(uint16_t idx) const;

    /// The oldest retained sample.
    int16_t oldest() const { return at(0); }

    /// The newest (most recently added) sample.
    int16_t newest() const { return at(count_ - 1); }

    /// Smallest retained sample (no samples -> 0).
    int16_t minimum() const { return count_ ? min_ : 0; }

    /// Largest retained sample (no samples -> 0).
    int16_t maximum() const { return count_ ? max_ : 0; }

    /// Difference between maximum and minimum (never negative).
    int16_t range() const { return maximum() - minimum(); }

    /**
     * @brief Running average, scaled by ten.
     *
     * Samples are already x10, so the true average is averageX10()/10.
     * The extra factor of ten keeps 0.1 resolution through the integer math.
     */
    int32_t averageX10() const;

private:
    int16_t buf_[kMaxCapacity];
    uint16_t capacity_;
    uint16_t count_;
    uint16_t head_;   ///< Index where the next sample will be written.
    int16_t min_;
    int16_t max_;
    int32_t sum_;
};

}  // namespace OledDashboard

#endif  // OLED_DASHBOARD_STATISTICS_H
