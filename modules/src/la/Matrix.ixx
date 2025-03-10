//
// Created by amirn on 04/03/2025.
//

module;
#include <array>
#include <concepts>
#include <memory>
#include <ranges>
#include <vector>
export module Geometry:Matrix;

export namespace geometry {

/**
 *
 *
 *
 * Notes:
 * -  Adding modifier methods i.e. non-const methods should always set m_matrix_modified
 *    to true. This is the least garauntee that I can give to constness and efficiency of the calculations.
 *
 *
 *
 *
 * @tparam T
 * @tparam r
 * @tparam c
 * @tparam row_major
 * @tparam is_static
 */
template <typename T, size_t r, size_t c, bool row_major = true,
          bool is_static = true>
  requires std::is_arithmetic_v<T>
class Matrix {

  using UnderlyingDataType =
      std::conditional_t<is_static, std::array<T, r * c>, std::vector<T>>;

  using GeneralizedUnderlyingField =
      std::conditional_t<std::is_floating_point_v<T>, long double, double>;

  struct LUDecomposition {
    std::unique_ptr<Matrix> P;
    std::unique_ptr<Matrix> L;
    std::unique_ptr<Matrix> U;
  };

  struct SVDecomposition {
    std::unique_ptr<Matrix> S;
    std::unique_ptr<Matrix> V;
  };

 public:
  Matrix() {
    if constexpr (is_static) {
      for (size_t i = 0; i < m_matrix_size; ++i) {
        m_data.at(i) = 0;
      }
    } else {
      m_data = std::vector<T>(m_matrix_size, 0);
    }
    m_matrix_has_been_modified = true;
  }

  Matrix(Matrix const& other) : Matrix() {
    // if it is vector create a vector of that size
    // if constexpr (not is_static) {
    //   m_data = std::vector<T>(m_matrixSize, 0);
    // }
    for (size_t i = 0; i < m_matrix_size; ++i) {
      m_data.at(i) = other.m_data.at(i);
    }
    m_matrix_has_been_modified = true;
  }

  // getters
  T& operator[](size_t const i, size_t const j) {
    size_t const index = mappedIndex(i, j);
    m_matrix_has_been_modified = true;
    return m_data.at(index);
  }

  T const& operator[](size_t const i, size_t const j) const {
    size_t const index = mappedIndex(i, j);
    return m_data.at(index);
  }

 private:
  [[nodiscard]] static size_t mappedIndex(size_t const i, size_t const j) {
    if constexpr (row_major) {
      size_t const index = (i * r) + j;
      return index;
    } else {
      size_t const index = (j * c) + i;
      return index;
    }
  }

 public:
  std::ranges::random_access_range auto column(size_t const j) const noexcept {
    using namespace std::ranges::views;
    size_t skip_elements = j * r;
    size_t strd = 0ul;
    if constexpr (row_major) {
      strd = c;
    } else {
      strd = 1;
    }
    return m_data | stride(strd) | drop(skip_elements) | take(r) | as_const;
  }

  std::ranges::random_access_range auto column(size_t const j) noexcept {
    using namespace std::ranges::views;
    size_t skip_elements = j * r;
    size_t strd = 0ul;
    if constexpr (row_major) {
      strd = c;
    } else {
      strd = 1;
    }
    m_matrix_has_been_modified = true;
    return m_data | stride(strd) | drop(skip_elements) | take(r);
  }

  /**
   * Get a copy of column
   * @param j
   * @return copy of column j
   */
  std::vector<T> columnAsVector(size_t const j) const noexcept {
    return std::ranges::to<std::vector<T>>(column(j));
  }

  std::ranges::random_access_range auto row(size_t const i) const noexcept {
    using namespace std::ranges::views;
    size_t skip_elements = i * c;
    size_t strd = 0ul;
    if constexpr (row_major) {
      strd = 1;
    } else {
      strd = r;
    }
    return m_data | stride(strd) | drop(skip_elements) | take(c) | as_const;
  }

  std::ranges::random_access_range auto row(size_t const i) noexcept {
    using namespace std::ranges::views;
    size_t skip_elements = i * c;
    size_t strd = 0ul;
    if constexpr (row_major) {
      strd = 1;
    } else {
      strd = r;
    }
    m_matrix_has_been_modified = true;
    return m_data | stride(strd) | drop(skip_elements) | take(c);
  }

  /**
   * Get a copy of the row
   * @param i
   * @return a vector of row i
   */
  std::vector<T> rowAsVector(size_t const i) const noexcept {
    return std::ranges::to<std::vector<T>>(row(i));
  }

  // setters
  void transpose() noexcept {
    UnderlyingDataType newData;
    if constexpr (not is_static) {
      newData = std::vector<T>(m_matrix_size, 0);
    }
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        newData.at(j, i) = m_data.at(i, j);
      }
    }
    m_data = std::move(newData);
    m_matrix_has_been_modified = true;
  }

  // arithmetic

  /**
   * Calculates the determinant of the matrix, uses LU decomposition. In case
   * it is calculated, uses the old calculated values. Hence, a const instance
   * can be utilized efficiently.
   * @return
   */
  [[nodiscard]] GeneralizedUnderlyingField determinant() const noexcept
    requires(r == c)
  {
    if (m_matrix_has_been_modified || m_lu_decomposition == nullptr) lud();

    auto const& [p, l, u] = *m_lu_decomposition;

    GeneralizedUnderlyingField product = 0;

    for (size_t i = 0; i < r; ++i) {
      product += l[i, i] * u[i, i] * p[i, i];
    }

    return product;
  }

private:
  /**
   * Calculates the LU Decomposition of the matrix
   */
  void lud() noexcept
    requires(r == c)
  {
    // TODO
    m_lu_decomposition = std::make_unique<LUDecomposition>();
    m_matrix_has_been_modified = false;
  }

  void svd() noexcept {
    // TODO
    m_svd_decomposition = std::make_unique<SVDecomposition>();
    m_matrix_has_been_modified = false;
  }

  void inverse() noexcept requires (r == c)
  {
    // TODO
    m_inverse_matrix = std::make_unique<Matrix>();
    m_matrix_has_been_modified = false;
  }

 private:
  UnderlyingDataType m_data;
  size_t const m_matrix_size = r * c;
  std::unique_ptr<LUDecomposition> m_lu_decomposition;
  std::unique_ptr<SVDecomposition> m_svd_decomposition;
  std::unique_ptr<Matrix> m_inverse_matrix;
  bool m_matrix_has_been_modified = false;
};

}  // namespace geometry