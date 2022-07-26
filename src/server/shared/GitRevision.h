#ifndef __GITREVISION_H__
#define __GITREVISION_H__

#include <string>
#include "Define.h"

namespace GitRevision
{
    char const* GetHash();
    char const* GetDate();
    char const* GetBranch();
    char const* GetBuildDirectory();
    char const* GetFullVersion();
    char const* GetCompanyNameStr();
    char const* GetLegalCopyrightStr();
    char const* GetFileVersionStr();
    char const* GetProductVersionStr();
}

#endif
