#include "filesystem.hpp"
#include <iostream>
#include <assert.h>

int main() {
    FileSystem fs;
    fs.mkdir("home");
    fs.cd("home");
    fs.mkdir("user");
    fs.cd("user");
    fs.touch("config.txt");
    fs.mkdir("test");
    fs.ln_s("test", "testlink");

    std::cout << "Contents: ";
    fs.ls();

    fs.cd("testlink");
    assert(fs.resolvePath("/home/user") != nullptr);

    std::cout << "Current path: ";
    fs.pwd();
}
