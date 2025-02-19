
// Copyright (c) 2023 NVIDIA Corporation (reply-to: brycelelbach@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <flux/op/cartesian_product.hpp>
#include <flux/source/iota.hpp>
#include <flux/op/for_each.hpp>
#include <flux/op/filter.hpp>

#include "ranges_cartesian_product.hpp"

#include <ranges>
#include <algorithm>

void memset_2d_reference(double* A, flux::distance_t N, flux::distance_t M)
{
    for (flux::distance_t i = 0; i != N; ++i)
        for (flux::distance_t j = 0; j != M; ++j)
            A[i * M + j] = 0.0;
}

void memset_2d_std_cartesian_product_iota(double* A, flux::distance_t N, flux::distance_t M)
{
    std::ranges::for_each(
        std::views::cartesian_product(std::views::iota(0, N), std::views::iota(0, M)),
        flux::unpack([&] (auto i, auto j) {
            A[i * M + j] = 0.0;
        }));
}

void memset_2d_flux_cartesian_product_iota(double* A, flux::distance_t N, flux::distance_t M)
{
    flux::for_each(
        flux::cartesian_product(flux::ints(0, N), flux::ints(0, M)),
        flux::unpack([&] (auto i, auto j) {
            A[i * M + j] = 0.0;
        }));
}

void memset_diagonal_2d_reference(double* A, flux::distance_t N, flux::distance_t M)
{
    for (flux::distance_t i = 0; i != N; ++i)
        for (flux::distance_t j = 0; j != M; ++j)
            if (i == j) A[i * M + j] = 0.0;
}

void memset_diagonal_2d_std_cartesian_product_iota_filter(double* A, flux::distance_t N, flux::distance_t M)
{
    std::ranges::for_each(
        std::views::cartesian_product(std::views::iota(0, N), std::views::iota(0, M))
            | std::views::filter(flux::unpack([] (auto i, auto j) { return i == j; })),
        flux::unpack([&] (auto i, auto j) {
            A[i * M + j] = 0.0;
        }));
}

void memset_diagonal_2d_flux_cartesian_product_iota_filter(double* A, flux::distance_t N, flux::distance_t M)
{
    flux::for_each(
        flux::cartesian_product(flux::ints(0, N), flux::ints(0, M))
            .filter(flux::unpack([] (auto i, auto j) { return i == j; })),
        flux::unpack([&] (auto i, auto j) {
            A[i * M + j] = 0.0;
        }));
}

