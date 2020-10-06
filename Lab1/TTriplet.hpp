#ifndef TTriplet_HPP
#define TTriplet_HPP

namespace NStd{
    template <typename T1, typename T2, typename T3>
    class TTriplet {
    public:
        T1 first;
        T2 second;
        T3 third;
        TTriplet() {
        }
        TTriplet(T1 lhside, T2 rhside) {
            first = lhside;
            second = rhside;
        }
        ~TTriplet() {};
    };
}

#endif
