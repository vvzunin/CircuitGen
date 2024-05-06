#include <getopt.h>
#include <unistd.h>

#include <AbcUtils.hpp>
#include <CircuitGenGenerator/CircuitGenGenerator.hpp>
#include <YosysUtils.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  std::string json_path, file_path;

  bool makeBalanced = false;
  bool makeResyn2 = false;
  bool toBench = false;
  bool toFirrtl = false;

  std::string libName = "sky130.lib";

  const std::string defaultLibPath = "tech_libs/";
  // Use getopt to parse command line arguments

  const char *const short_opts = "hf:j:rbBFl:";
  const option long_opts[] = {{"json_path", required_argument, nullptr, 'j'},
                              {"file_path", required_argument, nullptr, 'f'},
                              {"make_resyn2", no_argument, nullptr, 'r'},
                              {"make_balance", no_argument, nullptr, 'b'},
                              {"to_bench", no_argument, nullptr, 'B'},
                              {"to_firrtl", no_argument, nullptr, 'F'},
                              {"help", no_argument, nullptr, 'h'},
                              {"lib_name", required_argument, nullptr, 'l'}};
  int c;

  int opt;
  while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) !=
         -1) {
    switch (opt) {
      case 'j':
        json_path = optarg;
        break;
      case 'f':
        file_path = optarg;
        break;
      case 'b':
        makeBalanced = true;
        break;
      case 'r':
        makeResyn2 = true;
        break;
      case 'B':
        toBench = true;
        break;
      case 'F':
        toFirrtl = true;
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

  // if both are not emply
  if (!json_path.empty() && !file_path.empty()) {
    std::cerr << "Not allowed to use at the same time path to verilog file and "
                 "to json."
              << std::endl;
    return 1;
  }

  if (json_path.size()) {
    // json_path = "../examples/json/sampleALU.json";
    std::vector<std::pair<std::string, std::vector<std::string>>> allRes =
        CircuitGenGenerator::runGenerationFromJsonForPath(json_path);

    if (!(makeResyn2 || makeBalanced || toBench || toFirrtl)) {
      return 0;
    }

    for (auto [path, allGraphs] : allRes) {
      for (auto graph : allGraphs) {
        if (makeBalanced) {
          AbcUtils::optimizeWithLib(graph + ".v", libName, path + "/" + graph,
                                    defaultLibPath);
        }

        if (makeResyn2) {
          AbcUtils::resyn2(graph, libName, path + "/" + graph, defaultLibPath);
        }

        if (toBench) {
          AbcUtils::verilogToBench(graph, path + "/" + graph);
        }

        if (toFirrtl) {
          YosysUtils::writeFirrtl(graph + ".v", graph + ".firrtl", path);
        }
      }
    }
  }
  // used for parsing one file
  else if (makeResyn2 || makeBalanced || toBench || toFirrtl) {
    std::string graph = std::filesystem::path(file_path).stem();
    std::string path = std::filesystem::path(file_path).parent_path();

    if (makeBalanced) {
      AbcUtils::optimizeWithLib(graph, libName, path, defaultLibPath);
    }

    if (makeResyn2) {
      AbcUtils::resyn2(graph, libName, path, defaultLibPath);
    }

    if (toBench) {
      AbcUtils::verilogToBench(graph, path);
    }

    if (toFirrtl) {
      YosysUtils::writeFirrtl(graph + ".v", graph + ".firrtl", path);
    }
  }

  return 0;
}