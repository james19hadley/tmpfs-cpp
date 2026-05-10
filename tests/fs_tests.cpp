#include <gtest/gtest.h>
#include "filesystem.hpp"

TEST(FileSystemTest, ResolvePathSimple) {
    FileSystem fs;
    fs.mkdir("home");

    auto node = fs.resolvePath("home");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->isDir());
}

TEST(FileSystemTest, ResolvePathComplex) {
    FileSystem fs;
    fs.mkdir("home");
    fs.cd("home");
    fs.mkdir("user");

    auto node = fs.resolvePath("/home/user");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->isDir());
}

TEST(FileSystemTest, AbsoluteAndRelative) {
    FileSystem fs;
    fs.mkdir("a");
    fs.cd("a");
    fs.mkdir("b");

    EXPECT_NE(fs.resolvePath("/a/b"), nullptr); // absolute from anywhere
    EXPECT_NE(fs.resolvePath("b"), nullptr); // relative path
}

TEST(FileSystemTest, ResolvePathCorrectness) {
    FileSystem fs;
    fs.mkdir("home");

    fs.cd("home");
    fs.touch("my_file.txt");

    auto expected_node = fs.resolvePath("my_file.txt");
    auto actual_node = fs.resolvePath("/home/my_file.txt");

    EXPECT_EQ(expected_node, actual_node);
}
