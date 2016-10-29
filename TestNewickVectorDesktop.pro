include(../RibiLibraries/DesktopApplicationNoWeffcpp.pri)

include(../RibiLibraries/Boost.pri)
include(../RibiLibraries/BigInteger.pri)

include(../RibiLibraries/GeneralConsole.pri)
include(../RibiLibraries/GeneralDesktop.pri)

include(../RibiClasses/CppFuzzy_equal_to/CppFuzzy_equal_to.pri)
include(../BinaryNewickVector/BinaryNewickVector.pri)
include(../Newick/Newick.pri)
include(../NewickVector/NewickVector.pri)
include(../TwoDigitNewick/TwoDigitNewick.pri)

include(TestNewickVectorDesktop.pri)

SOURCES += qtmain.cpp

# Debug and release mode
CONFIG += console debug_and_release
CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

# QResources give this error
QMAKE_CXXFLAGS += -Wno-unused-variable

# Qt:
# QtConcurrent::filterInternal(Sequence&, KeepFunctor, ReduceFunctor)’:
# /usr/include/qt4/QtCore/qtconcurrentfilter.h:108:47: error: typedef ‘Iterator’ locally defined but not used [-Werror=unused-local-typedefs]
# typedef typename Sequence::const_iterator Iterator;
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
