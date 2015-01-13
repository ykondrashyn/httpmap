#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <jansson.h>
#include <curl/curl.h>

#define BUFFER_SIZE (32 * 1024) /* 32 KB */
#define URL_FORMAT   "http://rest.db.ripe.net/search.json?query-string=%s" /* ripe-db in json format */
#define URL_SIZE     256

typedef int bool;
enum { 
    false, true 
};

struct write_result {
    char *data;
    int pos;
};

static size_t write_response(void *ptr, size_t size, size_t nmemb, void *stream) {
    struct write_result *result = (struct write_result *)stream;
    if(result->pos + size * nmemb >= BUFFER_SIZE - 1) {
        fprintf(stderr, "error: too small buffer\n");
        return 0;
    }
    memcpy(result->data + result->pos, ptr, size * nmemb);
    result->pos += size * nmemb;
    return size * nmemb;
}

static char *request(const char *url) {
    CURL *curl = NULL;
    CURLcode status;
    struct curl_slist *headers = NULL;
    char *data = NULL;
    long code;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(!curl)
        goto error;

    data = malloc(BUFFER_SIZE);
    if(!data)
        goto error;

    struct write_result write_result = {
        .data = data,
        .pos = 0
    };

    curl_easy_setopt(curl, CURLOPT_URL, url);

    headers = curl_slist_append(headers, "User-Agent: dcbip");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);

    status = curl_easy_perform(curl);
    if(status != 0) {
        fprintf(stderr, "error: unable to request data from %s:\n", url);
        fprintf(stderr, "%s\n", curl_easy_strerror(status));
        goto error;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    if(code != 200) {
        fprintf(stderr, "error: server responded with code %ld\n", code);
        goto error;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    curl_global_cleanup();

    /* zero-terminate the result */
    data[write_result.pos] = '\0';

    return data;

error:
    if(data)
        free(data);
    if(curl)
        curl_easy_cleanup(curl);
    if(headers)
        curl_slist_free_all(headers);
    curl_global_cleanup();
    return NULL;
}

int main(int argc, char *argv[]) {
    FILE *fd;
    size_t i,j;
    char *text;
    char url[URL_SIZE];
    json_t *root, *objects_, *object_arr, *data, *attributes_, *attributes_arr, *target, *value;
    char *strval;
    bool found;
    json_error_t error;

    if(argc != 3) {
        fprintf(stderr, "usage: %s ip city\n\n", argv[0]);
        fprintf(stderr, "Creates file with ip's by selected city.\n\n");
        return 2;
    }

    strval = NULL;
    found = false;

    snprintf(url, URL_SIZE, URL_FORMAT, argv[1]);

    text = request(url);
    if(!text)
        return 1;

    root = json_loads(text, 0, &error);
    free(text);

    if(!root) {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return 1;
    }

    if(!json_is_object(root)) {
        fprintf(stderr, "error: root is not an object\n");
        json_decref(root);
        return 1;
    }

     objects_ = json_object_get(root, "objects");
     if(!json_is_object(objects_))
     {
         fprintf(stderr, "is not an object\n", (int)(i + 1));
         json_decref(objects_);
         return 1;
     }

    object_arr = json_object_get(objects_, "object");
    if(!json_is_array(object_arr)) {
        fprintf(stderr, "error: object_arr is not an array\n");
        json_decref(object_arr);
        return 1;
    }

    for(i = 0; i < json_array_size(object_arr); i++) {
        if(found == true)
            break;
        data = json_array_get(object_arr, i);
        if(!json_is_object(data))
            fprintf(stderr, "error: data %d is not an object\n", (int)(i + 1));
        attributes_ = json_object_get(data, "attributes");
        if(!json_is_object(attributes_)) {
            fprintf(stderr, "is not an object\n", (int)(i + 1));
            json_decref(attributes_);
            return 1;
        }

        attributes_arr = json_object_get(attributes_, "attribute");
        if(!json_is_array(attributes_arr)) {
            fprintf(stderr, "error: attributes_arr is not an array\n");
            json_decref(attributes_arr);
            return 1;
        }

        for(j = 0; j < json_array_size(attributes_arr); j++) {
            target = json_array_get(attributes_arr, j);
            if(!json_is_object(target))
                fprintf(stderr, "error: data %d is not an object\n", (int)(i + 1));
            value = json_object_get(target, "value");
            strval = strstr(json_string_value(value), argv[2]);
            if(strval) {
                found = true;
                break;
            }
        }
    }

    if(strval) {
        printf("Found value '%s' in %s\nATTR: %s\n", argv[2], argv[1], strval);
        fd = fopen("log.txt","w");
        if(fd != NULL) {
            fputs (argv[1], fd);
            fputc ('\n', fd);
            fclose (fd);
        }
        else
            fprintf(stderr, "Error opening file");

    }
    json_decref(root);
    return 0;
}
