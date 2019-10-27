#ifndef MITAMA_RESULT_CONCEPTS_SATISFY_HPP
#define MITAMA_RESULT_CONCEPTS_SATISFY_HPP

namespace mitama {

template <class T, template <class> class Pred >
concept satisfy = Pred<T>::value;

}

#endif
