/***********************************************************
�ļ���	:	p2p_json.c
����		:	Faker
�汾��	:	1.0
����		:	2015.05.13

˵��:
		������json ���ܺ���

***********************************************************/

#include "all.h"






/***************************************
������: printf_t
����:��ӡn ��tab ��
***************************************/

void printf_t(int i)
{
	for(; i > 0; i--){
		printf("\t");
	}
}



/***************************************
������: printf_json
����:��ӡjson����
***************************************/

void printf_json(const char *json_str)
{
	int i = 0;
	while((*json_str) != 0){
		switch((*json_str)){
			case '{':
			{
				i ++;
				printf("{\n");
				printf_t(i);
				break;
			}
			case ',':
			{
				printf(",\n");
				printf_t(i);
				break;
			}
			case '}':
			{
				i --;
				printf("\n");
				printf_t(i);
				printf("}");
				break;
			}
			default:
			{
				printf("%c", (*json_str));
			}
		}
		json_str ++;
	}
	printf("\n");
}



/*
����׼��ʽ�洢json �ļ�
*/

void write_t_to_flie(FILE *fp, int i)
{
	for(; i > 0; i--){
		fputs("\t", fp);
	}
}
int save_json_to_file(const char *json_str, char *path)
{
	FILE *fp;
	int i = 0;
	
	if((fp=fopen(path,"w+"))== NULL) /*����c:\45.CH�ļ�*/
	{ 
		printf("\nopen %s error", path); 
		return -1;
	}
		
	while((*json_str) != 0){
		switch((*json_str)){
			case '{':
			{
				i ++;
				fputs("{\n",fp);
				write_t_to_flie(fp, i);
				break;
			}
			case ',':
			{
				fputs(",\n", fp);
				write_t_to_flie(fp, i);
				break;
			}
			case '}':
			{
				i --;
				fputs("\n", fp);
				write_t_to_flie(fp, i);
				fputs("}", fp);
				break;
			}
			default:
			{
				fputc((*json_str), fp);
			}
		}
		json_str ++;
	}
	fputs("\n", fp);

	fclose(fp);
	
	return 0;
}

/***************************************
������: __json_tokener_parse
����:���ַ���ת����json �ṹ��
***************************************/

int __json_tokener_parse(char *str, struct json_object *new_obj)
{

	new_obj = json_tokener_parse(str);

	if(is_error(new_obj))
	{
//		json_object_put(new_obj);
//		dbg_printf("json_tokener_parse err \n");
		return -1;
	}
	return 0;
}


/***************************************
������: __json_key_to_string
����:����key ��ȡstring
***************************************/

/* ��ʽ���ַ��� */
void __str(char *dest, char *src)
{
	int cnt = 0;
	while((*src) != 0)
	{
		switch((*src))
		{
			case '"':
				cnt ++;
				break;
			case '\\':
			{
				src ++;
				(*dest) = (*src);
				dest ++;
				break;
			}
			default:
			{
				(*dest) = (*src);
				dest ++;
				break;
			}
		}
//		if(cnt >= 2)
//			return ;
		src ++;
	}
	*dest = 0;
}

int __json_key_to_string(struct json_object *new_obj, char *key, char *str, int len)
{
	int ret;
	char tmp_str[len];
	struct json_object *sub1obj;

	sub1obj = json_object_object_get(new_obj, key);

	if(NULL==sub1obj) //�����Ҫ��NULL�ж���, �ò�����Ӧ��object����NULL
	{
//		printf("sub1obj err\n");
		return -1;
	}
	else
	{
		if((strlen(json_object_to_json_string(sub1obj)) + 1) > len)
		{
			ret = -1;
		}else{
//			printf("wocao \r\n");
			__strcpy(tmp_str, json_object_to_json_string(sub1obj), strlen(json_object_to_json_string(sub1obj)) + 1);
#if 0
			strncpy(str, tmp_str + 1, strlen(tmp_str) - 2);
			str[strlen(tmp_str) - 2] = '\0';
#else
//			printf("tmp_str %s \r\n", tmp_str);
			__str(str, tmp_str);
#endif
			ret = 0;
		}
		//json_object_put(sub1obj);	//�����ͷţ���Ϊ json_object_object_get ��û�����·����ڴ�
		return ret;
	}
}



