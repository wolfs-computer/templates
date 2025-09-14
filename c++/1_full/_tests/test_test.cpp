#include <gtest/gtest.h>

extern "C" {
#include "ProjectConfig.h"
#include "log_messages.h"
#include "formatter.h"
}



// Test if GTest work properly
TEST(Project, Version) {
    EXPECT_EQ(PROJECT_VERSION, "0.0.1");

    EXPECT_EQ(PROJECT_C_COMPILER, "gcc");
    EXPECT_EQ(PROJECT_CXX_COMPILER, "g++");
}



TEST(log_messages, LogMsg_creation) {
    char *s = strdup("aaa");

    LogMsg *msg = logmsg_create(s, NULL);

    // check msg->message
    EXPECT_EQ(strcmp("aaa", msg->message), 0) << "|" << msg->message << "|";
}


TEST(log_messages, LogMsg_Queue_add) {

    LogMsg_Queue *queue = (LogMsg_Queue*) calloc(1, sizeof(LogMsg_Queue));

    LogMsg *msg1 = logmsg_create(NULL, NULL);
    LogMsg *msg2 = logmsg_create(NULL, NULL);
    LogMsg *msg3 = logmsg_create(NULL, NULL);

    logmsg_add(queue, msg1);
    logmsg_add(queue, msg2);
    logmsg_add(queue, msg3);

    EXPECT_EQ(queue->tail, msg3);
    EXPECT_EQ(queue->head->next, msg2);
    EXPECT_EQ(queue->head, msg1);


    // invalid arguments

    logmsg_add(NULL, msg3);
    logmsg_add(queue, NULL);

    // nothing should be changed
    EXPECT_EQ(queue->tail, msg3);
    EXPECT_EQ(queue->head->next, msg2);
    EXPECT_EQ(queue->head, msg1);
}


TEST(log_messages, LogMsg_Queue_del) {

    LogMsg_Queue *queue = (LogMsg_Queue*) calloc(1, sizeof(LogMsg_Queue));

    LogMsg *msg1 = logmsg_create(NULL, NULL);
    LogMsg *msg2 = logmsg_create(NULL, NULL);
    LogMsg *msg3 = logmsg_create(NULL, NULL);

    logmsg_add(queue, msg1);
    logmsg_add(queue, msg2);
    logmsg_add(queue, msg3);


    // tail
    logmsg_del(queue, msg3);

    EXPECT_EQ(queue->tail, msg2);
    EXPECT_EQ(queue->head, msg1);


    // head
    logmsg_del(queue, msg1);

    EXPECT_EQ(queue->tail, queue->head);
    EXPECT_EQ(queue->tail, msg2);


    // last element
    logmsg_del(queue, msg2);

    EXPECT_EQ(queue->tail, queue->head);
    EXPECT_EQ(queue->tail, (LogMsg*) NULL);


    // middle

    LogMsg *msg_a1 = logmsg_create(NULL, NULL);
    LogMsg *msg_a2 = logmsg_create(NULL, NULL);
    LogMsg *msg_a3 = logmsg_create(NULL, NULL);
    LogMsg *msg_a4 = logmsg_create(NULL, NULL);

    logmsg_add(queue, msg_a1);
    logmsg_add(queue, msg_a2);
    logmsg_add(queue, msg_a3);
    logmsg_add(queue, msg_a4);

    logmsg_del(queue, msg_a3);

    EXPECT_EQ(queue->tail, msg_a4);
    EXPECT_EQ(msg_a2->next, msg_a4);
    EXPECT_EQ(msg_a1->next, msg_a2);
    EXPECT_EQ(queue->head, msg_a1);
    


    // invalid arguments

    LogMsg *msg_other = logmsg_create(strdup("gh for h"), NULL);


    // changes in queue

    LogMsg *tmp_tail = queue->tail;
    LogMsg *tmp_head = queue->head;

    logmsg_del(queue, msg_other);

    EXPECT_EQ(tmp_tail, queue->tail);
    EXPECT_EQ(tmp_head, queue->head);


    // changes in msg

    logmsg_del(NULL, msg_other);

    EXPECT_EQ(strcmp("gh for h", msg_other->message), 0);
}



