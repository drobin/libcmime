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

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <libgen.h>
#include <ctype.h>

#include "cmime_message.h"
#include "cmime_part.h"
#include "cmime_util.h"
#include "cmime_internal.h"
#include "cmime_base64.h"
#include "cmime_string.h"
#include "cmime_list.h"

/* replace or add a new mime part header */
void _set_part_header_value(CMimeList_T *l, const char *key, const char *value) {
	CMimeListElem_T *e = NULL;
	CMimeHeader_T *h = NULL;
	char *ptemp = NULL;
	
	assert(l);
	assert(key);
	assert(value);

	ptemp = (char *)value;
	ptemp = cmime_string_strip(ptemp);
	
	e = cmime_list_head(l);
	while(e != NULL) {
		h = (CMimeHeader_T *)cmime_list_data(e);
		if (strcasecmp(cmime_header_get_name(h),key)==0) {
			cmime_header_set_value(h,ptemp,1);
			return;
		}
		e = e->next;
	}
	
	// mime header seems not to be present
	h = cmime_header_new();
	cmime_header_set_name(h,key);
	cmime_header_set_value(h,ptemp,0);
	cmime_list_append(l,h);
	return;
}

char *_get_part_header_value(CMimeList_T *l, const char *key) {
	CMimeListElem_T *e;
	CMimeHeader_T *h;
	
	assert(l);
	assert(key);
	
	e = cmime_list_head(l);
	while(e != NULL) {
		h = (CMimeHeader_T *)cmime_list_data(e);
		if (strcasecmp(cmime_header_get_name(h),key)==0) {
			return(cmime_header_get_value(h,0));
		}
		e = e->next;
	}
	
	return(NULL);
}

/* search for header value in pointer p 
 * and return value as newly allocated string */
char *_parse_header(char *p) {
	char *it = NULL;
	char *in = NULL;
	char *t = NULL;
	char *out = NULL;
	int pos = 0;

	t = (char *)calloc(sizeof(char),sizeof(char));
	in = (char *)calloc(strlen(p) + sizeof(char),sizeof(char));
	strcpy(in,p);
	
	it = strchr(in,':');
	it++;
	while (*it != '\0') {
		if (*it != '\n') {
			t = (char *)realloc(t,strlen(t) + 2 + sizeof(char));
			t[pos++] = *it;
			t[pos] = '\0';
		} else {
			if (*(it + 1) != '\t') 
				break;
		}
		it++;
	}

	out = (char *)calloc(strlen(t) + 1, sizeof(char));
	strcpy(out,t);
	free(t);

	return(out);
}

CMimePart_T *cmime_part_new(void) {
	CMimePart_T *part = NULL;
	
	part = (CMimePart_T *)calloc((size_t)1, sizeof(CMimePart_T));
	
	if (cmime_list_new(&part->headers,_cmime_internal_header_destroy)!=0)
		return(NULL);
		
	part->content = NULL;
	
	return(part);
}

void cmime_part_free(CMimePart_T *part) {
	assert(part);
	
	cmime_list_free(part->headers);
	
	if (part->content != NULL)
		free(part->content);
		
	free(part);
}


void cmime_part_set_content_type(CMimePart_T *part, const char *s) {
	assert(part);
	assert(s);

	_set_part_header_value(part->headers,"Content-Type",s);
}

char *cmime_part_get_content_type(CMimePart_T *part) {
	assert(part);
	
	return(_get_part_header_value(part->headers,"Content-Type"));
}

void cmime_part_set_content_disposition(CMimePart_T *part, const char *s) {
	assert(part);
	assert(s);

	_set_part_header_value(part->headers,"Content-Disposition",s);
}

char *cmime_part_get_content_disposition(CMimePart_T *part) {
	assert(part);
	
	return(_get_part_header_value(part->headers,"Content-Disposition"));
}

void cmime_part_set_content_transfer_encoding(CMimePart_T *part, const char *s) {
	assert(part);
	assert(s);

	_set_part_header_value(part->headers,"Content-Transfer-Encoding",s);
}

