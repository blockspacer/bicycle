#include <iostream>
#include "bicycle.h"
#include "parameters.h"

namespace {
    const double fs = 200; // sample rate [Hz]
    const double dt = 1.0/fs; // sample time [s]
    const double v0 = 4.0; // forward speed [m/s]

    void print_parameters(const model::Bicycle& bicycle) {
        std::cout << "M:\n" << bicycle.M() << std::endl;
        std::cout << "C1:\n" << bicycle.C1() << std::endl;
        std::cout << "K0:\n" << bicycle.K0() << std::endl;
        std::cout << "K2:\n" << bicycle.K2() << std::endl;
        std::cout << "wheelbase: " << bicycle.wheelbase() << std::endl;
        std::cout << "trail: " << bicycle.trail() << std::endl;
        std::cout << "steer axis tilt: " << bicycle.steer_axis_tilt() << std::endl;
    }
} // namespace

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <parameter_file>\n";
        std::cerr << "\nConstruct a Bicycle object from file and print bicycle parameters.\n";
        return EXIT_FAILURE;
    }

    model::Bicycle bicycle1(parameters::benchmark::M, parameters::benchmark::C1,
            parameters::benchmark::K0, parameters::benchmark::K2,
            parameters::benchmark::wheelbase,
            parameters::benchmark::trail,
            parameters::benchmark::steer_axis_tilt,
            v0, dt);
    std::cout << "constructed bicycle from predefined parameters:\n";
    print_parameters(bicycle1);
    std::cout << "\n";

    model::Bicycle bicycle2(argv[1], v0, dt);
    std::cout << "constructed bicycle from input file: " << argv[1] << "\n";
    print_parameters(bicycle2);
    std::cout << "\n";

    return EXIT_SUCCESS;
}
