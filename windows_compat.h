/* ========================================
   SUB Language - Windows Compatibility Header
   Handles MSVC/Windows-specific function mappings
   Include this at the top of files using POSIX functions
   ======================================== */

#ifndef WINDOWS_COMPAT_H
#define WINDOWS_COMPAT_H

#ifdef _WIN32
    // MSVC uses _strdup instead of strdup
    #define strdup _strdup
    
    // MSVC uses _stricmp instead of strcasecmp
    #define strcasecmp _stricmp
    #define strncasecmp _strnicmp
    
    // Windows doesn't have <strings.h>, functions are in <string.h>
    #ifndef _STRINGS_H_INCLUDED
        #define _STRINGS_H_INCLUDED
    #endif
#endif

#endif /* WINDOWS_COMPAT_H */
