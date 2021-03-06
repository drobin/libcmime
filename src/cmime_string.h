/* libcmime - A C mime library
 * Copyright (C) 2012 SpaceNet AG and Axel Steiner <ast@treibsand.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/*!
 * @file cmime_string.h
 * @brief Defines helper functions and structs for string handling
 */

#ifndef _CMIME_STRING_H
#define _CMIME_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/*!
 * @struct CMimeStringList_T cmime_string.h
 * @brief A simple list with strings
 */
typedef struct {
    char **node; /**< list with string values */
    size_t count; /**< count of strings in list */
} CMimeStringList_T;

/*!
 * @fn CMimeStringList_T *cmime_string_list_new(void)
 * @brief Creates a new CMimeStringList_T object
 * @returns CMimeStringList_T pointer, or NULL on failure
 */
CMimeStringList_T *cmime_string_list_new(void);

/*!
 * @fn void cmime_string_list_free(CMimeStringList_T *sl)
 * @brief Free a CMimeStringList_t object 
 * @param sl CMimeStringList_T pointer
 */
void cmime_string_list_free(CMimeStringList_T *sl);

/*!
 * @fn void cmime_string_list_insert(CMimeStringList_T *sl, const char *s)
 * @brief Insert new string into list
 * @param sl CMimeStringList_T pointer
 * @param s a string
 */
void cmime_string_list_insert(CMimeStringList_T *sl, const char *s);

/*!
 * @fn char *cmime_string_list_get(CMimeStringList_T *sl, int pos)
 * @brief Return string at given position
 * @param sl CMimeStringList_T pointer
 * @param pos String position in list
 * @returns string value
 */
char *cmime_string_list_get(CMimeStringList_T *sl, int pos);

/*! 
 * @def cmime_string_list_get_count(sl)
 * @returns string list count
 */
#define cmime_string_list_get_count(sl) ((int)sl->count)

/*!
 * @fn char *cmime_string_strip(char *s)
 * @brief Strip leading/trailing spaces from string. 
 *   This function doesn't allocate or reallocate any memory; 
 *   it modifies string in place. 
 * @param s string with spaces
 * @returns string with removed spaces
 */
char *cmime_string_strip(char *s);

/*!
 * @fn char *cmime_string_chomp(char *s)
 * @brief remove trailing newline from string
 *   This function doesn't allocate or reallocate any memory;
 *   it modifies string in place.
 * @param s a string
 * @returns string with removed newline
 */
char *cmime_string_chomp(char *s);

/*!
 * int cmime_string_is_7bit(const char *s)
 * @brief Determine if given string is 7bit only or not
 * @param s string to check
 * @returns 0 if  string is 7bit, otherwise -1
 */
int cmime_string_is_7bit(const char *s);

/*!
 * int cmime_string_is_8bit(const char *s)
 * @brief Determine if given string contains 8bit characters
 * @param s string to check
 * @returns 0 if string contains 8bit characters, otherwise -1
 */
int cmime_string_is_8bit(const char *s);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_STRING_H */
