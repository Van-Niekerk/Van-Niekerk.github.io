/*************************************************************************
	> File Name: thread_pool.h
	> Author: 隔壁老孙无敌 
	> Mail:1686169484@qq.com 
	> Created Time: Fri Jun 16 18:32:57 2023
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H


namespace MyThread {
class ThreadPool {
public :
    ThreadPool(int n = 5);
    template<typename FUNCTION, typename ...ARGS>
    void addOneTask(FUNCTION &&, ARGS...);
    void stop();
private :
    std::vector<std::thread *> threads; // 存储若干个线程的地址
    std::queue<Task *>tasks; // 任务队列
    std::mutex m_mutex;
    std::condition_variable m_cond; // 条件信号量
    std::unordered_map<std::thread::id, bool> running; // 运行状态
    void worker_thread();
    Task *getOneTask();
    void stop_thread();
};

ThreadPool::ThreadPool(int n) {
    for (int i = 0; i < n; i++) {
        threads.push_back(new std::thread(&ThreadPool::worker_thread, this));
    }
}
void ThreadPool::worker_thread() {
    std::thread::id id = std::this_thread::get_id();
    running[id] = true;
    while(running[id]) {
        // 取任务
        Task *t = this->getOneTask();
        // 执行任务
        t->run();
        // 销毁任务
        delete t;
    }
    //std::cout << id << std::endl;
    return ;
}
template<typename FUNCTION, typename ...ARGS>
void ThreadPool::addOneTask(FUNCTION &&func, ARGS... args) {
    // 添加任务
    std::unique_lock<std::mutex> lock(m_mutex);
    tasks.push(new Task(func, std::forward<ARGS>(args)...));
    m_cond.notify_one(); // 通知其他线程现在有任务了
    return ;
}
Task *ThreadPool::getOneTask() {
    // 抢占互斥锁
    std::unique_lock<std::mutex> lock(m_mutex);
    while (tasks.empty()) {
        m_cond.wait(lock);  // lock.unlock() m_cond.wait()   等待条件成立 ： 任务队列非空 释放互斥锁 其他线程才能抢占互斥锁给任务队列添加任务
    }
    Task *t = tasks.front();
    tasks.pop();
    return t;
}

void ThreadPool::stop_thread() {
    std::thread::id id = std::this_thread::get_id();
    running[id] = false;
}

void ThreadPool::stop() {
    for (int i = 0; i < threads.size(); i++) {
        this->addOneTask(&ThreadPool::stop_thread, this); // 添加毒药任务
    }
    for(auto t : threads) {
        t->join();
    }
    return ;
}

} // end of MyThread

#endif

