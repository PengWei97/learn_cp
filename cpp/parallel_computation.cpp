#include <iostream>
#include <mpi.h>
#include <gperftools/profiler.h>
#include <chrono>
#include <random>
#include <cmath>
#include <csignal>

volatile std::sig_atomic_t signal_received = 0;

void signalHandler(int signal) {
    signal_received = 1;
}

void performHeavyComputation() {
    std::mt19937_64 rng;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double result = 0.0;

    while (!signal_received) { //
        for (int i = 0; i < 1000000; ++i) {
            double num = dist(rng); // 生成随机数
            result += std::sin(num) * std::cos(num) * std::tan(num); // 复杂的三角运算
        }
    }

    std::cout << "Computation interrupted. Final result: " << result << std::endl;
}

int main(int argc, char** argv) {
    std::signal(SIGINT, signalHandler);
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    std::string profileName = "profile_" + std::to_string(world_rank) + ".prof";
    ProfilerStart(profileName.c_str());

    std::cout << "Hello from process " << world_rank << " out of " << world_size << " processes.\n";

    if (!signal_received) {
        performHeavyComputation();
    }

    ProfilerStop();
    MPI_Finalize();

    if (signal_received) {
        std::cout << "Process " << world_rank << " exiting cleanly after signal.\n";
    }

    return 0;
}
