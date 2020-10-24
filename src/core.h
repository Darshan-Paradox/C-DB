typedef enum
{
	STRING,
	INT,
	FLOAT,
	POINTER,
	NONE
} Type;

int row_count(FILE *db);
void select_row(int row, FILE *db);			 //O(n = number of characters before given row)
void print_row(FILE *db);					 //O(n = KEY+TYPE+VALUE+1)
int get_type(FILE *db);						 //O(n = length of KEY + 1)
int get_type_with_key(char key[], FILE *db); //O(n = length of KEY + 1)
void get_key(char buffer[], FILE *db);
void get_value(char buffer[], FILE *db);
void value_from_key(char buffer[], char key[], FILE *db);
int verify_from_key(FILE *db, char key[]);
int verify(FILE *db);
int verify_update(Type type, char value[]);

//gets type of value in selected row
int get_type(FILE *db) //O(n = length of KEY + 1)
{
	int init_file_ptr = ftell(db);

	char c = fgetc(db);

	while (c != '%')
	{
		c = fgetc(db);
	}

	c = fgetc(db);

	fseek(db, init_file_ptr, SEEK_SET);

	switch (c)
	{
	case 's':
		return STRING;
	case 'i':
		return INT;
	case 'f':
		return FLOAT;
	case 'p':
		return POINTER;
	case '0':
		return NONE;
	default:
		printf("Cannot infer type.");
		return -1;
	}
}

//gets type of value using given key
int get_type_with_key(char key[], FILE *db) //O(n = length of KEY + 1)
{
	int init_file_ptr = ftell(db);

	for (int i = 0; i < row_count(db); i++)
	{
		select_row(i, db);
		char key_buffer[64];
		get_key(key_buffer, db);

		if (strcmp(key_buffer, key) == 0)
		{
			return get_type(db);
		}
	}

	fseek(db, init_file_ptr, SEEK_SET);
	return -1;
}

//selects a row by number (starts with 0)
void select_row(int row, FILE *db) //O(n = number of characters before given row)
{
	fseek(db, 0, SEEK_SET);

	for (int i = 0; i < row; i++)
	{
		char c = fgetc(db);

		while (c != '\n')
		{
			c = fgetc(db);
		}
	}
}

//selects row with given key
int select_row_with_key(char key[], FILE *db) //O(n = length of KEY + 1)
{
	int init_file_ptr = ftell(db);

	for (int i = 0; i < row_count(db); i++)
	{
		select_row(i, db);
		char key_buffer[64];
		get_key(key_buffer, db);

		if (strcmp(key_buffer, key) == 0)
		{
			return 0;
		}
	}

	fseek(db, init_file_ptr, SEEK_SET);
	return -1;
}

//counts total number of rows
int row_count(FILE *db)
{
	int init_file_ptr = ftell(db);
	fseek(db, 0, SEEK_SET);

	int count = 0;
	char c = fgetc(db);
	while (c != EOF)
	{
		if (c == '\n')
		{
			count++;
		}
		c = fgetc(db);
	}
	fseek(db, init_file_ptr, SEEK_SET);
	return count;
}

//prints currently selected row
void print_row(FILE *db) //O(n = KEY+TYPE+VALUE+1)
{

	int init_file_ptr = ftell(db);

	char c = fgetc(db);

	while ((c != '\n'))
	{
		printf("%c", c);

		c = fgetc(db);
	}

	printf("\n");
	fseek(db, init_file_ptr, SEEK_SET);
}

//gets the key of currently selected row
void get_key(char buffer[], FILE *db)
{
	int init_file_ptr = ftell(db);
	int buff_ptr = 0;

	buffer[buff_ptr] = fgetc(db);

	while (buffer[buff_ptr] != '%')
	{
		//printf("%c", buffer[buff_ptr]);
		buffer[++buff_ptr] = fgetc(db);
	}
	buffer[buff_ptr] = '\0';
	fseek(db, init_file_ptr, SEEK_SET);
}

