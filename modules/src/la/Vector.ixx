//
// Created by amirn on 04/03/2025.
//
module;
#include <array>
#include <concepts>
#include <valarray>
#include <vector>
#include <cstddef>
export module Geometry:Vector;

export namespace geometry {

template <typename T, size_t D, bool is_static = true>
  requires std::is_arithmetic_v<T> && (D > 0)
class Vector {

  using UnderlyingType =
      std::conditional_t<is_static, std::array<T, D>, std::vector<T>>;
  using GeneralizedUnderlyingField =
      std::conditional_t<std::is_floating_point_v<T>, long double, double>;

 public:
  // Constructors
  Vector() {
    if constexpr (not is_static) {
      m_elements = std::vector<T>(D, 0);
    } else {
      for (size_t i = 0; i < D; ++i) {
        m_elements[i] = 0;
      }
    }
  }

  Vector(UnderlyingType const& elements) {
    if constexpr (is_static) {
      static_assert(elements.size() == D);  // static check
    } else {
      assert(elements.size() == D);  // dynamic check
    }
    for (size_t i = 0; i < D; ++i) {
      m_elements[i] = elements[i];
    }
  }

  Vector(UnderlyingType&& elements) {
    UnderlyingType temp = std::forward<UnderlyingType>(elements);
    if constexpr (is_static) {
      static_assert(temp.size() == D);  // static check
    } else {
      assert(temp.size() == D);  // dynamic check
    }
    m_elements = std::move(temp);
  }

  // copy ctor
  Vector(Vector const& other) : Vector() {
    for (size_t i = 0; i < D; ++i) {
      m_elements[i] = other[i];
    }
  }

  Vector& operator=(Vector const& rhs) {
    // initialize the vector
    if constexpr (not is_static) {
      m_elements = std::vector<T>(D, 0);
    }
    for (size_t i = 0; i < D; ++i) {
      m_elements[i] = rhs[i];
    }
    return *this;
  }

  // getters
  [[nodiscard]] T& operator[](size_t const index) {
    return m_elements.at(index);
  }
  [[nodiscard]] T const& operator[](size_t const index) const {
    return m_elements.at(index);
  }

  [[nodiscard]] Vector unit() const noexcept {
    Vector result = *this;  // deep copies
    return result.normalize();
  }

  [[nodiscard]] bool isParallel(Vector const& other) const noexcept {
    auto const& self = *this;
    auto const selfUnitVector = self.unit();
    auto const otherUnitVector = other.unit();

    GeneralizedUnderlyingField const result =
        std::sqrt(std::abs(selfUnitVector.dot(otherUnitVector)));

    // calculate error bound
    GeneralizedUnderlyingField const propagatedErrorBound =
        std::sqrt(D) *
        std::numeric_limits<GeneralizedUnderlyingField>::epsilon();

    return std::abs(result - 1) <= propagatedErrorBound;
  }

  [[nodiscard]] GeneralizedUnderlyingField norm() const noexcept {
    Vector const& self = *this;
    auto result = self.dot(self);
    GeneralizedUnderlyingField const norm = std::sqrt(result);
    return norm;
  }

  /**
   * inner product
   * @param other
   * @return the dot product
   */
  [[nodiscard]] GeneralizedUnderlyingField dot(
      Vector const& other) const noexcept {
    Vector const& self = *this;
    GeneralizedUnderlyingField result = 0;
    for (size_t i = 0; i < D; ++i) {
      result += self[i] * other[i];
    }
    return result;
  }

  /**
  * inner product
  * @param other
  * @return the dot product
  */
  [[nodiscard]] GeneralizedUnderlyingField innerProduct(
      Vector const& other) const noexcept {
    return dot(other);
  }

  // /**
  //  * Outer product
  //  * @param other
  //  * @return
  //  */
  // [[nodiscard]] Vector outerProduct(Vector const& other) const noexcept {
  //   throw std::exception{"Not implemented"};
  //   return Vector();
  // }
  //
  // [[nodiscard]] Vector operator*(Vector const& other) const noexcept {
  //   return outerProduct(other);
  // }

  // also cross product

  // setters
  // normalizes the vector in place
  Vector& normalize() noexcept {
    Vector& self = *this;
    GeneralizedUnderlyingField const norm = self.norm();
    for (size_t i = 0; i < D; ++i) {
      self[i] /= norm;
    }
    return *this;
  }

 private:
  UnderlyingType m_elements;
};

}  // namespace geometry