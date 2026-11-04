/**
 * \file csv_reader.hpp
 * \author Nguen Khoang Shon (github:con2222)
 * \brief Чтение и парсинг CSV файлов с биржевыми данными
 * \date 2026
 * \version 1.0.0
 */
#pragma once

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

namespace app::io {

    /**
     * \brief Структура для хранения извлеченных данных записи
     */
    struct trade_record {
        std::uint64_t receive_ts;
        double price;
    };

    /**
     * \brief Класс для чтения CSV файлов
     */
    class csv_reader {
    public:
        /**
         * \brief Читает файл и извлекает receive_ts и price
         * \param file_path_ путь к CSV файлу
         * \return кортеж из вектора записей и статуса выполнения
         */
        static std::tuple<std::vector<trade_record>, bool> read_file(
            const std::string& file_path_) noexcept;
    };

}  // namespace app::io