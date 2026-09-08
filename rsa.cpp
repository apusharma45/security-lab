#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>

using namespace std;
using int128 = __int128_t;
using uint128 = __uint128_t;

int128 gcdValue(int128 a, int128 b) {
    while(b != 0) {
        int128 remainder = a%b;
        a = b;
        b = remainder;
    }
    return a;
}

int128 extendedGCD(int128 a, int128 b, int128& x, int128& y) {
    if(b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int128 x1, y1;
    int128 gcd = extendedGCD(b, a%b, x1, y1);
    x = y1;
    y = x1 - (a/b)*y1;

    return gcd;
}

int128 modInverse(int128 e, int128 phi) {
    int128 x, y;

    int128 gcd = extendedGCD(e, phi, x, y);
    if (gcd != 1) {
        throw runtime_error("Modular inverse does not exist");
    }
    return (x%phi+phi)%phi;
}

int128 modMultiply(int128 a, int128 b, int128 modulus) {
    int128 result = 0;

    a %= modulus;

    while (b > 0) {
        if (b % 2 == 1) {
            if (result >= modulus - a) {
                result = result - (modulus - a);
            } else {
                result += a;
            }
        }

        if (a >= modulus - a) {
            a = a - (modulus - a);
        } else {
            a += a;
        }

        b /= 2;
    }

    return result;
}

int128 modPower(int128 base, int128 exponent, int128 modulus) {
    int128 result = 1;
    base %= modulus;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = modMultiply(result, base, modulus);
        }
        base = modMultiply(base, base, modulus);
        exponent/=2;
    }
    return result;

}

int128 encryptRSA(int128 message, int128 e, int128 n) {
    if(message < 0 || message >= n) {
        throw invalid_argument("Message must satisfy 0 <= message < n");
    }
    return modPower(message, e, n);
}

int128 decryptRSA(int128 ciphertext, int128 d, int128 n) {
    return modPower(ciphertext, d, n);
}

string intToString(int128 value) {
    if(value == 0) return "0";
    string text;
    while(value) {
        text += '0' + value%10;
        value /= 10;
    }
    reverse(text.begin(), text.end());
    return text;
}

int128 stringToInt(string text) {
    int128 value = 0;
    for(int128 i=0; i<text.size(); ++i) {
        value = value*10 + (text[i]-'0');
    }
    return value;
}

int main() {
    int128 p = 10000000019;
    int128 q = 10000001041;
    int128 e = 65537;
    int128 n = p*q;
    int128 phi = (p-1)*(q-1);


    if(gcdValue(e,phi) != 1) {
        cerr << "Invalid e: e and phi must be coprime\n";
    }

    int128 d = modInverse(e, phi);
    int128 d1 = d+phi;
    int128 d2 = d+2*phi;
    cout << intToString(d2) << endl;

    cout << "public key(e,n): (" << intToString(e) << ", "
         << intToString(n) << ")" << endl;

    cout << "private key(d,n): (" << intToString(d) << ", "
         << intToString(n) << ")" << endl;

    int128 message;
    string text;
    cout << "enter message (int128): ";

    cin >> text;
    message = stringToInt(text);

    try {
        int128 ciphertext = encryptRSA(message, e, n);
        int128 decrypted = decryptRSA(ciphertext, d, n);

        cout << "Original message:  " << intToString(message) << '\n';
        cout << "Ciphertext:        " << intToString(ciphertext) << '\n';
        cout << "Decrypted message: " << intToString(decrypted) << '\n';
    }
    catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}