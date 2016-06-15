
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

#ifndef APPLESEED_RENDERER_KERNEL_LIGHTING_IMAGEBASEDLIGHTING_H
#define APPLESEED_RENDERER_KERNEL_LIGHTING_IMAGEBASEDLIGHTING_H

// appleseed.renderer headers.
#include "renderer/global/globaltypes.h"

// appleseed.foundation headers.
#include "foundation/math/dual.h"
#include "foundation/math/vector.h"

// Standard headers.
#include <cstddef>

// Forward declarations.
namespace renderer  { class BSDF; }
namespace renderer  { class BSSRDF; }
namespace renderer  { class EnvironmentEDF; }
namespace renderer  { class ShadingContext; }
namespace renderer  { class ShadingPoint; }

namespace renderer
{

//
// Compute image-based lighting at a given point in space.
//

// Compute image-based lighting via BSDF and environment sampling.
void compute_ibl(
    SamplingContext&                sampling_context,
    const ShadingContext&           shading_context,
    const EnvironmentEDF&           environment_edf,
    const ShadingPoint&             shading_point,
    const foundation::Dual3d&       outgoing,               // world space outgoing direction, unit-length
    const BSDF&                     bsdf,
    const void*                     bsdf_data,
    const int                       bsdf_sampling_modes,    // permitted scattering modes during BSDF sampling
    const int                       env_sampling_modes,     // permitted scattering modes during environment sampling
    const size_t                    bsdf_sample_count,      // number of samples in BSDF sampling
    const size_t                    env_sample_count,       // number of samples in environment sampling
    Spectrum&                       radiance);

// Compute image-based lighting via BSSRDF and environment sampling.
void compute_ibl(
    SamplingContext&                sampling_context,
    const ShadingContext&           shading_context,
    const EnvironmentEDF&           environment_edf,
    const BSSRDF&                   bssrdf,
    const void*                     bssrdf_data,
    const ShadingPoint&             incoming_point,
    const ShadingPoint&             outgoing_point,
    const foundation::Dual3d&       outgoing,
    const size_t                    bssrdf_sample_count,    // number of samples in BSSRDF sampling
    const size_t                    env_sample_count,       // number of samples in environment sampling
    Spectrum&                       radiance);

// Compute image-based lighting via BSDF sampling.
void compute_ibl_bsdf_sampling(
    SamplingContext&                sampling_context,
    const ShadingContext&           shading_context,
    const EnvironmentEDF&           environment_edf,
    const ShadingPoint&             shading_point,
    const foundation::Dual3d&       outgoing,               // world space outgoing direction, unit-length
    const BSDF&                     bsdf,
    const void*                     bsdf_data,
    const int                       bsdf_sampling_modes,    // permitted scattering modes during BSDF sampling
    const size_t                    bsdf_sample_count,      // number of samples in BSDF sampling
    const size_t                    env_sample_count,       // number of samples in environment sampling
    Spectrum&                       radiance);

// Compute image-based lighting via BSSRDF sampling.
void compute_ibl_bssrdf_sampling(
    SamplingContext&                sampling_context,
    const ShadingContext&           shading_context,
    const EnvironmentEDF&           environment_edf,
    const BSSRDF&                   bssrdf,
    const void*                     bssrdf_data,
    const ShadingPoint&             incoming_point,
    const ShadingPoint&             outgoing_point,
    const foundation::Dual3d&       outgoing,
    const size_t                    bssrdf_sample_count,    // number of samples in BSSRDF sampling
    const size_t                    env_sample_count,       // number of samples in environment sampling
    Spectrum&                       radiance);

// Compute image-based lighting via environment sampling.
void compute_ibl_environment_sampling(
    SamplingContext&                sampling_context,
    const ShadingContext&           shading_context,
    const EnvironmentEDF&           environment_edf,
    const ShadingPoint&             shading_point,
    const foundation::Dual3d&       outgoing,               // world space outgoing direction, unit-length
    const BSDF&                     bsdf,
    const void*                     bsdf_data,
    const int                       env_sampling_modes,     // permitted scattering modes during environment sampling
    const size_t                    bsdf_sample_count,      // number of samples in BSDF sampling
    const size_t                    env_sample_count,       // number of samples in environment sampling
    Spectrum&                       radiance);
void compute_ibl_environment_sampling(
    SamplingContext&                sampling_context,
    const ShadingContext&           shading_context,
    const EnvironmentEDF&           environment_edf,
    const BSSRDF&                   bssrdf,
    const void*                     bssrdf_data,
    const ShadingPoint&             incoming_point,
    const ShadingPoint&             outgoing_point,
    const foundation::Dual3d&       outgoing,
    const size_t                    bssrdf_sample_count,    // number of samples in BSSRDF sampling
    const size_t                    env_sample_count,       // number of samples in environment sampling
    Spectrum&                       radiance);

}       // namespace renderer

#endif  // !APPLESEED_RENDERER_KERNEL_LIGHTING_IMAGEBASEDLIGHTING_H