#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <limits>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <vector>

template<class T>
concept Numeric = std::is_arithmetic_v<T> && !std::same_as<T, bool>;

template<class T>
requires Numeric<T>
constexpr bool inRange(T value, T lower, T upper) {
    return lower <= value && value <= upper;
}

template<class T>
requires Numeric<T>
constexpr T clampValue(T value, T lower, T upper) {
    if (upper < lower) {
        throw std::invalid_argument("Lower bound exceeds upper bound");
    }
    if (value < lower) {
        return lower;
    }
    if (upper < value) {
        return upper;
    }
    return value;
}

template<class T>
void printValue(std::ostream& out, const T& value) {
    out << value;
}

void printValue(std::ostream& out, bool value);

struct Position {
    int x;
    int y;
};

template<class T>
class Grid {
public:
    Grid(int width, int height, const T& initialValue = T{})
        : width_(width), height_(height) {
        if (width < 0 || height < 0 || (width == 0) != (height == 0)) {
            throw std::invalid_argument("Use 0 x 0 or two positive dimensions");
        }
        if (width == 0) {
            return;
        }

        const auto columns = static_cast<std::size_t>(width);
        const auto rows = static_cast<std::size_t>(height);
        if (columns > cells_.max_size() / rows) {
            throw std::length_error("Grid is too large");
        }
        cells_.assign(columns * rows, initialValue);
    }

    int width() const {
        return width_;
    }

    int height() const {
        return height_;
    }

    bool empty() const {
        return cells_.empty();
    }

    bool contains(int x, int y) const {
        return inRange(x, 0, width_ - 1) && inRange(y, 0, height_ - 1);
    }

    T get(int x, int y) const {
        return cells_[index(x, y)];
    }

    void set(int x, int y, const T& value) {
        cells_[index(x, y)] = value;
    }

    void fill(const T& value) {
        std::fill(cells_.begin(), cells_.end(), value);
    }

    void print(std::ostream& out) const {
        for (int y = 0; y < height_; ++y) {
            for (int x = 0; x < width_; ++x) {
                if (x != 0) {
                    out << ' ';
                }
                printValue(out, get(x, y));
            }
            out << '\n';
        }
    }

    std::size_t count(const T& target) const
        requires std::equality_comparable<T> {
        std::size_t matches = 0;
        for (const T& value : cells_) {
            if (value == target) {
                ++matches;
            }
        }
        return matches;
    }

    std::optional<Position> findFirst(const T& target) const
        requires std::equality_comparable<T> {
        for (int y = 0; y < height_; ++y) {
            for (int x = 0; x < width_; ++x) {
                if (get(x, y) == target) {
                    return Position{x, y};
                }
            }
        }
        return std::nullopt;
    }

    T minimum() const requires Numeric<T> {
        if (empty()) {
            throw std::domain_error("Minimum of an empty grid");
        }
        return *std::min_element(cells_.begin(), cells_.end());
    }

    T maximum() const requires Numeric<T> {
        if (empty()) {
            throw std::domain_error("Maximum of an empty grid");
        }
        return *std::max_element(cells_.begin(), cells_.end());
    }

    long double mean() const requires Numeric<T> {
        if (empty()) {
            throw std::domain_error("Mean of an empty grid");
        }
        long double sum = 0.0L;
        for (T value : cells_) {
            sum += static_cast<long double>(value);
        }
        return sum / cells_.size();
    }

    void replaceBelow(T lower) requires Numeric<T> {
        for (T& value : cells_) {
            value = clampValue(value, lower, std::numeric_limits<T>::max());
        }
    }

private:
    std::size_t index(int x, int y) const {
        if (!contains(x, y)) {
            throw std::out_of_range("Grid coordinates are out of range");
        }
        return static_cast<std::size_t>(y) * width_ + x;
    }

    int width_;
    int height_;
    std::vector<T> cells_;
};
