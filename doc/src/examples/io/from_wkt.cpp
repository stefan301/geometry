// Boost.Geometry (aka GGL, Generic Geometry Library)
// QuickBook Example

// Copyright (c) 2014 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2020 Baidyanath Kundu, Haldia, India.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[from_wkt
//` Shows the usage of from_wkt

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/polygon.hpp>

int main()
{
    typedef boost::geometry::model::d2::point_xy<double> point_type;
    typedef boost::geometry::model::box<point_type> box_type;
    typedef boost::geometry::model::segment<point_type> segment_type;
    typedef boost::geometry::model::polygon<point_type> polygon_type;
    typedef boost::geometry::model::linestring<point_type> linestring_type;

    point_type a;
    box_type d;
    segment_type e;
    polygon_type c;
    linestring_type b;
    a = boost::geometry::from_wkt<point_type>("POINT(1 2)");
    d = boost::geometry::from_wkt<box_type>("BOX(0 0,3 3)");
    e = boost::geometry::from_wkt<segment_type>("SEGMENT(1 0,3 4)");
    c = boost::geometry::from_wkt<polygon_type>("POLYGON((0 0,0 7,4 2,2 0,0 0))");
    b = boost::geometry::from_wkt<linestring_type>("LINESTRING(0 0,2 2,3 1)");

    return 0;
}

//]

