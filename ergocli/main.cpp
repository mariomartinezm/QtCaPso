#include <iostream>

#include "Models/localcapso.h"

const int WIDTH  = 512;
const int HEIGHT = 512;

int main()
{
    LocalCaPso ca(WIDTH, HEIGHT);

    std::cout << ca.width() << ", " << ca.height() << std::endl;

    return 0;
}
