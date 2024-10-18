#include <getopt.h>
#include <unistd.h>

#include <AbcUtils.hpp>
#include <CircuitGenGenerator/CircuitGenGenerator.hpp>
#include <CircuitGenGenerator/ThreadPool.hpp>
#include <YosysUtils.hpp>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "GraphAbcFolder.hpp"

#define ELPP_DISABLE_LOGS
#include "lib/easyloggingpp/easylogging++.h"

#define ELPP_DISABLE_LOGS
INITIALIZE_EASYLOGGINGPP

using namespace std::chrono;
using recursive_directory_iterator =
    std::filesystem::recursive_directory_iterator;

void initLogging(std::string testName, std::string subtestName) {
  el::Configurations conf("easylogging++.conf");
  el::Loggers::reconfigureAllLoggers(conf);
  el::Loggers::reconfigureAllLoggers(
      el::ConfigurationType::Filename,
      "logs/" + testName + "/" + subtestName + ".log");
  el::Loggers::addFlag(el::LoggingFlag::NewLineForContainer);
  el::Loggers::addFlag(el::LoggingFlag::LogDetailedCrashReason);
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
}

int main(int argc, char** argv) {
  // initLogging("main", "main");
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
  // Use getopt to parse command line arguments

  const char* const short_opts = "ht:f:j:rbcd:mBFl:";
  const option long_opts[] = {{"json_path", required_argument, nullptr, 'j'},
                              {"file_path", required_argument, nullptr, 'f'},
                              {"threads", required_argument, nullptr, 't'},
                              {"make_resyn2", no_argument, nullptr, 'r'},
                              {"make_balance", no_argument, nullptr, 'b'},
                              {"move", no_argument, nullptr, 'm'},
                              {"directory", required_argument, nullptr, 'd'},
                              {"to_bench", no_argument, nullptr, 'B'},
                              {"to_firrtl", no_argument, nullptr, 'F'},
                              {"help", no_argument, nullptr, 'h'},
                              {"lib_name", required_argument, nullptr, 'l'}};
  int c;

  int opt;
  int32_t sub;

  while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) !=
         -1) {
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
        sub = atoi(optarg);
        if (sub <= 0) {
          std::cerr << "\tThreads number must be more than 0!" << std::endl;
          return 1;
        }
        if (sub > 255) {
          std::cerr << "\tThreads number must be less (or equal) than 255!"
                    << std::endl;
          return 1;
        }
        threadsNumber = sub;
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
      case 'c':
        calcStats = true;
        break;
      case 'h':
        std::cout << "---------------------------------------------------"
                  << std::endl;
        std::cout << "CircuitGenGenerator util.\nProvides possibility to "
                     "generate verolog circuits."
                  << std::endl;
        std::cout << "---------------------------------------------------"
                  << std::endl;

        std::cout << "Options: " << std::endl;
        std::cout
            << "\t-j <path> or --json_path <path>:\n\t\tUsed for setting path "
               "to json file with circuit parameters."
            << std::endl;
        std::cout
            << "\t-f <path> or --file_path <path>:\n\t\tUsed for setting path "
               "to verilog file, should be proccessed by Berkeley-abc."
            << std::endl;
        std::cout
            << "\t-d <directory> or --directory <directory>:\n\t\tUsed for "
               "setting dir "
               "to many verilog files, should be proccessed by Berkeley-abc."
            << std::endl;
        std::cout
            << "\t-m or --move:\n\t\tMove file to directory, where would be "
               "moved verilog file \n\t\tand where would be located created by "
               "abc files. \n\t\tUsage only with flag -d, default false"
            << std::endl;
        std::cout
            << "\t-b or --make_balance:\n\t\tRuns Berkeley-abc to optimize "
               "generated files with\n\t\tbasic balance commands."
            << std::endl;
        std::cout
            << "\t-r or --make_resyn2:\n\t\tRuns Berkeley-abc to optimize "
               "generated files with\n\t\toptimization algorythm named resyn2."
            << std::endl;
        std::cout
            << "\t-B or --to_bench:\n\t\tRuns Berkeley-abc to convert Verilog "
               "file to Bench format."
            << std::endl;
        std::cout << "\t-F or --to_firrtl:\n\t\tRuns Yosys to convert Verilog "
                     "file to Firrtl format."
                  << std::endl;
        std::cout
            << "\t-l <lib_name> or --lib_name <lib_name>:\n\t\tSets lib, used "
               "by Berkeley-abc (default sky130.lib).\n\t\tFile should be "
               "located in the folder tech_libs."
            << std::endl;
        std::cout
            << "\t-t <threads number> or --threads <threads number>:\n\t\tSets "
               "number of threads, used for running abc.\n\t\tNumber of "
               "threads should be more than 0 and less than 256."
            << std::endl;
        std::cout << "\t-h or --help:\n\t\tShows all commands being used."
                  << std::endl;
        return 0;
      case 'l':
        libName = optarg;
        break;
      case '?':
        // Unknown option or missing argument
        break;
      default:
        std::cerr << "Unknown error while parsing command line arguments."
                  << std::endl;
        return 1;
    }
  }

  // if all are not emply
  if (!jsonPath.empty() && !filePath.empty() && !directoryPath.empty()) {
    std::cerr << "Not allowed to use at the same time path to verilog file and "
                 "to json."
              << std::endl;
    return 1;
  }

  Threading::ThreadPool pool(threadsNumber);
  std::vector<std::shared_ptr<GraphAbcFolder>> folder;
  // we always have one function to calc start stats
  int funcCount = ((int)makeBalanced) + ((int)makeResyn2) + 1;
  auto runInMultithread = [&](std::string path, std::string graph) {
    std::shared_ptr<GraphAbcFolder> ptr(
        new GraphAbcFolder(path, graph, libName, defaultLibPath, funcCount));
    folder.push_back(ptr);

    pool.submit((*folder.back()).callGetStats);

    if (makeBalanced) {
      pool.submit((*folder.back()).callOptimize);
    }

    if (makeResyn2) {
      pool.submit((*folder.back()).callResyn2);
    }

    if (toBench) {
      pool.submit((*folder.back()).callToBench);
    }

    if (toFirrtl) {
      pool.submit((*folder.back()).callToFirrtl);
    }
  };

  auto start = high_resolution_clock::now();

  if (jsonPath.size()) {
    // jsonPath = "../examples/json/sampleALU.json";
    std::vector<std::pair<std::string, std::vector<std::string>>> allRes =
        CircuitGenGenerator::runGenerationFromJsonForPath(jsonPath);

    if (!(makeResyn2 || makeBalanced || toBench || toFirrtl || calcStats)) {
      return 0;
    }

    for (auto [main_path, allGraphs] : allRes) {
      for (auto graph : allGraphs) {
        std::string path = main_path + "/" + graph;

        runInMultithread(path, graph);
      }
    }
  }
  // used for parsing one file or dir
  else if (makeResyn2 || makeBalanced || toBench || toFirrtl || calcStats) {
    if (filePath.size()) {
      std::string graph = std::filesystem::path(filePath).stem();
      std::string path = std::filesystem::path(filePath).parent_path();

      runInMultithread(path, graph);
    }

    else {
      for (const auto& dirEntry : recursive_directory_iterator(directoryPath)) {
        if (!dirEntry.is_directory()) {
          const auto file = std::filesystem::canonical(dirEntry);
          auto filePath = file;

          if (file.extension() == ".v") {
            if (moveToFolder) {
              std::filesystem::create_directory(file.parent_path() /
                                                file.stem());

              filePath = file.parent_path() / file.stem() / file.filename();
              std::filesystem::rename(file, filePath);
            }

            runInMultithread(filePath.parent_path(), filePath.stem());
          }
        }
      }
    }
  }

  pool.wait();

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  std::clog << "Time taken: " << duration.count() << " microseconds"
            << std::endl;

  return 0;
}