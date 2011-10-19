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
#include <assert.h>

#include "../src/cmime_address.h"
#include "../src/cmime_message.h"
#include "../src/cmime_header.h"
#include "../src/cmime_list.h"
#include "../src/cmime_part.h"

#include "test_data.h"

char test_files[54][10] = {
	"m0001.txt","m0002.txt","m0003.txt","m0004.txt","m0005.txt",
	"m0006.txt","m0007.txt","m0008.txt","m0009.txt","m0010.txt",
	"m0011.txt","m0012.txt","m0013.txt","m0014.txt","m0015.txt",
	"m0016.txt","m0017.txt","m0018.txt","m1001.txt","m1002.txt",
	"m1003.txt","m1004.txt","m1005.txt","m1006.txt","m1007.txt",
	"m1008.txt","m1009.txt","m1010.txt","m1011.txt","m1012.txt",
	"m1013.txt","m1014.txt","m1015.txt","m1016.txt","m2001.txt",
	"m2002.txt","m2003.txt","m2004.txt","m2005.txt","m2006.txt",
	"m2007.txt","m2008.txt","m2009.txt","m2010.txt","m2011.txt",
	"m2012.txt","m2013.txt","m2014.txt","m2015.txt","m2016.txt",
	"m3001.txt","m3002.txt","m3003.txt","m3004.txt"
};

int main (int argc, char const *argv[]) {
	CMimeMessage_T *msg = cmime_message_new();
	char *s = NULL;
	char *s2 = NULL;
	char *msg_string = NULL;
	char *fname = NULL;
	CMimeHeader_T *h = NULL;
	CMimeList_T *recipient_list = NULL;
	CMimeListElem_T *elem;
	int retval = 0;
	FILE *fp = NULL;
	FILE *fp2 = NULL;
	long size = 0;
	int i = 0;
	
	cmime_message_set_sender(msg,addr_string1);
	s = cmime_message_get_sender(msg);
	assert(strcmp(s,addr_string1)==0);
	free(s);
	
	// check if sender will be overwritten
	cmime_message_set_sender(msg,addr_string2);
	s = cmime_message_get_sender(msg);
	assert(strcmp(s,addr_string2)==0);
	free(s);
	
	// set message id
	cmime_message_set_message_id(msg,message_id_string);
	s = cmime_message_get_message_id(msg);
	assert(strcmp(s,message_id_string)==0);
	
	// set a test header
	if (cmime_message_set_header(msg, header_string1)!=0) 
		return(-1);
	h = cmime_message_get_header(msg,header_string1_key);
	assert(strcmp(cmime_header_get_value(h,0),header_string1_value)==0);
	
	// overwrite header
	if (cmime_message_set_header(msg, header_string1_2)!=0)
		return(-1);
		
	h = cmime_message_get_header(msg,header_string1_key);
	assert(strcmp(cmime_header_get_value(h,0),header_string2_value)==0);

	// check with lower case key
	h = cmime_message_get_header(msg,header_string1_key_lower);
	assert(strcmp(cmime_header_get_value(h,0),header_string2_value)==0);

	// check recipients
	if (cmime_message_add_recipient(msg,addr_string1,CMIME_ADDRESS_TYPE_TO)!=0)
		return(-1);
	
	if (cmime_message_add_recipient(msg,addr_string2,CMIME_ADDRESS_TYPE_CC)!=0)
		return(-1);
		
	recipient_list = cmime_message_get_recipients(msg);
	assert(recipient_list);

	elem = cmime_list_head(recipient_list);
	while(elem != NULL) {
		s = cmime_address_to_string((CMimeAddress_T *)cmime_list_data(elem));
		assert(s);
		free(s);
		elem = elem->next;
	}
	
	// checking date
	if (cmime_message_set_date_now(msg)!=0)
		return(-1);
		
	s = cmime_message_get_date(msg);
	assert(s);
			
	// check boundary stuff
	s = cmime_message_generate_boundary();
	cmime_message_set_boundary(msg,s);
	s2 = cmime_message_get_boundary(msg);
	assert(strcmp(s,s2)==0);
	
	free(s);
	cmime_message_free(msg);

	for (i=0; i < 54; i++) {
		printf("Checking sample message [%s]...", test_files[i]);
		msg = cmime_message_new();
		asprintf(&fname,"%s/%s",SAMPLES_DIR,test_files[i]);
		retval = cmime_message_from_file(&msg,fname);
		if (retval != 0)
			return retval;
			
		msg_string = cmime_message_to_string(msg);
	
		if ((fp = fopen(fname, "rb")) == NULL) 
			return(-1);
		free(fname);	

		if (fseek(fp, 0, SEEK_END)!=0)
			return(-1);
		
		size = ftell(fp);
		rewind(fp);	
		s = (char*) calloc(sizeof(char), size + 20);
		fread(s, 1, size, fp);
		if(ferror(fp))
			return(-1);
		
		asprintf(&s2,"out_%s",test_files[i]);
		fp2 = fopen(s2,"wb");
		fwrite(msg_string,1,size,fp2);
		fclose(fp2);
		free(s2);
		
		
		printf("S:\n[%s]\n", msg_string);
		assert(strcmp(msg_string,s)==0);
		free(s);
		free(msg_string);
		cmime_message_free(msg);
		printf("passed!\n");
	}
	

	return(0);
}
