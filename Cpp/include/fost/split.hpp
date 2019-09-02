/**
    Copyright 2016-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <fost/unicode>


namespace fostlib {


    /// Iterates over a split. Use the `fostlib::splitter` functions to
    /// to create these for common use cases.
    /// * `C` some sort of View into a container
    /// * `R` the result of a part of the view
    /// * `PL` the length of the matched pattern
    /// * `L` the lambda that performs the matching
    template<
            typename C,
            typename R,
            std::size_t PL = 1u,
            typename L =
                    std::function<bool(typename C::const_iterator::value_type)>>
    class splitter_result {
      public:
        const C container; // TODO: Make private
        const L predicate; // TODO: Make private

        using container_type = C;
        using results_type = R;

        /// Construct from a container and lambda
        splitter_result(const C &c, L lambda)
        : container(c), predicate(lambda) {}

        /// Copy construct from a related type with another lambda
        template<typename OL>
        splitter_result(const splitter_result<C, R, PL, OL> &s)
        : container(s.container), predicate(s.predicate) {}

        /// Iterator for going over the results
        class const_iterator {
            const L &predicate;
            using c_iterator = typename C::const_iterator;
            c_iterator pos, next, end;

          public:
            const_iterator(const L &p, c_iterator b, c_iterator e)
            : predicate(p), pos(b), next(std::find_if(b, e, p)), end(e) {}

            results_type operator*() const { return results_type(pos, next); }

            const_iterator &operator++() {
                if (next != end) {
                    std::advance(next, PL);
                    pos = next;
                    next = std::find_if(pos, end, predicate);
                } else {
                    pos = next;
                }
                return *this;
            }

            bool operator==(const_iterator o) const {
                // It's probably enough to just compare `pos`
                return pos == o.pos && next == o.next && end == o.end;
            }
            bool operator!=(const_iterator o) const {
                return not((*this) == o);
            }
        };

        /// Return iterators
        const_iterator begin() const {
            return const_iterator(
                    predicate, container.begin(), container.end());
        }
        const_iterator end() const {
            return const_iterator(predicate, container.end(), container.end());
        }
    };


    /// Split a character string on a given code point
    inline auto splitter(f5::u8view str, utf32 cp) {
        auto lambda = [cp](utf32 c) { return cp == c; };
        return splitter_result<f5::u8view, f5::u8view, 1u, decltype(lambda)>(
                str, lambda);
    }


}
