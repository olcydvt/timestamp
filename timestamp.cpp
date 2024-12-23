#include <iostream>
#include <type_traits>
#include <filesystem>
#include <set>
#include <time.h>
#include <thread>
#include <stdlib.h>   // for putenv
#include <chrono>
class timestamp {

    int64_t _value;

public:
    timestamp() = default;

     // TODO: REMOVE
    // deprecated
    inline timestamp(int64_t value) {
        _value = value;
    }

    inline timestamp(const std::chrono::microseconds& v) {
        _value = v.count();
    }

    inline timestamp(const std::chrono::milliseconds& v) {
        _value = std::chrono::duration_cast<std::chrono::microseconds>(v).count();
    }

    inline timestamp(const std::chrono::seconds& v) {
        _value = std::chrono::duration_cast<std::chrono::microseconds>(v).count();
    }

    inline timestamp(const std::chrono::sys_time<std::chrono::microseconds>& v) {
        _value = v.time_since_epoch().count();
    }

    template <typename Duration>
    inline timestamp(const std::chrono::sys_time<Duration>& v) {
        _value = std::chrono::duration_cast<std::chrono::microseconds>(v.time_since_epoch()).count();
    }

    inline int64_t raw_value() const {
        return _value;
    }

    inline std::chrono::microseconds to_microseconds() const {
        return std::chrono::microseconds{ _value };
    }

    inline std::chrono::milliseconds to_milliseconds() const {
        return std::chrono::floor<std::chrono::milliseconds>(to_microseconds());
    }

    inline std::chrono::seconds to_seconds() const {
        return std::chrono::floor<std::chrono::seconds>(to_microseconds());
    }

    inline std::chrono::sys_time<std::chrono::microseconds> to_sys_microseconds() const {
        return std::chrono::sys_time<std::chrono::microseconds>{to_microseconds()};
    }

    inline std::chrono::sys_time<std::chrono::milliseconds> to_sys_milliseconds() const {
        return std::chrono::sys_time<std::chrono::milliseconds>{std::chrono::floor<std::chrono::milliseconds>(to_microseconds())};
    }

    inline std::chrono::sys_time<std::chrono::seconds> to_sys_seconds() const {
        return std::chrono::sys_time<std::chrono::seconds>{std::chrono::floor<std::chrono::seconds>(to_microseconds())};
    }

    inline operator std::chrono::microseconds() const {
        return to_microseconds();
    }

    inline operator std::chrono::milliseconds() const {
        return to_milliseconds();
    }

    inline operator std::chrono::seconds() const {
        return to_seconds();
    }

    inline operator std::chrono::sys_time<std::chrono::microseconds>() const {
        return to_sys_microseconds();
    }

    inline operator std::chrono::sys_time<std::chrono::milliseconds>() const {
        return std::chrono::sys_time<std::chrono::milliseconds>{std::chrono::floor<std::chrono::milliseconds>(std::chrono::microseconds{ _value })};
    }

    inline operator std::chrono::sys_time<std::chrono::seconds>() const {
        return std::chrono::sys_time<std::chrono::seconds>{std::chrono::floor<std::chrono::seconds>(std::chrono::microseconds{ _value })};
    }

    enum class time_mode : int {
        sec = 0,
        min = 1,
        hour = 2
    };

        template<time_mode mode>
        inline auto time_diff(const timestamp& other) const {

            std::chrono::system_clock::time_point other_time{ std::chrono::microseconds{other.raw_value()} };
            std::chrono::system_clock::time_point this_time{ std::chrono::microseconds{this->raw_value()} };

            if constexpr (mode == time_mode::sec) {
                auto other_sec = std::chrono::time_point_cast<std::chrono::seconds>(other_time);
                auto this_sec = std::chrono::time_point_cast<std::chrono::seconds>(this_time);
                std::chrono::duration elapsed_sec = this_sec - other_sec;
                return elapsed_sec.count();
            }
            else if constexpr (mode == time_mode::min) {
                auto other_min = std::chrono::time_point_cast<std::chrono::minutes>(other_time);
                auto this_min = std::chrono::time_point_cast<std::chrono::minutes>(this_time);
                std::chrono::duration elapsed_min = this_min - other_min;
                return elapsed_min.count();

            }
            else if constexpr (mode == time_mode::hour) {
                auto other_hour = std::chrono::time_point_cast<std::chrono::hours>(other_time);
                auto this_hour = std::chrono::time_point_cast<std::chrono::hours>(this_time);
                std::chrono::duration elapsed_hours = this_hour - other_hour;
                return elapsed_hours.count();
            }
        }

    inline friend bool operator==(timestamp t1, timestamp t2) = default;

    inline friend auto operator<=>(timestamp t1, timestamp t2) = default;
};




int main() {
    const auto tomorrow = std::chrono::time_point_cast<std::chrono::minutes>(std::chrono::ceil<std::chrono::days>(std::chrono::system_clock::now()));
    const auto today = std::chrono::time_point_cast<std::chrono::minutes>(std::chrono::system_clock::now());
    const std::chrono::duration<int, std::ratio<60>> elapsed_min = tomorrow - today;
    const int remaining_minutes_until_midnight = elapsed_min.count();
    return 0;
}

/* std::this_thread::sleep_for(30s);
const auto t2 = std::chrono::high_resolution_clock::now();
const std::chrono::duration<double, std::milli> fp_ms = t2 - t1;


   auto now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm ptm;
    localtime_s(&ptm, &now_c);
    int cur_minute = ptm.tm_min;

    now_c = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>(last_update_time.to_seconds()));
    localtime_s(&ptm, &now_c);
    int update_minute = ptm.tm_min;
*/
