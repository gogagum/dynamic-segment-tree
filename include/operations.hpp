#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include <algorithm>

namespace lst::op {

template <class ValT>
struct ValValMaxGetOp {
    ValT operator()(const ValT& f, const ValT& s) const {
        return std::max(f, s);
    }
};

template <class ValT>
struct ValValMinGetOp {
    ValT operator()(const ValT& f, const ValT& s) const {
        return std::min(f, s);
    }
};

template <class ValT>
struct ValKeyKeyIgnoringGetOp {
    template<std::integral KeyT>
    ValT operator()(const ValT& f, KeyT s) const {
        return f;
    }
};

}

#endif // OPERATIONS_HPP
