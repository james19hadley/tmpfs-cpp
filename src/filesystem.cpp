/**
 * @file filesystem.сpp
 * @brief Interface description for the virtual nodes of the In-Mempry file system
 * @author Jack Heather
 * @date 2026-05-10
 */

#include "filesystem.hpp"
#include "vnode.hpp"

#include <cstdio>
#include <memory>
#include <string>


void FileSystem::ls() const {
    for (auto const& [name, node] : current_dir->getChildren()) {
        std::cout << name << (node->isDir() ? "/" : "") << "\n";
    }
}

void FileSystem::ls(const std::string_view path) const {
    auto current = resolvePath(path);
    auto dir = std::dynamic_pointer_cast<Directory>(current);
    if (!dir) {
        std::cout << "Path is not a directory\n";
        return;
    }
    for (auto const& [name, node] : dir->getChildren()) {
        std::cout << name << (node->isDir() ? "/" : "") << "\n";
    }
}


FSError FileSystem::cd(const std::string_view path) {
    auto node = resolvePath(path);
    if (!node) return FSError::InvalidPath;

    auto next_dir = std::dynamic_pointer_cast<Directory>(node);
    if (next_dir) {
        current_dir = next_dir;
    } else {
        return FSError::NotADirectory;
    }

    return FSError::Success;
}

FSError FileSystem::mkdir(const std::string_view path) {
    auto pos = path.find_last_of("/");
    auto name = path;
    auto current = current_dir;
    if (pos != std::string::npos) {
        auto dir_path = path.substr(0, pos);
        if (dir_path == "") dir_path = "/";
        current = std::dynamic_pointer_cast<Directory>(resolvePath(dir_path));

        if (!current) return FSError::NotADirectory;
        name = path.substr(pos + 1);
    }

    if (name.empty()) return FSError::EmptyName;
    if (current->findChild(name)) return FSError::AlreadyExists;

    auto new_dir = std::make_shared<Directory>(/*parent=*/current);
    current->addNode(name, new_dir);

    return FSError::Success;
}

FSError FileSystem::touch(const std::string_view path) {
    auto pos = path.find_last_of("/");
    auto name = path;
    auto current = current_dir;
    if (pos != std::string::npos) {
        auto dir_path = path.substr(0, pos);
        current = std::dynamic_pointer_cast<Directory>(resolvePath(dir_path));

        if (!current) return FSError::InvalidPath;

        name = path.substr(pos + 1);
    }

    if (name.empty()) return FSError::EmptyName;

    if (current->findChild(name)) return FSError::AlreadyExists;

    auto new_file = std::make_shared<File>();
    current->addNode(name, new_file);

    return FSError::Success;
}

std::string FileSystem::pwd() const {
    auto now_dir = current_dir;
    std::vector<std::string_view> reversed_path;
    while (now_dir != root) {
        auto next_dir = now_dir->getParent();
        if (!next_dir) {
            std::cout << "Error. Some directory on the path to root is unreachable\n";
            return "";
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
        return "/";
    }
    std::string output = "";
    for (const auto& name : std::views::reverse(reversed_path)) {
        output += "/";
        output += name;
    }
    return output;
}

FSError FileSystem::ln_s(const std::string& target_name, const std::string& link_name) {
    auto target = current_dir->findChild(target_name);
    if (!target) {
        return FSError::NotFound;
    }
    auto new_symlink = std::make_shared<Symlink>(target);
    current_dir->addNode(link_name, new_symlink);

    return FSError::Success;
}

std::shared_ptr<VNode> FileSystem::resolvePath(std::string_view path) const {
    if (path.empty()) return current_dir;

    std::shared_ptr<VNode> current = nullptr;

    if (path.starts_with('/')) {
        current = root;
    } else if (path.starts_with('~')) {
        current = root; // for now we dont have a home dir
        path.remove_prefix(1);
    } else {
        current = current_dir;
    }

    for (auto const word : std::views::split(path, '/')) {
        std::string_view token{word.begin(), word.end()};

        if (token.empty() || token == ".") continue;

        auto dir = std::dynamic_pointer_cast<Directory>(current);
        if (!dir) {
            std::cerr << "Not a directory in the middle of the path\n";
            return nullptr;
        }

        if (token == "..") {
            auto parent = dir->getParent();
            current = parent ? parent : root;
        } else {
            current = dir->findChild(token);
        }

        if (!current) return nullptr;

        while (auto link = std::dynamic_pointer_cast<Symlink>(current)) {
            current = link->getTarget();
            if (!current) return nullptr;
        }
    }

    return current;
}

FSError FileSystem::writeToFile(std::string_view path, std::string_view content) {
    auto node = resolvePath(path);
    if (!node) return FSError::NotFound;

    auto file = std::dynamic_pointer_cast<File>(node);
    if (!file) return FSError::IsDirectory;

    file->write(content);
    return FSError::Success;
}

std::pair<std::string, FSError> FileSystem::readFromFile(std::string_view path) {
    auto node = resolvePath(path);
    if (!node) return {"", FSError::NotFound};

    auto file = std::dynamic_pointer_cast<File>(node);
    if (!file) return {"", FSError::IsDirectory};

    return {std::string(file->read()), FSError::Success};
}
