#include <iostream>
#include <stdexcept>
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

int128 modInverse(int128 e, int128 phi) {
    int128 x, y;
    extendedGCD(e, phi, x, y);
    return (x%phi+phi)%phi;
}

int128 modMultiply(int128 a, int128 b, int128 modulus) {
    int128 result = 0;
    a = a%modulus;
    while (b>0) {
        if(b%2==1) {
            if(result >= modulus-a) {
                result = result - (modulus-a);
            }
            else result = result + a;
        }
        if(a >= modulus - a) {
            a = a - (modulus-a);
        }
        else a+=a;
        b /= 2;
    }
    return result;
}

int128 modPower(int128 base, int128 e, int128 modulus){
    int128 result = 1;
    base = base%modulus;
    while(e>0) {
        if(e%2==1) {
            result = modMultiply(result, base, modulus);
        }
        base = modMultiply(base,base,modulus);
        e/=2;
    }
    return result;
}

int128 sign(int128 d, int128 n, int128 m) {
    return modPower(m,d,n);
}
bool verify(int128 e, int128 n, int128 m, int128 signedValue) {
    int128 recovered = modPower(signedValue, e, n);
    if(recovered == m) {
        return true;
    }
    return false;
}

string int128ToString(int128 n) {
    if(n==0) {
        return "0";
    }
    string text;
    while(n>0) {
        text += char('0' + n % 10);
        n/=10;
    }
    reverse(text.begin(), text.end());
    return text;
}

int main() {
    int128 p = 10000000019;
    int128 q = 10000001041;
    int128 e = 65537;

    int128 n = p*q;
    int128 phi = (p-1)*(q-1);

    int128 x, y;

    if(extendedGCD(e,phi, x, y) !=1){
        cout << "gcd must be equal to 1" << endl;
        return 0;
    }
    int128 d = modInverse(e, phi);
    int128 m = 100;
    int128 signedValue = sign(d, n, m);
    cout << "signed: " << int128ToString(signedValue) << endl;
    if(verify(e, n, m, signedValue)) {
        cout << "signature verified" << endl;
    }

    return 0;
}
