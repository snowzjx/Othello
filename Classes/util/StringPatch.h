//
//  StringPatch.h
//  Othello
//
//  Created by Snow on 2/8/14.
//
//

#ifndef _STRING_PATCH_H_
#define _STRING_PATCH_H_

#include <string>
#include <sstream>

namespace string_patch {
    template <typename T> std::string to_string(const T& value) {
        std::ostringstream stm;
        stm << value;
        return stm.str();
    }
}


#endif
