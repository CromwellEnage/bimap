// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  VC++ 8.0 warns on usage of certain Standard Library and API functions that
//  can be cause buffer overruns or other possible security issues if misused.
//  See http://msdn.microsoft.com/msdnmag/issues/05/05/SafeCandC/default.aspx
//  But the wording of the warning is misleading and unsettling, there are no
//  portable alternative functions, and VC++ 8.0's own libraries use the
//  functions in question. So turn off the warnings.
#define _CRT_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE

#include <boost/config.hpp>

// Boost.Core.LightweightTest
#include <boost/core/lightweight_test.hpp>

// Boost.Bimap
#include <boost/bimap/support/lambda.hpp>
#include <boost/bimap/bimap.hpp>

struct id
{
};

void test_bimap_modify()
{
    typedef boost::bimaps::bimap<int,long> bm;

    bm b;
    b.insert(bm::value_type(2, 200));

    BOOST_TEST(b.left.at(2) == 200);

    bool result;

    // replace
    //----------------------------------------------------------------------

    // successful replace in left map view
    {
        bm::left_iterator i = b.left.begin();

        result = b.left.replace(i, bm::left_value_type(1, 100));

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST((i->first == 1) && (i->second == 100));
        BOOST_TEST(b.left.at(1) == 100);

        result = b.left.replace_key(i, 2);

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST((i->first == 2) && (i->second == 100));
        BOOST_TEST(b.left.at(2) == 100);

        result = b.left.replace_data(i, 200);

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST((i->first == 2) && (i->second == 200));
        BOOST_TEST(b.left.at(2) == 200);
    }

    // successful replace in right map view
    {
        bm::right_iterator i = b.right.begin();

        result = b.right.replace(i, bm::right_value_type(100, 1));

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST((i->first == 100) && (i->second == 1));
        BOOST_TEST(b.right.at(100) == 1);

        result = b.right.replace_key(i, 200);

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST((i->first == 200) && (i->second == 1));
        BOOST_TEST(b.right.at(200) == 1);

        result = b.right.replace_data(i, 2);

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST((i->first == 200) && (i->second == 2));
        BOOST_TEST(b.right.at(200) == 2);
    }

    // successful replace in set of relations view
    {
        bm::iterator i = b.begin();

        result = b.replace(i, bm::value_type(1, 100));

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST((i->left == 1) && (i->right == 100));
        BOOST_TEST(b.left.at(1) == 100);
        BOOST_TEST(b.right.at(100) == 1);

        result = b.replace_left(i, 2);

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST((i->left == 2) && (i->right == 100));
        BOOST_TEST(b.left.at(2) == 100);
        BOOST_TEST(b.right.at(100) == 2);

        result = b.replace_right(b.begin(), 200);

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST((i->left == 2) && (i->right == 200));
        BOOST_TEST(b.left.at(2) == 200);
        BOOST_TEST(b.right.at(200) == 2);
    }

    b.clear();
    b.insert(bm::value_type(1, 100));
    b.insert(bm::value_type(2, 200));

    // fail to replace in left map view
    {
        bm::left_iterator i = b.left.begin();

        result = b.left.replace(i, bm::left_value_type(2, 100));

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 2);
        BOOST_TEST((i->first == 1) && (i->second == 100));
        BOOST_TEST(b.left.at(1) == 100);
        BOOST_TEST(b.left.at(2) == 200);

        result = b.left.replace_key(i, 2);

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 2);
        BOOST_TEST((i->first == 1) && (i->second == 100));
        BOOST_TEST(b.left.at(1) == 100);
        BOOST_TEST(b.left.at(2) == 200);

        result = b.left.replace_data(i, 200);

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 2);
        BOOST_TEST((i->first == 1) && (i->second == 100));
        BOOST_TEST(b.left.at(1) == 100);
        BOOST_TEST(b.left.at(2) == 200);
    }

    // fail to replace in right map view
    {
        bm::right_iterator i = b.right.begin();

        result = b.right.replace(i, bm::right_value_type(100, 2));

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 2);
        BOOST_TEST((i->first == 100) && (i->second == 1));
        BOOST_TEST(b.right.at(100) == 1);
        BOOST_TEST(b.right.at(200) == 2);

        result = b.right.replace_key(i, 200);

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 2);
        BOOST_TEST((i->first == 100) && (i->second == 1));
        BOOST_TEST(b.right.at(100) == 1);
        BOOST_TEST(b.right.at(200) == 2);

        result = b.right.replace_data(i, 2);

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 2);
        BOOST_TEST((i->first == 100) && (i->second == 1));
        BOOST_TEST(b.right.at(100) == 1);
        BOOST_TEST(b.right.at(200) == 2);
    }

    // fail to replace in set of relations view
    {
        bm::iterator i = b.begin();

        result = b.replace(i, bm::value_type(2, 100));

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 2);
        BOOST_TEST((i->left == 1) && (i->right == 100));
        BOOST_TEST(b.left.at(1) == 100);
        BOOST_TEST(b.right.at(100) == 1);

        result = b.replace(i, bm::value_type(1, 200));

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 2);
        BOOST_TEST((i->left == 1) && (i->right == 100));
        BOOST_TEST(b.left.at(1) == 100);
        BOOST_TEST(b.right.at(100) == 1);

        result = b.replace_left(i, 2);

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 2);
        BOOST_TEST((i->left == 1) && (i->right == 100));
        BOOST_TEST(b.left.at(1) == 100);
        BOOST_TEST(b.right.at(100) == 1);

        result = b.replace_right(b.begin(), 200);

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 2);
        BOOST_TEST((i->left == 1) && (i->right == 100));
        BOOST_TEST(b.left.at(1) == 100);
        BOOST_TEST(b.right.at(100) == 1);
    }

    // modify
    // ----------------------------------------------------------------------

    b.clear();
    b.insert(bm::value_type(1, 100));

    // successful modify in left map view
    {
        result = b.left.modify_key(b.left.begin(), boost::bimaps::_key = 2);

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST(b.left.at(2) == 100);

        result = b.left.modify_data(
            b.left.begin()
          , boost::bimaps::_data = 200
        );

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST(b.left.at(2) == 200);
    }

    // successful modify in right map view
    {
        result = b.right.modify_key(b.right.begin(), boost::bimaps::_key = 100);

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST(b.right.at(100) == 2);

        result = b.right.modify_data(b.right.begin(), boost::bimaps::_data = 1);

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST(b.right.at(100) == 1);
    }

    // fail to modify in left map view
    {
        b.insert(bm::value_type(2, 200));
        result = b.left.modify_key(b.left.begin(), boost::bimaps::_key = 2);

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST(b.left.find(1) == b.left.end());
        BOOST_TEST(b.right.find(100) == b.right.end());

        b.insert(bm::value_type(1, 100));
        result = b.left.modify_data(b.left.begin(), boost::bimaps::_data = 200);

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST(b.left.find(1) == b.left.end());
        BOOST_TEST(b.right.find(100) == b.right.end());
    }

    // fail to modify in right map view
    {
        b.insert(bm::value_type(1, 100));
        result = b.right.modify_key(
            b.right.begin()
          , boost::bimaps::_key = 200
        );

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST(b.left.find(1) == b.left.end());
        BOOST_TEST(b.right.find(100) == b.right.end());

        b.insert(bm::value_type(1, 100));
        result = b.right.modify_data(
            b.right.begin()
          , boost::bimaps::_data = 2
        );

        BOOST_TEST(!result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST(b.left.find(1) == b.left.end());
        BOOST_TEST(b.right.find(100) == b.right.end());
    }
}

