#include <getopt.h>
#include <unistd.h>

#include <AbcUtils.hpp>
#include <CircuitGenGenerator/CircuitGenGenerator.hpp>
#include <CircuitGenGenerator/ThreadPool.hpp>
#include <YosysUtils.hpp>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "GraphAbcFolder.hpp"

#define ELPP_DISABLE_LOGS
#include "lib/easyloggingpp/easylogging++.h"

#define ELPP_DISABLE_LOGS
INITIALIZE_EASYLOGGINGPP

using namespace std::chrono;
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

class CircuitBenchmark {
public:
    CircuitBenchmark(int argc, char** argv) : threadsNumber(1), pool(threadsNumber) {
        parseCommandLine(argc, argv);
    }

    void run() {
        auto start = high_resolution_clock::now();
        
        if (jsonPath.size()) {
            processJsonPath();
        } else {
            processFilePathOrDirectory();
        }

        pool.wait();

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        std::clog << "Time taken: " << duration.count() << " microseconds" << std::endl;
    }

private:
    std::string jsonPath, filePath, directoryPath;
    bool makeBalanced = false;
    bool makeResyn2 = false;
    bool toBench = false;
    bool toFirrtl = false;
    bool calcStats = false;
    bool moveToFolder = false;
    uint8_t threadsNumber = 1;
    std::string libName = "sky130.lib";
    const std::string defaultLibPath = "Generator/tech_libs/";
    Threading::ThreadPool pool;

    void parseCommandLine(int argc, char** argv) {
        const char* const short_opts = "ht:f:j:rbcd:mBFl:";
        const option long_opts[] = {
            {"json_path", required_argument, nullptr, 'j'},
            {"file_path", required_argument, nullptr, 'f'},
            {"threads", required_argument, nullptr, 't'},
            {"make_resyn2", no_argument, nullptr, 'r'},
            {"make_balance", no_argument, nullptr, 'b'},
            {"move", no_argument, nullptr, 'm'},
            {"directory", required_argument, nullptr, 'd'},
            {"to_bench", no_argument, nullptr, 'B'},
            {"to_firrtl", no_argument, nullptr, 'F'},
            {"help", no_argument, nullptr, 'h'},
            {"lib_name", required_argument, nullptr, 'l'}
        };

        int opt;
        while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1) {
            switch (opt) {
                case 'j':
                    jsonPath = optarg;
                    break;
                case 'f':
                    filePath = optarg;
                    break;
                case 'd':
                    directoryPath = optarg;
                    break;
                case 't':
                    threadsNumber = std::min(std::max(1, atoi(optarg)), 255);
                    break;
                case 'b':
                    makeBalanced = true;
                    break;
                case 'r':
                    makeResyn2 = true;
                    break;
                case 'm':
                    moveToFolder = true;
                    break;
                case 'B':
                    toBench = true;
                    break;
                case 'F':
                    toFirrtl = true;
                    break;
                case 'h':
                    displayHelp();
                    exit(0);
                case 'l':
                    libName = optarg;
                    break;
                case '?':
                    // Unknown option or missing argument
                    break;
                default:
                    std::cerr << "Unknown error while parsing command line arguments." << std::endl;
                    exit(1);
            }
        }

        if (!jsonPath.empty() && !filePath.empty() && !directoryPath.empty()) {
            std::cerr << "Not allowed to use at the same time path to verilog file and to json." << std::endl;
            exit(1);
        }
    }

    void displayHelp() {
        std::cout << "---------------------------------------------------" << std::endl;
        std::cout << "CircuitGenGenerator util. Provides possibility to generate verilog circuits." << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
        std::cout << "Options: " << std::endl;
        std::cout << "\t-j <path> or --json_path <path>: Used for setting path to json file with circuit parameters." << std::endl;
        std::cout << "\t-f <path> or --file_path <path>: Used for setting path to verilog file." << std::endl;
        std::cout << "\t-d <directory> or --directory <directory>: Used for setting directory for many verilog files." << std::endl;
        std::cout << "\t-m or --move: Move file to specified directory." << std::endl;
        std::cout << "\t-b or --make_balance: Optimizes files." << std::endl;
        std::cout << "\t-r or --make_resyn2: Optimizes files with resyn2." << std::endl;
        std::cout << "\t-B or --to_bench: Converts to Bench format." << std::endl;
        std::cout << "\t-F or --to_firrtl: Converts to Firrtl format." << std::endl;
        std::cout << "\t-l <lib_name> or --lib_name <lib_name>: Sets lib for Berkeley-abc." << std::endl;
        std::cout << "\t-t <threads number>: Sets number of threads, more than 0 and less than 256." << std::endl;
        std::cout << "\t-h or --help: Shows all commands being used." << std::endl;
    }

    void initThreadPool() {
        Threading::ThreadPool pool(threadsNumber);
    }

    void processJsonPath() {
        auto allRes = CircuitGenGenerator::runGenerationFromJsonForPath(jsonPath);
        if (!(makeResyn2 || makeBalanced || toBench || toFirrtl || calcStats)) {
            return;
        }
        for (const auto& [main_path, allGraphs] : allRes) {
            for (const auto& graph : allGraphs) {
                std::string path = main_path + "/" + graph;
                runInMultithread(path, graph);
            }
        }
    }

    void processFilePathOrDirectory() {
        if (filePath.size()) {
            std::string graph = std::filesystem::path(filePath).stem();
            std::string path = std::filesystem::path(filePath).parent_path();
            runInMultithread(path, graph);
        } else {
            for (const auto& dirEntry : recursive_directory_iterator(directoryPath)) {
                if (!dirEntry.is_directory()) {
                    const auto file = std::filesystem::canonical(dirEntry);
                    auto filePath = file;
                    if (file.extension() == ".v") {
                        if (moveToFolder) {
                            std::filesystem::create_directory(file.parent_path() / file.stem());
                            filePath = file.parent_path() / file.stem() / file.filename();
                            std::filesystem::rename(file, filePath);
                        }
                        runInMultithread(filePath.parent_path(), filePath.stem());
                    }
                }
            }
        }
    }

    void runInMultithread(std::string path, std::string graph) {
        auto ptr = std::make_shared<GraphAbcFolder>(path, graph, libName, defaultLibPath, (int)makeBalanced + (int)makeResyn2 + 1);
        pool.submit([ptr]() { ptr->callGetStats(); });
        if (makeBalanced) {
            pool.submit([ptr]() { ptr->callOptimize(); });
        }
        if (makeResyn2) {
            pool.submit([ptr]() { ptr->callResyn2(); });
        }
        if (toBench) {
            pool.submit([ptr]() { ptr->callToBench(); });
        }
        if (toFirrtl) {
            pool.submit([ptr]() { ptr->callToFirrtl(); });
        }
    }
};

int main(int argc, char** argv) {
    CircuitBenchmark benchmark(argc, argv);
    benchmark.run();
    return 0;
}
