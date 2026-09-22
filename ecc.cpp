#include <bits/stdc++.h>
using namespace std;

struct point {
    int x, y;
    bool infinity = false;
};

int gcd(int a, int b, int& x, int& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    int x1, y1;

    int gcdValue = gcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return gcdValue;
}

int modInverse(int a, int m) {
    int x, y;

    gcd(a, m, x, y);

    return (x % m + m) % m;
}

point add(point p, point q, int a, int m) {

    if (p.infinity)
        return q;

    if (q.infinity)
        return p;

    int x1 = p.x;
    int y1 = p.y;
    int x2 = q.x;
    int y2 = q.y;

    // P + (-P) = infinity
    if (x1 == x2 && (y1 + y2) % m == 0) {
        return {0, 0, true};
    }

    int s;

    // Point doubling: P + P
    if (x1 == x2 && y1 == y2) {

        int num = (3 * x1 * x1 + a) % m;
        int den = modInverse(2 * y1, m);

        s = (num * den) % m;
    }

    // Normal point addition: P + Q
    else {

        int num = (y2 - y1 + m) % m;
        int den = (x2 - x1 + m) % m;

        s = (num * modInverse(den, m)) % m;
    }

    int x3 = ((s * s - x1 - x2) % m + m) % m;
    int y3 = ((s * (x1 - x3) - y1) % m + m) % m;

    return {x3, y3, false};
}

point multiply(point p, int k, int a, int m) {

    point result = {0, 0, true};

    while (k > 0) {

        if (k % 2 == 1) {
            result = add(result, p, a, m);
        }

        p = add(p, p, a, m);

        k /= 2;
    }

    return result;
}

point negative(point p, int m) {

    if (p.infinity)
        return p;

    return {p.x, (m - p.y) % m, false};
}

void encrypt(point g, point q, point message, int k, int a, int m, point& c1, point& c2) {

    c1 = multiply(g, k, a, m);

    c2 = add(
        message,
        multiply(q, k, a, m),
        a,
        m
    );
}

point decrypt(int d, point c1, point c2, int a, int m) {

    point shared = negative(
        multiply(c1, d, a, m),
        m
    );

    return add(c2, shared, a, m);
}

int main() {

    // Curve:
    // y^2 = x^3 + 2x + 2 (mod 17)

    int p = 17;
    int a = 2;

    point G = {5, 1};

    // Private key
    int d = 7;

    // Public key Q = dG
    point publicKey = multiply(G, d, a, p);

    // Message represented as a point
    point message = {6, 3};

    // Random ephemeral key
    int k = 3;

    point C1, C2;

    encrypt(
        G,
        publicKey,
        message,
        k,
        a,
        p,
        C1,
        C2
    );

    cout << "Public key: (" 
         << publicKey.x << ", " 
         << publicKey.y << ")" << endl;

    cout << "Message: (" 
         << message.x << ", " 
         << message.y << ")" << endl;

    cout << "Encrypted C1: (" 
         << C1.x << ", " 
         << C1.y << ")" << endl;

    cout << "Encrypted C2: (" 
         << C2.x << ", " 
         << C2.y << ")" << endl;

    point recovered = decrypt(
        d,
        C1,
        C2,
        a,
        p
    );

    cout << "Recovered: (" 
         << recovered.x << ", " 
         << recovered.y << ")" << endl;

    return 0;
}