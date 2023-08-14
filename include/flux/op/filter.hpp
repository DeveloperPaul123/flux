
// Copyright (c) 2022 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FLUX_OP_FILTER_HPP_INCLUDED
#define FLUX_OP_FILTER_HPP_INCLUDED

#include <flux/core.hpp>
#include <flux/op/find.hpp>
#include <flux/op/for_each_while.hpp>

namespace flux {

namespace detail {

template <sequence Base, typename Pred>
class filter_adaptor : public inline_sequence_base<filter_adaptor<Base, Pred>>
{
    FLUX_NO_UNIQUE_ADDRESS Base base_;
    FLUX_NO_UNIQUE_ADDRESS Pred pred_;

public:
    constexpr filter_adaptor(decays_to<Base> auto&& base, decays_to<Pred> auto&& pred)
        : base_(FLUX_FWD(base)),
          pred_(FLUX_FWD(pred))
    {}

    [[nodiscard]]
    constexpr auto base() const& -> Base const& { return base_; }
    [[nodiscard]]
    constexpr auto base() && -> Base { return std::move(base_); }

    struct flux_sequence_traits {
        using self_t = filter_adaptor;

        static constexpr bool disable_multipass = !multipass_sequence<Base>;

        static constexpr auto first(self_t& self) -> cursor_t<Base>
        {
            return flux::find_if(self.base_, self.pred_);
        }

        static constexpr auto is_last(self_t& self, cursor_t<Base> const& cur) -> bool
        {
            return flux::is_last(self.base_, cur);
        }

        static constexpr auto read_at(self_t& self, cursor_t<Base> const& cur)
            -> element_t<Base>
        {
            return flux::read_at(self.base_, cur);
        }

        static constexpr auto inc(self_t& self, cursor_t<Base>& cur) -> void
        {
            flux::inc(self.base_, cur);
            cur = flux::slice(self.base_, std::move(cur), flux::last).find_if(self.pred_);
        }

        static constexpr auto dec(self_t& self, cursor_t<Base>& cur) -> void
            requires bidirectional_sequence<Base>
        {
            do {
                flux::dec(self.base_, cur);
            } while(!std::invoke(self.pred_, flux::read_at(self.base_, cur)));
        }

        static constexpr auto last(self_t& self) -> cursor_t<Base>
            requires bounded_sequence<Base>
        {
            return flux::last(self.base_);
        }

        static constexpr auto for_each_while(self_t& self, auto&& func) -> cursor_t<Base>
        {
            return flux::for_each_while(self.base_, [&](auto&& elem) {
                if (std::invoke(self.pred_, elem)) {
                    return std::invoke(func, FLUX_FWD(elem));
                } else {
                    return true;
                }
            });
        }
    };
};


struct filter_fn {
    template <adaptable_sequence Seq, std::move_constructible Pred>
        requires std::predicate<Pred&, element_t<Seq>>
    [[nodiscard]]
    constexpr auto operator()(Seq&& seq, Pred pred) const
    {
        return filter_adaptor<std::decay_t<Seq>, Pred>(FLUX_FWD(seq), std::move(pred));
    }
};

} // namespace detail

FLUX_EXPORT inline constexpr auto filter = detail::filter_fn{};

template <typename D>
template <typename Pred>
    requires std::predicate<Pred&, element_t<D>>
constexpr auto inline_sequence_base<D>::filter(Pred pred) &&
{
    return detail::filter_adaptor<D, Pred>(std::move(derived()), std::move(pred));
}


} // namespace flux

#endif // FLUX_OP_FILTER_HPP_INCLUDED

