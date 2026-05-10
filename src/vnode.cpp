/**
 * @file vnode.сpp
 * @brief Interface description for the virtual nodes of the In-Mempry file system
 * @author Jack Heather
 * @date 2026-05-10
 */

#include "vnode.hpp"

#include <memory>
#include <string_view>
#include <algorithm>

void Directory::addNode(std::string_view name, std::shared_ptr<VNode> node) {
    children[std::string(name)] = node;
}

std::shared_ptr<VNode> Directory::findChild(std::string_view name) {
    if (name == ".") return shared_from_this();
    if (name == "..") return parent.lock();

    auto it = children.find(std::string(name));
    if (it == children.end()) return nullptr;
    return it->second;
}

std::optional <std::string_view> Directory::findChildName(std::shared_ptr<VNode> child) {
    auto it = std::find_if(
        children.begin(),
        children.end(),
        [&child](const auto& pair) {
            return pair.second == child;
        }
    );
    if (it == children.end()) return std::nullopt;
    return it->first;
}


void File::write(std::string_view new_data) {
    data = std::string(new_data);
}

std::string_view File::read() const {
    return data;
}
