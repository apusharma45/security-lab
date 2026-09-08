#include <bits/stdc++.h>
using namespace std;

int gcd(int a, int b, int& x, int& y) {
    if(b==0) {
        x = 1;
        y = 0;
        return a;
    }
    int x1,y1;
    int gcdValue = gcd(b, a%b, x1, y1);
    x = y1;
    y = x1-(a/b)*y1;
    return gcdValue;
}

int modInverse(int a, int m) {
    int x, y;
    gcd(a, m, x, y);
    return (x%m+m)%m;
}

int modPower(int a, int b, int m) {
    int result = 1;
    a%=m;
    while(b>0) {
        if(b%2==1) {
            result = (result*a)%m;
        }
        a = (a*a)%m;
        b/=2;
    }
    return result;
}

void encrypt(int p, int g, int y, int k, int m, int& c1, int& c2) {
    c1 = modPower(g, k, p);
    c2 = (m*modPower(y,k,p))%p;
}
int decrypt(int p, int x, int c1, int c2) {
    int s = modPower(c1,x,p);
    return (c2*modInverse(s,p))%p;
}


int findGenerator(int p) {
    for(int g=2; g<p; ++g) {
        set<int> values;
        for(int i=1; i<p; ++i) {
            values.insert(modPower(g,i,p));
        }
        if(values.size()==p-1) return g;
    }
    return -1;
}

int main() {
    srand(time(0));
    int p = 467;
    int g = findGenerator(p);
    cout << "g: " << g << endl;
    int x = 127;
    int m = 100;
    int r = 10;
    int k = rand()%(p-2) + 1;
    int y = modPower(g,x,p);
    int c1,c2;
    encrypt(p,g,y,k,m,c1,c2);
    int c1_new, c2_new;
    c1_new = (c1*modPower(g, r, p))%p;
    c2_new = (c2*modPower(y, r, p))%p;

    cout << "k: " << k << endl;
    cout << "encrypted: " << c1 << " " << c2 << endl;
    cout << "encrypted: " << c1_new << " " << c2_new << endl;
    cout << "recovered: " << decrypt(p, x, c1, c2) << endl;
    cout << "recovered: " << decrypt(p, x, c1_new, c2_new) << endl;

    return 0;
}