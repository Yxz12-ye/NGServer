#include "Tools.hpp"

class BaseUtils
{
private:

    
public:
    BaseUtils(/* args */) = default;
    ~BaseUtils() = default;

    virtual SysInfo_t getSysInfo() = 0;
};

