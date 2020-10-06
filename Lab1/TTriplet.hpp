#ifndef TTriplet_HPP
#define TTriplet_HPP

namespace NStd{
    template <typename T1, typename T2>
    class TTriplet {
    public:
        T1 first;
        T2 second;
        int changed;
        TTriplet() {
            changed = 0;
        }
        TTriplet(T1 lhside, T2 rhside) {
            first = lhside;
            second = rhside;
            changed = 0;
        }
        ~TTriplet() {};
    };
}

#endif
