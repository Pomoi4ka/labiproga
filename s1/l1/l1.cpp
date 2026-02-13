/*
 * Лаба №1
 * Выполнил Максим Соболевский группа 5352 версия 1
 * M01: P(x) = -89,42*x^8 - 543,76*x^2 + 32,76*x;
 * Q(x) = 43,25432*x^15 + 342,321*x^10 + 651,432*x^5 + 54321,7;
 * P(x)/Q(x)
 * M02: Введите x:
 *
 * Для сборки всех версий можно использовать следующий Makefile:
 * all: v1 v2 v3
 *
 * v3 v2 v1: l1.cpp
 * 	$(CXX) -std=c++98 -DV=$(subst v,,$@) -o $@ $< -Wall -Wextra
 */

#include <iostream>
#include <iomanip>
#include <stdio.h>

#ifndef V
#define V 1
#endif

using std::cout;
using std::cin;
using std::endl;
using std::setw;
using std::setfill;
using std::setprecision;

int main()
{
    int m_counter(0);
    float x_raised_to_power_of_one;
    float x_raised_to_power_of_two;
    float x_raised_to_power_of_three;
    float x_raised_to_power_of_five;
    float x_raised_to_power_of_six;

    const float constant_value_first(-89.42f);
    const float constant_value_second(-543.76f);
    const float constant_value_third(32.76f);
    const float constant_value_fourth(43.25432f);
    const float constant_value_fifth(342.321f);
    const float constant_value_sixth(651.432f);
    const float constant_value_seventh(54321.7f);
    float p_value, q_value;

#if V == 1
    int step_counter(0);
    char var_name_for_steps('a' - 1);
    char prev_var_name;

    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "Лаба №1" << endl;
    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "Выполнил Максим Соболевский группа 5352 версия " << V << endl;
    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "P(x) = -89,42*x^8 - 543,76*x^2 + 32,76*x; "
            "Q(x) = 43,25432*x^15 + 342,321*x^10 + 651,432*x^5 + 54321,7; "
            "P(x)/Q(x)" << endl;

    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "Введите x: ";
    cin >> x_raised_to_power_of_one;

    // P(x) = ((-89,42*x^6 - 543,76)*x + 32,76)*x
    // Q(x) = ((43,25432*x^5 + 342,321)*x^5 + 651,432)*x^5 + 54321,7

    x_raised_to_power_of_two =
        x_raised_to_power_of_one * x_raised_to_power_of_one;
    x_raised_to_power_of_three =
        x_raised_to_power_of_one * x_raised_to_power_of_two;
    x_raised_to_power_of_five =
        x_raised_to_power_of_two * x_raised_to_power_of_three;
    x_raised_to_power_of_six =
        x_raised_to_power_of_one * x_raised_to_power_of_five;

    // P(x)

    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << ++step_counter << " шаг "
         << ++var_name_for_steps << " = "
         << "-89,42*x^6 - 543,76 = "
         << setprecision(11)
         << (p_value = constant_value_first
             * x_raised_to_power_of_six + constant_value_second)
         << endl;

    prev_var_name = var_name_for_steps;
    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << ++step_counter << " шаг "
         << ++var_name_for_steps << " = "
         << prev_var_name << "*x + 32,76 = "
         << setprecision(11)
         << (p_value = p_value*x_raised_to_power_of_one +
             constant_value_third)
         << endl;

    prev_var_name = var_name_for_steps;
    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << ++step_counter << " шаг "
         << "P(x) = "
         <<  prev_var_name << "*x = "
         << setprecision(11)
         << (p_value *= x_raised_to_power_of_one)
         << endl;

    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "Для x = " << x_raised_to_power_of_one << setw(20)
         << setfill(' ') << "P = "
         << setprecision(11) << p_value << endl;

    // Q(x)

    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << ++step_counter << " шаг "
         << ++var_name_for_steps << " = "
         << "43,25432*x^5 + 342,321 = "
         << setprecision(11)
         << (q_value = constant_value_fourth
             * x_raised_to_power_of_five + constant_value_fifth)
         << endl;

    prev_var_name = var_name_for_steps;
    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << ++step_counter << " шаг "
         << ++var_name_for_steps << " = "
         << prev_var_name << "*x^5 + 651,432 = "
         << setprecision(11)
         << (q_value = q_value * x_raised_to_power_of_five
             + constant_value_sixth)
         << endl;

    prev_var_name = var_name_for_steps;
    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << ++step_counter << " шаг "
         << ++var_name_for_steps << " = "
         << prev_var_name << "*x^5 + 54321,7 = "
         << setprecision(11)
         << (q_value = q_value * x_raised_to_power_of_five
             + constant_value_seventh)
         << endl;

    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "Для x = " << x_raised_to_power_of_one << setw(20)
         << setfill(' ') << "Q = " << setprecision(11)
         << q_value << endl;

    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "Для x = " << x_raised_to_power_of_one << setw(20)
         << setfill(' ') << "P/Q = " << setprecision(11)
         << p_value / q_value << endl;

#elif V == 2

    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "Лаба №1" << endl;
    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "Выполнил Максим Соболевский группа 5352 версия " << V << endl;
    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "P(x) = -89,42*x^8 - 543,76*x^2 + 32,76*x; "
            "Q(x) = 43,25432*x^15 + 342,321*x^10 + 651,432*x^5 + 54321,7; "
            "P(x)/Q(x)" << endl;

    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "Введите x: ";
    cin >> x_raised_to_power_of_one;

    // P(x) = ((-89,42*x^6 - 543,76)*x + 32,76)*x
    // Q(x) = ((43,25432*x^5 + 342,321)*x^5 + 651,432)*x^5 + 54321,7

    x_raised_to_power_of_two =
        x_raised_to_power_of_one * x_raised_to_power_of_one;
    x_raised_to_power_of_three =
        x_raised_to_power_of_one * x_raised_to_power_of_two;
    x_raised_to_power_of_five =
        x_raised_to_power_of_two * x_raised_to_power_of_three;
    x_raised_to_power_of_six =
        x_raised_to_power_of_one * x_raised_to_power_of_five;

    // P(x)

    p_value = constant_value_first * x_raised_to_power_of_six
        + constant_value_second;
    p_value = p_value * x_raised_to_power_of_one
        + constant_value_third;
    p_value *= x_raised_to_power_of_one;

    m_counter = 7;
    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "Для x = " << x_raised_to_power_of_one << setw(20)
         << setfill(' ') << "P = " << setprecision(11)
         << p_value << endl;

    // Q(x)

    q_value = constant_value_fourth * x_raised_to_power_of_five
        + constant_value_fifth;
    q_value = q_value * x_raised_to_power_of_five
        + constant_value_sixth;
    q_value = q_value * x_raised_to_power_of_five
        + constant_value_seventh;

    m_counter = 11;
    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "Для x = " << x_raised_to_power_of_one << setw(20)
         << setfill(' ') << "Q = " << setprecision(11)
         << q_value << endl;

    cout << "M" << setw(2) << setfill('0') << ++m_counter << ": "
         << "Для x = " << x_raised_to_power_of_one << setw(20)
         << setfill(' ') << "P/Q = " << setprecision(11)
         << p_value / q_value << endl;

#elif V == 3
    int step_counter(0);
    char var_name_for_steps('a' - 1);
    char prev_var_name;

    printf("M%02d: Лаба №1\n", ++m_counter);
    printf("M%02d: Выполнил Максим Соболевский группа 5352 версия %d\n", ++m_counter, V);
    printf("M%02d: P(x) = -89,42*x^8 - 543,76*x^2 + 32,76*x; "
           "Q(x) = 43,25432*x^15 + 342,321*x^10 + 651,432*x^5 + 54321,7; "
           "P(x)/Q(x)\n", ++m_counter);

    printf("M%02d: Введите x: ", ++m_counter);
    scanf("%f", &x_raised_to_power_of_one);

    // P(x) = ((-89,42*x^6 - 543,76)*x + 32,76)*x
    // Q(x) = ((43,25432*x^5 + 342,321)*x^5 + 651,432)*x^5 + 54321,7

    x_raised_to_power_of_two =
        x_raised_to_power_of_one * x_raised_to_power_of_one;
    x_raised_to_power_of_three =
        x_raised_to_power_of_one * x_raised_to_power_of_two;
    x_raised_to_power_of_five =
        x_raised_to_power_of_two * x_raised_to_power_of_three;
    x_raised_to_power_of_six =
        x_raised_to_power_of_one * x_raised_to_power_of_five;

    // P(x)

    printf("M%02d: %d шаг %c = -89,42*x^6 - 543,76 = %.11f\n",
           ++m_counter,
           ++step_counter, ++var_name_for_steps,
           p_value = constant_value_first
           * x_raised_to_power_of_six + constant_value_second);

    prev_var_name = var_name_for_steps;
    printf("M%02d: %d шаг %c = %c"
           "*x + 32,76 = "
           "%.11f\n",
           ++m_counter,
           ++step_counter, ++var_name_for_steps,
           prev_var_name,
           p_value = p_value*x_raised_to_power_of_one
           + constant_value_third);

    prev_var_name = var_name_for_steps;
    printf("M%02d: %d шаг %c = %c"
           "*x = "
           "%.11f\n",
           ++m_counter,
           ++step_counter, ++var_name_for_steps,
           prev_var_name,
           p_value *= x_raised_to_power_of_one);

    printf("M%02d: Для x = %.11f%6sP = %.11f\n",
           ++m_counter,
           x_raised_to_power_of_one,
           "", p_value);

    // Q(x)

    prev_var_name = var_name_for_steps;
    printf("M%02d: %d шаг %c = "
           "43,25432*x^5 + 342,321 = "
           "%.11f\n",
           ++m_counter,
           ++step_counter, ++var_name_for_steps,
           q_value = constant_value_fourth * x_raised_to_power_of_five
           + constant_value_fifth);

    prev_var_name = var_name_for_steps;
    printf("M%02d: %d шаг %c = %c"
           "*x^5 + 651,432 = "
           "%.11f\n",
           ++m_counter,
           ++step_counter, ++var_name_for_steps, prev_var_name,
           q_value = q_value * x_raised_to_power_of_five
           + constant_value_sixth);

    prev_var_name = var_name_for_steps;
    printf("M%02d: %d шаг %c = %c"
           "*x^5 + 54321,7 = "
           "%.11f\n",
           ++m_counter,
           ++step_counter, ++var_name_for_steps, prev_var_name,
           q_value = q_value * x_raised_to_power_of_five
           + constant_value_seventh);

    printf("M%02d: Для x = %.11f%6sQ = %.11f\n",
           ++m_counter,
           x_raised_to_power_of_one,
           "", q_value);
    printf("M%02d: Для x = %.11f%6sP/Q = %.11f\n", 
           ++m_counter,
           x_raised_to_power_of_one,
           "", p_value / q_value);

#endif
}
