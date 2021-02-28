#include <iostream>
#include <list>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <functional>

using namespace std;
namespace std {
    template <typename T>
    istream& operator >> (istream& in, tuple <T, T, T, T>& item) {
        T t1, t2;
        in >> t1 >> t2, item = make_tuple (t1, t2, 0, 0);
        return in;
    }

    template <typename T>
    ostream& operator << (ostream& out, const tuple <T, T, T, T>& item) {
        auto [t1, t2, t3, t4] = item;
        out << "{" << t1 << ", " << t2 << ", " << t3 << ", " << t4 << "} ";
        return out;
    }
}
using tp = tuple <int, int, int, int>;
using ltp = list <tp>;

int n;
ltp List;

template <class C, class F>
void custom_transform (C& Container, F Lambda) {
    for_each (Container.begin (), Container.end (), Lambda);
}

struct get_mmgl : unary_function <tp&, void> {
    static function <int (int, int)> get_util (const int& val) {
        switch (val) {
            case 0:
                return [](int a, int b) {
                    return a < b ? a : b;
                };

            case 1:
                return [](int a, int b) {
                    return a > b ? a : b;
                };

            case 2:
                return [](int a, int b) {
                    while (b) {
                        int r = a % b;
                        a = b, b = r;
                    }
                    return a;
                };

            case 3:
                return [](int a, int b) {
                    return get_util (4)(a, b) / get_util (2)(a, b);
                };

            default:
                return [](int a, int b) {
                    return a * b;
                };
        }
    }

    void operator () (tp& item) {
        auto [t1, t2, t3, t4] = item;
        item = make_tuple (get_util (0)(t1, t2),
                           get_util (1)(t1, t2),
                           get_util (2)(t1, t2),
                           get_util (3)(t1, t2));
    }
};

int main () {
    cin >> n;
    copy_n (istream_iterator <tp> (cin), n, back_inserter (List));

    custom_transform (List, get_mmgl ());

    copy_n (List.begin (), n, ostream_iterator <tp> (cout));

    return 0;
}
