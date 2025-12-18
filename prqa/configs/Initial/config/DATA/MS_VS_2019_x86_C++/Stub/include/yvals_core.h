#pragma once
#ifndef QACPP_YVALS_CORE_H_
#define QACPP_YVALS_CORE_H_

//
// Helper to remove definitions for library features that are not yet supported
#include_next "yvals_core.h"

#if ! defined (__cpp_concepts)

#  if defined (__cpp_lib_concepts)
#    undef __cpp_lib_concepts
#  endif

#  if defined (__cpp_lib_format)
#    undef __cpp_lib_format
#  endif

#  if defined (__cpp_lib_ranges)
#    undef __cpp_lib_ranges
#  endif

#endif //  ! defined (__cpp_concepts)

#endif // YVALS_CORE_H_
