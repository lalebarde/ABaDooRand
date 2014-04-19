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

#include <cstring>
#include <new>

#include "../include/utils.h"

template<typename T>
int get_args(T* mapArgs, const char* aArgs, const char* aDelim, const char* aArgArgsStart, const char* aArgArgsEnd) {
    int remaining = strlen(aArgs);
    if (!remaining)
        return -1;

    char *args = new char[strlen(aArgs)];
    strcpy(args, aArgs);
    char* argsToDelete = args;
    int start = strspn (args, " ");
    args += start; // trim possible white spaces

    char *argArgsStart = new char[strlen(aArgArgsStart + 1)];
    argArgsStart[0] = ' ';
    strcpy(argArgsStart + 1, aArgArgsStart);

    int delim = 0, len = 0;
    while (true) {
        remaining = strlen(args);
        delim = strcspn (args, aDelim); // ",;" // The length of the initial part of str1 not containing any of the characters that are part of str2 - http://www.cplusplus.com/reference/cstring/strcspn/
        args[delim] = '\0';
        len = strlen(args);
        start = strcspn (args, argArgsStart); // "([{" // The length of the initial part of str1 not containing any of the characters that are part of str2 - http://www.cplusplus.com/reference/cstring/strcspn/
        args[start] = '\0';
        if (start == len) {  // no arguments delimiter found
            mapArgs->insert(xmap_args_p(std::string(args), ""));
//            (*mapArgs)[std::string(args)] = ""; // added with no arguments
        }
        else {
            int trim = strspn (args + start + 1, argArgsStart); // The length of the initial portion of str1 containing only characters that appear in str2 - http://www.cplusplus.com/reference/cstring/strspn/
            char* argArgs = args + start + 1 + trim;
            int end = (strlen(aArgArgsEnd) ? strcspn (argArgs, aArgArgsEnd) : strlen(argArgs)); // ")]}"
            argArgs[end] = '\0';
            mapArgs->insert(xmap_args_p(std::string(args), std::string(argArgs)));
//            (*mapArgs)[std::string(args)] = std::string(argArgs);
        }
        if (delim == remaining) // no other argument
            break;
        int trim = strspn (args + delim + 1, aDelim);
        args += delim + 1 + trim;
        trim = strspn (args, " ");
        args += trim;
    }
    delete[] argsToDelete;
    delete[] argArgsStart;
    return mapArgs->size() ? 0 : -1;
}

template int get_args(multimap_args_t* mapArgs, const char* aArgs, const char* aDelim, const char* aArgArgsStart, const char* aArgArgsEnd);
template int get_args(map_args_t* mapArgs, const char* aArgs, const char* aDelim, const char* aArgArgsStart, const char* aArgArgsEnd);


template<typename T>
int dump(const T* data, const size_t size, const char* aFileName) {
    if (!(data && size && aFileName))
        return -1;
    FILE * pFile = fopen (aFileName,"w");
    if (!pFile)
        return -1;
    for (size_t i = 0; i < size; i++)
        fprintf (pFile, "%llu\n", data[i]);
    fclose(pFile);
    return 0;
}

template int dump(const uint32_t* data, const size_t size, const char* aFileName);
template int dump(const uint64_t* data, const size_t size, const char* aFileName);


