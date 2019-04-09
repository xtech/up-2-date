#include <iostream>

#include <served/served.hpp>



int main() {
    served::multiplexer mux;
    served::net::server server("0.0.0.0", "8080", mux, false);

    mux.handle("/update")
    .post([](served::response & res, const served::request & req) {

        std::cout << "We got a post request, it should contain some update data." << std::endl;

        std::cout << req.body() << std::endl;

        served::response::stock_reply(200, res);
        return;
    });

    std::cout << "Running! Try sending a POST request - e.g. in terminal: curl -X POST localhost:8080/update -d \"some data\"" << std::endl;
    server.run();
}