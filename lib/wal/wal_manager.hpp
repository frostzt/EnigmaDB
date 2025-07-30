//
// Created by frostzt on 7/24/2025.
//

#ifndef WAL_MANAGER_HPP
#define WAL_MANAGER_HPP

#include <functional>
#include <memory>
#include <thread>

#include "wal_writer.hpp"
#include "lib/entry/entry.hpp"

namespace WAL {
    /**
     * Represents a manager for Write-Ahead Logging (WAL). This class
     * handles operations such as appending entries, rotating WAL files,
     * and loading entries from WAL files. It ensures thread safety for
     * read and write operations.
     */
    class WALManager {
    private:
        /** Global directory to flush all the WAL files to */
        std::string walDir_;

        /** Flush thread represents std::thread which will run in the background */
        std::thread flushThread_;

        /** Represents if the flush thread is already in the running state */
        std::atomic<bool> flushThreadRunning_{false};

        /** Represents the interval for which the flush thread will auto-flush the writer */
        std::chrono::milliseconds flushInterval_{100};

        /** A pool of writers maintained by this manager */
        std::vector<std::unique_ptr<WALWriter> > writers_;

        /** Total count of all the writers currently running */
        size_t writersCount_;

    public:
        explicit WALManager(const size_t numWriters, const size_t maxFileSize, std::string &walDir) {
            // Store the wal dir for later use
            this->walDir_ = walDir;

            for (size_t i = 0; i < numWriters; i++) {
                writers_.emplace_back(std::make_unique<WALWriter>(i + 1, walDir, maxFileSize));
            }

            this->writersCount_ = numWriters;
        }

        ~WALManager() {
            this->stopFlushThread();
            this->close();
        }

        /**
         * Starts the flush thread responsible for periodically flushing data
         * from writer buffers to persistent storage. This thread ensures
         * that data in memory is written to WAL files at regular intervals
         * determined by the configured flush interval. Calling this method
         * has no effect if the flush thread is already running.
         */
        void startFlushThread();

        /**
         * Stops the flush thread responsible for periodically flushing data
         * from writer buffers to persistent storage. This method ensures
         * graceful termination of the thread, joining it if necessary,
         * and cleaning up associated resources. Calling this method has
         * no effect if the flush thread is not running.
         */
        void stopFlushThread();

        /**
         * Appends a new entry to the Write-Ahead Log (WAL) using a specific writer
         * determined by the current thread's ID. This method ensures thread-safe
         * appending of entries by distributing the workload among multiple writers.
         * The appended data is flushed to disk immediately.
         *
         * @param entry The entry to be appended to the WAL.
         * @return True if the entry was successfully appended, false otherwise.
         */
        [[nodiscard]] bool append(const core::Entry &entry) const;

        /**
         * Closes all writers managed by the WALManager, ensuring that any pending
         * data in the buffers of the writers is flushed and the resources are
         * released. This method is thread-safe and guarantees that all managed
         * writers are gracefully shut down.
         */
        void close() const;

        /**
         * Loads all entries from the Write-Ahead Log (WAL) files and invokes the provided
         * callback function for each retrieved entry. This method scans the WAL directory,
         * identifies valid WAL files, and processes them to extract individual entries.
         *
         * @param replyFn A callback function to be invoked for each entry retrieved from
         *                the WAL files. The function accepts an rvalue reference to
         *                an Entry object, enabling efficient transfer of ownership.
         */
        void loadAll(const std::function<void(core::Entry &&)> &replyFn) const;

        /**
         * Loads all entries from the Write-Ahead Log (WAL) files and returns them
         * as a sorted collection of entries. This method scans the WAL directory,
         * retrieves all valid entries from the log files, and sorts them based on
         * their timestamps in ascending order.
         *
         * @return A vector containing all entries from the WAL, sorted by timestamp.
         */
        [[nodiscard]] std::vector<core::Entry> loadAll() const;

        [[nodiscard]] uint32_t getWritersMetaData() const {
            uint32_t total = 0;
            for (int i = 0; i < this->writersCount_; i++) {
                const auto writer = this->writers_[i].get();
                const auto fileNumber = writer->getCurrentFileNumber();
                std::cout << "WRITER [ " << i << " ] " << "File Number: " << fileNumber << std::endl;

                total += fileNumber;
            }

            return total;
        };
    };
} // namespace WAL


#endif //WAL_MANAGER_HPP
