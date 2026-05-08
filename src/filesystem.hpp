/**
 * @file filesystem.hpp
 * @brief In-Mempry file system
 * @author Jack Heather
 * @date 2026-05-08
 */

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "vnode.hpp"

class FileSystem {
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> current_dir;

public:
    FileSystem() {
        root = std::make_shared<Directory>();
        current_dir = root;
    }

    void mkdir(const std::string &name) {
        auto new_dir = std::make_shared<Directory>(current_dir);
        current_dir->children[name] = new_dir;
    }

    void ls() {
        for (auto const& [name, node] : current_dir->children) {
           std::cout << name << (node->isDir() ? "/" : "") << "\n";
        }
    }

    void cd(const std::string& name) {
        auto it = current_dir->children.find(name);
        if (it != current_dir->children.end()) {
            auto next_dir = std::dynamic_pointer_cast<Directory>(it->second);
            if (next_dir) {
                current_dir = next_dir;
            } else {
                std::cout << "It's a file, not a directory!\n";
            }
        } else {
            std::cout << "No such directory!\n";
        }
    }

    void touch(const std::string& name) {
        auto new_file = std::make_shared<File>();
        current_dir->children[name] = new_file;
    }

    void pwd() {
        auto now_dir = current_dir;
        std::vector<std::string> reversed_path;
        while (now_dir != root) {
            auto next_dir = now_dir->parent.lock();
            if (next_dir == nullptr) {
                std::cout << "Some folder on the path to root is non reachable\n";
                return;
            }
            std::string now_name="";

            auto it = next_dir->children.begin();
            while (it != next_dir->children.end()) {
                auto prob_now = it->second;
                if (prob_now && prob_now == now_dir) {
                    now_name = it->first;
                    break;
                }
                ++it;
            }
            if (now_name == "") {
                std::cout << "Error. Didn't find some dir in it's parent children\n";
            } else {
                reversed_path.push_back(now_name);
            }
            now_dir = next_dir;
        }
        if (reversed_path.empty()) {
            std::cout << "/";
        }
        for (auto elem = reversed_path.rbegin(); elem < reversed_path.rend(); ++elem) {
            std::cout << *elem << "/";
        }
        std::cout << "\n";
    }
};
