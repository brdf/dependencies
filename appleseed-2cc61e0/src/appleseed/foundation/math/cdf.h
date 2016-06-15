
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010-2013 Francois Beaune, Jupiter Jazz Limited
// Copyright (c) 2014-2015 Francois Beaune, The appleseedhq Organization
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef APPLESEED_FOUNDATION_MATH_CDF_H
#define APPLESEED_FOUNDATION_MATH_CDF_H

// appleseed.foundation headers.
#include "foundation/core/concepts/noncopyable.h"

// Standard headers.
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <utility>
#include <vector>

namespace foundation
{

//
// Cumulative Distribution Function.
//
// Reference:
//
//     http://en.wikipedia.org/wiki/Cumulative_distribution_function
//

template <typename Item, typename Weight>
class CDF
  : public NonCopyable
{
  public:
    typedef std::pair<Item, Weight> ItemWeightPair;

    // Constructor.
    CDF();

    // Return true if the CDF is empty.
    bool empty() const;

    // Return true if the CDF has at least one item with a positive weight.
    bool valid() const;

    // Return the sum of the weight of all inserted items.
    Weight weight() const;

    // Remove all items from the CDF.
    void clear();

    // Allocate memory for a given number of items.
    void reserve(const size_t count);

    // Insert an item with a given non-negative weight.
    void insert(const Item& item, const Weight weight);

    // Access the i'th item.
    const ItemWeightPair& operator[](const size_t i) const;

    // Prepare the CDF for sampling.
    // This method must be called once and only once before sample() is called.
    void prepare();

    // Sample the CDF. x is in [0,1).
    const ItemWeightPair& sample(const Weight x) const;

  private:
    typedef std::vector<ItemWeightPair> ItemVector;
    typedef std::vector<Weight> DensityVector;

    ItemVector      m_items;
    Weight          m_weight_sum;
    DensityVector   m_densities;
};


//
// Sample CDF.
//

template <typename RandomAccessIter, typename Weight>
size_t sample_cdf(
    RandomAccessIter    begin,
    RandomAccessIter    end,
    Weight              x);

//
// Invert CDF.
//

// Numerically invert the CDF function cdf, with corresponding PDF pdf,
// using a combination of bisection and Newton's method.
template <typename CDF, typename PDF, typename T>
T invert_cdf_function(
    CDF             cdf,                // cdf function to invert
    PDF             pdf,                // pdf function
    const T         u,                  // uniform random sample in [0,1)
    T               xmin,               // lower bound of the root search interval
    T               xmax,               // upper bound of the root search interval
    const T         guess,              // initial root guess
    const T         eps,                // root precision
    const size_t    max_iterations);    // max root refinement iterations


//
// CDF class implementation.
//

template <typename Item, typename Weight>
inline CDF<Item, Weight>::CDF()
  : m_weight_sum(0.0)
{
}

template <typename Item, typename Weight>
inline bool CDF<Item, Weight>::empty() const
{
    return m_items.empty();
}

template <typename Item, typename Weight>
inline bool CDF<Item, Weight>::valid() const
{
    return m_weight_sum > Weight(0.0);
}

template <typename Item, typename Weight>
inline Weight CDF<Item, Weight>::weight() const
{
    return m_weight_sum;
}

template <typename Item, typename Weight>
inline void CDF<Item, Weight>::clear()
{
    m_items.clear();
    m_weight_sum = Weight(0.0);
}

template <typename Item, typename Weight>
inline void CDF<Item, Weight>::reserve(const size_t count)
{
    m_items.reserve(count);
}

template <typename Item, typename Weight>
inline void CDF<Item, Weight>::insert(const Item& item, const Weight weight)
{
    assert(weight >= Weight(0.0));
    m_items.push_back(std::make_pair(item, weight));
    m_weight_sum += weight;
}

template <typename Item, typename Weight>
inline const std::pair<Item, Weight>& CDF<Item, Weight>::operator[](const size_t i) const
{
    assert(i < m_items.size());
    return m_items[i];
}

template <typename Item, typename Weight>
void CDF<Item, Weight>::prepare()
{
    assert(valid());

    const size_t item_count = m_items.size();

    // Normalize weights so that they add up to 1.0.
    const Weight rcp_weight_sum = Weight(1.0) / m_weight_sum;
    for (size_t i = 0; i < item_count; ++i)
        m_items[i].second *= rcp_weight_sum;

    // Compute the distribution function.
    m_densities.resize(item_count);
    Weight cumulated_weight = Weight(0.0);
    for (size_t i = 0; i < item_count - 1; ++i)
    {
        cumulated_weight += m_items[i].second;
        m_densities[i] = cumulated_weight;
    }
    m_densities[item_count - 1] = Weight(1.0);
}

template <typename Item, typename Weight>
inline const std::pair<Item, Weight>& CDF<Item, Weight>::sample(const Weight x) const
{
    const size_t i =
        sample_cdf(
            m_densities.begin(),
            m_densities.end(),
            x);

    return m_items[i];
}

//
// Sample CDF implementation.
//

template <typename RandomAccessIter, typename Weight>
inline size_t sample_cdf(
    RandomAccessIter    begin,
    RandomAccessIter    end,
    Weight              x)
{
    assert(begin != end);
    assert(x >= Weight(0.0));
    assert(x < Weight(1.0));

    const RandomAccessIter i = std::upper_bound(begin, end, x);
    assert(i < end);

    return i - begin;
}

//
// Invert CDF implementation.
//

template <typename CDF, typename PDF, typename T>
inline T invert_cdf_function(
    CDF             cdf,
    PDF             pdf,
    const T         u,
    T               xmin,
    T               xmax,
    const T         guess,
    const T         eps,
    const size_t    max_iterations)
{
    assert(cdf(xmin) < u);
    assert(cdf(xmax) > u);

    T x = guess;
    for (size_t i = 0; i < max_iterations; ++i)
    {
        // Use bisection if we go out of bounds.
        if (x < xmin || x > xmax)
            x = (xmax + xmin) * 0.5;

        const T f = cdf(x) - u;

        // Convergence test.
        if (std::abs(f) <= eps)
            break;

        // Update bounds.
        f < 0.0 ? xmin = x : xmax = x;

        // Newton step.
        const T df = pdf(x);
        x -= f / df;
    }

    return x;
}

}       // namespace foundation

#endif  // !APPLESEED_FOUNDATION_MATH_CDF_H