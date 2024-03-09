#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double get_duration() {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;
        return duration.count();
    }

    double get_duration_ms() {
        return get_duration() * 1000;
    }


    double stop() {
        return get_duration();
    }

    double getTime() {
        return get_duration();
    }
};
