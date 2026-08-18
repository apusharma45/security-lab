#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>

using namespace std;
using int128 = __int128_t;


// Extended Euclidean Algorithm
int128 extendedGCD(int128 a, int128 b, int128& x, int128& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    int128 x1, y1;

    int128 gcd = extendedGCD(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return gcd;
}


// Find modular inverse of e modulo phi
int128 modInverse(int128 e, int128 phi) {
    int128 x, y;

    int128 gcd = extendedGCD(e, phi, x, y);

    if (gcd != 1) {
        throw runtime_error("Modular inverse does not exist");
    }

    return (x % phi + phi) % phi;
}


// Overflow-safe modular multiplication
int128 modMultiply(int128 a, int128 b, int128 modulus) {
    int128 result = 0;

    a %= modulus;

    while (b > 0) {

        if (b % 2 == 1) {
            if (result >= modulus - a)
                result = result - (modulus - a);
            else
                result += a;
        }

        if (a >= modulus - a)
            a = a - (modulus - a);
        else
            a += a;

        b /= 2;
    }

    return result;
}


// Modular exponentiation
int128 modPower(int128 base, int128 exponent, int128 modulus) {
    int128 result = 1;

    base %= modulus;

    while (exponent > 0) {

        if (exponent % 2 == 1) {
            result = modMultiply(result, base, modulus);
        }

        base = modMultiply(base, base, modulus);

        exponent /= 2;
    }

    return result;
}


// Create RSA signature using private key
int128 createSignature(int128 message, int128 d, int128 n) {

    if (message < 0 || message >= n) {
        throw invalid_argument("Message must satisfy 0 <= message < n");
    }

    return modPower(message, d, n);
}


// Verify RSA signature using public key
bool verifySignature(int128 message,
                     int128 signature,
                     int128 e,
                     int128 n) {

    int128 recoveredMessage =
        modPower(signature, e, n);

    return recoveredMessage == message;
}


// Convert int128 to string
string intToString(int128 value) {

    if (value == 0)
        return "0";

    string text;

    while (value > 0) {
        text += '0' + value % 10;
        value /= 10;
    }

    reverse(text.begin(), text.end());

    return text;
}


// Convert string to int128
int128 stringToInt(string text) {

    int128 value = 0;

    for (int i = 0; i < text.size(); ++i) {
        value = value * 10 + (text[i] - '0');
    }

    return value;
}


int main() {

    int128 p = 10000000019;
    int128 q = 10000001041;
    int128 e = 65537;

    int128 n = p * q;
    int128 phi = (p - 1) * (q - 1);


    // We only need the GCD here.
    // tempX and tempY are ignored.
    int128 tempX, tempY;

    if (extendedGCD(e, phi, tempX, tempY) != 1) {
        cerr << "Invalid e: e and phi must be coprime\n";
        return 1;
    }


    // Private exponent
    int128 d = modInverse(e, phi);


    cout << "Public key (e,n): ("
         << intToString(e) << ", "
         << intToString(n) << ")\n";

    cout << "Private key (d,n): ("
         << intToString(d) << ", "
         << intToString(n) << ")\n";


    string text;

    cout << "\nEnter message (integer): ";
    cin >> text;

    int128 message = stringToInt(text);


    try {

        // Sender signs using private key
        int128 signature =
            createSignature(message, d, n);


        cout << "\nOriginal message: "
             << intToString(message) << '\n';

        cout << "Signature:        "
             << intToString(signature) << '\n';


        // Receiver verifies using public key
        int128 recoveredMessage =
            modPower(signature, e, n);


        cout << "Recovered message: "
             << intToString(recoveredMessage) << '\n';


        if (verifySignature(message, signature, e, n)) {
            cout << "Signature verification: VALID\n";
        }
        else {
            cout << "Signature verification: INVALID\n";
        }

    }
    catch (const exception& error) {

        cerr << "Error: "
             << error.what()
             << '\n';

        return 1;
    }


    return 0;
}
