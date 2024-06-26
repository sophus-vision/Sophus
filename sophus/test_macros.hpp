#pragma once

#include <iostream>
#include <sstream>

#include <sophus/types.hpp>

namespace Sophus {
namespace details {

template <class Scalar, class Enable = void>
class Pretty {
 public:
  static std::string impl(Scalar s) {
    std::stringstream sstr;
    sstr << s;
    return sstr.str();
  }
};

template <class Ptr>
class Pretty<Ptr, std::enable_if_t<std::is_pointer<Ptr>::value>> {
 public:
  static std::string impl(Ptr ptr) {
    std::stringstream sstr;
    sstr << std::intptr_t(ptr);
    return sstr.str();
  }
};

template <class Scalar, int M, int N>
class Pretty<Eigen::Matrix<Scalar, M, N>, void> {
 public:
  static std::string impl(Matrix<Scalar, M, N> const& v) {
    std::stringstream sstr;
    sstr << "\n" << v << "\n";
    return sstr.str();
  }
};

template <class T>
std::string pretty(T const& v) {
  return Pretty<T>::impl(v);
}

}  // namespace details

void processTestResult(bool passed) {
  if (!passed) {
    // LCOV_EXCL_START
    std::cerr << "failed!" << std::endl << std::endl;
    exit(-1);
    // LCOV_EXCL_STOP
  }
  std::cerr << "passed." << std::endl << std::endl;
}
}  // namespace Sophus

#define SOPHUS_STRINGIFY(x) #x

/// Tests whether condition is true.
/// The in-out parameter passed will be set to false if test fails.
#define SOPHUS_TEST(passed, condition, descr, ...)                       \
  do {                                                                   \
    if (!(condition)) {                                                  \
      SOPHUS_DETAILS_FMT_LOG("Test failed: condition ``{}`` is false\n", \
                             SOPHUS_STRINGIFY(condition));               \
      passed = false;                                                    \
    }                                                                    \
  } while (false)

/// Tests whether left is equal to right given a threshold.
/// The in-out parameter passed will be set to false if test fails.
#define SOPHUS_TEST_EQUAL(passed, left, right, descr, ...)               \
  do {                                                                   \
    if (left != right) {                                                 \
      std::string msg = SOPHUS_DETAILS_FMT_STR(                          \
          "Test failed: {} (={}) is not equal to {} (={})\n",            \
          SOPHUS_STRINGIFY(left), Sophus::details::pretty(left),         \
          SOPHUS_STRINGIFY(right), Sophus::details::pretty(right));      \
      msg += SOPHUS_DETAILS_FMT_STR(descr, ##__VA_ARGS__);               \
      SOPHUS_DETAILS_FMT_LOG(                                            \
          "{} (={}) is not equal to {} (={})\n", SOPHUS_STRINGIFY(left), \
          Sophus::details::pretty(left), SOPHUS_STRINGIFY(right),        \
          Sophus::details::pretty(right));                               \
      passed = false;                                                    \
    }                                                                    \
  } while (false)

/// Tests whether left is equal to right given a threshold.
/// The in-out parameter passed will be set to false if test fails.
#define SOPHUS_TEST_NEQ(passed, left, right, descr, ...)             \
  do {                                                               \
    if (!(left != right)) {                                          \
      SOPHUS_DETAILS_FMT_LOG(                                        \
          "Test failed: {} (={}) should not be equal to {} (={})\n", \
          SOPHUS_STRINGIFY(left), Sophus::details::pretty(left),     \
          SOPHUS_STRINGIFY(right), Sophus::details::pretty(right));  \
      passed = false;                                                \
    }                                                                \
  } while (false)

/// Tests whether left is approximately equal to right given a threshold.
/// The in-out parameter passed will be set to false if test fails.
#define SOPHUS_TEST_APPROX(passed, left, right, thr, descr, ...)            \
  do {                                                                      \
    auto nrm = Sophus::maxMetric((left), (right));                          \
    if (!(nrm < (thr))) {                                                   \
      SOPHUS_DETAILS_FMT_LOG(                                               \
          "Test failed: {} (={}) is not approx {} (={}); {} is {}; nrm is " \
          "{}\n",                                                           \
          SOPHUS_STRINGIFY(left), Sophus::details::pretty(left),            \
          SOPHUS_STRINGIFY(right), Sophus::details::pretty(right),          \
          SOPHUS_STRINGIFY(thr), Sophus::details::pretty(thr),              \
          Sophus::details::pretty(nrm));                                    \
      passed = false;                                                       \
    }                                                                       \
  } while (false)

/// Tests whether left is NOT approximately equal to right given a
/// threshold. The in-out parameter passed will be set to false if test fails.
#define SOPHUS_TEST_NOT_APPROX(passed, left, right, thr, descr, ...) \
  do {                                                               \
    auto nrm = Sophus::maxMetric((left), (right));                   \
    if (nrm < (thr)) {                                               \
      SOPHUS_DETAILS_FMT_LOG(                                        \
          "Test failed: {} (={}) is approx {} (={}), but it should not!\n\
          nrm is {}\n",                                              \
          SOPHUS_STRINGIFY(left), Sophus::details::pretty(left),     \
          SOPHUS_STRINGIFY(right), Sophus::details::pretty(right),   \
          Sophus::details::pretty(nrm));                             \
      passed = false;                                                \
    }                                                                \
  } while (false)
