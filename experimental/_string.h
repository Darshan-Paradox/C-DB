typedef struct string
{
	int length;
	char str[];
}
string;

string *string_init(char *val)
{
	string *str = malloc(sizeof(string));
	str->length = 0;
	
	while (val[str->length] != '\0')
	{
		str->str[str->length] = val[str->length];
		str->length++;
	}

	return str;
}

void set_string(string *str, char *val)
{
	str->length = 0;
	
	while (val[str->length] != '\0')
	{
		str->str[str->length] = val[str->length];
		str->length++;
	}
	str->str[str->length] = '\0';
}

void string_free(string *str)
{
	free(str);
}
