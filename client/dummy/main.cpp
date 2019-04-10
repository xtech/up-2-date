#include <cstdlib>
#include <ctime>
#include <unistd.h>

#define EXIT_UPDATE -1;

using namespace std;

int main() {
    sleep(2);
    srand(time(nullptr));
    if(rand()%2)
        return EXIT_FAILURE;
    sleep(5);
    return EXIT_UPDATE;
}
