#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

int extendedGCD(int a, int b, int& x, int& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    int x1, y1;
    int gcd = extendedGCD(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return gcd;
}

int modInverse(int a, int modulus) {
    int x, y;

    extendedGCD(a, modulus, x, y);

    return (x % modulus + modulus) % modulus;
}

int modPower(int base, int e, int modulus) {
    int result = 1;

    base = base % modulus;

    while (e > 0) {

        if (e % 2 == 1) {
            result = (result * base) % modulus;
        }

        base = (base * base) % modulus;

        e /= 2;
    }

    return result;
}

void encrypt(int p, int g, int y,
             int k, int m,
             int& c1, int& c2) {

    c1 = modPower(g, k, p);

    int shared = modPower(y, k, p);

    c2 = (m * shared) % p;
}

int decrypt(int p, int x,
            int c1, int c2) {

    int shared = modPower(c1, x, p);

    int inverse = modInverse(shared, p);

    return (c2 * inverse) % p;
}

int main() {

    int p = 467;
    int g = 2;

    int x = 127;

    int y = modPower(g, x, p);

    int m = 100;

    int k = 53;

    int c1, c2;

    encrypt(p, g, y, k, m, c1, c2);

    cout << "c1: " << c1 << endl;
    cout << "c2: " << c2 << endl;

    int recovered = decrypt(p, x, c1, c2);

    cout << "recovered: "
         << recovered
         << endl;

    return 0;
}