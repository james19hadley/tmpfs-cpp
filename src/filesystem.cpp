#include "filesystem.hpp"
#include "vnode.hpp"
#include <memory>


void FileSystem::mkdir(const std::string &name) {
    auto new_dir = std::make_shared<Directory>(/*parent=*/current_dir);
    current_dir->addNode(name, new_dir);
}

void FileSystem::ls() {
    for (auto const& [name, node] : current_dir->getChildren()) {
        std::cout << name << (node->isDir() ? "/" : "") << "\n";
    }
}

void FileSystem::cd(const std::string& name) {
    auto node = current_dir->findChild(name);
    if (!node) {
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

void FileSystem::touch(const std::string& name) {
    auto new_file = std::make_shared<File>();
    current_dir->addNode(name, new_file);
}

void FileSystem::pwd() {
    auto now_dir = current_dir;
    std::vector<std::string_view> reversed_path;
    while (now_dir != root) {
        auto next_dir = now_dir->getParent();
        if (!next_dir) {
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

void FileSystem::ln_s(const std::string& target_name, const std::string& link_name) {
    auto target = current_dir->findChild(target_name);
    if (!target) {
        std::cout << "Error. No files with such name\n";
        return;
    }
    auto new_symlink = std::make_shared<Symlink>(target);
    current_dir->addNode(link_name, new_symlink);
}

std::shared_ptr<VNode> FileSystem::resolvePath(std::string_view path) {
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

        current = dir->findChild(token);

        if (!current) return nullptr;

        while (auto link = std::dynamic_pointer_cast<Symlink>(current)) {
            current = link->getTarget();
            if (!current) return nullptr;
        }
    }

    return current;
}
