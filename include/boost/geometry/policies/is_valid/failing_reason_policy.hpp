// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2015, Oracle and/or its affiliates.

// Contributed and/or modified by Menelaos Karavelas, on behalf of Oracle

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html

#ifndef BOOST_GEOMETRY_POLICIES_IS_VALID_FAILING_REASON_POLICY_HPP
#define BOOST_GEOMETRY_POLICIES_IS_VALID_FAILING_REASON_POLICY_HPP

#include <sstream>

#include <boost/geometry/io/dsv/write.hpp>
#include <boost/geometry/util/range.hpp>
#include <boost/geometry/algorithms/validity_failure_type.hpp>


namespace boost { namespace geometry
{


inline char const* validity_failure_type_message(validity_failure_type failure)
{
    switch (failure)
    {
    case no_failure:
        return "Geometry is valid";
    case failure_few_points:
        return "Geometry has too few points";
    case failure_wrong_topological_dimension:
        return "Geometry has wrong topological dimension";
    case failure_not_closed:
        return "Geometry is defined as closed but is open";
    case failure_spikes:
        return "Geometry has spikes";
    case failure_self_intersections:
        return "Geometry has invalid self-intersections";
    case failure_wrong_orientation:
        return "Geometry has wrong orientation";
    case failure_interior_rings_outside:
        return "Geometry has interior rings defined outside the outer boundary";
    case failure_nested_interior_rings:
        return "Geometry has nested interior rings";
    case failure_disconnected_interior:
        return "Geometry has disconnected interior";
    case failure_intersecting_interiors:
        return "Multi-polygon has intersecting interiors";
    case failure_duplicate_points:
        return "Geometry has duplicate (consecutive) points";
    case failure_wrong_corner_order:
        return "Box has corners in wrong order";
    default: // to avoid -Wreturn-type warning
        return "";
    }
}


template <bool AllowDuplicates = true, bool AllowSpikes = true>
class failing_reason_policy
{
private:
    static inline
    validity_failure_type transform_failure_type(validity_failure_type failure)
    {
        if (AllowDuplicates && failure == failure_duplicate_points)
        {
            return no_failure;
        }
        return failure;
    }

    static inline
    validity_failure_type transform_failure_type(validity_failure_type failure,
                                                 bool is_linear)
    {
        if (is_linear && AllowSpikes && failure == failure_spikes)
        {
            return no_failure;
        }
        return transform_failure_type(failure);
    }

    inline void set_failure_message(validity_failure_type failure)
    {
        m_oss.str("");
        m_oss.clear();
        m_oss << validity_failure_type_message(failure);
    }

    template
    <
        validity_failure_type Failure,
        typename Data1,
        typename Data2 = Data1,
        typename Dummy = void
    >
    struct process_data
    {
        static inline void apply(std::ostringstream&, Data1 const&)
        {
        }

        static inline void apply(std::ostringstream&,
                                 Data1 const&,
                                 Data2 const&)
        {
        }
    };

    template <typename SpikePoint>
    struct process_data<failure_spikes, bool, SpikePoint>
    {
        static inline void apply(std::ostringstream& oss,
                                 bool is_linear,
                                 SpikePoint const& spike_point)
        {
            if (is_linear && AllowSpikes)
            {
                return;
            }

            oss << ". A spike point was found with apex at "
                << geometry::dsv(spike_point);
        }
    };

    template <typename Turns>
    struct process_data<failure_self_intersections, Turns>
    {
        static inline void apply(std::ostringstream& oss,
                                 Turns const& turns)
        {
            oss << ". A self-intersection point was found at "
                << geometry::dsv(range::front(turns).point);
        }
    };

    template <typename Point>
    struct process_data<failure_duplicate_points, Point>
    {
        static inline void apply(std::ostringstream& oss,
                                 Point const& point)
        {
            if (AllowDuplicates)
            {
                return;
            }
            oss << ". Duplicate points were found near point "
                << geometry::dsv(point);
        }
    };

public:
    failing_reason_policy(std::ostringstream& oss)
        : m_oss(oss)
    {}

    template <validity_failure_type Failure>
    inline bool apply()
    {
        validity_failure_type const failure = transform_failure_type(Failure);
        set_failure_message(failure);
        return failure == no_failure;
    }

    template <validity_failure_type Failure, typename Data>
    inline bool apply(Data const& data)
    {
        validity_failure_type const failure = transform_failure_type(Failure);
        set_failure_message(failure);
        process_data<Failure, Data>::apply(m_oss, data);
        return failure == no_failure;
    }

    template <validity_failure_type Failure, typename Data1, typename Data2>
    inline bool apply(Data1 const& data1, Data2 const& data2)
    {
        validity_failure_type const failure
            = transform_failure_type(Failure, data1);
        set_failure_message(failure);
        process_data<Failure, Data1, Data2>::apply(m_oss, data1, data2);
        return failure == no_failure;
    }

private:
    std::ostringstream& m_oss;
};


}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_POLICIES_IS_VALID_FAILING_REASON_POLICY_HPP
