#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>


#include <SFML/Graphics.hpp>
#include <served/served.hpp>


using namespace std;

#define UPDATE_TEMP_DIR "/tmp"
#define UPDATE_FILE_NAME "selfomat.update"


sf::RenderWindow *window;
sf::Sprite *logoSprite;
sf::Font *font;
sf::Texture *logoTexture;

// true if file exists
bool fileExists(const std::string &file) {
    struct stat buf;
    return (stat(file.c_str(), &buf) == 0);
}


void drawUpdateScreen(std::string title, std::string subtitle) {
    window->setActive(true);
    window->clear(sf::Color::White);
    window->draw(*logoSprite);

    sf::Text sfTitle;
    sfTitle.setString(title);
    sfTitle.setFont(*font);
    sfTitle.setCharacterSize(50);
    sfTitle.setFillColor(sf::Color(0, 0, 0));
    sfTitle.setStyle(sf::Text::Bold);

    // center it
    auto windowSize = window->getSize();
    auto bounds = sfTitle.getLocalBounds();
    sfTitle.setPosition((windowSize.x - bounds.width) / 2, 120);

    window->draw(sfTitle);

    sf::Text sfSubtitle;
    sfSubtitle.setString(subtitle);
    sfSubtitle.setFont(*font);
    sfSubtitle.setCharacterSize(40);
    sfSubtitle.setFillColor(sf::Color(0, 0, 0));
    sfSubtitle.setStyle(sf::Text::Regular);

    // center it
    auto subtitleBounds = sfSubtitle.getLocalBounds();
    sfSubtitle.setPosition((windowSize.x - subtitleBounds.width) / 2, 600);

    window->draw(sfSubtitle);

    window->display();
    window->setActive(false);
}


const std::map<std::string, std::string> headers = {
        {"X-SELFOMAT-MODE",              "UPDATER"},
        {"Access-Control-Allow-Origin",  "*"},
        {"Access-Control-Allow-Methods", "POST, GET, OPTIONS, PUT"}
};

void setHeaders(served::response &res) {
    for (auto const &h : headers)
        res.set_header(h.first, h.second);
}

std::mutex updateMutex;
std::string target_dir = "/dev/null";
std::string force_update_file = "";
bool updateServerRunning = true;
int timeout = 0;

void *timeoutThread(void *ptr) {
    sleep(timeout);
    cout << "time is over - trying to quit" << endl;
    updateMutex.lock();
    served::net::server *server = static_cast<served::net::server *>(ptr);
    server->stop();
    updateServerRunning = false;
    return nullptr;
}

bool doUpdate(std::string receive_file, std::string temp_dir, std::string content_file, std::string signature_file) {
    // Decrypt the received file
    drawUpdateScreen("Updating...", "received file - decrypting...");
    if(system(("gpg --batch --yes --output /tmp/update.tar.gz --decrypt "+receive_file).c_str()) != 0) {
        drawUpdateScreen("ERROR!", "Could not decrypt update!");
        return false;
    }


    // Verify signature
    drawUpdateScreen("Updating...", "received file - verifying signature...");
    system(("tar xf /tmp/update.tar.gz -C " + temp_dir + " >/dev/null 2>&1").c_str());
    if (system(("gpg --verify --no-default-keyring --keyring ./verification_keyring.ring " + signature_file + " " + content_file).c_str()) != 0) {
        drawUpdateScreen("ERROR!", "Invalid Signature!");
        return false;
    }
    drawUpdateScreen("Updating...", "signature valid!");
    usleep(500000);
    drawUpdateScreen("Updating...", "extracting update... DO NOT DISCONNECT OR RESET ANYTHING!");
    if (system(
            ("./extract_update.sh " + content_file + " " + " " + target_dir + " self_o_mat").c_str()) !=
        0) {
        drawUpdateScreen("ERROR!", "error while unpacking data. reboot and try again!");
        return false;
    }
    drawUpdateScreen("Updating...", "updating firmware... DO NOT DISCONNECT OR RESET ANYTHING!");
    if (system(("./install_firmware.sh " + target_dir + "/firmware.hex").c_str()) != 0) {
        drawUpdateScreen("ERROR!", "error while writing firmware. Good Luck!");
        return false;
    }
    drawUpdateScreen("Update Done", "success!!!");
    return true;
}

