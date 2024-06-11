#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

struct student;
// 压缩函数
int  pack_student_bytebybyte(student* s, int sno, char* buf);
int  pack_student_whole(student* s, int sno, char* buf);

// 解压函数
int restore_student(char* buf, int len, student* s);

// 输出函数
void printMessage(char* message, int len);

// 延迟函数
void ddelay();

const int N = 5;

struct student
{
	char name[8];
	long long age;
	float score;
	char remark[196];
}new_s[N];

int main() {
	// 存放压缩的信息
	char message[500];
	//memset(message, 0, sizeof message);
	// 初始化
	student old_s[N] = {
		{"王国豪",15,43,"人生若只如初见"},
		{"啦啦",2,98,"何事秋风悲画扇"},
		{"笨蛋",3,96,"等闲变却故人心"},
		{"什么",4,94,"却道故人心易变"},
		{"鲁尼在",5,92,"沧海桑田"}
	};
	cout << "开始输出未压缩的内容......" << endl;
	// 打印 old_s[N]
	for (int i = 0; i < N; i++) {
		cout << old_s[i].name << " " << old_s[i].age << " " << old_s[i].score << "　" << old_s[i].remark << endl;
	}
	cout << "压缩前存放数据的长度为 :" << sizeof old_s << endl;
	cout << "下面开始按字节压缩前2条记录" << endl;
	//ddelay();

	// 按字节压缩
	int len = pack_student_bytebybyte(old_s, 2, message);

	cout << "此时message的长度为: " << len << endl;

	cout << "下面开始按一条一条压缩下3条记录" << endl;
	//ddelay();

	// 一条一条压缩
	len += pack_student_whole(&old_s[2], 3, message + len);

	cout << "此时message的长度为: " << len << endl;

	// 开始输出存储压缩信息的message中的内容
	cout << "开始输出两次压缩后message中的内容" << endl;
	printMessage(message, len);

	cout << "下面开始解压message中的信息到新结构体中" << endl;
	//ddelay();

	int num = restore_student(message, len, new_s);

	// 打印 new_s[N]
	for (int i = 0; i < num; i++) {
		cout << new_s[i].name << " " << new_s[i].age << " " << new_s[i].score << "　" << new_s[i].remark << endl;
	}

	char* pp = message;
	cout << "以十六进制的形式，输出message的前20个字节的内容 " << endl;
	for (int i = 0; i < 20; i++) {
		printf("%02X ", (unsigned char)(*(pp + i)));
	}

	return 0;
}

void printMessage(char* message, int len) {
	int cnt = 0;
	char* p = message;
	while (p - message < len)
	{
		cout << p << " ";   // 名字
		p += strlen(p) + 1;
		cout << *((short*)p) << " ";  // 年龄
		p += 2;
		cout << *((float*)p) << " ";
		p += 4;
		cout << p << " " << endl;
		p += strlen(p) + 1;
	}
}

// s为待压缩数组的起始地址； sno 为压缩人数； 
// buf 为压缩存储区的首地址；两个函数的返回均是调用函数压缩后的字节数
int  pack_student_bytebybyte(student* s, int sno, char* buf) {
	int cnts = 0;
	int cntname, cntage, cntscore, cntremark, cntbuf = 0;
	char* p = (char*)s;
	char* pp = buf;
	while (cnts < sno)
	{
		// 读取名字
		cntname = 0;
		while (cntname < 8) {
			if (*p) {  // 遇到\0
				*pp = *p;
				cntname++, cntbuf++;
				p++, pp++;
			}
			else {
				*pp = '\0';
				cntbuf++;
				p += (8 - cntname);                                   
				pp++;
				break;
			}
		}

		// 读取年龄
		cntage = 0;
		while (cntage < 2)
		{
			*pp = *p;
			cntbuf++, cntage++, p++, pp++;
		}

		p += 2;
		// 读入float,占 4 个字节
		cntscore = 0;
		while (cntscore < 4)
		{
			*pp = *p;
			cntbuf++, cntscore++, p++, pp++;
		}

		// 读取remark数组
		cntremark = 0;
		while (cntremark < 200)
		{
			if (*p) {
				*pp = *p;
				cntbuf++, cntremark++, p++, pp++;
			}
			else {
				*pp = 0;
				cntbuf++, pp++;
				p += (200 - cntremark);
				break;
			}
		}
		cnts++;
	}
	return cntbuf;
}

// 按
int  pack_student_whole(student* s, int sno, char* buf) {
	int cnts = 0;
	char* p = (char*)s;
	char* pp = buf;
	student* ppp = s;
	while (cnts < sno)
	{
		// 拷贝名字
		strcpy(pp, ppp[cnts].name);
		pp += strlen(ppp[cnts].name) + 1;
		// 拷贝age
		*((short*)pp) = ppp[cnts].age;
		pp += 2;
		// 拷贝score
		*((float*)pp) = ppp[cnts].score;
		pp += 4;
		// 拷贝备注
		strcpy(pp, ppp[cnts].remark);
		pp += strlen(ppp[cnts].remark) + 1;
		cnts++;

	}
	return pp - buf;
}


// 解压函数
//buf 为压缩区域存储区的首地址；len为buf中存放数据的长度；
// s为存放解压数据的结构数组的起始地址； 返回解压的人数。
int restore_student(char* buf, int len, student* s) {
	int cnt = 0; // 记录解压的人数
	char* p = buf;
	student* pp = s;
	int record = 0;
	while ((p - buf) < len)
	{
		// 解压名字
		strcpy(pp[cnt].name, p);
		p += strlen(pp[cnt].name) + 1;
		// 解压年龄
		pp[cnt].age = *((short*)p);
		p += 2;
		// 解压分数
		pp[cnt].score = *((float*)p);
		p += 4;
		// 解压备注
		strcpy(pp[cnt].remark, p);
		p += strlen(pp[cnt].remark) + 1;
		// 条数加一
		cnt++;
	}
	return cnt;
}

void ddelay() {
	for (int i = 0; i <= 4; i++) {
		cout << "*";
		Sleep(1000);
	}
	cout << endl;
}
