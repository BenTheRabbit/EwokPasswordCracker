#include "config.h"


void config_get(char *file_path, config *conf)
{
    /***********************************************/
	/** Initialization of configuration variables **/
	/***********************************************/
    
    conf->server_port=36000;
    
	/************************/
	/** Read existing file **/
	/************************/
    
    conf->server_port = config_get_int(file_path, "SERVER_PORT");
    
	/************************/
	/** Confirm parameters **/
	/************************/
    
    //conf->server_port = ui_input_int("Server port", conf->server_port, 1, 65535);
    
	/*******************************/
	/** Write preferences in file **/
	/*******************************/
    
    remove(file_path);
    
	config_set_int(file_path,"SERVER_PORT", conf->server_port);
    
    
    
}



/**********************************************/
/** Get a string from the configuration file **/
/**********************************************/



void config_get_string(char* file_path, char* search, char* output)
{
    char tmp1[30], tmp2[30];
    
    FILE * file = NULL;
    
    file = fopen(file_path, "r+");
    
    if(file != NULL)
    {
        while(fscanf(file, "%s %s", tmp1, tmp2) != EOF)
        {
            
            if(!strcmp(search, tmp1))
            {
                strcpy(output, tmp2);
            }
            
        }
        
        fclose(file);
        
    }
}

/********************************************/
/** Get an int from the configuration file **/
/********************************************/    


int config_get_int(char* file_path, char* search)
{
    char tmp1[30], tmp2[30];
    
    FILE * file = NULL;
    
    file = fopen(file_path, "r+");
    
    if(file != NULL)
    {
        while(fscanf(file, "%s %s", tmp1, tmp2) != EOF)
        {
            if(!strcmp(search, tmp1))
            {
                return atoi(tmp2);
            }
            
        }
        
        fclose(file);
        
        
    }
    
    return 0;
}

void config_set_int(char* file_path, char* word, int value)
{
    FILE* file = NULL;
	FILE* file_tmp = NULL;
    char tmp1[30], tmp2[30];
    file = fopen(file_path, "r");
	file_tmp = fopen("conf.tmp", "w+");
	
    
    if((file != NULL))
    {
        while(fscanf(file, "%s %s", tmp1, tmp2) != EOF)
        {
            if(strcmp(word, tmp1))
            {
                fprintf(file_tmp, "%s %s\n", tmp1, tmp2);
            }
        }
        
        
        fclose(file);
	}
    fprintf(file_tmp, "%s %d\n", word, value);
    
	fclose(file_tmp);
    
    remove(file_path);
    rename("conf.tmp", file_path);
    
    
    
}


void config_set_string(char* file_path, char* word, char* value)
{
    FILE* file = NULL;
	FILE* file_tmp = NULL;
    char tmp1[30], tmp2[30];
    file = fopen(file_path, "r");
	file_tmp = fopen("conf.tmp", "w+");
	
    
    if((file != NULL))
    {
        while(fscanf(file, "%s %s", tmp1, tmp2) != EOF)
        {
            if(strcmp(word, tmp1))
            {
                fprintf(file_tmp, "%s %s\n", tmp1, tmp2);
            }
        }
        
        
        fclose(file);
	}
    fprintf(file_tmp, "%s %s\n", word, value);
    
	fclose(file_tmp);
    
    remove(file_path);
    rename("conf.tmp", file_path);
    
    
}




