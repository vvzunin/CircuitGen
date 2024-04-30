#include <getopt.h>
#include <unistd.h>

#include <AbcUtils.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <CircuitGenGenerator/CircuitGenGenerator.hpp>


int main(int argc, char **argv) {
  std::string json_path;
  bool makeOptimized = false;
  std::string libName = "sky130.lib";

  const std::string defaultLibPath = "tech_libs/";
  // Use getopt to parse command line arguments

  const char *const short_opts = "j:m:l:n:";
  const option long_opts[] = {
      {"json_path", required_argument, nullptr, 'j'},
      {"make_optimized", required_argument, nullptr, 'm'},
      {"lib_name", required_argument, nullptr, 'l'},
      {"num_nodes", required_argument, nullptr, 'n'}};
  int c;

  int opt;
  while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) !=
         -1) {
    switch (opt) {
      case 'j':
        json_path = optarg;
        break;
      case 'm':
        makeOptimized = true;
        break;
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

  // json_path = "../examples/json/sampleALU.json";
  std::vector<std::pair<std::string, std::vector<GraphPtr>>> allRes =
      CircuitGenGenerator::runGenerationFromJson(json_path);
  if (!makeOptimized) {
    return 0;
  }

  for (auto [path, allGraphs] : allRes) {
    for (auto graph : allGraphs) {
      AbcUtils::optimizeWithLib(graph->getName(), libName,
                                path + "/" + graph->getName(), defaultLibPath);
      AbcUtils::resyn2(graph->getName(), libName, path + "/" + graph->getName(),
                       defaultLibPath);
    }
  }

  return 0;
}