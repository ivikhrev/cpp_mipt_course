
#include <algorithm>
#include <ostream>
#include <iostream>
#include <cmath>

template <class T>
class Buffer {
public:
    Buffer(int size = 0, T val = T{}) : data(new T[size]), size(size) {
        std::fill_n(data, size, val);
    }

    template<class It>
    Buffer(int size, It start, It end) : data(new T[size]), size(size) {
        std::copy(start, end, data);
    }


    ~Buffer() {
        delete[] data;
    }

    Buffer(const Buffer &rhs) : data(new T[rhs.size]), size(rhs.size) {
        std::copy_n(rhs.data, size, data);
    }

    template <class U>
    Buffer(const U &rhs) : data(new T[rhs.get_size()]), size(rhs.get_size()) {
        std::copy_n(&rhs[0], size, data);
    }

    Buffer(Buffer &&rhs) noexcept : data(rhs.data), size(rhs.size) {
        rhs.data = nullptr;
    }

    Buffer& operator=(const Buffer &rhs) {
        if (this == &rhs) {
            return *this;
        }
        auto* tmp = new T[rhs.size];
        std::copy_n(rhs.data, rhs.size, tmp);

        delete[] data;
        data = tmp;
        size = rhs.size;

        return *this;
    }

    Buffer& operator=(Buffer &&rhs) noexcept {
        if (this == &rhs) {
            return *this;
        }
        std::swap(this->data, rhs.data);
        size = rhs.size;

        return *this;
    }

    T& operator[](int m) {
        return data[m];
    }

    const T& operator[](int m) const {
        return data[m];
    }

    int get_size() const {
        return size;
    }

private:
    T* data;
    int size;
};

template <class T>
class MatrixBuffer {
private:

    template<class Data>
    struct ProxyRow {
        Data& data;
        int idx;
        T& operator[](int n) { return data[idx + n]; }
        const T& operator[](int n) const { return data[idx + n]; }
    };

public:
    MatrixBuffer(int rows = 0, int cols = 0, T val = T{}) : rows(rows), cols(cols),
        size(rows * cols), data(size, val) {}

    template<class It>
    MatrixBuffer(int rows, int cols, It start, It end) : rows(rows), cols(cols),
        size(rows * cols), data(size, start, end) {}

    template<class U>
    MatrixBuffer(const MatrixBuffer<U>& mb) : rows(mb.nrows()), cols(mb.ncols()), size(rows * cols), data(mb.get_data()) {}

    auto operator[](int row) {
        return ProxyRow<Buffer<T>>{data, row * cols};
    }

    const auto operator[](int row) const {
        return ProxyRow<const Buffer<T>>{data, row * cols};
    }

    int ncols() const { return cols; }
    int nrows() const { return rows; }

    void swap_rows(int m, int n) {
        if (m == n) return;
        for (int i = 0; i < cols; ++i) {
            std::swap((*this)[m][i], (*this)[n][i]);
        }
    }

    Buffer<T>& get_data() {
        return data;
    }

    const Buffer<T>& get_data() const {
        return data;
    }

private:
    int rows;
    int cols;
    int size;
    Buffer<T> data;
};

template <class T>
class MatrixJaggedBuffer {
public:
    MatrixJaggedBuffer(int rows = 0, int cols = 0, T val = T{}) : rows(rows), cols(cols),
        size(rows * cols), data(rows, Buffer<T>{cols, val}) {}

    template<class It>
    MatrixJaggedBuffer(int rows, int cols, It start, It end): rows(rows), cols(cols),
        size(rows * cols), data(rows, Buffer<T>{cols, T{}}) {
        for (int i = 0; i < rows; ++i) {
            data[i] = Buffer<T>(cols, start, start + cols);
            start += cols;
        }
    }

