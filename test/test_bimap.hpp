// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LIBS_BIMAP_TEST_BIMAP_TEST_HPP
#define LIBS_BIMAP_TEST_BIMAP_TEST_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/next_prior.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <boost/assert.hpp>
#include <algorithm>

template <typename Container, typename Data>
void test_container(Container& c, Data const& d)
{
    BOOST_ASSERT(d.size() > 2);

    c.clear();
    BOOST_TEST(c.size() == 0);
    BOOST_TEST(c.empty());

    c.insert(*d.begin());
    c.insert(++d.begin(), d.end());

    BOOST_TEST(c.size() == d.size());
    BOOST_TEST(c.size() <= c.max_size());
    BOOST_TEST(!c.empty());

    c.erase(c.begin());
    BOOST_TEST(c.size() == d.size() - 1);

    c.erase(c.begin(), c.end());
    BOOST_TEST(c.empty());

    c.insert(*d.begin());
    BOOST_TEST(c.size() == 1);

    c.insert(c.begin(), *(++d.begin()));
    BOOST_TEST(c.size() == 2);
    BOOST_TEST(c.begin() != c.end());
}

template <typename Container, typename Data>
void test_sequence_container(Container& c, Data const& d)
{
    BOOST_ASSERT(d.size() > 2);

    c.clear();
    BOOST_TEST(c.size() == 0);
    BOOST_TEST(c.empty());

    c.push_front(*d.begin());
    c.push_back(*(++d.begin()));
    BOOST_TEST(c.front() == *c.begin());
    BOOST_TEST(c.back() == *(++c.begin()));
    BOOST_TEST(c.size() == 2);
    BOOST_TEST(c.size() <= c.max_size());
    BOOST_TEST(!c.empty());

    c.erase(c.begin());
    BOOST_TEST(c.size() == 1);

    c.insert(c.begin(), *(++d.begin()));
    c.erase(c.begin(), c.end());
    BOOST_TEST(c.empty());

    c.push_front(*d.begin());
    BOOST_TEST(c.size() == 1);
    BOOST_TEST(c.begin() != c.end());

    c.clear();
    BOOST_TEST(c.empty());

    // assign

    c.assign(d.begin(), d.end());
    BOOST_TEST(c.size() == d.size());
    BOOST_TEST(std::equal(c.begin(), c.end(), d.begin()));

    c.assign(d.size(), *d.begin());
    BOOST_TEST(c.size() == d.size());
    BOOST_TEST(*c.begin() == *d.begin());

    // Check insert(IterPos, InputIter, InputIter)

    c.clear();
    c.insert(c.begin(), d.begin(), d.end());
    c.insert(boost::next(c.begin(), 2), d.begin(), d.end());

    BOOST_TEST(
       std::equal(
           boost::next(c.begin(), 2)
         , boost::next(c.begin(), 2 + d.size())
         , d.begin()
       )
    );

    // Check resize

    c.clear();
    c.resize(4, *d.begin());
    BOOST_TEST(c.size() == 4);
    BOOST_TEST(*c.begin() == *d.begin());
    BOOST_TEST(c == c);
    BOOST_TEST(!(c != c));
    BOOST_TEST(!(c < c));
    BOOST_TEST((c <= c));
    BOOST_TEST(!(c > c));
    BOOST_TEST((c >= c));
}

template <typename Container, typename Data>
void test_vector_container(Container& c, Data const& d)
{
    BOOST_ASSERT(d.size() > 2);

    c.clear();
    c.reserve(2);
    BOOST_TEST(c.capacity() >= 2);

    c.assign(d.begin(), d.end());
    BOOST_TEST(c.capacity() >= c.size());
    BOOST_TEST(c[0] == *d.begin());
    BOOST_TEST(c.at(1) == *boost::next(d.begin()));
    
    test_sequence_container(c, d);
}

template <typename Container, typename Data>
void test_associative_container(Container& c, Data const& d)
{
    BOOST_ASSERT(d.size() > 2);

    c.clear();
    c.insert(d.begin(), d.end());

    for (
        BOOST_DEDUCED_TYPENAME Data::const_iterator di = d.begin(
        ), de = d.end();
        di != de;
        ++di
    )
    {
        BOOST_TEST(c.find(*di) != c.end());
    }

    BOOST_DEDUCED_TYPENAME Data::const_iterator da = d.begin();
    BOOST_DEDUCED_TYPENAME Data::const_iterator db = ++d.begin();

    c.erase(*da);
    BOOST_TEST(c.size() == d.size() - 1);
    BOOST_TEST(c.count(*da) == 0);
    BOOST_TEST(c.count(*db) == 1);
    BOOST_TEST(c.find(*da) == c.end());
    BOOST_TEST(c.find(*db) != c.end());
    BOOST_TEST(c.equal_range(*db).first != c.end());

    c.clear();
    BOOST_TEST(c.equal_range(*da).first == c.end());
}

