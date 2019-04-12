#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char *argv[]) {

    // Get args
    if(argc != 5) {
        cerr << "Wrong number of arguments" << endl;
        return EXIT_FAILURE;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    int timeout = atoi(argv[3]);
    const char* filename = argv[4];

    // Socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Failed to open socket" << endl;
        return EXIT_FAILURE;
    }

    // Server address
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    unsigned long addr = inet_addr(ip);
    memcpy((char*)&server.sin_addr, &addr, sizeof(addr));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Try to connect once a second until timeout
    cout << "Trying to connect" << endl;
    const auto start = std::chrono::steady_clock::now();
    while(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        const auto t = std::chrono::steady_clock::now();
        if(std::chrono::duration_cast<std::chrono::seconds>(t-start).count() > timeout) {
            cerr << "Connection timeout" << endl;
            close(sock);
            return EXIT_FAILURE;
        }
        sleep(1);
    }
    cout << "Connected" << endl;

    // Send file size and file
    int f = open(filename, O_RDONLY);
    struct stat st;
    fstat(f, &st);
    uint32_t num_bytes_read = st.st_size;
    send(sock, &num_bytes_read, 4, 0);
    int num_bytes_sent = sendfile(sock, f, 0, num_bytes_read);
    close(f);
    cout << "Sent file" << endl;

    // Receive acknowledgement
    char buffer[16];
    recv(sock, buffer, 16, 0);
    close(sock);

    // Check
    if(!strcmp(buffer, "Received update") && num_bytes_sent == num_bytes_read) {
        cout << "Success" << endl;
        return EXIT_SUCCESS;
    } else {
        cerr << "Error" << endl;
        return EXIT_FAILURE;
    }
}
