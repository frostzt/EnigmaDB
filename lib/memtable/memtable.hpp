//
// Created by aiden on 7/13/2025.
//

#ifndef MEMTABLE_HPP
#define MEMTABLE_HPP

#include <shared_mutex>

#include "lib/abstract/avl.hpp"
#include "lib/entry/entry.hpp"

namespace memtable {
    /**
     * @class MemTable
     * @brief Manages an in-memory data structure for storing and manipulating entries.
     *
     * The MemTable class is designed to handle the temporary storage of key-value
     * entries, providing efficient insertion, deletion, retrieval, and ordering
     * operations using an AVL tree. It is suitable for applications that need
     * fast in-memory manipulations and can be "frozen" to prevent further modifications.
     */
    class MemTable {
    private:
        /** The name of the table this MemTable maintains */
        std::string tableName_;

        /** Core AVL Tree for this MemTable */
        std::unique_ptr<AVLTree<core::Entry> > tree_;

        /** Read/Write mutex for this MemTable */
        mutable std::shared_mutex rwMutex_;

        /** Represents if this MemTable is frozen or not */
        std::atomic<bool> frozen_;

    public:
        using KeyType = std::string;

        /**
         * @brief Constructs a MemTable with the specified name.
         *
         * Initializes the MemTable with a given name and sets up its internal AVL tree
         * for storing entries. The table is set to a non-frozen state initially, allowing
         * for modifications.
         *
         * @param tableName The name of the table used to identify this MemTable instance.
         */
        explicit MemTable(std::string tableName): tableName_(std::move(tableName)),
                                                  tree_(std::make_unique<AVLTree<core::Entry> >()),
                                                  frozen_(false) {
        }

        /**
         * @brief Inserts an entry into the MemTable.
         *
         * Adds the given entry into the internal AVL tree structure. If the MemTable
         * is in a frozen state, an exception is thrown as modifications are not permitted.
         * This operation is thread-safe, ensuring exclusive access during insertion.
         *
         * @param entry The entry to be inserted into the MemTable.
         * @throw std::runtime_error Thrown if the MemTable is frozen and insertion is attempted.
         */
        void put(const core::Entry &entry) const;

        /**
         * @brief Retrieves an entry associated with the given key from the MemTable.
         *
         * Searches the AVL tree for an entry matching the provided key. If such an
         * entry exists, it is returned within a std::optional; otherwise, an empty
         * std::optional is returned. Access to the MemTable is thread-safe.
         *
         * @param key The key to search for within the MemTable.
         * @return An optional containing the found entry, or std::nullopt if no
         *         matching entry exists.
         */
        std::optional<core::Entry> get(const core::Key &key) const;

        /**
         * @brief Deletes an entry associated with the given key from the MemTable.
         *
         * Creates a tombstone marker for the specified key with the given timestamp,
         * indicating the logical deletion of the entry. If the MemTable is frozen,
         * the operation is not permitted, and an exception is thrown.
         *
         * @param key The key associated with the entry to be deleted.
         * @param timestamp The timestamp to associate with the tombstone marker.
         * @throw std::runtime_error Thrown if the MemTable is frozen and deletion is attempted.
         */
        void del(const core::Key &key, uint64_t timestamp) const;

        /**
         * @brief Freezes the MemTable to prevent further modifications.
         *
         * Sets the MemTable to a frozen state, ensuring that no further insertions,
         * deletions, or updates can be performed. This method acquires a write lock
         * to guarantee thread safety. Once frozen, the state cannot be reverted.
         */
        void freeze();

        /**
         * @brief Checks whether the MemTable is in a frozen state.
         *
         * Determines if the MemTable is currently frozen, meaning that no further
         * modifications (e.g., insertions, deletions, or updates) are allowed. This method
         * provides a thread-safe way to query the current state of the MemTable.
         *
         * @return True if the MemTable is frozen, false otherwise.
         */
        bool isFrozen() const;

        /**
         * @brief Retrieves all entries in the MemTable in sorted order.
         *
         * Returns a vector containing all entries stored in the MemTable, sorted in
         * ascending order based on their natural comparison criteria. The method
         * leverages the internal AVL tree structure to ensure the entries are
         * efficiently sorted. This operation is thread-safe.
         *
         * @return A vector of entries sorted in ascending order.
         */
        std::vector<core::Entry> orderedEntries() const;

        /**
         * @brief Applies a new entry to the MemTable by inserting it into the internal data structure.
         *
         * Inserts the provided entry into the AVL tree managed by the MemTable. If the table
         * is in a frozen state, the operation is not allowed, and an exception is raised.
         * Thread safety is ensured by locking during the operation.
         *
         * @param entry The entry to be inserted into the MemTable.
         * @throw std::runtime_error Thrown if modifications are attempted while the MemTable is frozen.
         */
        void applyEntry(const core::Entry &entry) const;

        /**
         * @brief Retrieves the number of entries currently stored in the MemTable.
         *
         * This method provides a thread-safe way to determine the total count of
         * key-value entries present in the MemTable.
         *
         * @return The current number of entries in the MemTable.
         */
        size_t size() const;
    };
} // namespace memtable

#endif //MEMTABLE_HPP
