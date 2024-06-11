#include<iostream>
#include<vector>
using namespace std;

// 1.返回 x 的绝对值
int absVal(int x);
// 评定函数1
int absVal_standard(int x) { return (x < 0) ? -x : x; };

// 2.不使用负号，实现 -x
int negate(int x);
// 评定函数2
int negate_standard(int x) { return -x; }

// 3.仅使用 ~ 和 |，实现 &
int bitAnd(int x, int y);
// 评定函数3
int bitAnd_standard(int x, int y) { return x & y; }

// 4.仅使用 ~ 和 &，实现 |
int bitOr(int x, int y);
// 评定函数4
int bitOr_standard(int x, int y) { return x | y; }

// 5.仅使用 ~ 和 &，实现 ^
int bitXor(int x, int y);
// 评定函数5
int bitXor_standard(int x, int y) { return x ^ y; }

// 6.判断x是否为最大的正整数（7FFFFFFF）
int isTmax(int x);
// 评定函数6
int isTmax_standard(int x) { return x == 0x7fffffff; }

// 7.统计x的二进制表示中 1 的个数
int bitCount(int x);
// 评定函数7
int bitCount_standard(int x) {
	int a = 0;
	for (int i = 0; i < 32; i++) {
		if (((x >> i) & 1)) {
			a++;
		}
	}
	return a;
}

// 8.产生从lowbit 到 highbit 全为1，其他位为0的数
int bitMask(int highbit, int lowbit);
// 评定函数8
int bitMask_standard(int highbit, int lowbit) {
	int ans = 0;
	for (int i = lowbit; i <= highbit; i++) {
		ans += (1 << (i));
	}
	return ans;
}

// 9.当x+y 会产生溢出时返回1，否则返回 0
int addOK(int x, int y);
// 评定函数9
int addOK_standard(int x, int y) {
	long long lsum = (long long)x + y;
	return !(lsum == (int)lsum);
}

// 10.将x的第n个字节与第m个字节交换，返回交换后的结果。
int byteSwap(int x, int n, int m);
// 评定函数10
int byteSwap_standard(int x, int n, int m) {
    unsigned int nmask, mmask;

    switch (n) {
    case 0:
        nmask = x & 0xFF;
        x &= 0xFFFFFF00;
        break;
    case 1:
        nmask = (x & 0xFF00) >> 8;
        x &= 0xFFFF00FF;
        break;
    case 2:
        nmask = (x & 0xFF0000) >> 16;
        x &= 0xFF00FFFF;
        break;
    default:
        nmask = ((unsigned int)(x & 0xFF000000)) >> 24;
        x &= 0x00FFFFFF;
        break;
    }

    switch (m) {
    case 0:
        mmask = x & 0xFF;
        x &= 0xFFFFFF00;
        break;
    case 1:
        mmask = (x & 0xFF00) >> 8;
        x &= 0xFFFF00FF;
        break;
    case 2:
        mmask = (x & 0xFF0000) >> 16;
        x &= 0xFF00FFFF;
        break;
    default:
        mmask = ((unsigned int)(x & 0xFF000000)) >> 24;
        x &= 0x00FFFFFF;
        break;
    }

    nmask <<= 8 * m;
    mmask <<= 8 * n;

    return x | nmask | mmask;
}

int main() {
    int n,m,u;
    
    int t = 3;
    while (t--)
    {
        cout << "请输入x和y，将x的第n个字节与第m个字节交换，返回交换后的结果" << endl;
        cin >> n >> m >> u;
        cout << byteSwap(n,m,u) << endl;
        cout << "测试函数结果： " << byteSwap_standard(n,m,u) << endl;
        if (byteSwap(n,m,u) == byteSwap_standard(n,m,u)) {
            cout << "二者匹配，验证成功" << endl;
        }
        else {
            cout << "验证失败" << endl;
        }
    }
    
	return 0;
}

// 遇到的问题：


int absVal(int x) {
	return (x + (x >> 31)) ^ (x >> 31);
}

// 直接取反加一
int negate(int x) {
	return (~x)+1;
}

// 一开始想的是用逻辑电路里面的 ~(x | y),但是发现行不通
// & 有0则为false，则相当于 与门
// | 有1则为true,则相当于  或门
// 直接两次取反就可以
int bitAnd(int x, int y) {
	return ~((~x) | (~y));
}

// 直接借鉴上面的
int bitOr(int x, int y) {
	return ~((~x) & (~y));
}

// 借鉴数字电路中的异或门实现
int bitXor(int x, int y) {
	return ~((~((~x) & y)) & (~(x & (~y))));
}

// -1 的取反是 0
int isTmax(int x) {
	return !(~(x+(1<<31)));
}

// 巧妙使用
int bitCount(int x) {
	int mask = 0x11111111;
	int s = x & mask;
	s += x >> 1 & mask;
	s += x >> 2 & mask;
	s += x >> 3 & mask;
	s = s + (s >> 16);
	mask = 0xF | (0xF << 8);
	s = (s & mask) + ((s >> 4) & mask);
	return (s + (s >> 8)) & 0x3F;
}

// 10000 - 100
int bitMask(int highbit, int lowbit) {
	return (1 << (highbit + 1)) + ((~(1 << lowbit)) + 1);
}

// 如果异号的话前面为 1 ，直接为真
// 如果异号的情况下，((x ^ y) >> 31) 为0，对后续不影响,
// ~(((x + y) ^ x) >> 31) 这个判断计算出来的是否和之前的同号
int addOK(int x, int y) {
	return !((((x ^ y) >> 31) | ~(((x + y) ^ x) >> 31)) & 1);
}


int byteSwap(int x, int n, int m) {
	int p = ((x >> (n << 3)) ^ (x >> (m << 3))) & ((1 << 8) + (~0));
	return x ^ (((p) << (n << 3)) | ((p) << (m << 3)));
}

