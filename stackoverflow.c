#include "stackoverflow-cli.h"

#include <json/json.h>

int searchCommand (stackoverflow_cli_opts *opts) {
    responseObject response;
    char *url = buildUrl("search", 5,
                         "nottagged", opts->nottagged,
                         "tagged", opts->tagged,
                         "intitle", opts->intitle,
                         "pagesize", opts->pagesize,
                         "page", opts->page);

    if (url == NULL)
        return 0;

    makeWebRequest(url, &response);

    if (response.size == 0)
        return 0;

    json_object *jsonDoc = json_tokener_parse(response.data);

    if (jsonDoc == NULL || is_error(jsonDoc)) {
        fprintf(stderr, "Not a valid JSON response?!\n");
        freeWebResponse(&response);
        free(url);
        return 0;
    }

    json_object *jerror = json_object_object_get(jsonDoc, "error");

    if (jerror != NULL) {
        fprintf(stderr, "%s\n", json_object_get_string(json_object_object_get(jerror, "message")));
        freeWebResponse(&response);
        free(url);
        return 0;
    }

    json_object *questions = json_object_object_get(jsonDoc, "questions");

    if (json_object_get_type(questions) != json_type_array) {
        fprintf(stderr, "Questions is expected to be an array!");
        freeWebResponse(&response);
        free(url);
        return 0;
    }

    for (int i = 0; i < json_object_array_length(questions); i++) {
        json_object *question = json_object_array_get_idx(questions, i);

        time_t created_stamp = json_object_get_int(json_object_object_get(question, "creation_date"));
        struct tm *created = gmtime(&created_stamp);

        printf("id: %i || ", json_object_get_int(json_object_object_get(question, "question_id")));
        printf("title: %s || ", json_object_get_string(json_object_object_get(question, "title")));
        printf("answers: %i || ", json_object_get_int(json_object_object_get(question, "answer_count")));
        printf("author: %s || ", json_object_get_string(
                   json_object_object_get(json_object_object_get(question, "owner"), "display_name")));
        printf("up_votes: %i || ", json_object_get_int(json_object_object_get(question, "up_vote_count")));
        printf("down_votes: %i || ", json_object_get_int(json_object_object_get(question, "down_vote_count")));
        printf("created: %i/%i/%i %i:%i:%i || ",
               created->tm_mday,
               created->tm_mon + 1,
               created->tm_year + 1900,
               created->tm_hour + 1,
               created->tm_min + 1,
               created->tm_sec + 1);
        printf("view_count: %i", json_object_get_int(json_object_object_get(question, "view_count")));
        printf("\n");
    }

    freeWebResponse(&response);
    free(url);

    return 1;
}
