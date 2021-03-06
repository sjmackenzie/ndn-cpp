
// Copyright Peter Dimov 2001-2002
// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//

// *Preprocessed* version of the main "arg.hpp" header
// -- DO NOT modify by hand!

NDNBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN
template<> struct arg< -1 >
{
    NDNBOOST_STATIC_CONSTANT(int, value  = -1);
    NDNBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    NDNBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          typename U1, typename U2, typename U3, typename U4, typename U5
        >
    struct apply
    {
        typedef U1 type;
        NDNBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

template<> struct arg<1>
{
    NDNBOOST_STATIC_CONSTANT(int, value  = 1);
    typedef arg<2> next;
    NDNBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    NDNBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          typename U1, typename U2, typename U3, typename U4, typename U5
        >
    struct apply
    {
        typedef U1 type;
        NDNBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

template<> struct arg<2>
{
    NDNBOOST_STATIC_CONSTANT(int, value  = 2);
    typedef arg<3> next;
    NDNBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    NDNBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          typename U1, typename U2, typename U3, typename U4, typename U5
        >
    struct apply
    {
        typedef U2 type;
        NDNBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

template<> struct arg<3>
{
    NDNBOOST_STATIC_CONSTANT(int, value  = 3);
    typedef arg<4> next;
    NDNBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    NDNBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          typename U1, typename U2, typename U3, typename U4, typename U5
        >
    struct apply
    {
        typedef U3 type;
        NDNBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

template<> struct arg<4>
{
    NDNBOOST_STATIC_CONSTANT(int, value  = 4);
    typedef arg<5> next;
    NDNBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    NDNBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          typename U1, typename U2, typename U3, typename U4, typename U5
        >
    struct apply
    {
        typedef U4 type;
        NDNBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

template<> struct arg<5>
{
    NDNBOOST_STATIC_CONSTANT(int, value  = 5);
    typedef arg<6> next;
    NDNBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    NDNBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          typename U1, typename U2, typename U3, typename U4, typename U5
        >
    struct apply
    {
        typedef U5 type;
        NDNBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

NDNBOOST_MPL_AUX_NONTYPE_ARITY_SPEC(1,int, arg)

NDNBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE
