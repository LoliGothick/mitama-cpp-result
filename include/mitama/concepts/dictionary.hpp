#ifndef MITAMA_RESULT_CONCEPTS_DICTIONARY_HPP
#define MITAMA_RESULT_CONCEPTS_DICTIONARY_HPP

namespace mitama {
    template <class T>
    concept dictionary = requires {
        typename T::key_type;
        typename T::mapped_type;
        
    };
}

#endif
