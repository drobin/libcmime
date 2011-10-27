/* libcmime - A C mime library
 * Copyright (C) 2011 Axel Steiner <ast@treibsand.com>
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
 *
 * This header is intended for use by libcmime only.
 *
 * @example test_string.c
 */

#ifndef _CMIME_STRING_H
#define _CMIME_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

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
 * @fn CMimeStringList_T *cmime_string_split(const char *s, const char *sep)
 * @brief Return a list of the words in the string s, using sep as the delimiter string.
 * @param s string to split
 * @param sep split separator
 * @param maxsplit maximal number of splittings
 * @returns a newly allocated CMimeStringList_T list
 */
CMimeStringList_T *cmime_string_split(const char *s, const char *sep, int maxsplit);

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
 * @fn char *cmime_string_strstr_last(const char *s1, const char *s2)
 * @brief Locate the last occurrence of the null-terminated string s2 in 
 *   the null-terminated string s1.
 * @param s1 string to search in
 * @param s2 string to search for
 * @returns If s2 is an empty string, s1 is returned; if s2 occurs nowhere 
 *   in s1, NULL is returned; otherwise a pointer to the first character of 
 *   the last occurrence of s2 is returned.
 */
char *cmime_string_strstr_last(const char *s1, const char *s2);

char *cmime_string_strsep_last(const char *s, const char *sep);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_STRING_H */
