
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

// appleseed.foundation headers.
#include "foundation/math/quaternion.h"
#include "foundation/math/scalar.h"
#include "foundation/math/vector.h"
#include "foundation/utility/gnuplotfile.h"
#include "foundation/utility/iostreamop.h"
#include "foundation/utility/test.h"

// Imath headers.
#ifdef APPLESEED_ENABLE_IMATH_INTEROP
#include "foundation/platform/exrheaderguards.h"
BEGIN_EXR_INCLUDES
#include "OpenEXR/ImathQuat.h"
#include "OpenEXR/ImathVec.h"
END_EXR_INCLUDES
#endif

// Standard headers.
#include <cmath>
#include <cstddef>
#include <vector>

using namespace foundation;
using namespace std;

TEST_SUITE(Foundation_Math_Quaternion)
{
#ifdef APPLESEED_ENABLE_IMATH_INTEROP

    TEST_CASE(ConstructFromImathQuat)
    {
        const Imath::Quatd source(1.0, Imath::V3d(2.0, 3.0, 4.0));
        const Quaterniond copy(source);

        EXPECT_EQ(Quaterniond(1.0, Vector3d(2.0, 3.0, 4.0)), copy);
    }

    TEST_CASE(ConvertToImathQuat)
    {
        const Quaterniond source(1.0, Vector3d(2.0, 3.0, 4.0));
        const Imath::Quatd copy(source);

        EXPECT_EQ(Imath::Quatd(1.0, Imath::V3d(2.0, 3.0, 4.0)), copy);
    }

#endif

    TEST_CASE(ExtractAxisAngle)
    {
        const Vector3d ExpectedAxis = normalize(Vector3d(-1.0, 1.0, 1.0));
        const double ExpectedAngle = Pi / 4.0;
        const Quaterniond q = Quaterniond::rotation(ExpectedAxis, ExpectedAngle);

        Vector3d axis;
        double angle;
        q.extract_axis_angle(axis, angle);

        EXPECT_FEQ(ExpectedAxis, axis);
        EXPECT_FEQ(ExpectedAngle, angle);
    }

    TEST_CASE(ExtractAxisAngle_GivenAxisIsZero_ReturnsXAxis)
    {
        const Quaterniond q(1.0, Vector3d(0.0));

        Vector3d axis;
        double angle;
        q.extract_axis_angle(axis, angle);

        EXPECT_EQ(Vector3d(1.0, 0.0, 0.0), axis);
        EXPECT_EQ(0.0, angle);
    }

    TEST_CASE(ExtractAxisAngle_GivenAngleIsZero_ReturnsXAxis)
    {
        const Quaterniond q = Quaterniond::rotation(Vector3d(0.0, 1.0, 0.0), 0.0);

        Vector3d axis;
        double angle;
        q.extract_axis_angle(axis, angle);

        EXPECT_EQ(Vector3d(1.0, 0.0, 0.0), axis);
        EXPECT_EQ(0.0, angle);
    }

    TEST_CASE(ExtractAxisAngle_HandlesNotQuiteNormalizedQuaternion)
    {
        const Quaterniond q(
            1.0000000000000002,
            Vector3d(-3.4700225332029433e-011, 5.3246259831829512e-011, 1.3098189199922672e-011));

        Vector3d axis;
        double angle;
        q.extract_axis_angle(axis, angle);

        EXPECT_EQ(Vector3d(1.0, 0.0, 0.0), axis);
        EXPECT_EQ(0.0, angle);
    }

    TEST_CASE(Rotate_FromTo)
    {
        const Vector3d from = normalize(Vector3d(1.0, 1.0, 0.0));
        const Vector3d to = normalize(Vector3d(1.0, 0.0, 1.0));

        const Quaterniond q = Quaterniond::rotation(from, to);
        const Vector3d result = rotate(q, from);

        EXPECT_FEQ(to, result);
    }

    TEST_CASE(Rotate_AxisAngle)
    {
        const Quaterniond q = Quaterniond::rotation(Vector3d(0.0, 1.0, 0.0), deg_to_rad(45.0));

        const Vector3d result = rotate(q, Vector3d(1.0, 0.0, 0.0));

        EXPECT_FEQ(Vector3d(RcpSqrtTwo, 0.0, -RcpSqrtTwo), result);
    }

    TEST_CASE(PlotFastSlerpError)
    {
        GnuplotFile plotfile;
        plotfile.set_title("Fast Spherical Linear Interpolation of Quaternions");
        plotfile.set_xlabel("t");
        plotfile.set_ylabel("Absolute Angular Error");

        const Quaterniond q1 = Quaterniond::rotation(Vector3d(0.0, 0.0, 1.0), 0.0);
        const Quaterniond q2 = Quaterniond::rotation(Vector3d(0.0, 0.0, 1.0), Pi);

        const size_t PointCount = 1000;
        vector<Vector2d> points;

        for (size_t i = 0; i < PointCount; ++i)
        {
            const double t = fit<size_t, double>(i, 0, PointCount - 1, 0.0, 1.0);
            const Quaterniond q_slerp = slerp(q1, q2, t);
            const Quaterniond q_fast_slerp = fast_slerp(q1, q2, t);
            const double e = 2.0 * abs(acos(q_slerp.s) - acos(q_fast_slerp.s));
            points.push_back(Vector2d(t, e));
        }

        plotfile
            .new_plot()
            .set_points(points)
            .set_color("red");

        plotfile.write("unit tests/outputs/test_quaternion_slerp.gnuplot");
    }
}