#ifndef TYRA_LOGGER_H
#define TYRA_LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/null_sink.h"

#include <memory>
#include <mutex>

#include <iostream>

namespace tyra {
    typedef std::shared_ptr<spdlog::sinks::rotating_file_sink<std::mutex>> sink_type; 
    // typedef std::shared_ptr<spdlog::sinks::null_sink<spdlog::details::null_mutex>> sink_type;
    inline void init_loggers() {
        static bool init = false;
        if (!init) {
            sink_type sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt> ("tyra_log", 1024*1024, 5);
            // sink_type sink = std::make_shared<spdlog::sinks::null_sink_st>();
            spdlog::register_logger(std::make_shared<spdlog::logger>("world", sink));
            spdlog::register_logger(std::make_shared<spdlog::logger>("system", sink));
            spdlog::register_logger(std::make_shared<spdlog::logger>("component", sink));
            spdlog::register_logger(std::make_shared<spdlog::logger>("default", sink));
            init = true;
        }
    }
}

#endif