char *cmime_part_get_content_transfer_encoding(CMimePart_T *part) {
	assert(part);
	
	return(_get_part_header_value(part->headers,"Content-Transfer-Encoding"));
}

void cmime_part_set_content_id(CMimePart_T *part, const char *s) {
	assert(part);
	assert(s);
	
	_set_part_header_value(part->headers,"Content-Id",s);
}

char *cmime_part_get_content_id(CMimePart_T *part) {
	return(_get_part_header_value(part->headers,"Content-Id"));
}

void cmime_part_set_content(CMimePart_T *part, const char *s) {
	assert(part);
	assert(s);
	
	if (part->content != NULL)
		free(part->content);
	
	part->content = (char *)malloc(strlen(s) + sizeof(char));
	strcpy(part->content,s);
}

char *cmime_part_to_string(CMimePart_T *part) {
	char *out = NULL;
	char *content = NULL; 
	char *ptemp = NULL;
	int with_headers = 0;
	CMimeListElem_T *e;
	CMimeHeader_T *h;
	
	assert(part);

	content = cmime_part_get_content(part);
	if (content==NULL)
		return(NULL);
		
	out = (char *)calloc(1,sizeof(char));
	if (cmime_list_size(part->headers)!=0) {
		e = cmime_list_head(part->headers);
		while(e != NULL) {
			h = (CMimeHeader_T *)cmime_list_data(e);
			ptemp = cmime_header_to_string(h);
			if (strstr(ptemp,CRLF)!=NULL) {
				out = (char *)realloc(out,strlen(out) + strlen(ptemp) + 1);
				strcat(out,ptemp);
			} else {
				out = (char *)realloc(out,strlen(out) + strlen(ptemp) + strlen(CRLF) + 1);
				strcat(out,ptemp);
				strcat(out,CRLF);
			} 
			free(ptemp);
			e = e->next;
		} 
		with_headers = 1; 
	}
	
	if (with_headers==1) {
		out = (char *)realloc(out,strlen(out) + strlen(CRLF) + 1);
		strcat(out,CRLF);
	} 

	out = (char *)realloc(out,strlen(out) + strlen(content) + 1);
	strcat(out,content);

	return(out);
}

int cmime_part_from_file(CMimePart_T **part, char *filename) {
	struct stat fileinfo;
	int retval = 0;
	FILE *fp = NULL;
	int encode = 0;
	int i = 0;
	int len = 0;
	int blocksout = 0;
	int pos = 0;
	unsigned char in[3], out[4];
	char *ptemp = NULL;
	
	assert((*part));
	assert(filename);
	
	/* only regular files please */
	retval = stat(filename,&fileinfo);
	if (retval == 0) {
		if(S_ISREG(fileinfo.st_mode)) {
			ptemp = cmime_util_get_mimetype(filename);
			
			/* set default mime type if no one found */
			if(ptemp == NULL) {
				asprintf(&ptemp,"%s%s",MIMETYPE_DEFAULT,CRLF);
			} else {
				ptemp = (char *)realloc(ptemp,strlen(ptemp) + strlen(CRLF) + sizeof(char));
				strcat(ptemp,CRLF);
			}

			cmime_part_set_content_type((*part),ptemp);
			encode = (strncmp(ptemp,MIMETYPE_TEXT_PLAIN,strlen(MIMETYPE_TEXT_PLAIN)) == 0) ? 0 : 1;
			free(ptemp);
					
			if (encode == 1)
				asprintf(&ptemp,"base64%s",CRLF);
			else 
				asprintf(&ptemp,"7bit%s",CRLF);
			
			cmime_part_set_content_transfer_encoding((*part),ptemp);
			free(ptemp);
			
			asprintf(&ptemp,"attachment; filename=%s%s",basename(filename),CRLF);
			cmime_part_set_content_disposition((*part),ptemp);		
			free(ptemp);
			
			fp = fopen(filename, "rb");
			if (fp != NULL) {
				(*part)->content = (char *)calloc(1,sizeof(char));
				while(!feof(fp)) {
					len = 0;
					
					for(i=0; i<3; i++) {
						in[i] = (unsigned char)fgetc(fp);

						if(!feof(fp)) {
							len++;
						} else {
							in[i] = 0;
						}
					}
					
					if (len) {
						if (encode == 0) {
							(*part)->content = (char *)realloc((*part)->content,strlen((*part)->content) + sizeof(in) + sizeof(char));
							strcat((*part)->content,(char *)in);
						} else {
							cmime_base64_encode_block(in,out,len);
							(*part)->content = (char *)realloc((*part)->content,strlen((*part)->content) + sizeof(out) + sizeof(char));
							for (i=0; i<4;i++) {
								(*part)->content[pos++] = out[i];
							}
							(*part)->content[pos] = '\0';						}
						blocksout++;
					}

					if(blocksout >= (LINE_LENGTH / 4) || feof(fp)) {
						if(blocksout && (encode == 1)) {
							/* if base64 data, we need to do a line break after LINE_LENGTH chars */
							(*part)->content = (char *)realloc((*part)->content,strlen((*part)->content) + strlen(CRLF) + sizeof(char));
							// TODO: add support for other line endings
							(*part)->content[pos++] = '\r';
							(*part)->content[pos++] = '\n';
							(*part)->content[pos] = '\0';
						}
					blocksout = 0;
					} 
				}
				fclose(fp);
			} else {
				return (-3); /* failed to open file */
			}
			
		} else {
			return(-2); /* not regular file */
		} 
	} else {
		return(-1); /* stat error */
	}
	
	return(0);
}

