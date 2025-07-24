//
// Created by frostzt on 7/24/2025.
//

#include <sys/stat.h>

#include "wal_manager.hpp"

namespace WAL {
    size_t WALManager::getFileSize(const std::string &path) {
        struct stat fileStat; /* POSIX ONLY */
        if (stat(path.c_str(), &fileStat) < 0) {
            return 0;
        }

        return fileStat.st_size;
    }

    std::string WALManager::filePath(const uint32_t fileId) const {
        std::string filepath = walDir_ +  "/" + std::to_string(fileId) + ".wal";
        return filepath;
    }

} // namespace WAL
