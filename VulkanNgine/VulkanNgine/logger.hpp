#pragma once

#include <filesystem>
#include <iostream>

inline std::string fileName(const std::string& file)
{
    std::filesystem::path path(file);
    return path.stem().string();
}

#define VKNGINE_LOG(I, T, X) std::I << "[" << fileName(__FILE__) << "] " << T << ": " << X << std::endl

#define VKNGINE_LOG_ERROR(X) VKNGINE_LOG(cerr, "ERROR", X);
#define VKNGINE_LOG_WARNING(X) VKNGINE_LOG(cerr, "WARNING", X);
#define VKNGINE_LOG_INFO(X) VKNGINE_LOG(cout, "INFO", X);
#define VKNGINE_LOG_VERBOSE(X) VKNGINE_LOG(cout, "VERBOSE", X);
#define VKNGINE_LOG_UNKNOWN(X) VKNGINE_LOG(cout, "UNKNOWN", X);
