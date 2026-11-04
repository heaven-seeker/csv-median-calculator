/**
 * \file config_parser.cpp
 * \author Nguen Khoang Shon (github:con2222)
 * \brief Реализация парсера конфигурации
 * \date 2026
 * \version 1.0.0
 */

#include "config_parser.hpp"

#include <iostream>
#include <tuple>
#include <vector>

#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>
#include <toml++/toml.hpp>

namespace app::config {

    std::tuple<app_config, bool> config_parser::parse(
        int argc_,
        char* argv_[]) noexcept {

        app_config result_config;
        std::string config_path = "config.toml";

        try {
            // 1. Настройка параметров командной строки
            namespace po = boost::program_options;
            po::options_description desc("Разрешенные параметры");
            desc.add_options()
                ("help", "показать справку")
                ("config", po::value<std::string>(), "путь к config.toml")
                ("cfg", po::value<std::string>(), "путь к config.toml (алиас)");

            po::variables_map vm;
            int style = po::command_line_style::default_style |
                po::command_line_style::allow_long_disguise;
            po::store(po::parse_command_line(argc_, argv_, desc, style), vm);
            po::notify(vm);

            // Обработка флага справки
            if (vm.count("help")) [[unlikely]] {
                std::cout << desc << "\n";
                return {result_config, false};
            }

            // Определение пути к файлу конфигурации
            if (vm.count("config")) {
                config_path = vm["config"].as<std::string>();
            } else if (vm.count("cfg")) {
                config_path = vm["cfg"].as<std::string>();
            }

            spdlog::info("Чтение конфигурации: {}", config_path);

            auto tbl = toml::parse_file(config_path);

            // Извлечение обязательного параметра входной директории
            auto input_node = tbl["main"]["input"];
            if (!input_node) {
                spdlog::error("Обязательный параметр 'input' не найден в конфиге");
                return {result_config, false};
            }
            result_config.input_dir = input_node.value_or(std::string{});

            // Извлечение опциональной выходной директории (по умолчанию "output")
            result_config.output_dir =
                tbl["main"]["output"].value_or(std::string{"output"});

            auto mask_node = tbl["main"]["filename_mask"];
            if (mask_node && mask_node.is_array()) {
                auto mask_array = mask_node.as_array();
                for (auto&& elem : *mask_array) {
                    if (elem.is_string()) {
                        result_config.filename_masks.push_back(
                            elem.value_or(std::string{}));
                    }
                }
            }

            return {result_config, true};

        } catch (const toml::parse_error& err_) {
            // Обработка синтаксических ошибок в самом TOML файле
            spdlog::error("Ошибка парсинга TOML: {}", err_.description());
            return {result_config, false};
        } catch (const std::exception& err_) {
            // Обработка ошибок boost::program_options и прочих стандартных исключений
            spdlog::error("Ошибка конфигурации: {}", err_.what());
            return {result_config, false};
        }
    }

}  // namespace app::config