/*******************************************************************************
The content of the files in this repository include portions of the
AUDIOKINETIC Wwise Technology released in source code form as part of the SDK
package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use these files in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Copyright (c) 2021 Audiokinetic Inc.
*******************************************************************************/

// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2020
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
// https://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// Version: 4.0.2019.08.13

#pragma once

#include <Mathematics/DCPQuery.h>
#include <Mathematics/Rectangle.h>
#include <Mathematics/Vector3.h>

namespace gte
{
    template <typename Real>
    class DCPQuery<Real, Vector3<Real>, Rectangle3<Real>>
    {
    public:
        struct Result
        {
            Real distance, sqrDistance;
            Real rectangleParameter[2];
            Vector3<Real> rectangleClosestPoint;
        };

        Result operator()(Vector3<Real> const& point, Rectangle3<Real> const& rectangle)
        {
            Result result;

            Vector3<Real> diff = rectangle.center - point;
            Real b0 = Dot(diff, rectangle.axis[0]);
            Real b1 = Dot(diff, rectangle.axis[1]);
            Real s0 = -b0, s1 = -b1;
            result.sqrDistance = Dot(diff, diff);

            if (s0 < -rectangle.extent[0])
            {
                s0 = -rectangle.extent[0];
            }
            else if (s0 > rectangle.extent[0])
            {
                s0 = rectangle.extent[0];
            }
            result.sqrDistance += s0 * (s0 + (Real)2 * b0);

            if (s1 < -rectangle.extent[1])
            {
                s1 = -rectangle.extent[1];
            }
            else if (s1 > rectangle.extent[1])
            {
                s1 = rectangle.extent[1];
            }
            result.sqrDistance += s1 * (s1 + (Real)2 * b1);

            // Account for numerical round-off error.
            if (result.sqrDistance < (Real)0)
            {
                result.sqrDistance = (Real)0;
            }

            result.distance = std::sqrt(result.sqrDistance);
            result.rectangleParameter[0] = s0;
            result.rectangleParameter[1] = s1;
            result.rectangleClosestPoint = rectangle.center;
            for (int i = 0; i < 2; ++i)
            {
                result.rectangleClosestPoint += result.rectangleParameter[i] * rectangle.axis[i];
            }
            return result;
        }
    };
}
