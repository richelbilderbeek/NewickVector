include(../RibiLibraries/DesktopApplicationNoWeffcpp.pri)

include(../RibiLibraries/Boost.pri)
include(../RibiLibraries/BigInteger.pri)

include(../RibiLibraries/GeneralConsole.pri)
include(../RibiLibraries/GeneralDesktop.pri)

include(../RibiClasses/CppBinaryNewickVector/CppBinaryNewickVector.pri)
include(../RibiClasses/CppFuzzy_equal_to/CppFuzzy_equal_to.pri)
include(../RibiClasses/CppNewick/CppNewick.pri)
include(../RibiClasses/CppNewickVector/CppNewickVector.pri)
include(../RibiClasses/CppTwoDigitNewick/CppTwoDigitNewick.pri)

include(TestNewickVectorDesktop.pri)

SOURCES += qtmain.cpp
