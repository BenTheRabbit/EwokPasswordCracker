#include "ui.h"


void ui_thread(void* param)
{
	/*****************/
	/** Declaration **/
	/*****************/
    
    
    ui_thread_parameter *p = (ui_thread_parameter *)param;
    
	WINDOW  *w_server_info, *w_hash, *w_client;
    
	int max_col, max_row;
	int tab_select = TAB_HASH;
	int input;
    char question[100];
    
	/********************/
	/** Initialization **/
	/********************/
    
	
    ui_init();
    
    getmaxyx(stdscr, max_row, max_col);
    w_server_info=newwin(3,max_col,0,0);
    w_hash=newwin(max_row-3,max_col,3,0);
    w_client=newwin(max_row-3,max_col,3,0);
    
    ui_get_int("Choose server port", &p->conf->server_port, 1,65535,max_row,max_col, p->conf->server_port);

	config_set_int(CONF_FILE_NAME, "SERVER_PORT", p->conf->server_port);
    
    pthread_mutex_unlock(&p->mutex[MUTEX_PORT]);
    
	while(1)
	{
		getmaxyx(stdscr, max_row, max_col);
        
        ui_display_server_info(w_server_info, max_col, max_row, p->conf);
		if(tab_select == TAB_HASH)
		{
        	ui_display_hash(w_hash, max_col, max_row, p->hashes);
		}
		else
		{
        	ui_display_client(w_client, max_col, max_row, p->clients);
		}
        
		input = getch();
        
		switch(input)
		{	case 9:
				if(tab_select == TAB_HASH)
				{
					tab_select = TAB_CLIENT;
				}
				else
				{
					tab_select = TAB_HASH;
				}
				break;
		}
		
	}
    
    
	endwin();
    
}


void ui_init()
{
	// Init ncurses
	initscr();
    
	// Intercept all key
	//raw();
    
	keypad(stdscr, TRUE);
    
	halfdelay(10);
    
	curs_set(0);
    
	noecho();
    
	start_color();
    
	init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    
}

void ui_display_server_info(WINDOW *win, int x, int y, config *conf)
{
    char uptime[100];
    ui_format_time(time(NULL)-conf->up_time, uptime);
    wclear(win);
    mvwprintw(win, 1,2,"Version : %s | Port : %d | Uptime : ", VERSION, conf->server_port);
    wattron(win, COLOR_PAIR(2));
    wprintw(win, "%s", uptime);
    wattroff(win, COLOR_PAIR(2));
    box(win, 0, 0);
    wrefresh(win);
    
}


void ui_format_time(long number, char *texte)
{
    int hour=0, minute=0, second=0, day=0;
    
    second=number%60;
    number -= second;
    number= number/60;
    
    minute=number%60;
    number -= minute;
    number = number/60;
    
    hour=number%24;
    number -=hour;
    number = number/24;
    
    day = number;
    
    
    if(day>1)
    {
        sprintf(texte, "%d days, %02d:%02d:%02d", day, hour, minute, second);
    }
    else if(day==1)
    {
        sprintf(texte, "%d day, %02d:%02d:%02d", day, hour, minute, second);
    }
    else
    {
        sprintf(texte, "%02d:%02d:%02d", hour, minute, second);
    }
}

void ui_display_hash(WINDOW *win, int x, int y, list* hash)
{
    int i;
    hash_t *tmp_hash;
    hash_md5 *tmp_md5;
	char tmp_word[100];
    
    wclear(win);
	
	//Print legend
    
	mvwprintw(win, 1, 2, " ID |   TYPE   | PRIORITY |   STATUS   |   CURRENT PASSWORD   |  TIME ELAPSED  ");
    
	for(i = 1; i < x-1; i++)
	{
		mvwprintw(win,2, i, "%c", '-');
	}
    
    
    
    for(i=0; i<list_get_size((void*)hash); i++)
    {
        tmp_hash = (hash_t*)list_get((void*)hash, i);
        tmp_md5 = (hash_md5*)tmp_hash->hash; 
		
        
        
		// Print id
        
		mvwprintw(win, i+3,2," %d", tmp_hash->id);
        
        
        
		// Printf type
        
		switch(tmp_hash->type)
		{
			case HASH_TYPE_MD5:
				strcpy(tmp_word, "MD5");
				break;
		}
        
        mvwprintw(win, i+3,6,"| %s", tmp_word);
		
        
        
		// Print priority
        
        mvwprintw(win, i+3,17,"| %d", tmp_hash->priority);
        
		
		
		// Print status
		
        mvwprintw(win, i+3,28,"| ");
        
		switch(tmp_hash->status)
		{
			case HASH_STATUS_FOUND:
				strcpy(tmp_word, "FOUND");
				wattron(win, COLOR_PAIR(2));
        		mvwprintw(win, i+3,30,"%s", tmp_word);
				wattroff(win, COLOR_PAIR(2));
				break;
                
			case HASH_STATUS_NOT_FOUND:
				strcpy(tmp_word, "SEARCHING");
        		mvwprintw(win, i+3,30,"%s", tmp_word);
				break;
		}
        
        
        
		// Print password
        
		if(tmp_hash->status != HASH_STATUS_FOUND)
		{
			int2word(tmp_hash->next_calc, tmp_md5->charset, strlen(tmp_md5->charset), tmp_word);
        	mvwprintw(win, i+3,41,"| %s", tmp_word);
		}
		else
		{
        	
			mvwprintw(win, i+3,41,"| ");
            
			wattron(win, COLOR_PAIR(2));
        	mvwprintw(win, i+3,43,"%s", tmp_hash->clear_password);
			wattroff(win, COLOR_PAIR(2));
		}
        
        
		// Printf time elapsed
        
		if(tmp_hash->status != HASH_STATUS_FOUND)
		{
			ui_format_time(time(NULL) - tmp_hash->start_time, tmp_word);
		}
		else
		{
			ui_format_time(tmp_hash->end_time - tmp_hash->start_time, tmp_word);
		}
        
		mvwprintw(win, i+3,64,"| %s", tmp_word);
        
        
    }
    
    
    box(win, 0, 0);
    wrefresh(win);
    
    
}

