/**
 * @file filesystem.hpp
 * @brief In-Mempry file system
 * @author Jack Heather
 * @date 2026-05-08
 */

#pragma once

#include "vnode.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <ranges>
#include <string_view>
#include <algorithm>
#include <optional>


class FileSystem {

public:
    FileSystem() {
        root = std::make_shared<Directory>();
        current_dir = root;
    }

    void mkdir(const std::string &name);
    void ls();
    void cd(const std::string& name);
    void touch(const std::string& name);
    void pwd();
    void ln_s(const std::string& target_name, const std::string& link_name);
    std::shared_ptr<VNode> resolvePath(std::string_view path);

private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> current_dir;
};