    template<class U>
    MatrixJaggedBuffer(const MatrixJaggedBuffer<U>& mb) : rows(mb.nrows()), cols(mb.ncols()),
        size(rows * cols), data(rows, Buffer<T>{cols, T{}}) {
        for (int i = 0; i < rows; ++i) {
            data[i] = Buffer(mb[i]);
        }
    }

    Buffer<T>& operator[](int i) {
        return data[i];
    }

    const Buffer<T>& operator[](int i) const {
        return data[i];
    }

    int ncols() const { return rows; }
    int nrows() const { return cols; }

    void swap_rows(int m, int n) {
        if (m == n) return;
        std::swap(data[m], data[n]);
    }

    Buffer<Buffer<T>>& get_data() {
        return data;
    }

    const Buffer<Buffer<T>>& get_data() const {
        return data;
    }

private:
    int rows;
    int cols;
    int size;
    Buffer<Buffer<T>> data;
};

template<class T, template<class> class DataRepr = MatrixJaggedBuffer>
class Matrix {
private:
    static constexpr float epsilon = 1e-12;

public:
    Matrix(int rows, int cols, T val = T{}) : data(rows, cols, val) {}

    template<class It>
    Matrix(int rows, int cols, It start, It end) : data(rows, cols, start, end) {}

    Matrix(const DataRepr<T>& data) : data(data) {}

    static Matrix eye(int rows, int cols);

    bool operator==(const Matrix& rhs) const;
    bool operator!=(const Matrix& rhs) const { return !(*this == rhs); }

    auto operator[](int m) -> decltype(DataRepr<T>()[0]){
        return data[m];
    }

    auto operator[](int m) const -> decltype(std::declval<const DataRepr<T>>()[0]) {
        return data[m];
    }

// base
    int ncols() const { return data.ncols(); }
    int nrows() const { return data.nrows(); }
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
    DataRepr<T> data;
};

template<class T, template<class> class DataRepr>
Matrix<T, DataRepr> Matrix<T, DataRepr>::eye(int rows, int cols) {
    if (rows != cols) {
        throw std::runtime_error("Identity is defined only for square matrices");
    }
    Matrix<T, DataRepr> I(rows, cols, 0);
    for (int i = 0; i < rows; ++i) {
        I[i][i] = 1;
    }
    return I;
}


template<class T, template<class> class DataRepr>
void Matrix<T, DataRepr>::swap_rows(int m, int n) {
    data.swap_rows(m, n);
}


template<class T, template<class> class DataRepr>
bool Matrix<T, DataRepr>::operator==(const Matrix& rhs) const {
    if (rhs.nrows() != nrows() && rhs.ncols() != ncols()) {
        return false;
    }

    for (int i = 0; i < nrows(); ++i) {
        for (int j = 0; j < ncols(); ++j) {
            if ((*this)[i][j] != rhs[i][j]) {
                return false;
            }
        }
    }

    return true;
}

template<class T, template<class> class DataRepr>
T Matrix<T, DataRepr>::det() const {
    if (nrows() != ncols()) {
        throw std::runtime_error("Determinant is defined only for square matrices");
    }
    Matrix<double, DataRepr> matrix(data);

    int sign = 1;
    for (int k = 0; k < ncols() - 1; ++k) {
        int row = k + 1;
        for (; row < nrows() && std::abs(matrix[k][k]) < epsilon; ++row) {
            if (matrix[row][k] != 0) {
                matrix.swap_rows(k, row);
                sign *= -1;
                break;
            }
        }
        // if not found any row with non zero element on Kth place
        if (row == nrows()) break;

        for (int i = k + 1; i < nrows(); ++i) {
            double coeff = matrix[i][k] / matrix[k][k];;
            for (int j = k; j < ncols(); ++j) {
                matrix[i][j] -= matrix[k][j] * coeff;
            }
        }
    }

    double d{1};
    for (int i = 0; i < nrows(); ++i) {
        d *= matrix[i][i];
    }

    if constexpr (std::is_same<T, int>::value) {
        return sign * lround(d);
    }

    return sign * d;
}