#include <gtest/gtest.h>
#include "filesystem.hpp"

TEST(FileSystemTest, ResolvePathSimple) {
    FileSystem fs;
    EXPECT_EQ(fs.mkdir("home"), FSError::Success);

    auto node = fs.resolvePath("home");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->isDir());
}

TEST(FileSystemTest, ResolvePathComplex) {
    FileSystem fs;
    EXPECT_EQ(fs.mkdir("home"), FSError::Success);
    EXPECT_EQ(fs.cd("home"), FSError::Success);
    EXPECT_EQ(fs.mkdir("user"), FSError::Success);

    auto node = fs.resolvePath("/home/user");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->isDir());
}

TEST(FileSystemTest, AbsoluteAndRelative) {
    FileSystem fs;
    EXPECT_EQ(fs.mkdir("a"), FSError::Success);
    EXPECT_EQ(fs.cd("a"), FSError::Success);
    EXPECT_EQ(fs.mkdir("b"), FSError::Success);

    EXPECT_NE(fs.resolvePath("/a/b"), nullptr); // absolute from anywhere
    EXPECT_NE(fs.resolvePath("b"), nullptr); // relative path
}

TEST(FileSystemTest, ResolvePathCorrectness) {
    FileSystem fs;
    EXPECT_EQ(fs.mkdir("home"), FSError::Success);

    EXPECT_EQ(fs.cd("home"), FSError::Success);
    EXPECT_EQ(fs.touch("my_file.txt"), FSError::Success);

    auto expected_node = fs.resolvePath("my_file.txt");
    auto actual_node = fs.resolvePath("/home/my_file.txt");

    EXPECT_EQ(expected_node, actual_node);
}
