#include <iostream>
#include "utils/PlatformAbstractLayer.hpp"

int main(int, char**){
    PlatformAbstractLayer a;
    auto b = a.GetSysInfo();
    return 0;
}