/***************************************
������: __json_key_to_int
����:����key ��ȡint ���͵���ֵ
***************************************/

int __json_key_to_int(struct json_object *new_obj, char *key, int *val)
{
	struct json_object *sub1obj = json_object_object_get(new_obj, key);
	if(NULL==sub1obj) //�����Ҫ��NULL�ж���, �ò�����Ӧ��object����NULL
	{
//		printf("sub1obj err\n");
		return -1;
	}
	else
	{
		*val = json_object_get_int(sub1obj);
		//json_object_put(sub1obj); //�����ͷţ���Ϊ json_object_object_get ��û�����·����ڴ�
		return 0;
	}
}


/*
http://twobit.blog.chinaunix.net/uid-23412956-id-3784975.html

*/



/*
function description:
if the value of input str is : "vlan.algo-table.1.src-ip", the output top_node_buf will be "vlan"
if the value of input str is : "algo-table.1.src-ip", the output top_node_buf will be "algo-table"
*/
int get_top_node(const char *str, char *top_node_buf, int buf_len)
{
    const char *p_start = NULL;
    const char *p_end = NULL;

    if (!str || !top_node_buf)
    {
        return -1;//fail
    }

    p_start = str;
    p_end = strchr(str, '.');

    if (!p_end)
    {
        snprintf(top_node_buf, buf_len, "%s", str);
    }
    else
    {
        int i = 0;
        int len = p_end - p_start;
        
        if ((len) > (buf_len-1))
        {
            return -1;//no enough buffer
        }

        for (i=0; i<len; ++i)
        {
            top_node_buf[i] = p_start[i];
        }
        top_node_buf[i] = '\0';;
    }

    return 0;
}


/*
return key and json object, so we can change the value

only work for json_type_object, for json_type_array, the function will not work
*/
int json_get_obj(struct json_object *obj, const char *str, key_obj* output_obj)
{
    json_type type = json_object_get_type(obj);
    char top_node[128] = {0};
    const char *p = NULL;
    int match = 0;

    if (!obj || !str)
    {
        return -1;
    }

    if (get_top_node(str, top_node, sizeof(top_node)) != 0)
    {
        return -1;
    }

    p = strchr(str, '.');
    //for object
    if (type == json_type_object)
    {
        __json_object_object_foreach(obj, key, val) {
            struct json_object *tmp_obj = val;
            if (!strcmp(top_node, key))
            {
                match = 1;
                //leaf now,return node obj
                if (!p)
                {
                    output_obj->key = key;
                    output_obj->obj = tmp_obj;
                    output_obj->parent_obj = obj;
                    return 0;
                }
                else
                {
                    return json_get_obj(tmp_obj, ++p, output_obj);
                }
                break;
            }
        }

        if (!match)
        {
            return -1;
        }
    }

    return 0;

}


/*
��json�����޸�ָ��·����ֵ
*/

int json_set_value(struct json_object *root_obj, const char *str, const char *val)
{
    key_obj node;
    char key[64] = {0};
    struct json_object *obj = NULL;

    if (json_get_obj(root_obj, str, &node) != 0)
    {
        return -1;
    }

    obj = json_object_new_string(val);
    snprintf(key, sizeof(key), "%s", node.key);
    json_object_object_add(node.parent_obj, key, obj);

    return 0;
}

/* 
��json�����ж�ȡָ��·����ֵ

*/
const char * json_get_value(struct json_object *root_obj, const char *str)
{
    key_obj node;

    if (json_get_obj(root_obj, str, &node) != 0)
    {
        return NULL;
    }

    return (const char *)json_object_get_string(node.obj);
}



/*
��json����ָ��·��������µĶ���

*/

int json_add_json_obj(struct json_object *root_obj, const char *path,
    const char *key, struct json_object *new_obj)
{
    key_obj node;

    if (json_get_obj(root_obj, path, &node) != 0)
    {
        return -1;
    }

    json_object_object_add(node.obj, key, new_obj);

    return 0;
}


/*
��json����ɾ��ָ��·���Ķ���

*/
int json_del_json_obj(struct json_object *root_obj, const char *path)
{
    key_obj node;

    if (json_get_obj(root_obj, path, &node) != 0)
    {
        return -1;
    }

    json_object_object_del(node.parent_obj, node.key);

    return 0;
}