TEST(formatter, Arg_List_add) {

    Arg_List *list = (Arg_List*) calloc(1, sizeof(Arg_List));

    Arg *arg1 = args_create("d", (union Arg_data) {.d = 86});
    Arg *arg2 = args_create("d", (union Arg_data) {.c = 'i'});
    Arg *arg3 = args_create("d", (union Arg_data) {.d = 99999});

    args_add(list, arg1);
    args_add(list, arg2);
    args_add(list, arg3);

    EXPECT_EQ(list->tail->data.d, 99999);
    EXPECT_EQ(list->tail->prev->data.c, 'i');
    EXPECT_EQ(list->head->data.d, 86);


    // invalid arguments

    args_add(NULL, arg1);
    args_add(list, NULL);

    // nothing changed
    EXPECT_EQ(list->tail->data.d, 99999);
    EXPECT_EQ(list->tail->prev->data.c, 'i');
    EXPECT_EQ(list->head->data.d, 86);
}



TEST(formatter, Arg_List_del) {

    Arg_List *list = (Arg_List*) calloc(1, sizeof(Arg_List));

    Arg *arg1 = args_create("d", (union Arg_data) {.d = 86});
    Arg *arg2 = args_create("d", (union Arg_data) {.c = 'i'});
    Arg *arg3 = args_create("d", (union Arg_data) {.d = 99999});

    args_add(list, arg1);
    args_add(list, arg2);
    args_add(list, arg3);


    // tail
    args_del(list, list->tail);

    EXPECT_EQ(list->tail, arg2);
    EXPECT_EQ(list->head, arg1);


    // content
    EXPECT_EQ(list->tail->data.c, 'i');
    EXPECT_EQ(list->head->data.d, 86);


    // head
    args_del(list, list->head);

    EXPECT_EQ(list->tail, list->head);
    EXPECT_EQ(list->tail, arg2);


    // last element
    args_del(list, arg2);

    EXPECT_EQ(list->tail, list->head);
    EXPECT_EQ(list->tail, (Arg*) NULL);


    // middle
    Arg *arg_a1 = args_create("d", (union Arg_data) {.d = 0});
    Arg *arg_a2 = args_create("d", (union Arg_data) {.d = 0});
    Arg *arg_a3 = args_create("d", (union Arg_data) {.d = 0});
    Arg *arg_a4 = args_create("d", (union Arg_data) {.d = 0});

    args_add(list, arg_a1);
    args_add(list, arg_a2);
    args_add(list, arg_a3);
    args_add(list, arg_a4);

    args_del(list, arg_a3);

    EXPECT_EQ(list->tail, arg_a4);
    EXPECT_EQ(arg_a2->next, arg_a4);
    EXPECT_EQ(arg_a1->next, arg_a2);
    EXPECT_EQ(list->head, arg_a1);
    


    // invalid arguments

    Arg *arg_other = args_create("gh for h", (union Arg_data) {.d = 0});


    // changes in list

    Arg *tmp_tail = list->tail;
    Arg *tmp_head = list->head;

    args_del(list, arg_other);

    EXPECT_EQ(tmp_tail, list->tail);
    EXPECT_EQ(tmp_head, list->head);


    // changes in msg

    args_del(NULL, arg_other);

    EXPECT_EQ(strcmp("gh for h", arg_other->type), 0);
}


Arg_List *test_argumnet_getter(const char *format, ...) {
    va_list args;
    va_start(args, format);

    Arg_List *list = args_to_list(format, args);

    va_end(args);

    return list;
}


