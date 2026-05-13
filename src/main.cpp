/**
 * @file main.cpp
 * @brief shell emulator
 * @author Jack Heather
 * @date 2026-05-02
 */

#include "filesystem.hpp"
#include <iostream>
#include <assert.h>
#include <sstream>
#include <vector>

int main() {
    FileSystem fs;
    std::string line;

    while (true) {
        std::cout << "[ " << fs.pwd() << " ] $ ";

        std::string line;
        if (!std::getline(std::cin, line) || line == "exit") break;
        if (line.empty()) continue;

        std::stringstream ss(line); //error: variable ‘std::stringstream ss’ has initializer but incomplete type

        std::string word;
        std::vector<std::string> args;
        while (ss >> word) {
            args.push_back(word);
        }

        const std::string& cmd = args[0];

        if (cmd == "ls") {
            if (args.size() >= 2) {
                fs.ls(args[1]);
            } else {
                fs.ls();
            }
        } else if (cmd == "pwd") {
            std::cout << fs.pwd() << "\n";
        } else if (cmd == "cd") {
            if (args.size() < 2) {
                std::cerr << "cd: missing operand\n";
            } else {
                auto err = fs.cd(args[1]);
                if (err != FSError::Success) {
                    std::cerr << "cd: " << errorToString(err) << "\n";
                }
            }
        } else if (cmd == "mkdir") {
            if (args.size() < 2) {
                std::cerr << "mkdir: missing operand\n";
            } else {
                FSError err = fs.mkdir(args[1]);
                if (err != FSError::Success) {
                    std::cerr << "mkdir: " << errorToString(err) << "\n";
                }
            }
        } else if (cmd == "touch") {
            if (args.size() < 2) std::cerr << "touch: missing operand\n";
            else {
                auto err = fs.touch(args[1]);
                if (err != FSError::Success) std::cerr << "touch: " << errorToString(err) << "\n";
            }
        }
        else if (cmd == "echo") {
            // echo "hello" path
            if (args.size() < 3) std::cerr << "Usage: echo <content> <path>\n";
            else {
                auto err = fs.writeToFile(args[2], args[1]);
                if (err != FSError::Success) std::cerr << "echo: " << errorToString(err) << "\n";
            }
        }
        else if (cmd == "cat") {
            if (args.size() < 2) std::cerr << "cat: missing operand\n";
            else {
                auto [content, err] = fs.readFromFile(args[1]);
                if (err == FSError::Success) std::cout << content << "\n";
                else std::cerr << "cat: " << errorToString(err) << "\n";
            }
        }
        else if (cmd == "ln") {
            // ln -s target link
            if (args.size() < 4 || args[1] != "-s") std::cerr << "Usage: ln -s <target> <link>\n";
            else {
                auto err = fs.ln_s(args[2], args[3]);
                if (err != FSError::Success) std::cerr << "ln: " << errorToString(err) << "\n";
            }
        } else {
            std::cerr << "Unknown command!\n";
        }
    }

    return 0;
}
