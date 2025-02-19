
// Copyright (c) 2023 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

import flux;

int main()
{
    constexpr int arr[] = {1, 2, 3, 4, 5};
    static_assert(flux::sum(arr) == 15);
}
