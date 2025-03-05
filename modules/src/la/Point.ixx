// Copyright 2025 by Amir Nourinia

module;
#include <array>
#include <concepts>
#include <initializer_list>
#include <type_traits>
#include <vector>
export module Geometry:Point;

import :Vector;
import :Matrix;

export namespace geometry {

template <typename T = double, size_t D = 3, bool is_static = true>
  requires std::is_arithmetic_v<T>
class Point {

  using UnderlyingDataType =
      std::conditional_t<is_static, std::array<T, D>, std::vector<T>>;

 public:
  Point() {
    if constexpr (is_static) {
      for (auto i = 0; i < D; ++i) {
        m_coordinates.at(i) = T{};
      }
    } else {
      m_coordinates = std::vector<T>(D, 0);
    }
  }

  Point(UnderlyingDataType&& initList) {
    static_assert(initList.size() == D);
    m_coordinates = std::move(initList);
  }

  explicit Point(T const x)
    requires(D == 1)
      : m_coordinates{1} {
    m_coordinates[0] = x;
  }

  Point(T const x, T const y)
    requires(D == 2)
      : m_coordinates(2) {
    m_coordinates[0] = x;
    m_coordinates[1] = y;
  }

  Point(T const x, T const y, T const z)
    requires(D == 3)
  {
    m_coordinates[0] = x;
    m_coordinates[1] = y;
    m_coordinates[2] = z;
  }

  Point(T const x, T const y, T const z, T const t)
    requires(D == 4)
  {
    m_coordinates[0] = x;
    m_coordinates[1] = y;
    m_coordinates[2] = z;
    m_coordinates[3] = t;
  }
  // getters

  // access operators
  [[nodiscard]] T& operator[](size_t const i) { return m_coordinates.at(i); }

  [[nodiscard]] T const& operator[](size_t const i) const {
    return m_coordinates.at(i);
  }

  [[nodiscard]] constexpr size_t dimension() const noexcept { return D; }

  [[nodiscard]] constexpr bool isStatic() const noexcept { return is_static; }

  // arithmetic operators
  [[nodiscard]] Vector<T, D, is_static> operator-(Point rhs) const {
    Point const& self = *this;
    // left difference
    UnderlyingDataType diff;
    if constexpr (not is_static) {
      diff = std::vector<T>(D, 0);
    }
    for (size_t i = 0; i < D; ++i) {
      diff[i] = self[i] - rhs[i];
    }
    Vector<T, D, is_static> diffVector = std::move(diff);
    return diffVector;
  }

  [[nodiscard]] bool collinear(Point const& lhs,
                               Point const& rhs) const noexcept
    requires(D > 1)
  {
    Point const& self = *this;
    Vector rhsDiff = self - rhs;
    Vector lhsDiff = self - lhs;
    bool const areParallel = rhsDiff.isParallel(lhsDiff);
    return areParallel;
  }

  [[nodiscard]] bool counterClockWiseTurn(Point const& lhs, Point const& rhs)
    requires(D == 2)
  {}

 private:
  UnderlyingDataType m_coordinates;
  using FieldType = T;
};

// template<typename Element>
// Point(std::initializer_list<Element>&& initList)-> Point<Element, calculateSize(initList)>;

}  // namespace geometry
