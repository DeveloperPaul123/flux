
// Copyright (c) 2022 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FLUX_CORE_MACROS_HPP_INCLUDED
#define FLUX_CORE_MACROS_HPP_INCLUDED

#include <version>

#define FLUX_FWD(x) static_cast<decltype(x)&&>(x)

#define FLUX_DECLVAL(...)  ((static_cast<__VA_ARGS__(*)()noexcept>(nullptr))())

#ifdef __GNUC__
#define FLUX_ALWAYS_INLINE [[gnu::always_inline]]
#else
#define FLUX_ALWAYS_INLINE
#endif

#define FLUX_NO_UNIQUE_ADDRESS [[no_unique_address]]

#define FLUX_FOR(_flux_var_decl_, ...)                    \
    if (auto&& _flux_seq_ = __VA_ARGS__; true)           \
        for (auto _flux_cur_ = ::flux::first(_flux_seq_);   \
             !::flux::is_last(_flux_seq_, _flux_cur_);     \
              ::flux::inc(_flux_seq_, _flux_cur_))         \
            if (_flux_var_decl_ = ::flux::read_at(_flux_seq_, _flux_cur_); true)

#ifdef FLUX_MODULE_INTERFACE
#define FLUX_EXPORT export
#else
#define FLUX_EXPORT
#endif

#endif // FLUX_CORE_MACROS_HPP_INCLUDED
