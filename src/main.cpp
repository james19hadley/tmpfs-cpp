#include "filesystem.hpp"
#include <iostream>

int main() {
    FileSystem fs;
    fs.mkdir("home");
    fs.cd("home");
    fs.mkdir("user");
    fs.cd("user");
    fs.touch("config.txt");

    std::cout << "Current path: ";
    fs.pwd();
    std::cout << "Contents: ";
    fs.ls();
}