template <typename Container>
void test_mapped_container(Container&)
{
    BOOST_TEST_TRAIT_TRUE((
        boost::is_same<
            BOOST_DEDUCED_TYPENAME Container::key_type
          , BOOST_DEDUCED_TYPENAME Container::value_type::first_type
        >
    ));
    BOOST_TEST_TRAIT_TRUE((
        boost::is_same<
            BOOST_DEDUCED_TYPENAME Container::data_type
          , BOOST_DEDUCED_TYPENAME Container::value_type::second_type
        >
    ));
    BOOST_TEST_TRAIT_TRUE((
        boost::is_same<
            BOOST_DEDUCED_TYPENAME Container::mapped_type
          , BOOST_DEDUCED_TYPENAME Container::value_type::second_type
        >
    ));
}

template <typename Container, typename Data>
void test_pair_associative_container(Container& c, Data const& d)
{
    test_mapped_container(c);

    BOOST_ASSERT(d.size() > 2);

    c.clear();
    c.insert(d.begin(), d.end());

    for (
        BOOST_DEDUCED_TYPENAME Data::const_iterator di = d.begin(
        ), de = d.end();
        di != de;
        ++di
    )
    {
        BOOST_TEST(c.find(di->first) != c.end());
    }

    BOOST_DEDUCED_TYPENAME Data::const_iterator da = d.begin();
    BOOST_DEDUCED_TYPENAME Data::const_iterator db = ++d.begin();

    c.erase(da->first);
    BOOST_TEST(c.size() == d.size() - 1);
    BOOST_TEST(c.count(da->first) == 0);
    BOOST_TEST(c.count(db->first) == 1);
    BOOST_TEST(c.find(da->first) == c.end());
    BOOST_TEST(c.find(db->first) != c.end());
    BOOST_TEST(c.equal_range(db->first).first != c.end());

    c.clear();
    BOOST_TEST(c.equal_range(da->first).first == c.end());
}

template <typename Container, typename Data>
void
    test_simple_ordered_associative_container_equality(
        Container& c
      , Data const& d
    )
{
    BOOST_TEST(std::equal(c.begin(), c.end(), d.begin()));
    BOOST_TEST(std::equal(c.rbegin(), c.rend(), d.rbegin()));
    BOOST_TEST(c.lower_bound(*d.begin()) == c.begin());
    BOOST_TEST(c.upper_bound(*d.begin()) == ++c.begin());
}

template <typename Container, typename Data>
void test_simple_ordered_associative_container(Container& c, Data const& d)
{
    BOOST_ASSERT(d.size() > 2);

    c.clear();
    c.insert(d.begin(), d.end());

    for (
        BOOST_DEDUCED_TYPENAME Data::const_iterator di = d.begin(
        ), de = d.end();
        di != de;
        ++di
    )
    {
        BOOST_DEDUCED_TYPENAME Container::const_iterator ci = c.find(*di);
        BOOST_TEST(ci != c.end());
        BOOST_TEST(!c.key_comp()(*ci, *di));
        BOOST_TEST(!c.value_comp()(*ci, *di));
    }

    test_simple_ordered_associative_container_equality(c, d);

    Container const& cr = c;

    test_simple_ordered_associative_container_equality(cr, d);

    BOOST_TEST(c == c);
    BOOST_TEST(!(c != c));
    BOOST_TEST(!(c < c));
    BOOST_TEST((c <= c));
    BOOST_TEST(!(c > c));
    BOOST_TEST((c >= c));
#if 0
    BOOST_TEST(
        c.range(
            *c.begin() <= ::boost::lambda::_1
          , ::boost::lambda::_1 <= *(++c.begin())
        ).first == c.begin()
    );
#endif
}

template <typename Container, typename Data>
void
    test_pair_ordered_associative_container_equality(
        Container& c
      , Data const& d
    )
{
    BOOST_TEST(std::equal(c.begin(), c. end(), d.begin()));
    BOOST_TEST(std::equal(c.rbegin(), c.rend(), d.rbegin()));
    BOOST_TEST(c.lower_bound(d.begin()->first) == c.begin());
    BOOST_TEST(c.upper_bound(d.begin()->first) == ++c.begin());
}

