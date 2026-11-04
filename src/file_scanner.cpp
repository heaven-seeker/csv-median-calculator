/**
 * \file file_scanner.cpp
 * \author Nguen Khoang Shon (github:con2222)
 * \brief Реализация сканера файлов
 * \date 2026
 * \version 1.0.0
 */

#include "file_scanner.hpp"

#include <algorithm>
#include <filesystem>
#include <spdlog/spdlog.h>

namespace app::io {

std::tuple<std::vector<std::string>, bool> file_scanner::get_csv_files(
    const std::string& dir_path_,
    const std::vector<std::string>& masks_) noexcept {

    std::vector<std::string> result_files;
    namespace fs = std::filesystem;

    try {
        fs::path target_dir{dir_path_};

        // Проверяем, существует ли указанный путь
        if (!fs::exists(target_dir)) {
            spdlog::error("Директория не существует: {}", dir_path_);
            return {result_files, false};
        }

        // Проверяем, что это именно папка, а не файл
        if (!fs::is_directory(target_dir)) {
            spdlog::error("Указанный путь не является директорией: {}", dir_path_);
            return {result_files, false};
        }

        // Итерируемся по содержимому папки
        for (const auto& entry : fs::directory_iterator(target_dir)) {
            if (!entry.is_regular_file()) {
                continue;
            }

            auto path = entry.path();
            if (path.extension() != ".csv") {
                continue;
            }

            std::string filename = path.filename().string();
            bool is_match = false;

            // Если маски не заданы, берем все файлы
            if (masks_.empty()) {
                is_match = true;
            } else {
                // Проверяем вхождение хотя бы одной маски
                for (const auto& mask : masks_) {
                    if (filename.find(mask) != std::string::npos) {
                        is_match = true;
                        break;
                    }
                }
            }

            if (is_match) {
                // Сохраняем полный путь
                result_files.push_back(path.string());
            }
        }

        std::sort(result_files.begin(), result_files.end());

        return {result_files, true};

    } catch (const fs::filesystem_error& err_) {
        spdlog::error("Ошибка файловой системы: {}", err_.what());
        return {result_files, false};
    } catch (const std::exception& err_) {
        spdlog::error("Неизвестная ошибка при поиске файлов: {}", err_.what());
        return {result_files, false};
    }
}

}  // namespace app::io