void *forceUpdateThread(void *ptr) {
    served::net::server *server = static_cast<served::net::server *>(ptr);

    while(updateServerRunning) {
        sleep(1);

        if (fileExists(force_update_file)) {
            if(updateMutex.try_lock()) {
                drawUpdateScreen("USB Update Found!",
                                 "Updating ...");
                std::string signature_file = std::string(UPDATE_TEMP_DIR) + "/app.sig";
                std::string content_file = std::string(UPDATE_TEMP_DIR) + "/app.tar.gz";
                auto success = doUpdate(force_update_file, UPDATE_TEMP_DIR, content_file, signature_file);

                if (success) {
                    system((std::string("rm ") + force_update_file).c_str());
                    updateServerRunning = false;
                    server->stop();
                    return nullptr;
                } else {
                    updateMutex.unlock();
                    sleep(10);
                }
            }
        }
    }

    return nullptr;
}

int main(int argc, char *argv[]) {

    updateServerRunning = true;
    // Get args
    if (argc != 6) {
        cerr
                << "Wrong number of arguments. expected: port, timeout, target_dir first run [true|false] force_update_file"
                << endl;
        return EXIT_FAILURE;
    }
    timeout = atoi(argv[2]);
    target_dir = argv[3];

    std::string mode = argv[4];
    bool firstRun = mode == "FIRST";
    bool boot = mode == "BOOT";

    force_update_file = argv[5];

    // Show update screen
    font = new sf::Font();
    if (!font->loadFromFile("./assets/AlegreyaSans-Bold.ttf")) {
        delete (font);
        cerr << "Error loading font" << endl;
        return EXIT_FAILURE;
    }

    window = new sf::RenderWindow(sf::VideoMode(1280, 800), "self-o-mat", sf::Style::Fullscreen);
    window->setActive(false);
    logoTexture = new sf::Texture();
    if (!logoTexture->loadFromFile("./assets/logo.png")) {
        delete (window);
        delete (logoTexture);
        delete (font);
        cerr << "Error loading logo texture" << endl;
        return EXIT_FAILURE;
    }

    logoSprite = new sf::Sprite();
    logoSprite->setTexture(*logoTexture);

    // center logo in the screen
    auto windowSize = window->getSize();
    auto logoSize = logoTexture->getSize();
    logoSprite->setPosition((windowSize.x - logoSize.x) / 2, (windowSize.y - logoSize.y) / 2);


    const auto start = std::chrono::steady_clock::now();
    if(boot) {
        drawUpdateScreen("", "Starting Up ...");
    } else {
        if (firstRun) {
            drawUpdateScreen("Welcome!",
                             "Congratulations! You have successfully built your self-o-mat!\nPlug in a USB drive containing the selfomat.update file to install the firmware.");
        } else {
            drawUpdateScreen("Waiting for Connection...", "plug in a USB drive containing the selfomat.update file.\nUpdater Version 2.1");
        }
    }


    served::multiplexer mux;
    served::net::server *server = nullptr;
    while (true) {
        try {
            server = new served::net::server("0.0.0.0", "9081", mux, true);
            break;
        } catch (boost::exception &e) {
            cerr << "Error opening server. Trying again in 1 sec" << endl;
            sleep(1);
        }
    }


    mux.handle("/version")
            .get([](served::response &res, const served::request &req) {
                setHeaders(res);

                // try to read the current version. if it fails don't set it
                std::string version_file = target_dir + "/version";
                ifstream f(version_file, ios::in);
                string file_contents{istreambuf_iterator<char>(f), istreambuf_iterator<char>()};

                res.set_status(200);
                res.set_body(file_contents);
            });

    mux.handle("/update")
            .post([&server](served::response &res, const served::request &req) {
                updateMutex.lock();

                setHeaders(res);

                drawUpdateScreen("Updating...", "phone connected!");


                // Receive file
                std::string temp_dir = UPDATE_TEMP_DIR;
                std::string receive_file = std::string(UPDATE_TEMP_DIR) + "/" + UPDATE_FILE_NAME;
                std::string signature_file = std::string(UPDATE_TEMP_DIR) + "/app.sig";
                std::string content_file = std::string(UPDATE_TEMP_DIR) + "/app.tar.gz";
                ofstream f(receive_file, ios::out | ios::binary);

                f.write(req.body().c_str(), req.body().size());



                auto success = doUpdate(receive_file, temp_dir, content_file, signature_file);

                if (!success) {
                    served::response::stock_reply(400, res);
                    updateMutex.unlock();
                    return;
                }

                served::response::stock_reply(200, res);

                server->stop();
                updateServerRunning = false;
            });


    pthread_t handle;
    pthread_create(&handle, nullptr, timeoutThread, server);

    pthread_t forceHandle;
    pthread_create(&forceHandle, nullptr, forceUpdateThread, server);


    // 1 and blocking is needed for the gui to work (main thread will be handler thread then)
    server->run(1, true);

    cout << "server was shut down!" << endl;

    window->close();

    delete (logoSprite);
    delete (window);
    delete (logoTexture);
    delete (font);


    return 0;
}
