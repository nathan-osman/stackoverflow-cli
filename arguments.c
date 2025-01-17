#include "stackoverflow-cli.h"

#include <getopt.h>

void printUsage (const char *name, const char *msg) {
    if (msg != NULL)
        fprintf(stderr, "%s\n", msg);

    printf("Usage: %s <operation> [options...]\n\n", name);
    printf("Operations has to be one of the following:\n\n");
    printf("Search:\n");
    printf("\t--search\t\tSpecies this operation.\n");
    printf("\t--intitle <string>\tA string that must appear verbatim in the title.\n");
    printf("\t--tagged <string>\tList of tags delimited by semicolons at least one has to be on the questions.\n");
    printf("\t--nottagged <string>\tList of tags delimited by semicolons that must not be in the question.\n");
    printf("\t--intitle <string>\tA string that must appear verbatim in the title.\n");
    printf("\n");
    printf("Generic parameters:\n\n");
    printf("\t--pagesize <size>\tHow many items per page? 100 maximum.\n");
    printf("\t--page <page>\t\tWhat page do you want to see?\n");
    printf("\n");
}

void processArguments (int argc, char **argv, stackoverflow_cli_opts *opts) {
    static const struct option getopt_long_opts[] = {
        { "search",    no_argument,       NULL, 'S' },
        { "nottagged", required_argument, NULL, 'n' },
        { "tagged",    required_argument, NULL, 'g' },
        { "intitle",   required_argument, NULL, 't' },
        { "pagesize",  required_argument, NULL, 's' },
        { "page",      required_argument, NULL, 'p' },
        { NULL, no_argument, NULL, 0 }
    };

    char *optstring = "Sn:g:t:s:p:h?";

    memset(opts, 0, sizeof(stackoverflow_cli_opts));

    /* Some sane defaults */
    opts->operation = none;

    for (;;) {
        int opt_index = 0;
        int opt = getopt_long(argc, argv, optstring, getopt_long_opts, &opt_index);

        if (opt == -1)
            break;

        switch (opt) {
        case 'S':
            opts->operation = search;
            break;
        case 'n':
            opts->nottagged = optarg;
            break;
        case 'g':
            opts->tagged = optarg;
            break;
        case 't':
            opts->intitle = optarg;
            break;
        case 's':
            opts->pagesize = optarg;
            break;
        case 'p':
            opts->page = optarg;
            break;
        case 'h':
        case '?':
        default:
            break;

        }
    }

    if (opts->operation == none) {
        printUsage(argv[0], NULL);
        return;
    }
}
