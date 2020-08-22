#ifndef HEADER_PUGIXMLW_HPP
#define HEADER_PUGIXMLW_HPP

// https://pugixml.org/docs/manual.html

// https://stackoverflow.com/questions/14329075/using-pugixml-in-wchar-t-mode-and-in-usual-mode-in-one-project

#define PUGIXML_WCHAR_MODE
#define pugi pugiw

#undef HEADER_PUGIXML_HPP
#undef HEADER_PUGICONFIG_HPP
#undef SOURCE_PUGIXML_CPP

#undef PUGIXML_TEXT
#undef PUGIXML_CHAR

#include <pugixml.hpp>

#undef PUGIXML_TEXT
#undef PUGIXML_CHAR

#undef HEADER_PUGIXML_HPP
#undef HEADER_PUGICONFIG_HPP
#undef SOURCE_PUGIXML_CPP

#undef pugi
#undef PUGIXML_WCHAR_MODE

#endif
