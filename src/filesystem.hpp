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
#include <ranges>
#include <string_view>
#include <algorithm>

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
        std::vector<std::string_view> reversed_path;
        while (now_dir != root) {
            auto next_dir = now_dir->parent.lock();
            if (next_dir == nullptr) {
                std::cout << "Error. Some directory on the path to root is unreachable\n";
                return;
            }

            auto it = std::find_if(
                next_dir->children.begin(),
                next_dir->children.end(),
                [&now_dir](const auto& pair) {
                    return pair.second == now_dir;
                }
            );

            if (it == next_dir->children.end()) {
                std::cout << "Error. Didn't find some directory in it's parent children\n";
            } else {
                reversed_path.push_back(it->first);
            }
            now_dir = next_dir;
        }
        if (reversed_path.empty()) {
            std::cout << "/";
        }
        for (const auto& name : std::views::reverse(reversed_path)) {
            std::cout << "/" << name;
        }
        std::cout << "\n";
    }
};
