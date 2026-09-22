#include <bits/stdc++.h>
using namespace std;

struct point {
    int x, y;
    bool infinity = false;
};

int gcd(int a, int b, int& x, int& y) {
    if(b==0) {
        x=1;
        y=0;
        return a;
    }

    int x1, y1;
    int gcdValue = gcd(b, a%b, x1, y1);

    x=y1;
    y=x1-(a/b)*y1;

    return gcdValue;
}

int modInverse(int a, int m) {
    int x, y;

    gcd(a,m,x,y);

    return (x%m+m)%m;
}

point add(point p, point q, int a, int m) {
    if(p.infinity) return q;
    if(q.infinity) return p;

    int x1 = p.x;
    int x2 = q.x;
    int y1 = p.y;
    int y2 = q.y;

    if(x1==x2 && (y1+y2)%m == 0)
        return {0, 0, true};

    int s;

    if(x1==x2 && y1==y2) {
        int num = (3*x1*x1+a)%m;
        int den = modInverse(2*y1, m);

        s = (num*den)%m;
    }
    else {
        int num = (y2-y1+m)%m;
        int den = (x2-x1+m)%m;

        s = (num*modInverse(den, m))%m;
    }

    int x3 = ((s*s-x1-x2)%m+m)%m;
    int y3 = ((s*(x1-x3)-y1)%m+m)%m;

    return {x3, y3, false};
}

point multiply(int k, point p, int a, int m) {
    point result = {0, 0, true};

    while(k>0) {
        if(k%2==1) {
            result = add(result, p, a, m);
        }

        p = add(p, p, a, m);
        k/=2;
    }

    return result;
}

point negative(point p, int m) {
    if(p.infinity) return p;

    return {p.x, (m-p.y)%m, false};
}

void encrypt(point g, point q, point message, int k, int a, int m, point& c1, point& c2) {
    c1 = multiply(k, g, a, m);

    c2 = add(
        message,
        multiply(k, q, a, m),
        a,
        m
    );
}

point decrypt(int d, point c1, point c2, int a, int m) {
    point shared = negative(
        multiply(d, c1, a, m),
        m
    );

    return add(c2, shared, a, m);
}

int main() {

    int m = 17;
    int a = 2;

    point g = {5, 1};

    int d = 7;

    point q = multiply(d, g, a, m);

    point message = {6, 3};

    int k = 3;

    point c1, c2;

    encrypt(g, q, message, k, a, m, c1, c2);

    cout << "Public key: ("
         << q.x << ", " << q.y << ")" << endl;

    cout << "Message: ("
         << message.x << ", " << message.y << ")" << endl;

    cout << "Encrypted C1: ("
         << c1.x << ", " << c1.y << ")" << endl;

    cout << "Encrypted C2: ("
         << c2.x << ", " << c2.y << ")" << endl;

    point recovered = decrypt(
        d,
        c1,
        c2,
        a,
        m
    );

    cout << "Recovered: ("
         << recovered.x << ", " << recovered.y << ")" << endl;

    return 0;
}