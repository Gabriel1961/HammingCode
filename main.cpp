#include <bit>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <string>
#include <cassert>
typedef unsigned int uint;
using namespace std;
#define VERBOSE

constexpr int GetCountRedundant(uint x) {
    return popcount(x - 1) + 1;
}

template<size_t T>
void Print(const bitset<T>& b) {
    for (int i = 0; i < T; i++)
        cout << b[i];
}

template<size_t size,size_t redCount = GetCountRedundant(size)>
class HammingMsg
{
public:

    void Print() {
        for (int i = 0; i < size; i++, cout << (i % 4 != 0 ? "" : "\n"))
            cout << msg[i];
    }

    bitset<size> msg;


    template<size_t dSize>
    HammingMsg(const bitset<dSize>& data) {
        assert(popcount(size) == 1); // make sure that size is a power of 2
        assert(dSize <= size - redCount); // make sure that we have enough space for data
        bitset<redCount> par;
        int idx = 4;
        for (size_t it = 3, i = 0; i < dSize; i++, it++) { // copy data
            if (it != idx) {
                msg[it] = data[i];
                if (data[i]) {
                    par = par ^ bitset<redCount>(it);
                }
            }
            else
                i--, idx <<= 1;
        }

        for (int i = 0; i < redCount - 1; i++) {
            msg[1ull << i] = par[i];
        }

        for (int i = 0; i < size; i++)
            msg[0] = msg[i] ^ msg[0];
    }

    bitset<size - redCount> Decode() {
        bitset<size - redCount> ans;
        bool hasOddErr = 0;
        bool parity = 0;
        for (int i = 0; i < size; i++)
            parity ^= msg[i];
        if (parity != msg[0])
            hasOddErr = 1;
        
        //check for errors in quadrants
        bitset<redCount> ep;
        for (size_t it = 1; it < size; it++) { // copy data
            if (msg[it]) {
                ep = ep ^ bitset<redCount>(it);
            }
        }
        size_t errPos = ep.to_ullong();
#ifdef VERBOSE
        if (hasOddErr == 0) {
            if (errPos == 0) {
                cout << "No error detected\n";
            } else {
                cout << "Even number of errors detected\n";
            }
        } else {
            if (errPos == 0){
                cout << "Odd number of errors detected\n";
            }
            else {
                cout << "1 Error detected\n";
            }
        }
#endif
        msg[errPos].flip();
        int idx = 4;
        for (int k = 3, it = 0; k < size; k++,it++)
            if (k == idx)
                idx <<= 1, it--;
            else
                ans[it] = msg[k];

        return ans;
    }
};


int main()
{
    string str = "00110001110";
    reverse(str.begin(), str.end());
    bitset<11> b{str.c_str()};
    HammingMsg<16> msg(b);
    msg.Print();
    Print(msg.Decode());
}
