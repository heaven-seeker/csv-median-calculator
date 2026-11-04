/**
 * \file csv_reader.cpp
 * \author Nguen Khoang Shon (github:con2222)
 * \brief Реализация парсера CSV
 * \date 2026
 * \version 1.0.0
 */

#include "csv_reader.hpp"

#include <algorithm>
#include <charconv>
#include <fstream>
#include <string_view>

#include <spdlog/spdlog.h>

namespace app::io {

namespace {
// Вспомогательная функция для разбиения строки без аллокаций памяти
std::vector<std::string_view> split(std::string_view str_, char delim_) {
    std::vector<std::string_view> result;
    std::size_t pos = 0;

    while (pos < str_.size()) {
        std::size_t next = str_.find(delim_, pos);
        if (std::string_view::npos == next) {
            result.push_back(str_.substr(pos));
            break;
        }
        result.push_back(str_.substr(pos, next - pos));
        pos = next + 1;
    }
    return result;
}
}  // namespace

std::tuple<std::vector<trade_record>, bool> csv_reader::read_file(
    const std::string& file_path_) noexcept {

    std::vector<trade_record> records;
    std::ifstream file{file_path_};

    if (!file.is_open()) {
        spdlog::error("Не удалось открыть файл: {}", file_path_);
        return {records, false};
    }

    std::string line;
    if (!std::getline(file, line)) {
        spdlog::error("Файл пуст: {}", file_path_);
        return {records, false};
    }

    // Парсинг заголовка
    auto headers = split(line, ';');
    int ts_idx = -1;
    int price_idx = -1;

    for (std::size_t i = 0; i < headers.size(); ++i) {
        if ("receive_ts" == headers[i]) {
            ts_idx = static_cast<int>(i);
        } else if ("price" == headers[i]) {
            price_idx = static_cast<int>(i);
        }
    }

    if (-1 == ts_idx || -1 == price_idx) {
        spdlog::error("Отсутствуют обязательные колонки в файле: {}",
            file_path_);
        return {records, false};
    }

    std::size_t line_num = 1;
    int max_idx = std::max(ts_idx, price_idx);

    // Построчное чтение и парсинг
    while (std::getline(file, line)) {
        ++line_num;
        if (line.empty()) {
            continue;
        }

        auto tokens = split(line, ';');

        // Проверка количества колонок
        if (static_cast<int>(tokens.size()) <= max_idx) {
            spdlog::error("Недостаточно колонок (строка {}) в файле: {}",
                line_num, file_path_);
            records.clear(); // Полностью отбраковываем файл
            return {records, false};
        }

        trade_record rec{};
        auto ts_str = tokens[ts_idx];
        auto price_str = tokens[price_idx];

        // Парсинг временной метки
        auto ts_res = std::from_chars(
            ts_str.data(), ts_str.data() + ts_str.size(), rec.receive_ts);

        if (ts_res.ec != std::errc{}) {
            spdlog::error("Ошибка парсинга receive_ts (строка {}) в: {}",
                line_num, file_path_);
            records.clear();
            return {records, false};
        }

        // Парсинг цены
        auto price_res = std::from_chars(
            price_str.data(), price_str.data() + price_str.size(), rec.price);

        if (price_res.ec != std::errc{}) {
            spdlog::error("Ошибка парсинга price (строка {}) в: {}",
                line_num, file_path_);
            records.clear();
            return {records, false};
        }

        records.push_back(rec);
    }

    return {records, true};
}

}  // namespace app::io