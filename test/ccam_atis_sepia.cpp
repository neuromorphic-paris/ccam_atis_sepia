#include "../source/ccam_atis_sepia.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    try {
        sepia::capture_exception capture_exception;
        auto camera = ccam_atis_sepia::make_camera(
            sepia::make_split<sepia::type::atis>(
                sepia::make_split<sepia::type::dvs>(
                    [](sepia::simple_event simple_event) {
                        std::cout << "+";
                        std::cout.flush();
                    },
                    [](sepia::simple_event simple_event) {
                        std::cout << "-";
                        std::cout.flush();
                    }),
                [](sepia::threshold_crossing threshold_crossing) {
                    if (threshold_crossing.is_second) {
                        std::cout << '/';
                    } else {
                        std::cout << '\\';
                    }
                    std::cout.flush();
                }),
            std::ref(capture_exception));
        capture_exception.wait();
        capture_exception.rethrow_unless<>();
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return 1;
    }
    return 0;
}
