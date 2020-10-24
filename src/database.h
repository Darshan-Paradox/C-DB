typedef struct DB
{
	FILE *db;
	int size;
	char name[256];
} DB;

int size(DB *db)
{
	fseek(db->db, 0, SEEK_END);
	int size_file = ftell(db->db);
	rewind(db->db);
	return size_file;
}

DB *init(char file_name[])
{
	DB *db = malloc(sizeof(DB));

	db->db = fopen(file_name, "r+");
	db->size = size(db);
	strcpy(db->name, file_name);

	return db;
}

void update(DB *db)
{
	fclose(db->db);
	db->db = fopen(db->name, "r+");
	db->size = size(db);
}

void close_db(DB *db)
{
	fclose(db->db);
	free(db);
}
