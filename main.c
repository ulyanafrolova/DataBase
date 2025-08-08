#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    char* buffer;
    size_t bufferlength;
    ssize_t input_length;
} InputBuffer;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
    StatementType type;
}Statement;

InputBuffer* newinputbuffer() {
    InputBuffer* inputbuffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    inputbuffer->buffer = NULL;
    inputbuffer->bufferlength = 0;
    inputbuffer->input_length = 0;
    return inputbuffer;
}

void printprompt() {
    printf(">>>");
}

void readinput(InputBuffer* inputbuffer) {
    ssize_t readbytes = getline(&(inputbuffer->buffer), &(inputbuffer->bufferlength), stdin);
    if (readbytes <= 0) {
        printf("Error reading input. \n");
        exit(EXIT_FAILURE);
    }
    inputbuffer->input_length = readbytes-1;
    inputbuffer->buffer[readbytes-1] = 0;
}

void closeinputbuffer(InputBuffer* inputbuffer) {
    free(inputbuffer->buffer);
    free(inputbuffer);
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

MetaCommandResult do_metacommand(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        closeinputbuffer(input_buffer);
        exit(EXIT_SUCCESS);
    }
    else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

void execute_statement(Statement* statement) {
  switch (statement->type) {
    case (STATEMENT_INSERT):
        printf("insert.\n"); // to add later
        break;
    case (STATEMENT_SELECT):
        printf("select\n"); // to add later
        break;
  }
}

int main () {
    InputBuffer* inputbuffer = newinputbuffer();
    while (true) {
        printprompt();
        readinput(inputbuffer);
        
        if (inputbuffer->buffer[0] == '.') {
            switch(do_metacommand(inputbuffer)) {
                case(META_COMMAND_SUCCESS): continue;
                case(META_COMMAND_UNRECOGNIZED_COMMAND): 
                    printf("Unrecognized command '%s'\n", inputbuffer->buffer);
                    continue;
            }
        }
        Statement statement;
        switch(prepare_statement(inputbuffer, &statement)) {
            case(PREPARE_SUCCESS): break;
            case(PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n", inputbuffer->buffer);
                continue;
        }
        execute_statement(&statement);
        printf("Executed successfully.\n");
    }
}