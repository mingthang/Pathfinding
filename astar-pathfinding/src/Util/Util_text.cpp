#include "Util.h"

namespace Util {
    bool StrCmp(const char* queryA, const char* queryB) {
        return strcmp(queryA, queryB) == 0;
    }
}