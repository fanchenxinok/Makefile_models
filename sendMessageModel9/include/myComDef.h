#ifndef __MY_COM_DEF__
#define __MY_COM_DEF__

#include <string>
using namespace std;

typedef struct myInfo
{
    string name;
    int age;
    char sex;
}MY_INFO;

#define PROTO_MSG

#ifdef PROTO_MSG
    #include "msgDef.pb.h"
    #define USE_PROTO_BUF (1)
#else
    #define USE_PROTO_BUF (0)
#endif

#endif
