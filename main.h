/*************************************************************************
	> File Name: main.h
	> Author: 隔壁老孙无敌 
	> Mail:1686169484@qq.com 
	> Created Time: Sun Jun 18 11:58:34 2023
 ************************************************************************/

#ifndef _MAIN_H
#define _MAIN_H
class Task {
public :
    template<typename FUNCTION, typename ...ARGS>
    Task(FUNCTION &&func, ARGS ...args);
    void run();
private:
    std::function<void()> func;
};

template<typename FUNCTION, typename ...ARGS>
Task::Task(FUNCTION &&func, ARGS ...args) {
    this->func = std::bind(func, std::forward<ARGS>(args)...);
}

void Task::run() {
    //std::cout << "Task run : ";
    func();
    return ;
}
#endif