template <typename Container, typename Data>
void test_pair_ordered_associative_container(Container& c, Data const& d)
{
    BOOST_ASSERT(d.size() > 2);

    c.clear();
    c.insert(d.begin(), d.end());

    for (
        BOOST_DEDUCED_TYPENAME Container::const_iterator ci = c.begin(
        ), ce = c.end();
        ci != ce;
        ++ci
    )
    {
        BOOST_DEDUCED_TYPENAME Data::const_iterator di = d.find(ci->first);
        BOOST_TEST(di != d.end());
        BOOST_TEST(!c.key_comp()(di->first, ci->first));
        BOOST_TEST(!c.value_comp()(*ci, *di));
    }

    test_pair_ordered_associative_container_equality(c, d);

    Container const& cr = c;

    test_pair_ordered_associative_container_equality(cr, d);

    BOOST_TEST(
        c.range(
            c.begin()->first <= ::boost::lambda::_1
          , ::boost::lambda::_1 <= (++c.begin())->first
        ).first == c.begin()
    );
}

#include <iterator>

template <typename Container, typename Data>
void test_simple_unordered_associative_container(Container& c, Data const& d)
{
    c.clear();
    c.reserve(std::distance(d.begin(), d.end()));
    c.insert(d.begin(), d.end());

    Container const& const_c = c;
    BOOST_TEST(
        const_c.bucket_count() * const_c.max_load_factor() >= d.size()
    );
    BOOST_TEST(const_c.max_bucket_count() >= const_c.bucket_count());

    for (
        BOOST_DEDUCED_TYPENAME Data::const_iterator di = d.begin(
        ), de = d.end();
        di != de;
        ++di
    )
    {
        // non const
        {
            BOOST_DEDUCED_TYPENAME Container::size_type nb = c.bucket(
                *c.find(*di)
            );
            BOOST_TEST(c.begin(nb) != c.end(nb));
        }

        // const
        {
            // Hash collisions should have no effect on the correctness of an
            // unordered simple associative container. -- Cromwell D. Enage
//            BOOST_TEST(const_c.bucket_size(const_c.bucket(*di)) == 1);

            BOOST_DEDUCED_TYPENAME Container::size_type nb = const_c.bucket(
                *const_c.find(*di)
            );
            BOOST_TEST(const_c.begin(nb) != const_c.end(nb));
        }
    }

    BOOST_TEST(c.load_factor() < c.max_load_factor());
    c.max_load_factor(0.75);
    BOOST_TEST(c.max_load_factor() == 0.75);
    c.rehash(10);
}

template <typename Container, typename Data>
void test_pair_unordered_associative_container(Container& c, Data const& d)
{
    c.clear();
    c.reserve(std::distance(d.begin(), d.end()));
    c.insert(d.begin(), d.end());

    Container const& const_c = c;
    BOOST_TEST(
        const_c.bucket_count() * const_c.max_load_factor() >= d.size()
    );
    BOOST_TEST(const_c.max_bucket_count() >= const_c.bucket_count());

    for (
        BOOST_DEDUCED_TYPENAME Data::const_iterator di = d.begin(
        ), de = d.end();
        di != de;
        ++di
    )
    {
        // non const
        {
            BOOST_DEDUCED_TYPENAME Container::size_type nb = c.bucket(
                c.find(di->first)->first
            );
            BOOST_TEST(c.begin(nb) != c.end(nb));
        }

        // const
        {
            // The test commented out below fails on 32-bit Windows platforms
            // but works on 64-bit Windows platforms and others.  Regardless,
            // hash collisions should have no effect on the correctness of an
            // unordered pair associative container. -- Cromwell D. Enage
//            BOOST_TEST(const_c.bucket_size(const_c.bucket(di->first)) == 1);

            BOOST_DEDUCED_TYPENAME Container::size_type nb = const_c.bucket(
                const_c.find(di->first)->first
            );
            BOOST_TEST(const_c.begin(nb) != const_c.end(nb));
        }
    }

    BOOST_TEST(c.load_factor() < c.max_load_factor());
    c.max_load_factor(0.75);
    BOOST_TEST(c.max_load_factor() == 0.75);
    c.rehash(10);
}

template <typename Container, typename Data>
void test_unique_container(Container& c, Data& d)
{
    c.clear();
    c.insert(d.begin(), d.end());
    c.insert(*d.begin());
    BOOST_TEST(c.size() == d.size());
}

template <typename Container, typename Data>
void test_non_unique_container(Container& c, Data& d)
{
    c.clear();
    c.insert(d.begin(), d.end());
    c.insert(*d.begin());
    BOOST_TEST(c.size() == (d.size() + 1));
}

template <
    typename Bimap, typename Data, typename LeftData, typename RightData
