#pragma once
#define STATIC_ASSERT(expr, msg)   \
typedef char ______Assertion_Failed_____##msg[1] ; \
typedef char ______Assertion_Failed_____##msg[(expr)?1:2] ;