void ui_display_client(WINDOW *win, int x, int y, list* clients)
{
    int i;
    client *tmp_client;
	char tmp_word[100];
    
    wclear(win);
    
    
	// Print legend
    
	mvwprintw(win, 1, 2, "        CLIENT        |    REF IDP    |  TIME  ");
	
	for(i = 1; i < x-1; i++)
	{
		mvwprintw(win,2, i, "%c", '-');
	}
    
    
    
    for(i=0; i<list_get_size((void*)clients); i++)
    {
		
		// Print ip and port
        tmp_client = (client*)list_get((void*)clients, i);
        mvwprintw(win, i+3,2,"%s:%d", tmp_client->ip, tmp_client->port);
        
        
        
		// Print idp
        
		mvwprintw(win, i+3, 24, "| %d", tmp_client->idp[0]);
        
        
        
		// Print time
        
		ui_format_time(time(NULL) - tmp_client->connect_time, tmp_word);
		mvwprintw(win, i+3, 40, "| %s", tmp_word);
        
    }
    
    box(win, 0, 0);
    wrefresh(win);
    
    
}

void ui_get_int(char * question, int * output, int limit_min, int limit_max, int window_size_x, int window_size_y, int default_value)
{
    
	/*****************/
	/** Declaration **/
	/*****************/
    
    int row, col, question_size, tmp_input;
	int condition = 0;
	int input;
    char *tmp_default_value, tmp_test[10];
    
    FIELD *field_int[2];
    FORM *form_int;
    WINDOW *w_form;
   



    
	// Calculate window size
    question_size = strlen(question);
	if (question_size < 10) 
	{
        question_size = 10;
    }
    
    // Init field
    field_int[0] = new_field(1, 10, 2, 2, 0, 0);
    field_int[1] = NULL;
    
    set_field_type(field_int[0], TYPE_INTEGER, 0, limit_min, limit_max);
    
    // Create form
    form_int = new_form(field_int);
    
	// Create window
    w_form = newwin(4, question_size+4, ((window_size_x/2)-2), ((window_size_y/2)-(question_size+4)/2));
    
    // Link and show form
    set_form_sub(form_int, w_form);

    post_form(form_int);
    mvwprintw(w_form, 1,2,question);
    box(w_form,0,0);

    wrefresh(w_form);
    
   	// Set cursos visible and focus on new window
    curs_set(2);
	keypad(w_form, TRUE);
    
   	
	// Set default value
    sprintf(tmp_test, "%d", default_value);
    set_field_buffer(field_int[0], 0,tmp_test);
    
	while(condition == 0)
	{
		input = wgetch(w_form);
        
		switch(input)
		{
            case KEY_LEFT:
                form_driver(form_int, REQ_PREV_CHAR);
                break;
                
            case KEY_RIGHT:
                form_driver(form_int, REQ_NEXT_CHAR);
                break;
                
			// Del caracter	
            case 127:
			case 8:
			case 263:
			case 330:
                form_driver(form_int, REQ_DEL_PREV);
                break;
                
            // Key ENTER
            case 10:

				if(form_driver(form_int, REQ_VALIDATION) == E_OK)
				{
                	tmp_default_value = field_buffer(field_int[0],0);
                	tmp_input = atoi(tmp_default_value);

					*output = tmp_input;
	
					condition = 1;
				}

                break;

			// If timeout end
			case -1:
				break;
            default:
                form_driver(form_int, input);
                break;
                
        }
    }
    
    
    unpost_form(form_int);
    free_form(form_int); 
    free_field(field_int[0]);
    delwin(w_form);
    

	// Reset original parameters
    keypad(stdscr, TRUE);
    curs_set(0);
    
}





