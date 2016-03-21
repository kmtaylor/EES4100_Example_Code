#include <stdio.h>
#include <stdlib.h>
#include <modbus-tcp.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

struct list_object_s {
    char *string;                   /* 8 bytes */
    int strlen;                     /* 4 bytes */
    struct list_object_s *next;     /* 8 bytes */
};
/* list_head is initialised to NULL on application launch as it is located in 
 * the .bss */
struct list_object_s *list_head;

void add_to_list(char *input) {
    /* Allocate memory */
    struct list_object_s *last_item;
    struct list_object_s *new_item = malloc(sizeof(struct list_object_s));
    if (!new_item) {
        fprintf(stderr, "Malloc failed\n");
        exit(1);
    }

    /* Set up the object */
    new_item->string = strdup(input);
    new_item->strlen = strlen(input);
    new_item->next = NULL;

    if (list_head == NULL) {
        /* Adding the first object */
        list_head = new_item;
    } else {
        /* Adding the nth object */
        last_item = list_head;
        while (last_item->next) last_item = last_item->next;
        last_item->next = new_item;
    }
}

void print_and_free(void) {
    struct list_object_s *old_object, *cur_object;

    cur_object = list_head;

    do {
        old_object = cur_object;
        cur_object = cur_object->next;
        printf("String is: %s\n", old_object->string);
        printf("String length is %i\n", old_object->strlen);
        free(old_object->string);
        free(old_object);
    } while (cur_object);
}

int main(int argc, char **argv) {
    modbus_t *ctx;
    int option_index, c, counter, counter_given = 0;
    char input[256]; /* On the stack */

    struct option long_options[] = {
        { "count",      required_argument,  0, 'c' },
        { "directive",  no_argument,        0, 'd' },
        { 0 }
    };

    while (1) {
        c = getopt_long(argc, argv, "c:d", long_options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 'c':
                printf("Got count argument with value %s\n", optarg);
                counter = atoi(optarg);
                counter_given = 1;
                break;
            case 'd':
                printf("Got directive argument\n");
                break;
        }
    }

    while (scanf("%256s", input) != EOF) {
        /* Add word to the bottom of a linked list */
        add_to_list(input);
        if (counter_given) {
            counter--;
            if (!counter) break;
        }
    }

    /* Print out all items of linked list and free them */
    print_and_free();

    printf("Linked list object is %li bytes long\n",
                    sizeof(struct list_object_s));

    return 0;
}
