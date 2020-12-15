#include "logger.h"

std::shared_ptr<spdlog::logger> getLogger(const std::string& name) {
	
	auto max_size = 1048576 * 5;
	auto max_files = 3;

	return spdlog::rotating_logger_mt("logger", "C:\\logs\\atm\\logs.txt", max_size, max_files);
}

