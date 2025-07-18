#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <vector>

namespace Utility {
    inline void init() {
        const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::debug);
        console_sink->set_pattern("[%^%l%$] %v");

        const auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("lsmtree.log", true);
        file_sink->set_level(spdlog::level::debug);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");

        std::vector<spdlog::sink_ptr> sinks { console_sink, file_sink };
        const auto logger = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());
        logger->set_level(spdlog::level::debug);

        spdlog::set_default_logger(logger);
    }

    // Guarantee this runs before main or test entry
    struct Initializer {
        Initializer() {
            init();
        }
    };

    static Initializer forceInit;

    // Logging wrapper namespace
    namespace Logger {
        template <typename... Args>
        void debug(Args&&... args) {
            spdlog::debug(std::forward<Args>(args)...);
        }

        template <typename... Args>
        void info(Args&&... args) {
            spdlog::info(std::forward<Args>(args)...);
        }

        template <typename... Args>
        void warn(Args&&... args) {
            spdlog::warn(std::forward<Args>(args)...);
        }

        template <typename... Args>
        void error(Args&&... args) {
            spdlog::error(std::forward<Args>(args)...);
        }

        template <typename... Args>
        void critical(Args&&... args) {
            spdlog::critical(std::forward<Args>(args)...);
        }
    }
}

#endif // LOGGER_HPP
