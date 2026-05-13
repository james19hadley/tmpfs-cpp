/**
 * @file filesystem.hpp
 * @brief In-Mempry file system
 * @author Jack Heather
 * @date 2026-05-08
 */

#pragma once

#include "vnode.hpp"

#include <bits/types/cookie_io_functions_t.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <ranges>
#include <string_view>
#include <algorithm>
#include <optional>

enum class FSError {
    Success = 0,
    NotFound,
    AlreadyExists,
    NotADirectory,
    NotAFile,
    InvalidPath,
    IsDirectory,
    EmptyName
};

inline std::string errorToString(FSError err) {
    switch (err) {
        case FSError::Success:       return "Success";
        case FSError::NotFound:      return "No such file or directory";
        case FSError::AlreadyExists: return "File or directory already exists";
        case FSError::NotADirectory: return "Not a directory";
        case FSError::NotAFile:      return "Not a file";
        case FSError::InvalidPath:   return "Invalid path";
        case FSError::IsDirectory:   return "Is a directory";
        case FSError::EmptyName:     return "Empty name";
        default:                     return "Unknown error";
    }
}

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
    void ls() const;
    void ls(const std::string_view path) const;

    /** @brief Prints the absolute path of the current directory. */
    std::string pwd() const;

    /** @brief Changes the current directory to the specified path. */
    [[nodiscard]] FSError cd(const std::string_view path);

    /** @brief Creates a directory at the given path. */
    [[nodiscard]] FSError mkdir(const std::string_view path);

    /** @brief Creates an empty file at the given path. */
    [[nodiscard]] FSError touch(const std::string_view path);

    /** @brief Creates a symbolic link. */
    [[nodiscard]] FSError ln_s(const std::string& target_name, const std::string& link_name);

    [[nodiscard]] FSError writeToFile(std::string_view path, std::string_view content);

    [[nodiscard]] std::pair<std::string, FSError> readFromFile(std::string_view path);

    /**
    * @brief Resolves a string path into a VNode pointer.
    * @param path The string path to resolve (absolute, relative or ~).
    * @return shared_ptr to the node or nullptr if not found.
    */
    std::shared_ptr<VNode> resolvePath(std::string_view path) const;

private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> current_dir;
};
