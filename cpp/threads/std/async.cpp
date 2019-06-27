#include <future>
#include <iostream>

int main() {
    std::cout << "Starting main() from thread " << std::this_thread::get_id() << std::endl;

    // fn is started in a different thread
    std::future<int> myFuture = std::async(std::launch::async, []() {
        std::cout << "async fn() started from thread " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "async fn() stopped" << std::endl;
        return 8;
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Waiting for data" << std::endl;
    int val = myFuture.get();
    std::cout << "async returned " << val << std::endl;
}
