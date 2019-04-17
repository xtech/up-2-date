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

#define UPDATE_TEMP_DIR "/tmp"
#define UPDATE_FILE_NAME "update.tar"


sf::RenderWindow *window;
sf::Sprite *logoSprite;
sf::Font *font;
sf::Texture *logoTexture;

void drawUpdateScreen(std::string title, std::string subtitle) {
    window->clear(sf::Color::White);
    window->draw(*logoSprite);

    sf::Text sfTitle;
    sfTitle.setString(title);
    sfTitle.setFont(*font);
    sfTitle.setCharacterSize(50);
    sfTitle.setFillColor(sf::Color(0,0,0));
    sfTitle.setStyle(sf::Text::Bold);

    // center it
    auto windowSize = window->getSize();
    auto bounds = sfTitle.getLocalBounds();
    sfTitle.setPosition((windowSize.x - bounds.width)/2,120);

    window->draw(sfTitle);

    sf::Text sfSubtitle;
    sfSubtitle.setString(subtitle);
    sfSubtitle.setFont(*font);
    sfSubtitle.setCharacterSize(40);
    sfSubtitle.setFillColor(sf::Color(0,0,0));
    sfSubtitle.setStyle(sf::Text::Regular);

    // center it
    auto subtitleBounds = sfSubtitle.getLocalBounds();
    sfSubtitle.setPosition((windowSize.x - subtitleBounds.width)/2,600);

    window->draw(sfSubtitle);

    window->display();
}

int do_update(std::string target_dir, int port, int timeout, int &sock, int &sock2) {

    // Socket
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        drawUpdateScreen("ERROR!", "failed to open socket!");
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
        drawUpdateScreen("ERROR!", "error in setsockopt!");
        cerr << "Error in setsockopt" << endl;
        return EXIT_FAILURE;
    }
    if(bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        drawUpdateScreen("ERROR!", "error in bind!");
        cerr << "Error in bind" << endl;
        return EXIT_FAILURE;
    }

    // Listen
    if(listen(sock, 1) < 0) {
        drawUpdateScreen("ERROR!", "error in listen!");
        cerr << "Error in listen" << endl;
        return EXIT_FAILURE;
    }


    // Set timeout
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sock, &rfds);
    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    if(select(sock+1, &rfds, (fd_set*)0, (fd_set*)0, &tv) <= 0) {
        drawUpdateScreen("ERROR!", "connection timed out - retrying!");
        cerr << "Error in select" << endl;
        return EXIT_FAILURE;
    }

    // Accept
    struct sockaddr_in client;
    unsigned int size_sai = sizeof(client);
    sock2 = accept(sock, (struct sockaddr*)&client, &size_sai);
    if (sock2 < 0) {
        drawUpdateScreen("ERROR!", "connection timed out - retrying!");
        cerr << "Connection timeout" << endl;
        return EXIT_FAILURE;
    }
    drawUpdateScreen("updating...", "phone connected!");
    cout << "Connected with " << inet_ntoa(client.sin_addr) << endl;

    // Receive file size
    char buffer[1024];
    int bytes_read = recv(sock2, buffer, 4, 0);
    if(bytes_read != 4) {
        drawUpdateScreen("ERROR!", "invalid file size!");
        cerr << "Wrong filesize" << endl;
        return EXIT_FAILURE;
    }
    int bytes_file;
    memcpy(&bytes_file, buffer, 4);

    // Receive file
    std::string temp_dir = UPDATE_TEMP_DIR;
    std::string receive_file = std::string(UPDATE_TEMP_DIR)+"/"+UPDATE_FILE_NAME;
    std::string signature_file = std::string(UPDATE_TEMP_DIR)+"/app.sig";
    std::string content_file = std::string(UPDATE_TEMP_DIR)+"/app.tar.gz";
    ofstream f(receive_file, ios::out | ios::binary);
    while(bytes_file > 0) {
        bytes_read = recv(sock2, buffer, 1024, 0);
        f.write(buffer, bytes_read);
        bytes_file -= bytes_read;
    }
    drawUpdateScreen("updating...", "Received File - Verifying Signature!");
    cout << "Received file" << endl;

    // Verify signature
    system(("tar xf "+receive_file+" -C "+temp_dir+" >/dev/null 2>&1").c_str());
    if(system(("gpg --verify "+signature_file+" "+content_file).c_str()) != 0) {
        drawUpdateScreen("ERROR!", "Invalid Signature!");
        return EXIT_FAILURE;
    }
    drawUpdateScreen("updating...", "Signature Correct!");
    usleep(500000);
    drawUpdateScreen("Updating...", "unpacking update... DO NOT DISCONNECT OR RESET ANYTHING!");
    if(system(("./extract_update.sh "+content_file+" "+" "+target_dir+" self_o_mat").c_str()) != 0) {
        drawUpdateScreen("ERROR!", "error while unpacking data. reboot and try again!");
        return EXIT_FAILURE;
    }
    drawUpdateScreen("Updating...", "updating firmware... DO NOT DISCONNECT OR RESET ANYTHING!");
    if(system(("./install_firmware.sh " + target_dir+"/firmware.hex").c_str()) != 0) {
        drawUpdateScreen("ERROR!", "error while writing firmware. Good Luck!");
        return EXIT_FAILURE;
    }
    drawUpdateScreen("Update Done", "Successfully Updated!");

    // Send acknowledgement
    const char* msg = "Received update";
    send(sock2, msg, strlen(msg), 0);

    return EXIT_SUCCESS;
}



