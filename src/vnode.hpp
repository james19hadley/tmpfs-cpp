/**
 * @file vnode.hpp
 * @brief Interface description for the virtual nodes of the In-Mempry file system
 * @author Jack Heather
 * @date 2026-05-02
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <optional>

/**
 * @brief Basic class for all the nodes of the file system
 * Node doesn't know it's name, only stores the meta-data
 * and offers an interface to work with data
 */
class VNode {
public:
    VNode() = default;
    virtual ~VNode() = default;

    /**
     * @brief Checks if the node is a directory
     * @return true, if it is a directory, else true
     */
    virtual bool isDir() const = 0;
};


/**
 * @brief Class for files
 * Has data
 */
class File: public VNode {
public:
    bool isDir() const override { return false; }
    void write(std::string_view content);
    std::string_view read() const;

private:
    std::string data;
};


/**
 * @brief Directory class
 * Has parent
 * Has children
 */
class Directory: public VNode, public std::enable_shared_from_this<Directory> {
public:
    Directory() = default;
    Directory(std::shared_ptr<Directory> _parent) : parent(_parent) {}

    bool isDir() const override { return true; }

    std::shared_ptr<Directory> getParent() const { return parent.lock(); }
    const std::unordered_map<std::string, std::shared_ptr<VNode>>& getChildren() const {
        return children;
    }

    void addNode(std::string_view name, std::shared_ptr<VNode> node);
    std::shared_ptr<VNode> findChild(std::string_view name);
    std::optional<std::string_view> findChildName(std::shared_ptr<VNode> child);

private:
    std::weak_ptr<Directory> parent;
    std::unordered_map<std::string, std::shared_ptr<VNode>>children;
};


/**
 * @brief Symlink class
 */
class Symlink: public VNode {
public:
    Symlink() = default;
    Symlink(std::weak_ptr<VNode> _target) : target(_target) {}

    bool isDir() const override {
        return false; // In Linux Symlink is a separate type, not a directory
    }
    std::shared_ptr<VNode> getTarget() const {
        return target.lock();
    }

private:
    std::weak_ptr<VNode> target;
};
