#ifndef _MODULE_MGR_H
#define _MODULE_MGR_H

#include "Define.h"
#include <string_view>
#include <vector>


namespace Firelands::Module {
    FC_COMMON_API void SetEnableModulesList(std::string_view modules_list);
    FC_COMMON_API std::vector<std::string_view> GetEnableModulesList();
}

#endif
