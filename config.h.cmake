#ifndef DEFINE_H
#define DEFINE_H

#ifndef NULL
#	if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
#		define NULL nullptr
#	else
#		define NULL 0x0
#	endif
#endif

#define PLUGIN_API_VERSION_MAJOR @PLUGIN_API_VERSION_MAJOR@
#define PLUGIN_API_VERSION_MINOR @PLUGIN_API_VERSION_MINOR@

#define VERIF_VERSION_MAJOR @VERIF_VERSION_MAJOR@
#define VERIF_VERSION_MINOR @VERIF_VERSION_MINOR@

#endif /* end of include guard */
//vim:ft=cpp
