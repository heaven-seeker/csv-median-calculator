/**
 * \file median_calculator.hpp
 * \author Nguen Khoang Shon (github:con2222)
 * \brief Инкрементальный расчет медианы
 * \date 2026
 * \version 1.0.0
 */
#pragma once

#include <functional>
#include <queue>
#include <vector>

namespace app::calc {

    /**
     * \brief Инкрементальный калькулятор медианы на основе двух куч
     */
    class median_calculator {
    public:
        /**
         * \brief Добавляет новое значение цены и перебалансирует кучи
         * \param value_ новая цена
         */
        void add(double value_) noexcept;

        /**
         * \brief Возвращает текущую медиану
         * \return рассчитанное значение медианы
         */
        double get() const noexcept;

    private:
        // Max-heap для хранения меньшей половины чисел
        std::priority_queue<double> _left_heap;

        // Min-heap для хранения большей половины чисел
        std::priority_queue<double, std::vector<double>, std::greater<double>>
            _right_heap;
    };

}  // namespace app::calc