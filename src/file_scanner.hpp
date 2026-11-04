/**
 * \file file_scanner.hpp
 * \author Nguen Khoang Shon (github:con2222)
 * \brief Поиск и фильтрация CSV файлов в директории
 * \date 2026
 * \version 1.0.0
 */
#pragma once

#include <string>
#include <tuple>
#include <vector>

namespace app::io {

/**
 * \brief Утилита для сканирования файловой системы
 */
class file_scanner {
public:
    /**
     * \brief Ищет CSV-файлы в директории с учетом масок и сортирует их
     * \param dir_path_ путь к директории для поиска
     * \param masks_ список подстрок для фильтрации (пустой = все файлы)
     * \return кортеж: отсортированный список путей и флаг успешности
     */
    static std::tuple<std::vector<std::string>, bool> get_csv_files(
        const std::string& dir_path_,
        const std::vector<std::string>& masks_) noexcept;
};

}  // namespace app::io