// Host-side unit test for Statistics.
#include <Statistics.h>

#include <stdio.h>

using OledDashboard::Statistics;

static int failures = 0;

#define CHECK(cond, msg)                                            \
    do {                                                            \
        if (!(cond)) {                                              \
            printf("FAIL: %s\n", msg);                              \
            ++failures;                                             \
        }                                                           \
    } while (0)

int main() {
    // Basic min/max/avg.
    {
        Statistics s(8);
        s.add(236);
        s.add(240);
        s.add(232);
        CHECK(s.count() == 3, "count == 3");
        CHECK(s.minimum() == 232, "min == 232");
        CHECK(s.maximum() == 240, "max == 240");
        CHECK(s.averageX10() == 2360, "avg x10 == 2360");
        CHECK(s.at(0) == 236, "at(0) oldest");
        CHECK(s.at(2) == 232, "at(2) newest");
        CHECK(s.newest() == 232, "newest");
        CHECK(s.oldest() == 236, "oldest");
    }

    // Ring buffer wrap + eviction keeps statistics correct.
    {
        Statistics s(4);
        for (int i = 0; i < 10; ++i) s.add(100 + i);  // 100..109
        CHECK(s.count() == 4, "wrap count == 4");
        CHECK(s.oldest() == 106, "wrap oldest == 106");
        CHECK(s.newest() == 109, "wrap newest == 109");
        CHECK(s.minimum() == 106, "wrap min == 106");
        CHECK(s.maximum() == 109, "wrap max == 109");
        CHECK(s.averageX10() == 1075, "wrap avg x10 == 1075");  // (106+107+108+109)=430
    }

    // Evicting the running min/max forces a rescan.
    {
        Statistics s(3);
        s.add(100);  // min
        s.add(200);  // max
        s.add(150);
        s.add(50);   // evicts 100; new min is 50
        CHECK(s.minimum() == 50, "rescan min == 50");
        s.add(500);  // evicts 200; new max is 500
        CHECK(s.maximum() == 500, "rescan max == 500");
        CHECK(s.count() == 3, "rescan count == 3");
    }

    // Negative values.
    {
        Statistics s(6);
        s.add(-123);
        s.add(-100);
        s.add(-150);
        CHECK(s.minimum() == -150, "neg min == -150");
        CHECK(s.maximum() == -100, "neg max == -100");
        CHECK(s.range() == 50, "neg range == 50");
    }

    // Capacity clamping.
    {
        Statistics s(0);
        CHECK(s.capacity() == Statistics::kMaxCapacity, "cap 0 -> max");
        Statistics s2(1000);
        CHECK(s2.capacity() == Statistics::kMaxCapacity, "cap 1000 -> max");
    }

    // setCapacity clears.
    {
        Statistics s(10);
        s.add(5);
        s.setCapacity(3);
        CHECK(s.count() == 0, "setCapacity clears");
        CHECK(s.capacity() == 3, "setCapacity size");
    }

    // Average rounding for negative values.
    {
        Statistics s(4);
        s.add(-10);
        s.add(-20);
        CHECK(s.averageX10() == -150, "neg avg x10 == -150");  // (-30/2)*10
    }

    if (failures == 0) {
        printf("ALL STATISTICS TESTS PASSED\n");
        return 0;
    }
    printf("%d FAILURES\n", failures);
    return 1;
}
