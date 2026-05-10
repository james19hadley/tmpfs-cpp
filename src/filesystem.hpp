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

/**
 * @brief Core class of the file system. Manages the root and current directory.
 */
class FileSystem {

public:
    FileSystem() {
        root = std::make_shared<Directory>();
        current_dir = root;
    }

    /** @brief Lists the contents of the current directory. */
    void ls();

    /** @brief Changes the current directory to the specified path. */
    void cd(const std::string_view path);

    /** @brief Creates a directory at the given path. */
    void mkdir(const std::string_view path);

    /** @brief Creates an empty file at the given path. */
    void touch(const std::string_view path);

    /** @brief Prints the absolute path of the current directory. */
    void pwd();

    /** @brief Creates a symbolic link. */
    void ln_s(const std::string& target_name, const std::string& link_name);

    /**
    * @brief Resolves a string path into a VNode pointer.
    * @param path The string path to resolve (absolute, relative or ~).
    * @return shared_ptr to the node or nullptr if not found.
    */
    std::shared_ptr<VNode> resolvePath(std::string_view path);

private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> current_dir;
};