TEST(formatter, args_to_list) {

    // one argument

    Arg_List *list = test_argumnet_getter("|%d|", 64);

    // check type
    EXPECT_EQ(strcmp(list->head->type, Arg_types[T_DECIMAL]), 0);
    // check data
    EXPECT_EQ(list->head->data.d, 64);
    //check list head and tail
    EXPECT_EQ(list->head, list->tail);

    // delete list
    while (list->head) args_del(list, list->head);


    // two arguments

    Arg_List *list2 = test_argumnet_getter("|%d| |%d|", 64, 92);

    // check type
    EXPECT_EQ(strcmp(list2->head->type, Arg_types[T_DECIMAL]), 0);
    EXPECT_EQ(strcmp(list2->tail->type, Arg_types[T_DECIMAL]), 0);
    // check data
    EXPECT_EQ(list2->head->data.d, 64);
    EXPECT_EQ(list2->tail->data.d, 92);
    //check list2 head and tail
    EXPECT_NE(list2->head, list2->tail);

    // delete list2
    while (list2->head) args_del(list2, list2->head);


    // two arguments (invalid argument) (wrong type specifier)

    Arg_List *list_i1 = test_argumnet_getter("|%d| |%d|", 64, 'c');

    // check type
    EXPECT_EQ(strcmp(list_i1->head->type, Arg_types[T_DECIMAL]), 0);
    EXPECT_EQ(strcmp(list_i1->tail->type, Arg_types[T_DECIMAL]), 0);
    // check data
    EXPECT_EQ(list_i1->head->data.d, 64);
    EXPECT_EQ(list_i1->tail->data.c, 'c');
    EXPECT_EQ(list_i1->tail->data.d, 99);
    //check list_i1 head and tail
    EXPECT_NE(list_i1->head, list_i1->tail);

    // delete list_i1
    while (list_i1->head) args_del(list_i1, list_i1->head);


    // two arguments (invalid argument) (missing argument)

    Arg_List *list_i2 = test_argumnet_getter("|%d| |%d|", 64);

    // check type
    EXPECT_EQ(strcmp(list_i2->head->type, Arg_types[T_DECIMAL]), 0);
    // check data
    EXPECT_EQ(list_i2->head->data.d, 64);
    //check list_i2 head and tail
    EXPECT_NE(list_i2->head, list_i2->tail);

    // delete list_i2
    while (list_i2->head) args_del(list_i2, list_i2->head);


    // two arguments (invalid argument) (missing argument)

    Arg_List *list_i3 = test_argumnet_getter("|%d| |%d|", 64, 92, 520);

    // check type
    EXPECT_EQ(strcmp(list_i3->head->type, Arg_types[T_DECIMAL]), 0);
    EXPECT_EQ(strcmp(list_i3->tail->type, Arg_types[T_DECIMAL]), 0);
    // check data
    EXPECT_EQ(list_i3->head->data.d, 64);
    EXPECT_EQ(list_i3->tail->data.d, 92);
    //check list_i3 head and tail
    EXPECT_NE(list_i3->head, list_i3->tail);

    // delete list_i3
    while (list_i3->head) args_del(list_i3, list_i3->head);


    // data type specifiers check

    Arg_List *list3 = test_argumnet_getter("|%d| |%c| %%", 64, 'c');

    // check type
    EXPECT_EQ(strcmp(list3->head->type, Arg_types[T_DECIMAL]), 0);
    EXPECT_EQ(strcmp(list3->head->next->type, Arg_types[T_CHAR]), 0);
    // check data
    EXPECT_EQ(list3->head->data.d, 64);
    EXPECT_EQ(list3->head->next->data.c, 'c');
    // check number of elements
    int count = 0;
    for (Arg *tmp = list3->head; tmp; tmp = tmp->next) {
        count++;
    }
    EXPECT_EQ(count, ARG_TYPES_LEN);
    // check tail and head
    EXPECT_NE(list3->head, list3->tail);

    // delete list3
    while (list3->head) args_del(list3, list3->head);



    // invalid arguments

    // too small format string
    Arg_List *list_i4 = test_argumnet_getter("|", 64, 'c');
    EXPECT_EQ(list_i4, (Arg_List*) NULL);

    // no arguments in format string
    Arg_List *list_i5 = test_argumnet_getter("|||||", 64, 'c');
    EXPECT_EQ(list_i5, (Arg_List*) NULL);
}



