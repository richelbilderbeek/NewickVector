#!/bin/bash
cd ..

if [ ! -d RibiClasses ]; then
 git clone https://github.com/richelbilderbeek/RibiClasses
fi

if [ ! -d RibiLibraries ]; then
 git clone https://github.com/richelbilderbeek/RibiLibraries
fi

if [ ! -d BinaryNewickVector ]; then
 git clone https://github.com/richelbilderbeek/BinaryNewickVector
fi

if [ ! -d Newick ]; then
 git clone https://github.com/richelbilderbeek/Newick
fi

if [ ! -d TwoDigitNewick ]; then
 git clone https://github.com/richelbilderbeek/TwoDigitNewick
fi