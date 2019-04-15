#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <SFML/Graphics.hpp>

using namespace std;

int transmission(int argc, char *argv[], int sock, int sock2) {
    // Get args
    if(argc != 5) {
        cerr << "Wrong number of arguments" << endl;
        return EXIT_FAILURE;
    }
    int port = atoi(argv[1]);
    int timeout = atoi(argv[2]);
    string filename = argv[3];
    string contentname = argv[4];

    // Socket
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        cerr << "Failed to open socket" << endl;
        return EXIT_FAILURE;
    }

    // Bind
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        cerr << "Error in setsockopt" << endl;
        return EXIT_FAILURE;
    }
    if(bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        cerr << "Error in bind" << endl;
        return EXIT_FAILURE;
    }

    // Listen
    if(listen(sock, 1) < 0) {
        cerr << "Error in listen" << endl;
        return EXIT_FAILURE;
    }
    cout << "Waiting for client" << endl;

    // Set timeout
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sock, &rfds);
    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    if(select(sock+1, &rfds, (fd_set*)0, (fd_set*)0, &tv) <= 0) {
        cerr << "Error in select" << endl;
        return EXIT_FAILURE;
    }

    // Accept
    struct sockaddr_in client;
    unsigned int size_sai = sizeof(client);
    sock2 = accept(sock, (struct sockaddr*)&client, &size_sai);
    if (sock2 < 0) {
        cerr << "Connection timeout" << endl;
        return EXIT_FAILURE;
    }
    cout << "Connected with " << inet_ntoa(client.sin_addr) << endl;

    // Receive file size
    char buffer[1024];
    int bytes_read = recv(sock2, buffer, 4, 0);
    if(bytes_read != 4) {
        cerr << "Wrong filesize" << endl;
        return EXIT_FAILURE;
    }
    int bytes_file;
    memcpy(&bytes_file, buffer, 4);

    // Receive file
    ofstream f(filename, ios::out | ios::binary);
    while(bytes_file > 0) {
        bytes_read = recv(sock2, buffer, 1024, 0);
        f.write(buffer, bytes_read);
        bytes_file -= bytes_read;
    }
    cout << "Received file" << endl;

    // Verify signature
    system(("tar xf "+filename+" >/dev/null 2>&1").c_str());
    if(system(("gpg --verify "+contentname+".sig "+contentname+".tar.gz").c_str()) != 0) {
        cerr << "Verification failed" << endl;
        return EXIT_FAILURE;
    }
    cout << "Verification successful" << endl;

    // Send acknowledgement
    const char* msg = "Received update";
    send(sock2, msg, strlen(msg), 0);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    // Show update screen
    sf::RenderWindow window(sf::VideoMode(1280, 800), "self-o-mat");
    sf::Texture backgroundTexture;
    if(!backgroundTexture.loadFromFile("updater.png")) {
	    cerr << "Error loading background texture" << endl;
	    return EXIT_FAILURE;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(1,1);
    backgroundSprite.setPosition(0,0);
    const auto start = std::chrono::steady_clock::now();
    window.clear();
    window.draw(backgroundSprite);
    window.display();

    // Receive file and close sockets
    int sock = -1, sock2 = -1;
    bool success = transmission(argc, argv, sock, sock2);
    if(sock >= 0)
        close(sock);
    if(sock2 >= 0)
        close(sock2);

    // Show update screen for at least 5 seconds
    const auto end = std::chrono::steady_clock::now();
    auto d = std::chrono::duration_cast<std::chrono::seconds>(end-start).count();
    if(d < 5)
        sleep(5-d);

    window.clear();
    window.draw(backgroundSprite);
    window.display();
    sleep(5);
    window.close();

    return success;
}
