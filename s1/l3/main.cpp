#include <iostream>
#include <iomanip>
#include <fstream>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::setw;

int main()
{
    const char *const separator = "|";
    int n;
    float sequence_term, z_value, sequence_sum,
          epsilon, sum_difference, z_value_square;
    std::ofstream table("table.txt");

    if (!table) {
        cerr << "ОШИБКА: проблема с файлом" << endl;
        return 1;
    }

    cout << std::setprecision(12);
    table << std::setprecision(12);

    cout << "Лаба №3" << endl
         << "Выполнил Соболевский Максим гр. 5352" << endl;
    n = 0; while (n < 3) {
        cout << "Введите эпсилон (0; 1e-2] = ";
        cin >> epsilon;
        if (0 < epsilon && epsilon <= 1e-2) break;
        cerr << "Неверное значение epsilon. " << endl;
        n++;
    }

    if (n == 3) {
        cerr << "ЗАМЕЧАНИЕ: Использование неверных данных"
             << endl;
        return 1;
    }

    n = 0; while (n < 3) {
        cout << "Введите z [-1; 1] = ";
        cin >> z_value;
        if ((z_value < 0 ? -z_value : z_value) <= 1) break;
        cerr << "Неверное значение z. " << endl;
        n++;
    }

    if (n == 3) {
        cerr << "ЗАМЕЧАНИЕ: Использование неверных данных"
             << endl;
        return 1;
    }

    z_value_square = z_value * z_value;
    n = 0;
    sequence_sum = sequence_term = z_value;

    table << setw(24) << "n" << separator
          << setw(24) << "a_n" << separator
          << setw(24) << "S" << endl;
    do {
        table << setw(24) << n << separator
              << setw(24) << sequence_term << separator
              << setw(24) << sequence_sum << endl;

        sequence_term *= 1 - (4.*n+3)/(4.*n*n + 8.*n + 4);
        sequence_term *= 1 - 1./(2.*n + 3.);
        sequence_term *= z_value_square;
        sequence_sum += sequence_term;

        n++;
        sum_difference = sequence_term;
        if (sum_difference < 0)
            sum_difference *= -1;
    } while (sum_difference >= epsilon);

    table << setw(24) << n << separator
          << setw(24) << sequence_term << separator
          << setw(24) << sequence_sum << endl;

    cout << "S = " << sequence_sum << endl;
    cout << "N = " << n << endl;
}
