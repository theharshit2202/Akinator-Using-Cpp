//
// Created by Harshit Gupta and Manya Verma on 15.01.2023.
//

#ifndef AKINATOR_MYSTR_H
#define AKINATOR_MYSTR_H

#include "stdio.h"
#include "stdlib.h"
#include <cstring>

char* text_from_file(FILE* file_name, size_t size_of_file, size_t* number_of_lines);

/*!
*  \function size_t Size_of_file(FILE* name_of_file);
*  \brief Function finds size of file
*  \param name_of_file - a FILE*
*  \return size_t - number of chars
*/
size_t Size_of_file(FILE* name_of_file);


int replace_spaces(char* word);


#endif //AKINATOR_MYSTR_H