TEST(formatter, format_string) {

    // decimal

    char *str_d1 = strdup("%d");
    Arg_List *list_d1 = test_argumnet_getter(str_d1, 64);
    int skip_d1 = 0;

    char *out_d1 = format_string(str_d1, list_d1, " ", &skip_d1);
    ASSERT_STREQ(out_d1, "64");
    EXPECT_EQ(skip_d1, 2);


    // char

    char *str_c1 = strdup("%c");
    Arg_List *list_c1 = test_argumnet_getter(str_c1, 'i');
    int skip_c1 = 0;

    char *out_c1 = format_string(str_c1, list_c1, " ", &skip_c1);
    ASSERT_STREQ(out_c1, "i");
    EXPECT_EQ(skip_c1, 2); // because of '%'



    // any white space: \t, \n, ...


    // extra spaces

    char *str_inv1 = strdup("%d   ");
    Arg_List *list_inv1 = test_argumnet_getter(str_inv1, 64);
    int skip_inv1 = 0;

    char *out_inv1 = format_string(str_inv1, list_inv1, " ", &skip_inv1);
    ASSERT_STREQ(out_inv1, "64");
    EXPECT_EQ(skip_inv1, 2);


    // extra spaces

    char *str_inv2 = strdup("%d t  d \tdfddfdf\nd ");
    Arg_List *list_inv2 = test_argumnet_getter(str_inv2, 64);
    int skip_inv2 = 0;

    char *out_inv2 = format_string(str_inv2, list_inv2, " ", &skip_inv2);
    ASSERT_STREQ(out_inv2, "64");
    EXPECT_EQ(skip_inv2, 2);
}



TEST(formatter, get_formatted_string) {

    // only data type

    char *str_o1 = strdup("%d");
    Arg_List *list_o1 = test_argumnet_getter(str_o1, 64);
    char *fstr_o1 = get_formatted_string(str_o1, list_o1);

    ASSERT_STREQ(fstr_o1, "64");


    char *str_o2 = strdup("%c");
    Arg_List *list_o2 = test_argumnet_getter(str_o2, 'i');
    char *fstr_o2 = get_formatted_string(str_o2, list_o2);

    ASSERT_STREQ(fstr_o2, "i");



    // all data types

    char *str_a = strdup("%d %c %d");
    Arg_List *list_a = test_argumnet_getter(str_a, 64, 'c', 9999);
    char *fstr_a = get_formatted_string(str_a, list_a);

    ASSERT_STREQ(fstr_a, "64 c 9999");


    // additional characters

    char *str_char1 = strdup("%d gh for h");
    Arg_List *list_char1 = test_argumnet_getter(str_char1, 64);
    char *fstr_char1 = get_formatted_string(str_char1, list_char1);

    ASSERT_STREQ(fstr_char1, "64 gh for h");


    // a lot of additional characters

    char *str_char2 = strdup("%d gh for h pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp");
    Arg_List *list_char2 = test_argumnet_getter(str_char2, 64);
    char *fstr_char2 = get_formatted_string(str_char2, list_char2);

    ASSERT_STREQ(fstr_char2, "64 gh for h pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp");


    // no data specifiers

    char *str_char3 = strdup("gh for h");
    Arg_List *list_char3 = test_argumnet_getter(str_char3);
    char *fstr_char3 = get_formatted_string(str_char3, list_char3);

    ASSERT_STREQ(fstr_char3, "gh for h");


    // no text

    char *str_char4 = strdup("");
    Arg_List *list_char4 = test_argumnet_getter(str_char4);
    char *fstr_char4 = get_formatted_string(str_char4, list_char4);

    ASSERT_STREQ(fstr_char4, "");


}
