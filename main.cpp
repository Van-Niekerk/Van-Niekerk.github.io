/*************************************************************************
	> File Name: main.cpp
	> Author: 隔壁老孙无敌 
	> Mail:1686169484@qq.com 
	> Created Time: Sat Jun 17 19:43:54 2023
 ************************************************************************/
#include "head.h"

void func(int a, int b, int c) {
    LOG_INFO << a << " " << b << " " << c;
    return ;
}

int main() {
    //SET_LEVEL(MyLog::LogLevel::DEBUG);
    /*LOG_INFO << " hello world " << 123 << " " << 12.45;
    LOG_WARNING << " hello world " << 123 << " " << 12.45;
    LOG_DEBUG << " hello world " << 123 << " " << 12.45;
    LOG_ERROR << " hello world " << 123 << " " << 12.45;
    LOG_FATAL << " hello world " << 123 << " " << 12.45;*/
    MyThread::ThreadPool tp;
    for (int i = 0; i < 100; i++) {
        tp.addOneTask(func, i, 2 * i, 3 * i);
    }
    tp.stop();
    return 0;
}
