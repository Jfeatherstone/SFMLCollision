#include "Line.hpp"

int main() {

    Line l(Vector2f(0, 0), Vector2f(2, 2));
    cout << l.getSlope() << endl;
    cout << l.getAngle() << endl;
    cout << l.getIntercept() << endl;
    return 0;
}