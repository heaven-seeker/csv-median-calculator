/**
 * \file main.cpp
 * \author Nguen Khoang Shon (github:con2222)
 * \brief Точка входа в приложение для расчета медианы цен
 * \date 2026
 * \version 1.0.0
 */

#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

#include <spdlog/spdlog.h>

#include "config_parser.hpp"
#include "csv_reader.hpp"
#include "file_scanner.hpp"
#include "median_calculator.hpp"

int main(int argc, char* argv[]) {
    spdlog::info("Запуск приложения csv_median_calculator v1.0.0");

    // 1. Чтение конфигурации
    auto [config, is_config_ok] = app::config::config_parser::parse(argc, argv);
    if (!is_config_ok) {
        spdlog::error("Не удалось загрузить конфигурацию. Завершение работы.");
        return 1;
    }

    // 2. Поиск файлов
    auto [csv_files, is_fs_ok] = app::io::file_scanner::get_csv_files(
        config.input_dir, config.filename_masks);

    if (!is_fs_ok) {
        spdlog::error("Ошибка при сканировании директории. Завершение работы.");
        return 1;
    }

    // 3. Чтение и объединение данных
    std::vector<app::io::trade_record> all_records;
    for (const auto& file_path : csv_files) {
        spdlog::info("Чтение файла: {}", file_path);
        auto [records, is_read_ok] = app::io::csv_reader::read_file(file_path);

        if (is_read_ok) {
            all_records.insert(all_records.end(), records.begin(), records.end());
            spdlog::info("  Успешно прочитано записей: {}", records.size());
        } else {
            spdlog::warn("  Файл пропущен из-за ошибок формата.");
        }
    }

    spdlog::info("Всего валидных записей для обработки: {}", all_records.size());
    if (all_records.empty()) {
        spdlog::info("Нет данных для обработки. Завершение работы.");
        return 0;
    }

    // 4. Стабильная сортировка по времени
    spdlog::info("Сортировка данных...");
    std::stable_sort(all_records.begin(), all_records.end(),
        [](const auto& lhs, const auto& rhs) {
            return lhs.receive_ts < rhs.receive_ts;
        });

    // 5. Подготовка к записи
    namespace fs = std::filesystem;
    fs::path out_dir{config.output_dir};

    if (!fs::exists(out_dir)) {
        fs::create_directories(out_dir);
    }

    fs::path out_file = out_dir / "median_result.csv";
    std::ofstream out_stream{out_file};
    if (!out_stream.is_open()) {
        spdlog::error("Не удалось создать выходной файл: {}", out_file.string());
        return 1;
    }

    out_stream << "receive_ts;price_median\n";

    app::calc::median_calculator calc;
    std::string prev_median_str = "";
    std::size_t written_count = 0;

    spdlog::info("Расчет медианы и запись результатов...");

    // 6. Инкрементальный расчет
    for (const auto& rec : all_records) {
        calc.add(rec.price);
        double current_median = calc.get();

        std::string current_median_str = std::format("{:.8f}", current_median);

        // Запись только при изменении
        if (current_median_str != prev_median_str) {
            out_stream << rec.receive_ts << ";" << current_median_str << "\n";
            prev_median_str = current_median_str;
            ++written_count;
        }
    }

    spdlog::info("Записано изменений медианы: {}", written_count);
    spdlog::info("Результат сохранен: {}", out_file.string());
    spdlog::info("Завершение работы");

    return 0;
}