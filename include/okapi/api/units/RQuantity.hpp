/*
 * @author Mikhail Semenov
 * @author Benjamin Jurke
 * @author Ryan Benasutti, WPI
 * @author Wesley Chalmers
 *
 * This code is a modified version of Benjamin Jurke's work in 2015. You can read his blog post
 * here:
 * https://benjaminjurke.com/content/articles/2015/compile-time-numerical-unit-dimension-checking/
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <cmath>
#include <ratio>

namespace okapi {
template <typename MassDim, typename LengthDim, typename TimeDim, typename AngleDim>
class RQuantity {
  public:
  explicit constexpr RQuantity() : value(0.0) {
  }

  explicit constexpr RQuantity(double val) : value(val) {
  }

  explicit constexpr RQuantity(long double val) : value(static_cast<double>(val)) {
  }

  // The intrinsic operations for a quantity with a unit is addition and subtraction
  constexpr RQuantity const &operator+=(const RQuantity &rhs) {
    value += rhs.value;
    return *this;
  }

  constexpr RQuantity const &operator-=(const RQuantity &rhs) {
    value -= rhs.value;
    return *this;
  }

  constexpr RQuantity operator-() {
    return RQuantity(value * -1);
  }

  constexpr RQuantity const &operator*=(const double rhs) {
    value *= rhs;
    return *this;
  }

  constexpr RQuantity const &operator/=(const double rhs) {
    value /= rhs;
    return *this;
  }

  // Returns the value of the quantity in multiples of the specified unit
  constexpr double convert(const RQuantity &rhs) const {
    return value / rhs.value;
  }

  // returns the raw value of the quantity (should not be used)
  constexpr double getValue() const {
    return value;
  }

  constexpr RQuantity<MassDim, LengthDim, TimeDim, AngleDim> abs() const {
    return RQuantity<MassDim, LengthDim, TimeDim, AngleDim>(std::fabs(value));
  }

  constexpr RQuantity<std::ratio_divide<MassDim, std::ratio<2>>,
                      std::ratio_divide<LengthDim, std::ratio<2>>,
                      std::ratio_divide<TimeDim, std::ratio<2>>,
                      std::ratio_divide<AngleDim, std::ratio<2>>>
  sqrt() const {
    return RQuantity<std::ratio_divide<MassDim, std::ratio<2>>,
                     std::ratio_divide<LengthDim, std::ratio<2>>,
                     std::ratio_divide<TimeDim, std::ratio<2>>,
                     std::ratio_divide<AngleDim, std::ratio<2>>>(std::sqrt(value));
  }

  private:
  double value;
};

// Predefined (physical unit) quantity types:
// ------------------------------------------
#define QUANTITY_TYPE(_Mdim, _Ldim, _Tdim, _Adim, name)                                            \
  typedef RQuantity<std::ratio<_Mdim>, std::ratio<_Ldim>, std::ratio<_Tdim>, std::ratio<_Adim>>    \
    name;

// Unitless
QUANTITY_TYPE(0, 0, 0, 0, Number)
constexpr Number number(1.0);

// Standard arithmetic operators:
// ------------------------------
template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, A> operator+(const RQuantity<M, L, T, A> &lhs,
                                          const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, A>(lhs.getValue() + rhs.getValue());
}
template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, A> operator-(const RQuantity<M, L, T, A> &lhs,
                                          const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, A>(lhs.getValue() - rhs.getValue());
}
template <typename M1,
          typename L1,
          typename T1,
          typename A1,
          typename M2,
          typename L2,
          typename T2,
          typename A2>
constexpr RQuantity<std::ratio_add<M1, M2>,
                    std::ratio_add<L1, L2>,
                    std::ratio_add<T1, T2>,
                    std::ratio_add<A1, A2>>
operator*(const RQuantity<M1, L1, T1, A1> &lhs, const RQuantity<M2, L2, T2, A2> &rhs) {
  return RQuantity<std::ratio_add<M1, M2>,
                   std::ratio_add<L1, L2>,
                   std::ratio_add<T1, T2>,
                   std::ratio_add<A1, A2>>(lhs.getValue() * rhs.getValue());
}
template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, A> operator*(const double &lhs, const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, A>(lhs * rhs.getValue());
}
template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, A> operator*(const RQuantity<M, L, T, A> &lhs, const double &rhs) {
  return RQuantity<M, L, T, A>(lhs.getValue() * rhs);
}
template <typename M1,
          typename L1,
          typename T1,
          typename A1,
          typename M2,
          typename L2,
          typename T2,
          typename A2>
constexpr RQuantity<std::ratio_subtract<M1, M2>,
                    std::ratio_subtract<L1, L2>,
                    std::ratio_subtract<T1, T2>,
                    std::ratio_subtract<A1, A2>>
operator/(const RQuantity<M1, L1, T1, A1> &lhs, const RQuantity<M2, L2, T2, A2> &rhs) {
  return RQuantity<std::ratio_subtract<M1, M2>,
                   std::ratio_subtract<L1, L2>,
                   std::ratio_subtract<T1, T2>,
                   std::ratio_subtract<A1, A2>>(lhs.getValue() / rhs.getValue());
}
template <typename M, typename L, typename T, typename A>
constexpr RQuantity<std::ratio_subtract<std::ratio<0>, M>,
                    std::ratio_subtract<std::ratio<0>, L>,
                    std::ratio_subtract<std::ratio<0>, T>,
                    std::ratio_subtract<std::ratio<0>, A>>
operator/(const double &x, const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<std::ratio_subtract<std::ratio<0>, M>,
                   std::ratio_subtract<std::ratio<0>, L>,
                   std::ratio_subtract<std::ratio<0>, T>,
                   std::ratio_subtract<std::ratio<0>, A>>(x / rhs.getValue());
}
template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, A> operator/(const RQuantity<M, L, T, A> &rhs, const double &x) {
  return RQuantity<M, L, T, A>(rhs.getValue() / x);
}

// Comparison operators for quantities:
// ------------------------------------
template <typename M, typename L, typename T, typename A>
constexpr bool operator==(const RQuantity<M, L, T, A> &lhs, const RQuantity<M, L, T, A> &rhs) {
  return (lhs.getValue() == rhs.getValue());
}
template <typename M, typename L, typename T, typename A>
constexpr bool operator!=(const RQuantity<M, L, T, A> &lhs, const RQuantity<M, L, T, A> &rhs) {
  return (lhs.getValue() != rhs.getValue());
}
template <typename M, typename L, typename T, typename A>
constexpr bool operator<=(const RQuantity<M, L, T, A> &lhs, const RQuantity<M, L, T, A> &rhs) {
  return (lhs.getValue() <= rhs.getValue());
}
template <typename M, typename L, typename T, typename A>
constexpr bool operator>=(const RQuantity<M, L, T, A> &lhs, const RQuantity<M, L, T, A> &rhs) {
  return (lhs.getValue() >= rhs.getValue());
}
template <typename M, typename L, typename T, typename A>
constexpr bool operator<(const RQuantity<M, L, T, A> &lhs, const RQuantity<M, L, T, A> &rhs) {
  return (lhs.getValue() < rhs.getValue());
}
template <typename M, typename L, typename T, typename A>
constexpr bool operator>(const RQuantity<M, L, T, A> &lhs, const RQuantity<M, L, T, A> &rhs) {
  return (lhs.getValue() > rhs.getValue());
}

// Common math functions:
// ------------------------------

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, A> abs(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, A>(std::abs(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<std::ratio_divide<M, std::ratio<2>>,
                    std::ratio_divide<L, std::ratio<2>>,
                    std::ratio_divide<T, std::ratio<2>>,
                    std::ratio_divide<A, std::ratio<2>>>
sqrt(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<std::ratio_divide<M, std::ratio<2>>,
                   std::ratio_divide<L, std::ratio<2>>,
                   std::ratio_divide<T, std::ratio<2>>,
                   std::ratio_divide<A, std::ratio<2>>>(std::sqrt(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<std::ratio_divide<M, std::ratio<3>>,
                    std::ratio_divide<L, std::ratio<3>>,
                    std::ratio_divide<T, std::ratio<3>>,
                    std::ratio_divide<A, std::ratio<3>>>
cbrt(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<std::ratio_divide<M, std::ratio<3>>,
                   std::ratio_divide<L, std::ratio<3>>,
                   std::ratio_divide<T, std::ratio<3>>,
                   std::ratio_divide<A, std::ratio<3>>>(std::cbrt(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, A> hypot(const RQuantity<M, L, T, A> &lhs,
                                      const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, A>(std::hypot(lhs.getValue(), rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, A> max(const RQuantity<M, L, T, A> &lhs,
                                    const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, A>(std::max(lhs.getValue(), rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, A> min(const RQuantity<M, L, T, A> &lhs,
                                    const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, A>(std::min(lhs.getValue(), rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, A> mod(const RQuantity<M, L, T, A> &lhs,
                                    const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, A>(std::fmod(lhs.getValue(), rhs.getValue()));
}

template <typename M1,
          typename L1,
          typename T1,
          typename A1,
          typename M2,
          typename L2,
          typename T2,
          typename A2>
constexpr RQuantity<M1, L1, T1, A1> copysign(const RQuantity<M1, L1, T1, A1> &lhs,
                                             const RQuantity<M2, L2, T2, A2> &rhs) {
  return RQuantity<M1, L1, T1, A1>(std::copysign(lhs.getValue(), rhs.getValue()));
}

// Common trig functions:
// ------------------------------

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, std::ratio_subtract<A, std::ratio<1>>>
sin(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, std::ratio_subtract<A, std::ratio<1>>>(std::sin(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, std::ratio_subtract<A, std::ratio<1>>>
cos(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, std::ratio_subtract<A, std::ratio<1>>>(std::cos(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, std::ratio_subtract<A, std::ratio<1>>>
tan(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, std::ratio_subtract<A, std::ratio<1>>>(std::tan(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, std::ratio_add<A, std::ratio<1>>>
asin(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, std::ratio_add<A, std::ratio<1>>>(std::asin(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, std::ratio_add<A, std::ratio<1>>>
acos(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, std::ratio_add<A, std::ratio<1>>>(std::acos(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, std::ratio_add<A, std::ratio<1>>>
atan(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, std::ratio_add<A, std::ratio<1>>>(std::atan(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, std::ratio_subtract<A, std::ratio<1>>>
sinh(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, std::ratio_subtract<A, std::ratio<1>>>(std::sinh(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, std::ratio_subtract<A, std::ratio<1>>>
cosh(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, std::ratio_subtract<A, std::ratio<1>>>(std::cosh(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, std::ratio_subtract<A, std::ratio<1>>>
tanh(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, std::ratio_subtract<A, std::ratio<1>>>(std::tanh(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, std::ratio_add<A, std::ratio<1>>>
asinh(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, std::ratio_add<A, std::ratio<1>>>(std::asinh(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, std::ratio_add<A, std::ratio<1>>>
acosh(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, std::ratio_add<A, std::ratio<1>>>(std::acosh(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<M, L, T, std::ratio_add<A, std::ratio<1>>>
atanh(const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<M, L, T, std::ratio_add<A, std::ratio<1>>>(std::atanh(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>
atan2(const RQuantity<M, L, T, A> &lhs, const RQuantity<M, L, T, A> &rhs) {
  return RQuantity<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>(
    std::atan2(lhs.getValue(), rhs.getValue()));
}

inline namespace literals {
constexpr long double operator"" _pi(long double x) {
  return static_cast<double>(x) * 3.1415926535897932384626433832795;
}
constexpr long double operator"" _pi(unsigned long long int x) {
  return static_cast<double>(x) * 3.1415926535897932384626433832795;
}
} // namespace literals
} // namespace okapi

// Conversion macro, which utilizes the string literals
#define ConvertTo(_x, _y) (_x).convert(1.0_##_y)
