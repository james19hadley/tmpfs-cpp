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

TEST(FileSystemTest, TouchAndMkdirDuplicateError) {
    FileSystem fs;

    EXPECT_EQ(fs.touch("file.txt"), FSError::Success);
    EXPECT_EQ(fs.touch("file.txt"), FSError::AlreadyExists);

    EXPECT_EQ(fs.mkdir("dir"), FSError::Success);
    EXPECT_EQ(fs.touch("dir"), FSError::AlreadyExists);
}

TEST(FileSystemTest, ResolveRelativeDotDot) {
    FileSystem fs;
    EXPECT_EQ(fs.mkdir("dir1"), FSError::Success);
    EXPECT_EQ(fs.cd("dir1"), FSError::Success);
    EXPECT_EQ(fs.mkdir("dir2"), FSError::Success);

    auto root_node = fs.resolvePath("..");
    ASSERT_NE(root_node, nullptr);

    auto dir2_node = fs.resolvePath("../dir1/dir2");
    ASSERT_NE(dir2_node, nullptr);
}

TEST(FileSystemTest, DeepParentNavigation) {
    FileSystem fs;
    EXPECT_EQ(fs.mkdir("dir1"), FSError::Success);
    EXPECT_EQ(fs.cd("dir1"), FSError::Success);
    EXPECT_EQ(fs.mkdir("dir2"), FSError::Success);
    EXPECT_EQ(fs.cd("dir2"), FSError::Success);
    EXPECT_EQ(fs.touch("fileInDir2.txt"), FSError::Success);

    // from /dir1/dir2 look at .. (should be dir2)
    auto dir1_node = fs.resolvePath("..");
    ASSERT_NE(dir1_node, nullptr);

    auto dir1_casted = std::dynamic_pointer_cast<Directory>(dir1_node);
    ASSERT_NE(dir1_casted, nullptr);
    // check ".." is in dir2
    EXPECT_TRUE(dir1_casted->findChild("dir2") != nullptr);

    // from /dir1/dir2 look at ../.. (should be root)
    auto root_node = fs.resolvePath("../..");
    ASSERT_NE(root_node, nullptr);
    auto root_casted = std::dynamic_pointer_cast<Directory>(root_node);
    EXPECT_TRUE(root_casted->findChild("dir1") != nullptr);
}
