//
// Created by frostzt on 9/15/25.
//

#ifndef ENIGMA_DB_ENGINE_HPP
#define ENIGMA_DB_ENGINE_HPP

#include <cstdint>
#include <string_view>
#include <string>

namespace io_engine {
    struct SegmentHandle {
        uint64_t id = 0;
    };

    class IoEngine {
    public:
        /**
         * Opens a segment file within the specified parent directory and returns a handle to the segment.
         * The segment file is identified by its name, provided in the segmentName parameter.
         *
         * @param parentDir The directory path where the segment resides.
         * @param segmentName The name of the segment to be opened.
         * @return A handle to the opened segment, encapsulated in a SegmentHandle structure.
         */
        SegmentHandle openSegment(std::string_view parentDir, std::string_view segmentName);

        /**
         * Writes data to the specified segment at its current position.
         * The data is provided as a pointer to a memory buffer, and the method writes up to the specified length.
         *
         * @param seg The handle of the segment to which the data will be written.
         * @param data A pointer to the memory buffer containing the data to be written.
         * @param len The number of bytes to write from the data buffer.
         * @return The number of bytes actually written, or a negative value if an error occurs.
         */
        long long write(SegmentHandle seg, const void *data, std::size_t len);

        /**
         * Flushes any pending write operations to the specified segment,
         * ensuring that all buffered data is committed to persistent storage.
         *
         * @param seg The handle of the segment to be flushed.
         * @return A boolean indicating whether the flush operation was successful (true) or if it failed (false).
         */
        bool flush(SegmentHandle seg);

        /**
         * Closes the specified segment, releasing any associated resources and ensuring any pending operations are finalized.
         *
         * @param seg The handle of the segment to be closed.
         * @return A boolean indicating whether the segment was successfully closed (true) or if the operation failed (false).
         */
        bool close(SegmentHandle seg);
    };
} // namespace io_engine

#endif //ENIGMA_DB_ENGINE_HPP
