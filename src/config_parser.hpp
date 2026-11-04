/**
 * \file config_parser.hpp
 * \author Nguen Khoang Shon (github:con2222)
 * \brief Парсинг аргументов командной строки и конфигурационного файла TOML
 * \date 2026
 * \version 1.0.0
 */
#pragma once

#include <string>
#include <tuple>
#include <vector>

namespace app::config {

    /**
     * \brief Пассивный объект для хранения настроек приложения
     */
    struct app_config {
        std::string input_dir;
        std::string output_dir;
        std::vector<std::string> filename_masks;
    };

    /**
     * \brief Класс для обработки параметров запуска и конфигурации
     */
    class config_parser {
    public:
        /**
         * \brief Читает аргументы и парсит TOML-файл
         * \param argc_ количество аргументов
         * \param argv_ массив аргументов
         * \return кортеж из конфигурации и статуса выполнения (true в случае успеха)
         */
        static std::tuple<app_config, bool> parse(
            int argc_,
            char* argv_[]) noexcept;
    };

}  // namespace app::config