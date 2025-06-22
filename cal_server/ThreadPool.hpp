#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
static const int g_default_thread_count = 20;//默认线程的数量
template <typename T>
class ThreadPool
{
public:
    // 获取单例线程池
    static ThreadPool<T>* GetInstance(int thread_count = g_default_thread_count)
    {
        if (instance_ == nullptr)
        {
            std::lock_guard<std::mutex> lock(init_mutex_);
            if (instance_ == nullptr)
            {
                instance_ = new ThreadPool<T>(thread_count);
                instance_->Init();
                instance_->Start();
            }
        }
        return instance_;
    }

    // 入队任务
    void Enqueue(const T& task)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            if (!is_running_) return;
            task_queue_.push(task);
        }//解锁
        condition_.notify_one();
    }

    // 停止线程池
    void Stop()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            is_running_ = false;
        }
        condition_.notify_all();//停止前要先完成所有任务
        for (auto& thread : workers_)
        {
            if (thread.joinable())
                thread.join();
        }
        std::cout << "ThreadPool stopped." << std::endl;
    }

    ~ThreadPool()
    {
        Stop();
    }

private:
    ThreadPool(int thread_count)
        : thread_count_(thread_count), is_running_(false)
    {
    }

    ThreadPool(const ThreadPool<T>&) = delete;
    ThreadPool& operator=(const ThreadPool<T>&) = delete;

    void Init()
    {
        
    }

    void Start()
    {
        is_running_ = true;
        for (int i = 0; i < thread_count_; ++i)
        {
            //创建线程
            workers_.emplace_back([this, i]() {
                const std::string name = "thread-" + std::to_string(i + 1);
                while (true)
                {
                    T task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex_);
                        while (is_running_ && task_queue_.empty())  //当前线程池正在运行没有任务时,等任务过来
                        {
                            condition_.wait(lock);
                        }
                        if (!is_running_ && task_queue_.empty())
                        {
                            std::cout << name << " exiting." << std::endl;
                            return;
                        }
                        task = task_queue_.front();
                        task_queue_.pop();
                    }
                    // 执行任务
                    task();
                }
            });
        }
        std::cout << "ThreadPool started with " << thread_count_ << " threads." << std::endl;
    }

    int thread_count_;
    std::vector<std::thread> workers_;
    std::queue<T> task_queue_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> is_running_;

    static ThreadPool<T>* instance_;//线程池对象
    static std::mutex init_mutex_;//线程池锁
};

// 静态成员初始化
template <typename T>
ThreadPool<T>* ThreadPool<T>::instance_ = nullptr;

template <typename T>
std::mutex ThreadPool<T>::init_mutex_; 