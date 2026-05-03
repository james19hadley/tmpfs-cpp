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
     * @return true, if it is a directory, else false
     */
    virtual bool isDir() const = 0;
};


/**
 * @brief Class for files
 * Has data
 */
class File: public VNode {
public:
    std::string data;
    bool isDir() const override { return false; }
};


/**
 * @brief Directory class
 * Has parent
 * Has children
 */
class Directory: public VNode {
public:
    std::weak_ptr<Directory> parent;
    std::unordered_map<std::string, std::shared_ptr<VNode>>children;

    bool isDir() const override { return true; }
};
