//
// Created by frostzt on 7/24/2025.
//

#include "wal_manager.hpp"
#include "wal_codec.hpp"

#include <algorithm>
#include <filesystem>
#include <thread>

namespace WAL {
    bool WALManager::append(const Entry &entry) const {
        const auto threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
        const size_t writerIdx = threadId % this->writersCount_;
        return this->writers_[writerIdx]->append(entry, FlushMode::FORCE_FLUSH);
    }

    void WALManager::close() const {
        for (const auto &writer: this->writers_) {
            writer->close();
        }
    }

    void WALManager::loadAll(const std::function<void(Entry &&)> &replyFn) const {
        std::vector<std::filesystem::path> filepaths;

        // Iterate over the wal directory
        for (const auto &entry: std::filesystem::directory_iterator(this->walDir_)) {
            if (const auto fileName = entry.path().filename().string();
                entry.is_regular_file() && fileName.starts_with("w_") && fileName.ends_with(".wal")) {
                filepaths.emplace_back(entry.path());
            }
        }

        // Read data stream
        for (const auto &path: filepaths) {
            std::ifstream stream(path, std::ios::in | std::ios::binary);
            while (stream && stream.good() && !stream.eof()) {
                if (auto maybeEntry = WAL::readRecord(stream); maybeEntry.has_value()) {
                    replyFn(std::move(*maybeEntry));
                } else {
                    if (!stream.eof()) {
                        std::cout << "Failed to read record from WAL file, possibly corrupted: " << path << std::endl;
                    }
                }
            }
        }
    }

    std::vector<Entry> WALManager::loadAll() const {
        std::vector<Entry> entries;

        this->loadAll([&entries](Entry &&entry) {
            entries.push_back(std::move(entry));
        });

        // Sort all the entries
        std::ranges::sort(entries, [](const Entry &a, const Entry &b) {
            return a.timestamp_ < b.timestamp_;
        });

        return entries;
    }

    void WALManager::startFlushThread() {
        if (this->flushThreadRunning_) return;
        this->flushThreadRunning_ = true;

        this->flushThread_ = std::thread([this]() {
            while (this->flushThreadRunning_) {
                for (const auto &writer: this->writers_) {
                    writer->flush();
                }

                std::this_thread::sleep_for(this->flushInterval_);
            }
        });
    }


    void WALManager::stopFlushThread() {
        if (!this->flushThreadRunning_) return;
        this->flushThreadRunning_ = false;

        if (this->flushThread_.joinable()) {
            this->flushThread_.join();
        } else {
            this->flushThread_.detach();
        }
    }
} // namespace WAL
