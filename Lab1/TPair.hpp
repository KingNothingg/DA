#ifndef TPair_HPP
#define TPair_HPP

namespace NStd{
    template <typename T1, typename T2>
    class TPair {
    public:
        T1 first;
        T2 second;
        TPair() {
        }
        TPair(T1 lhside, T2 rhside) {
            first = lhside;
            second = rhside;
        }
        ~TPair() {};
    };
}

#endif
