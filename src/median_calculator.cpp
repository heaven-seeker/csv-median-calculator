/**
 * \file median_calculator.cpp
 * \author Nguen Khoang Shon (github:con2222)
 * \brief Реализация калькулятора медианы
 * \date 2026
 * \version 1.0.0
 */

#include "median_calculator.hpp"

namespace app::calc {

    void median_calculator::add(double value_) noexcept {
        if (_left_heap.empty() || value_ <= _left_heap.top()) {
            _left_heap.push(value_);
        } else {
            _right_heap.push(value_);
        }

        if (_left_heap.size() > _right_heap.size() + 1) {
            _right_heap.push(_left_heap.top());
            _left_heap.pop();
        } else if (_right_heap.size() > _left_heap.size()) {
            _left_heap.push(_right_heap.top());
            _right_heap.pop();
        }
    }

    double median_calculator::get() const noexcept {
        if (_left_heap.empty()) {
            return 0.0;
        }

        if (_left_heap.size() == _right_heap.size()) {
            return (_left_heap.top() + _right_heap.top()) / 2.0;
        }

        return _left_heap.top();
    }

}  // namespace app::calc