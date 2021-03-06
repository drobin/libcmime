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
 * @file cmime_config.h
 * @brief CMime's config constants. 
 *
 */

#ifndef _CMIME_CONFIG_H
#define _CMIME_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @def CMIME_VERSION
 * @returns CMime version
 */
#define CMIME_VERSION ${CMIME_VERSION_NUMBER}

/*!
 * @def CMIME_MAJOR_VERSION
 * @returns CMime's major version
 */
#define CMIME_MAJOR_VERSION ${CMIME_MAJOR_VERSION}

/*!
 * @def CMIME_MINOR_VERSION
 * @returns CMime's minor version
 */
#define CMIME_MINOR_VERSION ${CMIME_MINOR_VERSION}

/*!
 * @def CMIME_MICRO_VERSION
 * @returns CMime's micro version
 */
#define CMIME_MICRO_VERSION ${CMIME_MICRO_VERSION}

/*!
 * @def FILE_EXECUTABLE
 * @returns path to the file binary
 */
#define FILE_EXECUTABLE "${FILE_EXECUTABLE} -b --mime"

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_CONFIG_H */
