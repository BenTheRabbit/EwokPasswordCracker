#include "ui.h"


/*****************************************************/
/** Request an integer with a question in parameter **/
/*****************************************************/
 
 

int ui_input_int(char* question, int default_value, int min_int, int max_int)
{
    
    char tmp_in[100];
    int value;
    
    do
	{
        
		printf("%s [%d] : ", question, default_value);
		if(fgets(tmp_in, 100, stdin) == NULL)
		{
			continue;
		}
        value = atoi(tmp_in);
		
		if(strlen(tmp_in) == 1 && (default_value>=min_int && default_value<=max_int))
		{
			return default_value;
		}
		else if((value>=min_int) && (value<=max_int))
		{
			return value;
		}
		
	}
	while(1);
}


/*****************************************************/
/** Request an string with a question in parameter **/
/*****************************************************/

int ui_input_string(char* question, char* output, char* default_value)
{
    char tmp_in[100];
	char *pchar;
    
    printf("%s [%s] : ", question, default_value);
    
    if(fgets(tmp_in, 100, stdin) == NULL)
	{
		return 0;
	}
	
	if(strlen(tmp_in) != 1)
	{
		strcpy(output, tmp_in);
	}
	else
	{
		strcpy(output, default_value);
	}

	pchar = strrchr(output, '\n');
	if (pchar) 
	{
	    *pchar = '\0';
	}
    
    return 1;
}


char ui_multi_choice_question(char* question, char* possible, char default_value)
{
	char input;
	char tmp[100];
	int i;

	do
	{
		printf("%s [", question);

		for(i = 0; possible[i] != '\0'; i++)
		{
			if(possible[i] == default_value)
			{
				printf("%c", possible[i]-32);
			}
			else
			{
				printf("%c", possible[i]);
			}
		}
		printf("] : ");
		
		if(fgets(tmp, 100, stdin) == NULL)
		{
			return -1;
		}

		input = tmp[0];
		fflush(stdin);

		if(strlen(tmp) == 1)
		{
			return default_value;
		}
		else
		{
			for(i = 0; possible[i] != '\0'; i++)
			{
				if((input == possible[i]) || (input == (possible[i]-32)))
				{
					return possible[i];
				}
			}
		}
	

	}
	while(1);

	return default_value;
}


