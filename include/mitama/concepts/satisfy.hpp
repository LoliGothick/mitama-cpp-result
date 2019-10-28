#ifndef MITAMA_RESULT_CONCEPTS_SATISFY_HPP
#define MITAMA_RESULT_CONCEPTS_SATISFY_HPP

namespace mitama {

template <class T, template <class> class Pred, class... Types>
concept satisfy = Pred<T, Types...>::value;

template <class T, template <class> class Pred, class... Types>
concept decay_satisfy = Pred<std::decay_t<T>, Types...>::value;

}

#endif
