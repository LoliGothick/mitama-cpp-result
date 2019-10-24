#ifndef MITAMA_RESULT_CONCEPTS_FORMAT_HPP
#define MITAMA_RESULT_CONCEPTS_FORMAT_HPP

#include <mitama/result/traits/impl_traits.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <boost/format.hpp>

namespace mitama {
    template <class T>
    concept display = mitama::trait::formattable<std::decay_t<T>>::value;

    template <display T>
    class display_closure {
        T value_;
    public:
        template <class T_>
        constexpr explicit display_closure(T_&& v): value_(std::forward<T_>(v)) {}

        friend std::ostream& operator<<(std::ostream& os, display_closure const& closure) {
            using namespace std::literals::string_literals;
            auto _format = boost::hana::fix(boost::hana::overload_linearly(
                [](auto, auto const& x) -> std::enable_if_t<trait::formattable_element<std::decay_t<decltype(x)>>::value, std::string> {
                    return boost::hana::overload_linearly(
                        [](std::monostate) { return "()"s; },
                        [](std::string_view x) { return (boost::format("\"%1%\"") % x).str(); },
                        [](auto const& x) { return (boost::format("%1%") % x).str(); })
                        (x);        
                    },
                    [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_dictionary<std::decay_t<decltype(x)>>::value, std::string> {
                        if (x.empty()) return "{}"s;
                        using std::begin, std::end;
                        auto iter = begin(x);
                        std::string str = "{"s + (boost::format("%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
                        while (++iter != end(x)) {
                            str += (boost::format(",%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
                        }
                        return str += "}";
                    },
                    [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_range<std::decay_t<decltype(x)>>::value, std::string> {
                        if (x.empty()) return "[]"s;
                        using std::begin, std::end;
                        auto iter = begin(x);
                        std::string str = "["s + _fmt(*iter);
                        while (++iter != end(x)) {
                            str += (boost::format(",%1%") % _fmt(*iter)).str();
                        }
                        return str += "]";
                    },
                    [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_tuple<std::decay_t<decltype(x)>>::value, std::string> {
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