void test_bimap_replace_with_info() 
{
    typedef boost::bimaps
    ::bimap<int,long,boost::bimaps::with_info<int> > bm;

    bm b;
    b.insert(bm::value_type(2, 200, -2));

    BOOST_TEST(b.left.at(2) == 200);
    BOOST_TEST(b.left.info_at(2) == -2);

    // Use set view
    {
        bm::iterator i = b.begin();

        bool result = b.replace(i, bm::value_type(1, 100, -1));

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST((i->left == 1) && (i->right == 100));
        BOOST_TEST(i->info == -1);

        result = b.replace_left(i, 2);

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST((i->left == 2) && (i->right == 100));
        BOOST_TEST(i->info == -1);

        result = b.replace_right(i, 200);

        BOOST_TEST(result);
        BOOST_TEST(b.size() == 1);
        BOOST_TEST((i->left == 2) && (i->right == 200));
        BOOST_TEST(i->info == -1);
    }

    // Use map view
    {
        bm::left_iterator i = b.left.begin();

        bool result = b.left.replace(i, bm::left_value_type(1, 100, -1));

        BOOST_TEST(result);
        BOOST_TEST(b.left.size() == 1);
        BOOST_TEST((i->first == 1) && (i->second == 100));
        BOOST_TEST(i->info == -1);

        result = b.left.replace_key(i, 2);

        BOOST_TEST(result);
        BOOST_TEST(b.left.size() == 1);
        BOOST_TEST((i->first == 2) && (i->second == 100));
        BOOST_TEST(i->info == -1);

        result = b.left.replace_data(i, 200);

        BOOST_TEST(result);
        BOOST_TEST(b.left.size() == 1);
        BOOST_TEST((i->first == 2) && (i->second == 200));
        BOOST_TEST(i->info == -1);
    }
}

int main(int, char*[])
{
    test_bimap_modify();
    test_bimap_replace_with_info();
    return boost::report_errors();
}