int main(int argc, char *argv[]) {

    // Get args
    if(argc != 5) {
        cerr << "Wrong number of arguments. expected: port, timeout, target_dir first run [true|false]" << endl;
        return EXIT_FAILURE;
    }
    int port = atoi(argv[1]);
    int timeout = atoi(argv[2]);
    std::string target_dir = argv[3];
    bool firstRun = std::string(argv[4]) == "true";

    cout << "first run: " << firstRun << endl;

    // Show update screen
    font = new sf::Font();
    if(!font->loadFromFile("./assets/AlegreyaSans-Bold.ttf")) {
        delete(font);
        cerr << "Error loading font" << endl;
        return EXIT_FAILURE;
    }

    window = new sf::RenderWindow(sf::VideoMode(1280, 800), "self-o-mat");
    logoTexture = new sf::Texture();
    if(!logoTexture->loadFromFile("./assets/logo.png")) {
        delete(window);
        delete(logoTexture);
        delete(font);
	    cerr << "Error loading logo texture" << endl;
	    return EXIT_FAILURE;
    }

    logoSprite = new sf::Sprite();
    logoSprite->setTexture(*logoTexture);

    // center logo in the screen
    auto windowSize = window->getSize();
    auto logoSize = logoTexture->getSize();
    logoSprite->setPosition((windowSize.x - logoSize.x)/2,(windowSize.y - logoSize.y)/2);


    const auto start = std::chrono::steady_clock::now();
    if(firstRun) {
        drawUpdateScreen("Welcome!", "Congratulations! You have successfully built your self-o-mat!\nTo set it up, please open the app and follow the instructions!");
    } else {
        drawUpdateScreen("Waiting for connection...", "to update please open the app on your phone.");
    }


    // Receive file and close sockets
    int sock = -1, sock2 = -1;
    int success = do_update(target_dir, port, timeout, sock, sock2);

    if(success!=EXIT_SUCCESS) {
        // force on for users to see errors
        sleep(10);
    } else {
        sleep(1);
    }

    if(sock >= 0)
        close(sock);
    if(sock2 >= 0)
        close(sock2);

    // Show update screen for at least 5 seconds
    const auto end = std::chrono::steady_clock::now();
    auto d = std::chrono::duration_cast<std::chrono::seconds>(end-start).count();
    if(d < 5)
        sleep(5-d);


    window->close();

    delete(logoSprite);
    delete(window);
    delete(logoTexture);
    delete(font);

    return success;
}
