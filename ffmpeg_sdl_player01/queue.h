#ifndef QUEUE_H
#define QUEUE_H
#include <mutex>
#include <condition_variable>
#include <queue>
#include <QDebug>

template<typename T>
class Queue
{
public:
    Queue(){}
    ~Queue(){}
    int push(T t){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(t);
        m_cond.notify_one();    //唤醒等待队列中的第一个线程
        return 0;
    }
    T pop(const int timeout = 0){
        std::unique_lock<std::mutex> lock(m_mutex);
        if(m_queue.empty()){
            m_cond.wait_for(lock, std::chrono::milliseconds(timeout), [this]{
                return !m_queue.empty();
            });
        }
        if(m_queue.empty()){
            qDebug()<<"queue pop false";
            return NULL;
        }
        T t = m_queue.front();
        m_queue.pop();
        return t;
    }
    T front(){
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_queue.empty()){
            qDebug()<<"queue front false";
            return NULL;
        }
        return m_queue.front();
    }
    int size(){
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond;
};

#endif // QUEUE_H
