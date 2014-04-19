/*
    Copyright (c) 2014 Contributors as noted in the AUTHORS file
    
    This file is part of ABaDooRand.
    
    ABaDooRand is available under two different licensing options designed to accommodate the needs of our various users.
    
    ABaDooRand licensed under our commercial license agreement is appropriate for development of proprietary/commercial 
    software where you do not want to share any source code with third parties or otherwise cannot comply with the terms 
    of the [GNU General Public License (GPL) version 3.0](GNU-GPL-v3.0). 
    If you are under this use case, please contact the author.
    
    ABaDooRand licensed under the [GNU GPL version 3.0](GNU-GPL-v3.0) is appropriate for the development of 
    ABaDooRand applications where you wish to use such applications in combination with software subject to the terms 
    of the [GNU GPL version 3.0](GNU-GPL-v3.0) or where you are otherwise willing to comply with the terms of the 
    [GNU GPL version 3.0](GNU-GPL-v3.0).
    
    ABaDooRand documentation is licensed under the terms of the [GNU Free Documentation License (FDL) version 1.3](GNU-FDL-v1.3),
    as published by the Free Software Foundation.
    
    ABaDooRand is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
    
    You should have received a copy of the licences along with this program, LICENCE.txt and the GNU licenses.  If not,
    see <http://www.gnu.org/licenses/> for the GNU ones.
*/

#include <set>

#include "../include/utils.h"
#include "../src/err.hpp"

int test_utils(bool verbose) {
    map_args_t args;
    multimap_args_t multiArgs;

    // nominal use
    int rc = get_args(&multiArgs, "arg1 (aaa = 123, bb = AZERTY); arg2 (aaa = O0, cc = 000); arg3; arg4 (bb = f, aaa=0)", ";", "(", ")");
    abadoorand_assert(rc == 0);
    abadoorand_assert(multiArgs.size() == 4);
    auto arg = multiArgs.find("arg1");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "aaa = 123, bb = AZERTY");
    arg = multiArgs.find("arg2");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "aaa = O0, cc = 000");
    arg = multiArgs.find("arg3");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "");
    arg = multiArgs.find("arg4");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "bb = f, aaa=0");
    multiArgs.clear();

    // nominal use with duplicate arguments
    rc = get_args(&multiArgs, "arg1 (aaa = 123, bb = AZERTY); arg2 (aaa = O0, cc = 000); arg1; arg1 (bb = f, aaa=0)", ";", "(", ")");
    abadoorand_assert(rc == 0);
    abadoorand_assert(multiArgs.size() == 4);
    std::set<std::string> arg1Args = {"aaa = 123, bb = AZERTY", "bb = f, aaa=0", ""};
    for (auto arg : multiArgs) {
        if (arg.first == "arg1") {
            auto arg1Arg = arg1Args.find(arg.second);
            abadoorand_assert(arg1Arg != arg1Args.end());
            arg1Args.erase(arg1Arg);
        }
        else {
            abadoorand_assert(arg.first == "arg2");
            abadoorand_assert(arg.second == "aaa = O0, cc = 000");
        }
    }
    abadoorand_assert(arg1Args.empty());
//    auto arg = multiArgs.find("arg1");
//    abadoorand_assert(arg != multiArgs.end());
//    abadoorand_assert(arg->second == "aaa = 123, bb = AZERTY");
//    arg = multiArgs.find("arg2");
//    abadoorand_assert(arg != multiArgs.end());
//    abadoorand_assert(arg->second == "aaa = O0, cc = 000");
//    arg = multiArgs.find("arg3");
//    abadoorand_assert(arg != multiArgs.end());
//    abadoorand_assert(arg->second == "");
//    arg = multiArgs.find("arg4");
//    abadoorand_assert(arg != multiArgs.end());
//    abadoorand_assert(arg->second == "bb = f, aaa=0");
    multiArgs.clear();

    // more delimiters
    rc = get_args(&multiArgs, "arg1 (aaa = 123, bb:AZERTY); arg2 {aaa:O0 cc := 000}; arg3[  bb  =  f,aaa=0]", ";", "([{", ")]}");
    abadoorand_assert(rc == 0);
    abadoorand_assert(multiArgs.size() == 3);
    arg = multiArgs.find("arg1");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "aaa = 123, bb:AZERTY");
    arg = multiArgs.find("arg2");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "aaa:O0 cc := 000");
    arg = multiArgs.find("arg3");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "bb  =  f,aaa=0");
    multiArgs.clear();

    // extra spaces, or no spaces
    rc = get_args(&multiArgs, "  arg1 (aaa = 123, bb:AZERTY)  ;arg2 { aaa:O0 cc := 000};  arg3    ", ";", "({", ")}");
    abadoorand_assert(rc == 0);
    abadoorand_assert(multiArgs.size() == 3);
    arg = multiArgs.find("arg1");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "aaa = 123, bb:AZERTY");
    arg = multiArgs.find("arg2");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "aaa:O0 cc := 000");
    arg = multiArgs.find("arg3");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "");
    multiArgs.clear();

    // no arguments arguments
    rc = get_args(&multiArgs, "arg1; arg2,arg3", ";,", "");
    abadoorand_assert(rc == 0);
    abadoorand_assert(multiArgs.size() == 3);
    arg = multiArgs.find("arg1");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "");
    arg = multiArgs.find("arg2");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "");
    arg = multiArgs.find("arg3");
    abadoorand_assert(arg != multiArgs.end());
    abadoorand_assert(arg->second == "");
    multiArgs.clear();

    // nothing at all
    rc = get_args(&multiArgs, "", "", "");
    abadoorand_assert(rc == -1);
    abadoorand_assert(multiArgs.size() == 0);
    multiArgs.clear();

    // Let's try one of our test argument arguments
    rc = get_args(&args, "aaa = 123, bb = AZERTY", ",", "=", "");
    abadoorand_assert(rc == 0);
    abadoorand_assert(args.size() == 2);
    abadoorand_assert(args["aaa"] == "123");
    abadoorand_assert(args["bb"] == "AZERTY");
    args.clear();


    return 0;
}

