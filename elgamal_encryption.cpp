#include <iostream>
#include <algorithm>
#include <string>

using namespace std;
using int128 = __int128_t;

int128 extendedGCD(int128 a, int128 b, int128& x, int128& y){
    if(b==0) {
        x = 1;
        y = 0;
        return a;
    }

    int128 x1, y1;
    int128 gcd = extendedGCD(b, a%b, x1, y1);

    x = y1;
    y = x1-(a/b)*y1;

    return gcd;
}

int128 modInverse(int128 a, int128 modulus) {
    int128 x, y;
    extendedGCD(a, modulus, x, y);

    return (x%modulus+modulus)%modulus;
}

int128 modMultiply(int128 a, int128 b, int128 modulus) {
    int128 result = 0;

    a = a%modulus;

    while(b>0) {

        if(b%2==1) {
            if(result >= modulus-a) {
                result = result-(modulus-a);
            }
            else {
                result = result+a;
            }
        }

        if(a >= modulus-a) {
            a = a-(modulus-a);
        }
        else {
            a += a;
        }

        b /= 2;
    }

    return result;
}

int128 modPower(int128 base, int128 e, int128 modulus) {
    int128 result = 1;

    base = base%modulus;

    while(e>0) {

        if(e%2==1) {
            result = modMultiply(result, base, modulus);
        }

        base = modMultiply(base, base, modulus);

        e /= 2;
    }

    return result;
}

void encrypt(int128 p, int128 g, int128 y,
             int128 k, int128 m,
             int128& c1, int128& c2) {

    c1 = modPower(g, k, p);

    int128 shared = modPower(y, k, p);

    c2 = modMultiply(m, shared, p);
}

int128 decrypt(int128 p, int128 x,
               int128 c1, int128 c2) {

    int128 shared = modPower(c1, x, p);

    int128 inverse = modInverse(shared, p);

    return modMultiply(c2, inverse, p);
}

string int128ToString(int128 n) {
    if(n==0) {
        return "0";
    }

    string text;

    while(n>0) {
        text += char('0' + n%10);
        n /= 10;
    }

    reverse(text.begin(), text.end());

    return text;
}

int main() {

    int128 p = 467;
    int128 g = 2;

    int128 x = 127;

    int128 y = modPower(g, x, p);

    int128 m = 100;

    int128 k = 53;

    int128 c1, c2;

    encrypt(p, g, y, k, m, c1, c2);

    cout << "c1: " << int128ToString(c1) << endl;
    cout << "c2: " << int128ToString(c2) << endl;

    int128 recovered = decrypt(p, x, c1, c2);

    cout << "recovered: "
         << int128ToString(recovered)
         << endl;

    return 0;
}
