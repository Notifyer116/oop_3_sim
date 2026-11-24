#include "grid.hpp"

#include <iomanip>
#include <iostream>

int main() {
    std::cout << std::boolalpha;
    std::cout << "Range checks (in range / clamped value)\n";
    std::cout << "int: " << inRange(12, 1, 9)
              << " / " << clampValue(12, 1, 9) << '\n';
    std::cout << "float: " << inRange(0.5f, 0.0f, 1.0f)
              << " / " << clampValue(1.2f, 0.0f, 1.0f) << '\n';
    std::cout << "double: " << inRange<double>(2, 0.0, 5.0)
              << " / " << clampValue<double>(7, 0.0, 5.0) << '\n';

    const int width = 8;
    const int height = 6;
    Grid<bool> obstacles(width, height);
    Grid<double> distances(width, height);
    Grid<int> costs(width, height);
    Grid<float> probabilities(width, height);

    const Position obstacleCells[] = {
        {1, 0}, {5, 0}, {4, 1}, {2, 2},
        {6, 2}, {0, 3}, {3, 4}, {7, 5}
    };
    for (const Position& cell : obstacleCells) {
        obstacles.set(cell.x, cell.y, true);
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            distances.set(x, y, (x + y) * 0.5);
            costs.set(x, y, (x + 2 * y) % 9 + 1);
            probabilities.set(x, y, ((x + 2 * y) % 11) / 10.0f);
        }
    }

    distances.set(7, 5, 7.5);
    costs.set(0, 0, 8);
    probabilities.set(0, 0, 0.05f);

    std::cout << "\nObstacle map\n";
    obstacles.print(std::cout);
    std::cout << "Obstacles: " << obstacles.count(true) << '\n';
    if (const auto first = obstacles.findFirst(true)) {
        std::cout << "First obstacle: (" << first->x << ", " << first->y << ")\n";
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nDistance map\n";
    distances.print(std::cout);
    std::cout << "Min: " << distances.minimum() << "  Max: " << distances.maximum()
              << "  Mean: " << distances.mean() << '\n';

    std::cout << "\nTraversal cost map\n";
    costs.print(std::cout);
    std::cout << "Min: " << costs.minimum() << "  Max: " << costs.maximum()
              << "  Mean: " << costs.mean() << '\n';

    std::cout << "\nTraversal probability map\n";
    probabilities.print(std::cout);
    std::cout << "Min: " << probabilities.minimum() << "  Max: " << probabilities.maximum()
              << "  Mean: " << probabilities.mean() << '\n';

    probabilities.replaceBelow(0.25f);
    std::cout << "\nTraversal probability map after raising values below 0.25\n";
    probabilities.print(std::cout);
    std::cout << "Min: " << probabilities.minimum() << "  Max: " << probabilities.maximum()
              << "  Mean: " << probabilities.mean() << '\n';
}
