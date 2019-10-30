#ifndef MITAMA_MAYBE_RANGE_TO_MAYBE_HPP
#define MITAMA_MAYBE_RANGE_TO_MAYBE_HPP
#include <mitama/maybe/maybe.hpp>
#include <type_traits>
#include <utility>
#include <iterator>
#include <mitama/concepts/range.hpp>

namespace mitama {
    template <forward_range Range>
    auto range_to_maybe(Range&& range) {
        using std::begin, std::end;
        return begin(range) != end(range) ? maybe(just(*begin(std::forward<Range>(range)))) : nothing;
    }
}

#endif
