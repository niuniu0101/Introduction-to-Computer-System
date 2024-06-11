#include<iostream>
#include<vector>
using namespace std;

// 1.���� x �ľ���ֵ
int absVal(int x);
// ��������1
int absVal_standard(int x) { return (x < 0) ? -x : x; };

// 2.��ʹ�ø��ţ�ʵ�� -x
int negate(int x);
// ��������2
int negate_standard(int x) { return -x; }

// 3.��ʹ�� ~ �� |��ʵ�� &
int bitAnd(int x, int y);
// ��������3
int bitAnd_standard(int x, int y) { return x & y; }

// 4.��ʹ�� ~ �� &��ʵ�� |
int bitOr(int x, int y);
// ��������4
int bitOr_standard(int x, int y) { return x | y; }

// 5.��ʹ�� ~ �� &��ʵ�� ^
int bitXor(int x, int y);
// ��������5
int bitXor_standard(int x, int y) { return x ^ y; }

// 6.�ж�x�Ƿ�Ϊ������������7FFFFFFF��
int isTmax(int x);
// ��������6
int isTmax_standard(int x) { return x == 0x7fffffff; }

// 7.ͳ��x�Ķ����Ʊ�ʾ�� 1 �ĸ���
int bitCount(int x);
// ��������7
int bitCount_standard(int x) {
	int a = 0;
	for (int i = 0; i < 32; i++) {
		if (((x >> i) & 1)) {
			a++;
		}
	}
	return a;
}

// 8.������lowbit �� highbit ȫΪ1������λΪ0����
int bitMask(int highbit, int lowbit);
// ��������8
int bitMask_standard(int highbit, int lowbit) {
	int ans = 0;
	for (int i = lowbit; i <= highbit; i++) {
		ans += (1 << (i));
	}
	return ans;
}

// 9.��x+y ��������ʱ����1�����򷵻� 0
int addOK(int x, int y);
// ��������9
int addOK_standard(int x, int y) {
	long long lsum = (long long)x + y;
	return !(lsum == (int)lsum);
}

// 10.��x�ĵ�n���ֽ����m���ֽڽ��������ؽ�����Ľ����
int byteSwap(int x, int n, int m);
// ��������10
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
        cout << "������x��y����x�ĵ�n���ֽ����m���ֽڽ��������ؽ�����Ľ��" << endl;
        cin >> n >> m >> u;
        cout << byteSwap(n,m,u) << endl;
        cout << "���Ժ�������� " << byteSwap_standard(n,m,u) << endl;
        if (byteSwap(n,m,u) == byteSwap_standard(n,m,u)) {
            cout << "����ƥ�䣬��֤�ɹ�" << endl;
        }
        else {
            cout << "��֤ʧ��" << endl;
        }
    }
    
	return 0;
}

// ���������⣺


int absVal(int x) {
	return (x + (x >> 31)) ^ (x >> 31);
}

// ֱ��ȡ����һ
int negate(int x) {
	return (~x)+1;
}

// һ��ʼ��������߼���·����� ~(x | y),���Ƿ����в�ͨ
// & ��0��Ϊfalse�����൱�� ����
// | ��1��Ϊtrue,���൱��  ����
// ֱ������ȡ���Ϳ���
int bitAnd(int x, int y) {
	return ~((~x) | (~y));
}

// ֱ�ӽ�������
int bitOr(int x, int y) {
	return ~((~x) & (~y));
}

// ������ֵ�·�е������ʵ��
int bitXor(int x, int y) {
	return ~((~((~x) & y)) & (~(x & (~y))));
}

// -1 ��ȡ���� 0
int isTmax(int x) {
	return !(~(x+(1<<31)));
}

// ����ʹ��
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

// �����ŵĻ�ǰ��Ϊ 1 ��ֱ��Ϊ��
// �����ŵ�����£�((x ^ y) >> 31) Ϊ0���Ժ�����Ӱ��,
// ~(((x + y) ^ x) >> 31) ����жϼ���������Ƿ��֮ǰ��ͬ��
int addOK(int x, int y) {
	return !((((x ^ y) >> 31) | ~(((x + y) ^ x) >> 31)) & 1);
}


int byteSwap(int x, int n, int m) {
	int p = ((x >> (n << 3)) ^ (x >> (m << 3))) & ((1 << 8) + (~0));
	return x ^ (((p) << (n << 3)) | ((p) << (m << 3)));
}

