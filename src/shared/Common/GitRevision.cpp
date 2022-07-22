/*
 * This file is part of the Firelands Core Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "GitRevision.h"

char const* GitRevision::GetHash()
{
    return REVISION_HASH;
}

char const* GitRevision::GetDate()
{
    return REVISION_DATE;
}

char const* GitRevision::GetBranch()
{
    return REVISION_BRANCH;
}

char const* GitRevision::GetCMakeVersion()
{
    return CMAKE_VERSION;
}

char const* GitRevision::GetHostOSVersion()
{
    return "Compiled on: " CMAKE_HOST_SYSTEM;
}

// Platform Define
#if PLATFORM == PLATFORM_WINDOWS
#ifdef _WIN64
#define FIRELANDS_PLATFORM_STR "Win64"
#else
#define FIRELANDS_PLATFORM_STR "Win32"
#endif
#elif PLATFORM == PLATFORM_APPLE
#define FIRELANDS_PLATFORM_STR "MacOSX"
#elif PLATFORM == PLATFORM_INTEL
#define FIRELANDS_PLATFORM_STR "Intel"
#elif PLATFORM == PLATFORM_UNIX
#define FIRELANDS_PLATFORM_STR "Linux"
#else
#define FIRELANDS_PLATFORM_STR "Unknown System"
#endif

// Database Revision
char const* GitRevision::GetProjectRevision()
{
    return PROJECT_REVISION_NR;
}

char const* GitRevision::GetFullRevision()
{
    return "firelands-cata revision: " VER_PRODUCTVERSION_STR;
}

char const* GitRevision::GetRunningSystem()
{
    return "Running on: " CMAKE_HOST_SYSTEM;
}

char const* GitRevision::GetCompanyNameStr()
{
    return VER_COMPANY_NAME_STR;
}

char const* GitRevision::GetLegalCopyrightStr()
{
    return VER_LEGALCOPYRIGHT_STR;
}

char const* GitRevision::GetFileVersionStr()
{
    return VER_FILEVERSION_STR;
}

char const* GitRevision::GetProductVersionStr()
{
    return VER_PRODUCTVERSION_STR;
}
