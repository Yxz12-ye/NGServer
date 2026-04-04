#include "Tools.hpp"

class BaseUtils
{
private:

    
public:
    BaseUtils(/* args */) = default;
    virtual ~BaseUtils() = default;

    virtual SysInfo_t getSysInfo() = 0;
};

