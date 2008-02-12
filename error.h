#ifndef ERROR_H
#define ERROR_H

#define UNKNOWN_LINE -1


typedef enum
{
    ET_BAD_STRING,
    ET_IDF_TOO_LONG,
    ET_SYNTAX_ERROR,

    ET_TYPE_MISMATCH,
    ET_CLASS_NOT_FOUND,
    ET_FUNC_NOT_FOUND,
    ET_CTOR_NOT_FOUND,
    ET_NO_CTOR_FOUND,
    ET_CTOR_BAD_NAME,

    ET_MAIN_AMBIGUITY,
    ET_MAIN_NOT_FOUND,

    ET_CLASS_ALREADY_EXISTS,
    ET_FUNC_ALREADY_EXISTS,
    ET_FIELD_ALREADY_EXISTS,
    ET_LVAR_ALREADY_EXISTS,
    ET_CTOR_ALREADY_EXISTS,

    ET_RETURN_IN_PROC,
    ET_NO_RETURN_IN_FUNC,
} error_t;



void raise_error(error_t code, ...);

char * get_error_msg(error_t code);

#endif
