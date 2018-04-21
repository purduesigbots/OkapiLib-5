/**
 * @author Ryan Benasutti, WPI
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _OKAPI_COMPOSABLEFILTER_HPP_
#define _OKAPI_COMPOSABLEFILTER_HPP_

#include "okapi/filter/filter.hpp"
#include <functional>
#include <initializer_list>
#include <memory>
#include <vector>

namespace okapi {
class ComposableFilterArgs : public FilterArgs {
  public:
  ComposableFilterArgs(const std::initializer_list<std::shared_ptr<Filter>> ilist);

  const std::initializer_list<std::shared_ptr<Filter>> list;
};

class ComposableFilter : public Filter {
  public:
  /**
   * A composable filter is a filter that consists of other filters. The input signal is passed
   * through each filter in sequence. The final output of this filter is the output of the last
   * filter.
   */
  ComposableFilter();

  /**
   * A composable filter is a filter that consists of other filters. The input signal is passed
   * through each filter in sequence. The final output of this filter is the output of the last
   * filter.
   *
   * @param ilist the lambdas used to allocate filters
   */
  ComposableFilter(const std::initializer_list<std::shared_ptr<Filter>> &ilist);

  ComposableFilter(const ComposableFilterArgs &iparams);

  /**
   * Filters a value, like a sensor reading.
   *
   * @param ireading new measurement
   * @return filtered result
   */
  virtual double filter(const double ireading);

  /**
   * Returns the previous output from filter.
   *
   * @return the previous output from filter
   */
  virtual double getOutput() const;

  /**
   * Adds a filter to the end of the sequence.
   *
   * @param ifilter the filter to add
   */
  virtual void addFilter(std::shared_ptr<Filter> ifilter);

  protected:
  std::vector<std::shared_ptr<Filter>> filters;
  double output = 0;
};
} // namespace okapi

#endif
