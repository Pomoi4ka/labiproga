#include <iostream>
#include <fstream>
#include <iomanip>

template <typename... Args>
void eprintln(Args&&... args)
{
    ((std::cerr << args), ...);
    std::cerr << std::endl;
}

struct Matrix {
    int *items;
    size_t rows, cols;
    size_t stride, cap;
};

bool file_read_number_until_eol(std::ifstream &f, int &x)
{
    f >> std::noskipws;
    while (!f.eof()) {
        if (f >> x) return true;
        if (f.clear(), f.get() == '\n') return false;
    }
    return false;
}

void mat_update_stride(Matrix &mat, size_t new_stride)
{
    if (new_stride <= mat.stride) return;

    size_t cap = (mat.rows + 1) * new_stride;
    int *new_items = new int[cap];
    for (size_t i = 0; i < mat.cap && mat.stride; ++i) {
        const size_t row = i / mat.stride;
        const size_t col = i % mat.stride;
        new_items[row * new_stride + col] = mat.items[i];
    }
    delete[] mat.items;
    mat.items = new_items;
    mat.stride = new_stride;
    mat.cap = cap;
}

void mat_reserve(Matrix &mat)
{
    size_t old_cap = mat.cap;
    size_t count = mat.rows * mat.stride;

    if (count < mat.cap) return;

    while (mat.cap <= count) {
        mat.cap = mat.cap ? mat.cap * 2 : 1;
    }

    int *new_items = new int[mat.cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_items[i] = mat.items[i];
    delete[] mat.items;
    mat.items = new_items;
}

void file_read_row(std::ifstream &f, Matrix &mat)
{
    int i, element;
    for (i = 0; !f.eof(); ) {
        if (!file_read_number_until_eol(f, element)) break;
        if (mat.cols && (size_t) i >= mat.cols) continue;

        mat_update_stride(mat, i + 1);
        mat.items[mat.stride * mat.rows + i] = element;
        ++i;
    }
    if (!i) return;
    mat.cols = i;
    mat.rows++;
    mat_reserve(mat);
}

bool file_read_matrix(const char *path, Matrix &mat)
{
    std::ifstream f(path);
    if (!f) {
        eprintln("Проблема с файлом: ", path);
        return false;
    }

    while (!f.eof()) {
        file_read_row(f, mat);
    }
    return true;
}

void file_write_matrix_row(std::ofstream &f, const int *row, size_t cols)
{
    for (size_t i = 0; i < cols; ++i) {
        if (i > 0) f << " ";
        f << std::setw(4) << row[i];
    }
    f << "\n";
}

bool file_write_matrix(
        const char *path,
        Matrix const &mat,
        Matrix const &sorted)
{
    std::ofstream f(path);

    if (!f) {
        eprintln("Проблема с файлом: ", path);
        return false;
    }

    for (size_t i = 0; i < mat.rows; ++i) {
        file_write_matrix_row(f, &mat.items[i * mat.stride], mat.cols);
    }
    f << "------" << std::endl;
    for (size_t i = 0; i < sorted.rows; ++i) {
        file_write_matrix_row(f, &sorted.items[i * sorted.stride], sorted.cols);
    }
    return true;
}

int &mat_at(Matrix &mat, size_t i)
{
    const int row = i / mat.cols;
    const int col = i % mat.cols;
    return mat.items[row * mat.stride + col];
}

void mat_sort_even(Matrix &mat)
{
    const size_t n = mat.cols * mat.rows;
    for (size_t i = 0, j = n;;) {
        while (i < n && mat_at(mat, i) % 2 == 0) ++i;
        do --j; while (j && mat_at(mat, j) % 2 != 0);
        if (i >= j) break;
        mat_at(mat, i)
            ^= mat_at(mat, j)
            ^= mat_at(mat, i) 
            ^= mat_at(mat, j);
    }
}

Matrix mat_clone(Matrix const &mat)
{
    Matrix m = mat;
    m.cap = 0;
    m.items = nullptr;
    mat_reserve(m);
    for (size_t i = 0; i < mat.cap && i < m.cap; ++i)
        m.items[i] = mat.items[i];
    return m;
}

int main()
{
    bool ret;
    Matrix mat{}, mat_sorted;
    ret = file_read_matrix("input.txt", mat);
    if (!ret) return 1;
    mat_sorted = mat_clone(mat);
    mat_sort_even(mat_sorted);
    ret = file_write_matrix("output.txt", mat, mat_sorted);
    delete[] mat.items;
    delete[] mat_sorted.items;
    return !ret;
}
