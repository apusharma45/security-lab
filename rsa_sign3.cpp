#include <iostream>
#include<algorithm>
#include <string>

using namespace std;
using int128 = __int128_t;

int128 gcd(int128 a, int128 b, int128& x, int128& y) {
    if(b==0) {
        x = 1;
        y = 0;
        return a;
    }
    int128 x1, y1;
    int128 gcdValue = gcd(b, a%b, x1, y1);
    x = y1;
    y = x1 - (a/b)*y1;
    return gcdValue;
}

int128 modInverse(int128 e, int128 phi) {
    int128 x, y;
    gcd(e, phi, x, y);
    return (x%phi+phi)%phi;
}

int128 modMultiply(int128 a, int128 b, int128 m) {
    a %= m;
    int128 result = 0;
    while(b>0) {
        if(b%2==1) {
            if(result >= m-a) {
                result = result -(m-a);
            }
            else result+=a;
        }
        if(a >= m-a) {
            a = a-(m-a);
        }
        else a+=a;
        b/=2;
    }
    return result;
}

int128 modPower(int128 base, int128 b, int128 m) {
    base%=m;
    int128 result = 1;
    while(b>0) {
        if(b%2==1) {
            result = modMultiply(result, base, m);
        }
        base = modMultiply(base, base, m);
        b/=2;
    }
    return result;
}

int128 sign(int128 message, int128 d, int128 m) {
    return modPower(message, d, m);
}

bool verify(int128 message, int128 signed_message, int128 e, int128 m) {
    int128 recoverd = modPower(signed_message, e, m);
    if(recoverd == message) {
        cout << "verified" << endl;
        return true;
    }
    return false;
}

string converter(int128 n) {
    if(n==0) return "0";
    string text;
    while(n>0) {
        text+=char('0'+n%10);
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
    if(gcd(e, phi, x, y) != 1) {
        cout << "gcd must be 1" << endl;
        return 0;
    }
    int128 d = modInverse(e, phi);
    int128 message = 100;
    int128 signed_message = sign(message, d, n);
    cout << "signed: " << converter(signed_message) << endl;
    verify(message, signed_message, e, n);
}