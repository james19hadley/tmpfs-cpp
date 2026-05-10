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
#include <optional>


#include "vnode.hpp"


class FileSystem {

public:
    FileSystem() {
        root = std::make_shared<Directory>();
        current_dir = root;
    }


    void mkdir(const std::string &name) {
        auto new_dir = std::make_shared<Directory>(/*parent=*/current_dir);
        current_dir->addNode(name, new_dir);
    }

    void ls() {
        for (auto const& [name, node] : current_dir->getChildren()) {
           std::cout << name << (node->isDir() ? "/" : "") << "\n";
        }
    }

    void cd(const std::string& name) {
        auto node = current_dir->findChild(name);
        if (node == nullptr) {
            std::cout << "No such directory!\n";
            return;
        }
        while (auto link = std::dynamic_pointer_cast<Symlink>(node)) {
            node = link->getTarget();
            if (!node) {
                std::cout << "Broken link!\n";
                return;
            }
        }

        auto next_dir = std::dynamic_pointer_cast<Directory>(node);
        if (next_dir) {
            current_dir = next_dir;
        } else {
            std::cout << "It's a file, not a directory!\n";
        }

    }

    void touch(const std::string& name) {
        auto new_file = std::make_shared<File>();
        current_dir->addNode(name, new_file);
    }

    void pwd() {
        auto now_dir = current_dir;
        std::vector<std::string_view> reversed_path;
        while (now_dir != root) {
            auto next_dir = now_dir->getParent();
            if (next_dir == nullptr) {
                std::cout << "Error. Some directory on the path to root is unreachable\n";
                return;
            }

            auto now_name_opt = next_dir->findChildName(now_dir);
            if (!now_name_opt) {
                std::cout << "Error. Didn't find some directory in it's parent children\n";
            } else {
                reversed_path.push_back(*now_name_opt);
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

    void ln_s(const std::string& target_name, const std::string& link_name) {
        auto target = current_dir->findChild(target_name);
        if (!target) {
            std::cout << "Error. No files with such name\n";
            return;
        }
        auto new_symlink = std::make_shared<Symlink>(target);
        current_dir->addNode(link_name, new_symlink);
    }


private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> current_dir;
};
