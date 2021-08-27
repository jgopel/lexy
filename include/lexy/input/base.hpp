// Copyright (C) 2020-2021 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef LEXY_INPUT_BASE_HPP_INCLUDED
#define LEXY_INPUT_BASE_HPP_INCLUDED

#include <lexy/_detail/config.hpp>
#include <lexy/encoding.hpp>

#if 0
/// Readers are non-owning, cheaply copyable types.
class Reader
{
public:
    /// The encoding the input uses.
    using encoding = XXX_encoding;

    /// An iterator of char_type, not int_type.
    using iterator = ForwardIterator;

    /// If the reader is at eof, returns Encoding::eof().
    /// Otherwise, returns Encoding::to_int_type(/* current character */).
    typename Encoding::int_type peek() const;

    /// Advances to the next character in the input.
    void bump();

    /// Returns an iterator to the current character.
    /// The following code must produce a valid range:
    /// ```
    /// auto begin = reader.cur();
    /// reader.bump();
    /// ... // more bumps
    /// auto end = reader.cur();
    /// ```
    iterator cur() const;
};

/// An Input produces a reader.
class Input
{
public:
    /// Returns a reader to the beginning of the input.
    Reader reader() const &;
};
#endif

namespace lexy::_detail
{
template <typename Encoding, typename Iterator, typename Sentinel = Iterator>
class range_reader
{
public:
    using encoding = Encoding;
    using iterator = Iterator;

    constexpr explicit range_reader(Iterator begin, Sentinel end) noexcept : _cur(begin), _end(end)
    {}

    constexpr auto peek() const noexcept
    {
        if (_cur == _end)
            return encoding::eof();
        else
            return encoding::to_int_type(*_cur);
    }

    constexpr void bump() noexcept
    {
        ++_cur;
    }

    constexpr iterator cur() const noexcept
    {
        return _cur;
    }

private:
    Iterator                   _cur;
    LEXY_EMPTY_MEMBER Sentinel _end;
};
} // namespace lexy::_detail

namespace lexy
{
template <typename Input>
using input_reader = decltype(LEXY_DECLVAL(Input).reader());

template <typename Reader, typename CharT>
constexpr bool char_type_compatible_with_reader
    = (std::is_same_v<CharT, typename Reader::encoding::char_type>)
      || Reader::encoding::template is_secondary_char_type<CharT>();

/// Creates a reader that only reads until the given end.
template <typename Reader>
constexpr auto partial_reader(Reader reader, typename Reader::iterator end)
{
    return _detail::range_reader<typename Reader::encoding, typename Reader::iterator>(reader.cur(),
                                                                                       end);
}
} // namespace lexy

#endif // LEXY_INPUT_BASE_HPP_INCLUDED

