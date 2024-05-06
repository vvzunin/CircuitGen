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

void writeDataToJson(std::vector<CommandWorkResult> result, std::string i_path,
                     std::string i_circuitName) {
  std::ofstream outJson;

  outJson.open(i_path + "/" + i_circuitName + "AbcStats.json");
  if (!outJson) {
    std::cerr << "No json file to write" << std::endl;
    return;
  }

  outJson << "{" << std::endl;

  for (auto subres = result.begin(); subres != result.end(); ++subres) {
    std::string optType = (*subres).commandsOutput["optimization_type"];
    (*subres).commandsOutput.erase("optimization_type");
    outJson << "\t\"abcStats" << optType << "\": {" << std::endl;

    if ((*subres).correct) {
      bool first = true;
      for (const auto &data : (*subres).commandsOutput) {
        if (first) {
          first = false;
          outJson << "\t\t\"" << data.first << "\": " << data.second;
        } else {
          outJson << "," << std::endl
                  << "\t\t\"" << data.first << "\": " << data.second;
        }
      }
      outJson << std::endl;
    } else {
      for (int i = (*subres).commandsOutput["error"].find('"');
           i != std::string::npos;
           i = (*subres).commandsOutput["error"].find('"', i + 1))
        (*subres).commandsOutput["error"].erase(i, 1);

      for (int i = (*subres).commandsOutput["error"].find('\n');
           i != std::string::npos;
           i = (*subres).commandsOutput["error"].find('\n', i + 1))
        (*subres).commandsOutput["error"][i] = ';';

      outJson << "\t\t" << "\"error\": \"" << (*subres).commandsOutput["error"]
              << "\",\n";
      outJson << "\t\t" << "\"fileRead\": \""
              << (*subres).commandsOutput["fileRead"] << "\"\n";
    }

    outJson << "\t}";
    if (subres + 1 == result.end())
      outJson << std::endl << "}";
    else
      outJson << ',' << std::endl;
  }

  std::clog << i_circuitName << " ended\n";
}

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

    for (auto [main_path, allGraphs] : allRes) {
      for (auto graph : allGraphs) {
        std::vector<CommandWorkResult> data;
        std::string path = main_path + "/" + graph;

        if (makeBalanced) {
          data.push_back(AbcUtils::optimizeWithLib(
              graph, libName, path, defaultLibPath));
        }

        if (makeResyn2) {
          data.push_back(AbcUtils::resyn2(graph, libName, path,
                                          defaultLibPath));
        }

        if (toBench) {
          AbcUtils::verilogToBench(graph, path);
        }

        if (toFirrtl) {
          YosysUtils::writeFirrtl(graph + ".v", graph + ".firrtl", path);
        }

        writeDataToJson(data, path, graph);
      }
    }
  }
  // used for parsing one file
  else if (makeResyn2 || makeBalanced || toBench || toFirrtl) {
    std::string graph = std::filesystem::path(file_path).stem();
    std::string path = std::filesystem::path(file_path).parent_path();
    std::vector<CommandWorkResult> data;

    if (makeBalanced) {
      data.push_back(
          AbcUtils::optimizeWithLib(graph, libName, path, defaultLibPath));
    }

    if (makeResyn2) {
      data.push_back(AbcUtils::resyn2(graph, libName, path, defaultLibPath));
    }

    if (toBench) {
      AbcUtils::verilogToBench(graph, path);
    }

    if (toFirrtl) {
      YosysUtils::writeFirrtl(graph + ".v", graph + ".firrtl", path);
    }

    writeDataToJson(data, path, graph);
  }

  return 0;
}