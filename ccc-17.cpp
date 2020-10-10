#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <filesystem>
#include <string>
#include <algorithm>

TEST_CASE("std::filesystem::path supports == and .empty()") {
    std::filesystem::path empty_path;
    std::filesystem::path shadow_path { "/etc/shadow" };
    REQUIRE(empty_path.empty());
    REQUIRE(shadow_path == "/etc/shadow");
}

TEST_CASE("path offers disection methods") {
    std::filesystem::path kernel32 { R"(/mnt/c/Windows/System32/kernel32.dll)" }; // R"()" - raw characters. Interprets characters literally wihtout need to escape

    REQUIRE(kernel32.root_name() == "");
    REQUIRE(kernel32.relative_path() == "mnt/c/Windows/System32/kernel32.dll");
    REQUIRE(kernel32.filename() == "kernel32.dll");
    REQUIRE(kernel32.extension() == ".dll");
}

TEST_CASE("path offers modifiers") {
    std::filesystem::path kernel32 { R"(/mnt/c/Windows/System32/kernel32.dll)" };

    REQUIRE(kernel32.filename() == "kernel32.dll");
    kernel32.replace_filename("win32kfull.sys");
    REQUIRE(kernel32.filename() == "win32kfull.sys");

    kernel32.remove_filename();
    REQUIRE(kernel32.filename() == "");

    kernel32.clear();
    REQUIRE(kernel32.empty());
}

TEST_CASE("path offers compostion functions") {
    const auto temp_path = std::filesystem::temp_directory_path();
    const auto relative = std::filesystem::relative(temp_path);

    REQUIRE(temp_path == "/tmp");
    REQUIRE(relative == "../../../../../../tmp");
    REQUIRE_FALSE(relative.is_absolute());
}

/*
    I'm not going to add tests or anything, but know that there are tons of methods for
    inspecting file types (is_symlink, is_empty, is_directory, etc.), 
    files/directories (exists, permissions, last_write_time, etc.),
    and manipulating files/directories (copy, create_directory, rename, etc.)
*/

TEST_CASE("C++ offers means of iterating through directory") {
    const std::filesystem::path vscode_path { R"(./.vscode)" };
    const std::filesystem::directory_iterator dir_iter { vscode_path }; //directory_iterator won't go into sub directories

    auto file_count = 0;

    std::for_each(
        std::filesystem::begin(dir_iter), 
        std::filesystem::end(dir_iter), 
        [&file_count]( const auto& elem) { file_count++; }
    );

    REQUIRE(file_count == 4);
}