//gets value from currently selected row
void get_value(char buffer[], FILE *db)
{
	int init_file_ptr = ftell(db);
	char c = fgetc(db);

	while (c != ':')
	{
		c = fgetc(db);
	}

	c = fgetc(db);

	int buff_ptr = 0;
	while (c != '\n')
	{
		buffer[buff_ptr] = c;
		buff_ptr++;
		c = fgetc(db);
	}
	buffer[buff_ptr] = '\0';

	fseek(db, init_file_ptr, SEEK_SET);
}

//gets value from given key
void value_from_key(char buffer[], char key[], FILE *db)
{
	int init_file_ptr = ftell(db);

	for (int i = 0; i < row_count(db); i++)
	{
		select_row(i, db);
		char key_buffer[64];
		get_key(key_buffer, db);

		if (strcmp(key_buffer, key) == 0)
		{
			get_value(buffer, db);
			break;
		}
	}

	fseek(db, init_file_ptr, SEEK_SET);
}

int verify(FILE *db)
{
	Type type = get_type(db);

	char buffer[128];
	get_value(buffer, db);

	if (type == STRING)
	{
		int buff_ptr = 0;
		while (buffer[buff_ptr] != '\0')
		{
			if (isalpha(buffer[buff_ptr]) != 0)
			{
				return 0;
			}
			buff_ptr++;
		}
	}

	if (type == INT)
	{
		int buff_ptr = 0;

		while (buffer[buff_ptr] != '\0')
		{
			if (isdigit(buffer[buff_ptr]) == 0)
			{
				return -1;
			}
			buff_ptr++;
		}
	}

	if (type == FLOAT)
	{
		int buff_ptr = 0;

		while (buffer[buff_ptr] != '\0')
		{
			if ((isdigit(buffer[buff_ptr]) == 0) || (buffer[buff_ptr] != '.'))
			{
				return -1;
			}
			buff_ptr++;
		}
	}

	if (type == NONE)
	{
		if (buffer[0] != '\0')
		{
			return -1;
		}
	}

	return 0;
}

int verify_from_key(FILE *db, char key[])
{
	Type type = get_type_with_key(key, db);

	char buffer[128];
	value_from_key(buffer, key, db);

	if (type == STRING)
	{
		int buff_ptr = 0;
		while (buffer[buff_ptr] != '\0')
		{
			if (isalpha(buffer[buff_ptr]) != 0)
			{
				return 0;
			}
			buff_ptr++;
		}
	}

	if (type == INT)
	{
		int buff_ptr = 0;

		while (buffer[buff_ptr] != '\0')
		{
			if (isdigit(buffer[buff_ptr]) == 0)
			{
				return -1;
			}
			buff_ptr++;
		}
	}

	if (type == FLOAT)
	{
		int buff_ptr = 0;

		while (buffer[buff_ptr] != '\0')
		{
			if ((isdigit(buffer[buff_ptr]) == 0) || (buffer[buff_ptr] != '.'))
			{
				return -1;
			}
			buff_ptr++;
		}
	}

	if (type == NONE)
	{
		if (buffer[0] != '\0')
		{
			return -1;
		}
	}

	return 0;
}

int verify_update(Type type, char value[])
{

	if (type == STRING)
	{
		int buff_ptr = 0;
		while (value[buff_ptr] != '\0')
		{
			if (isalpha(value[buff_ptr]) != 0)
			{
				return 0;
			}
			buff_ptr++;
		}
	}

	if (type == INT)
	{
		int buff_ptr = 0;

		while (value[buff_ptr] != '\0')
		{
			if (isdigit(value[buff_ptr]) == 0)
			{
				return -1;
			}
			buff_ptr++;
		}
	}

	if (type == FLOAT)
	{
		int buff_ptr = 0;

		while (value[buff_ptr] != '\0')
		{
			if ((isdigit(value[buff_ptr]) == 0) || (value[buff_ptr] != '.'))
			{
				return -1;
			}
			buff_ptr++;
		}
	}

	if (type == NONE)
	{
		if (value[0] != '\0')
		{
			return -1;
		}
	}

	return 0;
}
