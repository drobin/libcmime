#include <stdio.h>

#include "../src/cmime_message.h"

int main(int argc, char const *argv[]) {
    CMimeMessage_T *msg = cmime_message_new();
    char *mid = NULL;
    char *out = NULL;

    

    /* set the sender and recipient */
    cmime_message_set_sender(msg,"John Doe <joe@example.org>");
    cmime_message_add_recipient(msg, "Jane Doe <jane@example.org>",CMIME_ADDRESS_TYPE_TO);

    cmime_message_set_subject(msg, "This is an exmaple");
    cmime_message_set_date_now(msg);

    /* generate a message id and add it */
    mid = cmime_message_generate_boundary();
    cmime_message_set_message_id(msg, mid);
    
    cmime_message_set_body(msg,"This is the message body");

    out = cmime_message_to_string(msg);
    printf("%s\n",out);

    /* free all stuff */
    free(out);
    free(mid);
    cmime_message_free(msg);
    return 0;
}
