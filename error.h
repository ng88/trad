#ifndef ERROR_H
#define ERROR_H

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
} error_t;



void raise_error(size_t line, error_t code, ...);

char * get_error_msg(error_t code);

#endif