int cmime_part_from_string(CMimePart_T **part, const char *content) {
	char *ptemp = NULL;
	char *ptemp2 = NULL;
	char *body = NULL;
	char *lb = NULL;
	char *dlb;
	char *it = NULL;

	assert((*part));
	assert(content);
	
	lb = _cmime_internal_determine_linebreak(content);
	if (lb == NULL)
		return(-1);
	
	asprintf(&dlb,"%s%s",lb,lb);

	ptemp = strstr(content,dlb);
	if (ptemp != NULL) {
		it = (char *)content;
		while (*it != '\0') {
			if (strncasecmp(it,PART_CONTENT_TYPE_PATTERN,strlen(PART_CONTENT_TYPE_PATTERN))==0) {				
				ptemp2 = _parse_header(it);
				cmime_part_set_content_type((*part),ptemp2);
				free(ptemp2);
			}

			if (strncasecmp(it,PART_CONTENT_TRANSFER_ENCODING_PATTERN,strlen(PART_CONTENT_TRANSFER_ENCODING_PATTERN))==0) {
				ptemp2 = _parse_header(it);
				cmime_part_set_content_transfer_encoding((*part),ptemp2);
				free(ptemp2);
			}
			
			if (strncasecmp(it,PART_CONTENT_DISPOSITION_PATTERN,strlen(PART_CONTENT_DISPOSITION_PATTERN))==0) {
				ptemp2 = _parse_header(it);
				cmime_part_set_content_disposition((*part),ptemp2);
				free(ptemp2);
			}	
				
			if (strncasecmp(it,PART_CONTENT_ID_PATTERN,strlen(PART_CONTENT_ID_PATTERN))==0) {
				ptemp2 = _parse_header(it);
				cmime_part_set_content_id((*part),ptemp2);
				free(ptemp2);
			}	

			// break if body begins 
			if (strncmp(it,dlb,strlen(dlb))==0)
				break;
				
			it++;
		}
		
		ptemp += strlen(dlb);
		body = (char *)calloc(strlen(ptemp) + 1,sizeof(char));
		strcpy(body,ptemp);
	} else {
		body = (char *)calloc(strlen(content) + 1, sizeof(char));
		strcpy(body,content);
	}
	
	cmime_part_set_content((*part),body);
	free(body);
	free(dlb);
	
	return(0);
}
