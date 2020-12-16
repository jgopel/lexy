// Copyright (C) 2020 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <lexy/dsl/eof.hpp>

#include "verify.hpp"

TEST_CASE("dsl::eof")
{
    constexpr auto rule = lexy::dsl::eof;
    CHECK(lexy::is_rule<decltype(rule)>);
    CHECK(lexy::is_token<decltype(rule)>);

    struct callback
    {
        const char* str;

        constexpr int success(const char* cur)
        {
            CONSTEXPR_CHECK(cur == str);
            return 0;
        }

        constexpr int error(test_error<lexy::expected_char_class> e)
        {
            CONSTEXPR_CHECK(e.position() == str);
            CONSTEXPR_CHECK(e.character_class() == "EOF");
            return -1;
        }
    };

    constexpr auto empty = verify<callback>(rule, "");
    CHECK(empty == 0);

    constexpr auto non_empty = verify<callback>(rule, "abc");
    CHECK(non_empty == -1);
}