>
void
    test_basic_bimap(
        Bimap& b
      , Data const& d
      , LeftData const& ld
      , RightData const& rd
    )
{
    using namespace boost::bimaps;

    test_container(b, d);

    BOOST_TEST(&b.left == &b.template by<member_at::left>());
    BOOST_TEST(&b.right == &b.template by<member_at::right>());

    test_container(b.left, ld);
    test_container(b.right, rd);
}

template <typename LeftTag, typename RightTag, typename Bimap, typename Data>
void test_tagged_bimap(Bimap& b, Data const& d)
{
    using namespace boost::bimaps;

    BOOST_TEST(&b.left == &b.template by<LeftTag>());
    BOOST_TEST(&b.right == &b.template by<RightTag>());

    b.clear();
    b.insert(*d.begin());

    BOOST_TEST(
        b.begin()->template get<LeftTag>() == b.template by<
            RightTag
        >().begin()->template get<LeftTag>()
    );
    BOOST_TEST(
        b.begin()->template get<RightTag>() == b.template by<
            LeftTag
        >().begin()->template get<RightTag>()
    );

    // const test
    {
        Bimap const& bc = b;

        BOOST_TEST(&bc.left == &bc.template by<LeftTag>());
        BOOST_TEST(&bc.right == &bc.template by<RightTag>());
        BOOST_TEST(
            bc.begin()->template get<LeftTag>() == bc.template by<
                RightTag
            >().begin()->template get<LeftTag>()
        );
        BOOST_TEST(
            bc.begin()->template get<RightTag>() == bc.template by<
                LeftTag
            >().begin()->template get<RightTag>()
        );
    }
}

template <
    typename Bimap, typename Data, typename LeftData, typename RightData
>
void
    test_set_set_bimap(
        Bimap& b
      , Data const& d
      , LeftData const& ld
      , RightData const& rd
    )
{
    using namespace boost::bimaps;

    test_basic_bimap(b, d, ld, rd);

    test_associative_container(b, d);
    test_simple_ordered_associative_container(b, d);

    test_pair_associative_container(b.left, ld);
    test_pair_ordered_associative_container(b.left, ld);
    test_unique_container(b.left, ld);

    test_pair_associative_container(b.right, rd);
    test_pair_ordered_associative_container(b.right, rd);
    test_unique_container(b.right, rd);

}

template <
    typename Bimap, typename Data, typename LeftData, typename RightData
>
void
    test_multiset_multiset_bimap(
        Bimap& b
      , Data const& d
      , LeftData const& ld
      , RightData const& rd
    )
{
    using namespace boost::bimaps;

    test_basic_bimap(b, d, ld, rd);
    test_associative_container(b, d);
    test_simple_ordered_associative_container(b, d);

    test_pair_associative_container(b.left, ld);
    test_pair_ordered_associative_container(b.left, ld);
    test_non_unique_container(b.left, ld);

    test_pair_associative_container(b.right, rd);
    test_pair_ordered_associative_container(b.right, rd);
    test_non_unique_container(b.right, rd);
}

template <
    typename Bimap, typename Data, typename LeftData, typename RightData
>
void
    test_unordered_set_unordered_multiset_bimap(
        Bimap& b
      , Data const& d
      , LeftData const& ld
      , RightData const& rd
    )
{
    using namespace boost::bimaps;

    test_basic_bimap(b, d, ld, rd);
    test_associative_container(b, d);
    test_simple_unordered_associative_container(b, d);

    test_pair_associative_container(b.left, ld);
    test_pair_unordered_associative_container(b.left, ld);
    test_unique_container(b.left, ld);

    test_pair_associative_container(b.right, rd);
    test_pair_unordered_associative_container(b.right, rd);

    // Caution, this side is a non unique container, but the other side is a
    // unique container so, the overall bimap is a unique one.
    test_unique_container(b.right, rd);
}

template <typename Bimap, typename Data>
void test_bimap_init_copy_swap(Data const& d)
{    
    Bimap b1(d.begin(), d.end());
    Bimap b2(b1);
    BOOST_TEST(b1 == b2);
    
    b2.clear();
    b2 = b1;
    BOOST_TEST(b2 == b1);

    b2.clear();
    b2.left = b1.left;
    BOOST_TEST(b2 == b1);

    b2.clear();
    b2.right = b1.right;
    BOOST_TEST(b2 == b1);

    b1.clear();
    b2.swap(b1);
    BOOST_TEST(b2.empty() && !b1.empty());

    b1.left.swap(b2.left);
    BOOST_TEST(b1.empty() && !b2.empty());

    b1.right.swap(b2.right);
    BOOST_TEST(b2.empty() && !b1.empty());
} 

#endif // LIBS_BIMAP_TEST_BIMAP_TEST_HPP

