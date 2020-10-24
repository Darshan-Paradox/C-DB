int insert(DB *data_base, char key[], char value[], Type type)
{
    if (get_type_with_key(key, data_base->db) != -1)
    {
        return -1;
    }

    if (verify_update(type, value) != 0)
    {
        return -1;
    }

    char buffer[512];

    fseek(data_base->db, 0, SEEK_END);

    char c;
    switch (type)
    {
    case STRING:
        c = 's';
        break;
    case INT:
        c = 'i';
        break;
    case FLOAT:
        c = 'f';
        break;
    case POINTER:
        c = 'p';
        break;
    case NONE:
        c = '0';
        break;
    default:
        c = 's';
        break;
    }
    sprintf(buffer, "%s%%%c:%s\n", key, c, value);

    fputs(buffer, data_base->db);

    update(data_base);

    return 0;
}

int update_value(DB *data_base, char key[], char value[], Type type)
{
    if (get_type_with_key(key, data_base->db) == -1)
    {
        insert(data_base, key, value, type);
        return 0;
    }

    if (verify_update(type, value) != 0)
    {
        return -1;
    }

    select_row_with_key(key, data_base->db);

    int row_start = ftell(data_base->db);

    char c = fgetc(data_base->db);

    while (c != '%')
    {
        c = fgetc(data_base->db);
    }

    int pos = ftell(data_base->db);
    fseek(data_base->db, pos, SEEK_SET);

    switch (type)
    {
    case STRING:
        fputc('s', data_base->db);
        break;
    case INT:
        fputc('i', data_base->db);
        break;
    case FLOAT:
        fputc('f', data_base->db);
        break;
    case POINTER:
        fputc('p', data_base->db);
        break;
    case NONE:
        fputc('0', data_base->db);
        break;
    default:
        fputc('s', data_base->db);
        break;
    }

    fseek(data_base->db, row_start, SEEK_SET);

    c = fgetc(data_base->db);

    while (c != ':')
    {
        c = fgetc(data_base->db);
    }

    pos = ftell(data_base->db);
    fseek(data_base->db, pos, SEEK_SET);

    fputs(value, data_base->db);

    update(data_base);

    return 0;
}

int delete (DB *data_base, char key[])
{
    if (get_type_with_key(key, data_base->db) == -1)
    {
        printf("key %s doesn't exists\n", key);
        return -1;
    }

    FILE *temp = fopen("./tmp.cdb", "w");

    for (int i = 0; i < row_count(data_base->db); i++)
    {
        select_row(i, data_base->db);
        char buffer[128];

        get_key(buffer, data_base->db);
        if (strcmp(key, buffer) != 0)
        {

            char c = fgetc(data_base->db);

            while (c != '\n')
            {
                fputc(c, temp);
                c = fgetc(data_base->db);
            }

            fputc('\n', temp);
        }
    }

    remove(data_base->name);
    rename("./tmp.cdb", data_base->name);

    fclose(temp);

    update(data_base);
    return 0;
}
