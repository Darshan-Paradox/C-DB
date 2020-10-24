#include "core.h"

int row(DB *data_base, int _row)
{
	if (data_base->size > 0)
	{
		if (_row >= 0)
		{
			select_row(_row, data_base->db);
			return -1;
		}
		else
		{
			print_row(data_base->db);
			return row_count(data_base->db);
		}
	}
	return -1;
}

void value(DB *data_base, char buffer[], char key[])
{
	for (int i = 0; i < strlen(key); i++)
	{
		if (isspace(key[i]))
		{
			key[i] = '\0';
		}
	}
	if (strcmp(key, "") == 0)
	{
		if (verify_from_key(data_base->db, key) == 0)
		{
			value_from_key(buffer, key, data_base->db);
		}
	}
	else
	{
		if (verify(data_base->db) == 0)
		{
			get_value(buffer, data_base->db);
		}
	}
}

void key(DB *data_base, char buffer[])
{
	get_key(buffer, data_base->db);
}
