#ifndef MITAMA_RESULT_CONCEPTS_FORMAT_HPP
#define MITAMA_RESULT_CONCEPTS_FORMAT_HPP

#include <mitama/concepts/formattable.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <boost/hana/functional/fix.hpp>
#include <fmt/core.h>

namespace mitama {
    template <class T>
    concept display = mitama::trait::formattable<std::decay_t<T>>::value;

    template <display T>
    class display_closure {
        T value_;
    public:
        template <class T_>
        constexpr explicit display_closure(T_&& v) // NOLINT(bugprone-forwarding-reference-overload)
            : value_(std::forward<T_>(v)) {}

        [[nodiscard]] std::string as_str() const {
            std::stringstream ss;
            ss << *this;
            return ss.str();
        }

        friend std::ostream& operator<<(std::ostream& os, display_closure const& closure) {
            using namespace std::literals::string_literals;

            auto _format = boost::hana::fix(boost::hana::overload_linearly(
                [](auto, trait::formattable_element auto const& x) {
                    return boost::hana::overload_linearly(
                        [](std::monostate) { return "()"s; },
                        [](std::string_view x) { return fmt::format("\"{}\"", x); },
                        [](auto const& x) {
                            std::stringstream ss;
                            ss << x;
                            return ss.str();
                        })
                        (x);
                    },
                    [](auto _fmt, trait::formattable_dictionary auto const& x) {
                        if (x.empty()) return "{}"s;
                        using std::begin, std::end;
                        auto iter = begin(x);
                        auto str = "{"s + fmt::format("{}: {}", _fmt(std::get<0>(*iter)), _fmt(std::get<1>(*iter)));
                        while (++iter != end(x)) {
                            str += fmt::format(",{}: {}", _fmt(std::get<0>(*iter)), _fmt(std::get<1>(*iter)));
                        }
                        return str += "}";
                    },
                    [](auto _fmt, trait::formattable_range auto const& x) {
                        if (x.empty()) return "[]"s;
                        using std::begin, std::end;
                        auto iter = begin(x);
                        std::string str = "["s + _fmt(*iter);
                        while (++iter != end(x)) {
                            str += fmt::format(",{}", _fmt(*iter));
                        }
                        return str += "]";
                    },
                    [](auto _fmt, trait::formattable_tuple auto const& x) {
                        if constexpr (std::tuple_size_v<std::decay_t<decltype(x)>> == 0) {
                            return "()"s;
                        }
                        else {
                            return std::apply(
                            [_fmt](auto const& head, auto const&... tail) {
                                return "("s + _fmt(head) + ((("," + _fmt(tail))) + ...) + ")"s;
                            }, x);
                        }
                    }));
            return os << _format( closure.value_ );
        }
    };

    inline constexpr auto as_display(display auto const& value) {
        return display_closure<decltype(value)>{value};
    }
}

#endif
