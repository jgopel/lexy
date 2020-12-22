// Copyright (C) 2020 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <lexy/dsl/until.hpp>

#include "verify.hpp"
#include <lexy/dsl/peek.hpp>

TEST_CASE("dsl::until()")
{
    constexpr auto rule = until(LEXY_LIT("!"));
    CHECK(lexy::is_rule<decltype(rule)>);

    struct callback
    {
        const char* str;

        constexpr int success(const char* cur)
        {
            return int(cur - str);
        }

        constexpr int error(test_error<lexy::expected_literal> e)
        {
            CONSTEXPR_CHECK(e.position() == lexy::_detail::string_view(str).end());
            CONSTEXPR_CHECK(e.string() == "!");
            return -1;
        }
    };

    constexpr auto empty = rule_matches<callback>(rule, "");
    CHECK(empty == -1);

    constexpr auto zero = rule_matches<callback>(rule, "!");
    CHECK(zero == 1);
    constexpr auto one = rule_matches<callback>(rule, "a!");
    CHECK(one == 2);
    constexpr auto two = rule_matches<callback>(rule, "ab!");
    CHECK(two == 3);

    constexpr auto unterminated = rule_matches<callback>(rule, "abc");
    CHECK(unterminated == -1);
}

TEST_CASE("dsl::until().or_eof()")
{
    constexpr auto atom = until(LEXY_LIT("!")).or_eof();

    constexpr auto empty = pattern_matches(atom, "");
    CHECK(empty);
    CHECK(empty.match().empty());

    constexpr auto zero = pattern_matches(atom, "!");
    CHECK(zero);
    CHECK(zero.match().size() == 1);
    constexpr auto one = pattern_matches(atom, "a!");
    CHECK(one);
    CHECK(one.match().size() == 2);
    constexpr auto two = pattern_matches(atom, "xy!");
    CHECK(two);
    CHECK(two.match().size() == 3);

    constexpr auto unterminated = pattern_matches(atom, "abc");
    CHECK(unterminated);
    CHECK(unterminated.match().size() == 3);
}

