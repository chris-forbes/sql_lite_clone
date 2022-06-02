#ifndef ROW_H
#define ROW_H
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255


typedef struct {
	uint32_t id;
	char username [COLUMN_USERNAME_SIZE + 1];
	char email [COLUMN_EMAIL_SIZE + 1];
} Row;
#endif


