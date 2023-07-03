
#include <algorithm>
#include <ostream>
#include <iostream>
#include <cmath>

template<class T>
class Matrix {
private:
    static constexpr float epsilon = 1e-12;
    struct ProxyRow {
        T  *row;
        const T& operator[](int n) const { return row[n]; }
        T& operator[](int n) { return row[n]; }
    };

public:
    Matrix(int rows, int cols, T val = T{});

    template<class It>
    Matrix(int rows, int cols, It start, It end);

    static Matrix eye(int rows, int cols);

    Matrix(const Matrix &rhs);
    Matrix(Matrix &&rhs) noexcept;

    Matrix& operator=(const Matrix &rhs);
    Matrix& operator=(Matrix &&rhs) noexcept;
    bool operator==(const Matrix& rhs) const;
    bool operator!=(const Matrix& rhs) const { return !(*this == rhs); }

    ProxyRow operator[](int m) {
        return {data + m * cols};
    }

    const ProxyRow operator[](int m) const {
        return {data + m * cols};
    }

    ~Matrix();


// base
    int ncols() const { return cols; }
    int nrows() const { return rows; }
    void swap_rows(int i, int j);

// aggregate TBD
    // T trace() const;
    // bool equal(const Matrix& other) const;
    // bool less(const Matrix& other) const;
    // void print(std::ostream& os) const;

    // Matrix& negate() &;
    // Matrix& transpose() &;
    T det() const;

private:
    T *data;
    int rows;
    int cols;
    int size;

};

template<class T>
Matrix<T>::Matrix(int rows, int cols, T val) : data(new T[rows * cols]),
    rows(rows), cols(cols), size(rows * cols) {
    std::fill_n(data, rows * cols, val);
}

template<class T>
template<class It>
Matrix<T>::Matrix(int rows, int cols, It start, It end) : data(new T[rows * cols]),
    rows(rows), cols(cols), size(rows * cols) {
    std::copy(start, end, data);
}

template<class T>
Matrix<T> Matrix<T>::eye(int rows, int cols) {
    if (rows != cols) {
        throw std::runtime_error("Identity is defined only for square matrices");
    }
    Matrix<T> I(rows, cols, 0);
    for (int i = 0; i < rows; ++i) {
        I[i][i] = 1;
    }
    return I;
}

template<class T>
Matrix<T>::Matrix(const Matrix &rhs) : data(new T[rhs.size]), rows(rhs.rows), cols(rhs.cols), size(rhs.size) {
    std::copy_n(rhs.data, size, data);
}

template<class T>
Matrix<T>::Matrix(Matrix &&rhs) noexcept : data(rhs.data), rows(rhs.rows), cols(rhs.cols), size(rhs.size) {
    rhs.data = nullptr;
}

template<class T>
Matrix<T>::~Matrix() {
    delete[] data;
}

template<class T>
void Matrix<T>::swap_rows(int m, int n) {
    if (m == n) return;
    for (int i = 0; i < cols; ++i) {
        std::swap((*this)[m][i], (*this)[n][i]);
    }
}

template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix &rhs) {
    if (this == &rhs) {
        return *this;
    }

    auto* tmp = new T[rhs.size];
    std::copy_n(rhs.data, rhs.size, tmp);

    delete[] data;
    data = tmp;
    size = rhs.size;
    rows = rhs.rows;
    cols = rhs.cols;

    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator=(Matrix &&rhs) noexcept {
    if (this == &rhs) {
        return *this;
    }

    std::swap(this->data, rhs.data);
    size = rhs.size;
    rows = rhs.rows;
    cols = rhs.cols;

    return *this;
}

template<class T>
bool Matrix<T>::operator==(const Matrix& rhs) const {
    if (rhs.rows != rows && rhs.cols != cols) {
        return false;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if ((*this)[i][j] != rhs[i][j]) {
                return false;
            }
        }
    }

    return true;
}

template<class T>
T Matrix<T>::det() const {
    if (rows != cols) {
        throw std::runtime_error("Determinant is defined only for square matrices");
    }
    Matrix<double> matrix(cols, rows, data, data + size);

    int sign = 1;
    for (int k = 0; k < cols - 1; ++k) {
        int row = k + 1;
        for (; row < rows && std::abs(matrix[k][k]) < epsilon; ++row) {
            if (matrix[row][k] != 0) {
                matrix.swap_rows(k, row);
                sign *= -1;
                break;
            }
        }
        // if not found any row with non zero element on Kth place
        if (row == rows) break;

        for (int i = k + 1; i < rows; ++i) {
            double coeff = matrix[i][k] / matrix[k][k];;
            for (int j = k; j < cols; ++j) {
                matrix[i][j] -= matrix[k][j] * coeff;
            }
        }
    }

    double d{1};
    for (int i = 0; i < rows; ++i) {
        d *= matrix[i][i];
    }

    if constexpr (std::is_same<T, int>::value) {
        return sign * lround(d);
    }

    return sign * d;
}