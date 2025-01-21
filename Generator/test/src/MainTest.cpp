#include <filesystem>

#include "main.cpp"

#include "../GraphAbcFolder.hpp"

#include <gtest/gtest.h>

//#include "easylogging++Init.hpp"
#include "fstream"

const std::string testDirectory = std::filesystem::current_path();

TEST(Circuit, FailTest){
  
    EXPECT_EQ(0, 1);
    std::cout<<"Fail";
}

