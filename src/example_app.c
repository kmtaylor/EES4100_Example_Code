#include <stdio.h>
#include <modbus-tcp.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char **argv) {
    modbus_t *ctx;
    int option_index, c;

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
                break;
            case 'd':
                printf("Got directive argument\n");
                break;
        }
    }

    ctx = modbus_new_tcp("127.0.0.1", 1502);

    printf("Hello world\n");
    return 0;
}
