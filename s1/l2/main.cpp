// ⠀⠀⠀⣀⣴⣶⣿⠃⠘⣿⣶⣦⣀⠀⠀⠀
// ⠀⢠⣾⣿⣿⣿⠁⠀⠀⠈⣿⣿⣿⣷⡄⠀
// ⢠⣆⠀⠀⠀⠈⢦⠀⠀⡴⠁⠀⠀⠀⣴⡄
// ⣾⣿⣧⣀⣀⣀⣈⣣⣜⣁⣀⣀⣀⣼⣿⣧
// ⢻⣿⠟⠀⠀⠀⢀⡟⢻⡀⠀⠀⠀⠻⣿⡟
// ⠘⠏⠀⠀⠀⢠⠎⠀⠀⠱⡀⠀⠀⠀⠹⠃
// ⠀⠈⢿⣿⣿⣿⡄⠀⠀⢠⣿⣿⣿⠟⠁⠀
// ⠀⠀⠀⠈⠛⠿⠿⡄⣠⠿⠿⠛⠁⠀⠀⠀

#include <iostream>
#include <cmath>

static const float PI = 3.14159265359f;

using std::cout;
using std::cin;
using std::endl;

int main()
{
    float radius, x_coordinate, y_coordinate, angle,
        sine_value, cosine_value, left_hand_side,
        right_hand_side;
    const float square_root_of_three = float(sqrtf(3));
    const float step = float(60.f/180.f*PI);
    const float epsilon = float(1e-19);

    angle = step * 0.5f;

    cout << "Лаба №2" << endl
         << "Выполнил Максим Соболевский гр. 5352" << endl
         << "Параметризация фигуры:" << endl
         << "    Введите r = ";
    cin >> radius;

    if (radius < 0) {
        cout << "Введённый радиус некорректен" << endl;
        return 1;
    }

    cout << "Координаты точки:" << endl
         << "    x = ";
    cin >> x_coordinate;
    cout << "    y = ";
    cin >> y_coordinate;

    // Преобразование координат в область,
    // ограниченную радиусом
    if (radius > 0) {
        y_coordinate = fmodf(y_coordinate, radius*2.f);
        x_coordinate = fmodf(x_coordinate, radius*2.f);
    }
    x_coordinate += x_coordinate > 0.f ? -radius : radius;
    y_coordinate += y_coordinate > 0.f ? -radius : radius;

    // Определение принадлежности точки
    // фигуре

    if (x_coordinate * x_coordinate + y_coordinate * y_coordinate
        > radius * radius) {
        cout << "Не принадлежит: точка в не радиуса окружности"
             << endl;
        return 0;
    }

    // Кусок #1
    sine_value = sinf(angle);
    cosine_value = cosf(angle);
    left_hand_side = square_root_of_three
        * fabsf(y_coordinate * cosine_value
                - x_coordinate * sine_value);
    right_hand_side = y_coordinate * sine_value
        + x_coordinate * cosine_value;
    angle += step;

    if (fabsf(left_hand_side - right_hand_side) <= epsilon) {
        cout << "Принадлежит" << endl;
        return 0;
    }

    if (left_hand_side <= right_hand_side &&
        left_hand_side <= radius - right_hand_side) {
        cout << "Не принадлежит: точка находится в "
            "вырезанной области" << endl;
        return 0;
    }

    // Кусок #2
    sine_value = sinf(angle);
    cosine_value = cosf(angle);
    left_hand_side = square_root_of_three
        * fabsf(y_coordinate * cosine_value
                - x_coordinate * sine_value);
    right_hand_side = y_coordinate * sine_value
        + x_coordinate * cosine_value;
    angle += step;

    if (fabsf(left_hand_side - right_hand_side) <= epsilon) {
        cout << "Принадлежит" << endl;
        return 0;
    }

    if (left_hand_side <= right_hand_side &&
        left_hand_side <= radius - right_hand_side) {
        cout << "Не принадлежит: точка находится в "
            "вырезанной области" << endl;
        return 0;
    }

    // Кусок #3
    sine_value = sinf(angle);
    cosine_value = cosf(angle);
    left_hand_side = square_root_of_three
        * fabsf(y_coordinate * cosine_value
                - x_coordinate * sine_value);
    right_hand_side = y_coordinate * sine_value
        + x_coordinate * cosine_value;
    angle += step;

    if (fabsf(left_hand_side - right_hand_side) <= epsilon) {
        cout << "Принадлежит" << endl;
        return 0;
    }

    if (left_hand_side <= right_hand_side &&
        left_hand_side <= radius - right_hand_side) {
        cout << "Не принадлежит: точка находится в "
            "вырезанной области" << endl;
        return 0;
    }

    // Кусок #4
    sine_value = sinf(angle);
    cosine_value = cosf(angle);
    left_hand_side = square_root_of_three
        * fabsf(y_coordinate * cosine_value
                - x_coordinate * sine_value);
    right_hand_side = y_coordinate * sine_value
        + x_coordinate * cosine_value;
    angle += step;

    if (fabsf(left_hand_side - right_hand_side) <= epsilon) {
        cout << "Принадлежит" << endl;
        return 0;
    }

    if (left_hand_side <= right_hand_side &&
        left_hand_side <= radius - right_hand_side) {
        cout << "Не принадлежит: точка находится в "
            "вырезанной области" << endl;
        return 0;
    }

    // Кусок #5
    sine_value = sinf(angle);
    cosine_value = cosf(angle);
    left_hand_side = square_root_of_three
        * fabsf(y_coordinate * cosine_value
                - x_coordinate * sine_value);
    right_hand_side = y_coordinate * sine_value
        + x_coordinate * cosine_value;
    angle += step;

    if (fabsf(left_hand_side - right_hand_side) <= epsilon) {
        cout << "Принадлежит" << endl;
        return 0;
    }

    if (left_hand_side <= right_hand_side &&
        left_hand_side <= radius - right_hand_side) {
        cout << "Не принадлежит: точка находится в "
            "вырезанной области" << endl;
        return 0;
    }

    // Кусок #6
    sine_value = sinf(angle);
    cosine_value = cosf(angle);
    left_hand_side = square_root_of_three
        * fabsf(y_coordinate * cosine_value
                - x_coordinate * sine_value);
    right_hand_side = y_coordinate * sine_value
        + x_coordinate * cosine_value;
    angle += step;

    if (fabsf(left_hand_side - right_hand_side) <= epsilon) {
        cout << "Принадлежит" << endl;
        return 0;
    }

    if (left_hand_side <= right_hand_side &&
        left_hand_side <= radius - right_hand_side) {
        cout << "Не принадлежит: точка находится в "
            "вырезанной области" << endl;
        return 0;
    }

    cout << "Принадлежит" << endl;
}
