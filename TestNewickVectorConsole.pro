include(../RibiLibraries/ConsoleApplication.pri)

include(../RibiLibraries/GeneralConsole.pri)
include(../RibiLibraries/Apfloat.pri)
include(../RibiLibraries/BigInteger.pri)
include(../RibiLibraries/Boost.pri)

include(../BinaryNewickVector/BinaryNewickVector.pri)
include(../RibiClasses/CppFuzzy_equal_to/CppFuzzy_equal_to.pri)
include(../Newick/Newick.pri)
include(../NewickVector/NewickVector.pri)
include(../TwoDigitNewick/TwoDigitNewick.pri)

include(TestNewickVectorConsole.pri)

SOURCES += main.cpp

# Debug and release mode
CONFIG += console debug_and_release
CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}
