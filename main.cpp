#include <future.hpp>

template<typename T>
void thread_func(spaghetti::promise<T>& prom) {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(4s);
    prom.set_value(2000);
}

int main () {
    spaghetti::promise<int> prom;
    spaghetti::future<int> fut = prom.get_future();

    std::thread thread(thread_func<int>, std::ref(prom));
    thread.detach();
    std::cout << fut.get() << std::endl;

    return 0;
}