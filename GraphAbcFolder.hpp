#pragma once

#include <AbcUtils.hpp>
#include <YosysUtils.hpp>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
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
      for (const auto& data : (*subres).commandsOutput) {
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

class GraphAbcFolder {
 public:
  std::string path;
  std::string graph;
  std::string libName;
  std::string libPath;
  std::vector<CommandWorkResult> data;
  std::mutex addData;
  uint8_t target;

  GraphAbcFolder& operator=(const GraphAbcFolder& other) {
    path = other.path;
    graph = other.graph;
    libName = other.libName;
    libPath = other.libPath;
    data = other.data;

    return *this;
  }

  GraphAbcFolder& operator=(GraphAbcFolder&& other) = default;
  GraphAbcFolder(const GraphAbcFolder& other) = default;

  GraphAbcFolder(GraphAbcFolder&& other) {
    path = other.path;
    graph = other.graph;
    libName = other.libName;
    libPath = other.libPath;
    data = other.data;

    other.data.clear();
  }

  GraphAbcFolder(std::string _path, std::string _graph, std::string _libName,
                 std::string _libPath, uint8_t _target)
      : path(_path),
        graph(_graph),
        libName(_libName),
        libPath(_libPath),
        target(_target) {}

  // lambda for Optimize
  std::function<void()> callOptimize = [this]() {
    auto res = AbcUtils::optimizeWithLib(graph, libName, path, libPath);

    addData.lock();
    data.push_back(res);
    bool flag = data.size() == target;
    addData.unlock();

    if (flag) {
      writeDataToJson(data, path, graph);
    }
  };
  // lambda for Resyn2
  std::function<void()> callResyn2 = [this]() {
    auto res = AbcUtils::resyn2(graph, libName, path, libPath);

    addData.lock();
    data.push_back(res);
    bool flag = data.size() == target;
    addData.unlock();

    if (flag) {
      writeDataToJson(data, path, graph);
    }
  };
  // lambda for main info
  std::function<void()> callGetStats = [this]() {
    auto res = AbcUtils::getStats(graph, libName, path, libPath);
    res.commandsOutput["optimization_type"] = "Default";

    addData.lock();
    data.push_back(res);
    bool flag = data.size() == target;
    addData.unlock();

    if (flag) {
      writeDataToJson(data, path, graph);
    }
  };

  std::function<void()> callToBench = [this]() {
    AbcUtils::verilogToBench(graph, path);
  };

  std::function<void()> callToFirrtl = [this]() {
    YosysUtils::writeFirrtl(graph + ".v", graph + ".firrtl", path);
  };
};
