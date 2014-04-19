#!/bin/sh

# Copyright (c) 2014 Contributors as noted in the AUTHORS file
#
# This file is part of ABaDooRand.
#
# ABaDooRand is available under two different licensing options designed to accommodate the needs of our various users.
#
# ABaDooRand licensed under our commercial license agreement is appropriate for development of proprietary/commercial 
# software where you do not want to share any source code with third parties or otherwise cannot comply with the terms 
# of the [GNU General Public License (GPL) version 3.0](GNU-GPL-v3.0). 
# If you are under this use case, please contact the author.
#
# ABaDooRand licensed under the [GNU GPL version 3.0](GNU-GPL-v3.0) is appropriate for the development of 
# ABaDooRand applications where you wish to use such applications in combination with software subject to the terms 
# of the [GNU GPL version 3.0](GNU-GPL-v3.0) or where you are otherwise willing to comply with the terms of the 
# [GNU GPL version 3.0](GNU-GPL-v3.0).
#
# ABaDooRand documentation is licensed under the terms of the [GNU Free Documentation License (FDL) version 1.3](GNU-FDL-v1.3),
# as published by the Free Software Foundation.
#
# ABaDooRand is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the licences along with this program, LICENCE.txt and the GNU licenses.  If not,
# see <http://www.gnu.org/licenses/> for the GNU ones.

# Script to generate all required files from fresh git checkout.

command -v libtool >/dev/null 2>&1
if  [ $? -ne 0 ]; then
    echo "autogen.sh: error: could not find libtool.  libtool is required to run autogen.sh." 1>&2
    exit 1
fi

command -v autoreconf >/dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "autogen.sh: error: could not find autoreconf.  autoconf and automake are required to run autogen.sh." 1>&2
    exit 1
fi

mkdir -p ./config
if [ $? -ne 0 ]; then
    echo "autogen.sh: error: could not create directory: ./config." 1>&2
    exit 1
fi

autoreconf --install --force --verbose -I config
if [ $? -ne 0 ]; then
    echo "autogen.sh: error: autoreconf exited with status $?" 1>&2
    exit 1
fi