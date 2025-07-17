#include <future.hpp>

void thread_func(spaghetti::Promise& prom) {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(4s);
    prom.set_value(2000);
}

int main () {
    spaghetti::Promise prom;
    spaghetti::Future fut = prom.get_future();

    std::thread thread(thread_func, std::ref(prom));
    thread.detach();
    std::cout << fut.get() << std::endl;

    return 0;
}