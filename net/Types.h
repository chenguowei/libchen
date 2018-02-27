//
// Created by 陈国威 on 2018/2/6.
//

#ifndef LIBCHEN_TYPES_H
#define LIBCHEN_TYPES_H

#include <string.h>

namespace chen
{

template<typename To, typename From>
inline To implicit_cast(From const &f)
{
 	return f;
}

}
#endif //LIBCHEN_TYPES_H
