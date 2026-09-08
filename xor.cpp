#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

string toBinary(int number) {
    if (number == 0) return "0";
    string binary;
    while (number > 0) {
        binary += char('0'+number%2);
        number /= 2;
    }
    reverse(binary.begin(), binary.end());
    return binary;
}

string manualXor(string a, string b) {
    int maxLength = max(a.length(),b.length());
    a = string(maxLength-a.length(), '0') + a;
    b = string(maxLength-b.length(), '0') + b;

    string res;

    for(int i=0; i<maxLength; ++i) {
        if(a[i]==b[i]) {
            res += '0';
        }
        else res+='1';
    }
    return res;

}

int toDecimal(string binary) {
    if(binary == "0") return 0;
    int decimal = 0;
    int factor = 1;
    for(int i=binary.length()-1; i>=0; --i) {
        decimal+=factor*int(binary[i]-'0');
        factor*=2;
    }
    return decimal;
}

int main() {
    int x, y;

    cout << "Enter two numbers: ";
    cin >> x >> y;

    string binaryX = toBinary(x);
    string binaryY = toBinary(y);

    string xorResult = manualXor(binaryX, binaryY);

    cout << "First binary : " << binaryX << '\n';
    cout << "Second binary: " << binaryY << '\n';
    cout << "XOR result   : " << xorResult << '\n';
    cout << "Decimal result: " << toDecimal(xorResult) << '\n';

    return 0;
}