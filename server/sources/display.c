#include "display.h"





void initDisplay()
{
    initscr();
    //raw();

    start_color();

    //init_pair(1, COLOR_RED, COLOR_BLACK);

    keypad(stdscr, TRUE);

}

void* displayThread(void* tmp_param)
{
    paramDisplayThread *param = NULL;
    param = (paramDisplayThread*)tmp_param;
    WINDOW *config_win;
    int config_win_height, config_win_width, config_win_x, config_win_y;
    int i;

    WINDOW * stat_win;
    int stat_win_height, stat_win_width, stat_win_x, stat_win_y;

    WINDOW * cli_win;
    int cli_win_height, cli_win_width, cli_win_x, cli_win_y;

    long start_tot = 0, stop_tot = 0;
    long start_act = 0, stop_act = 0, act_time = 0, act_time_tmp = 0, act_prev = 0;
    long start_60avg, stop_60avg;


    /****************************/
    /** Display config windows **/
    /****************************/


    config_win_height = 4;
    config_win_width = COLS;
    config_win_x = 0;
    config_win_y = 0;

    config_win = newwin(4, config_win_width, 0, 0);

    wprintw(config_win, "Port : %d\nCharset : %s\nHash : %s\n", "srv_conf.port", "conf.charset", "conf.hash");

    for(i=0; i<config_win_width; i++)
        wprintw(config_win, "-");


    wrefresh(config_win);


    stat_win_height = 5;
    stat_win_width = COLS;
    stat_win_x = 0;
    stat_win_y = 0;

    stat_win = newwin(stat_win_height, stat_win_width, 4, 0);



    cli_win_height = 20;
    cli_win_width = COLS-9;
    cli_win_x = 0;
    cli_win_y = 0;

    cli_win = newwin(cli_win_height, cli_win_width, 9, 0);

    client *cli_tmp;






    char crt_word[20], avg60_str[50];
    mpz_t avg60, last_val, next_calc_tmp;

    mpz_init_set_ui(avg60, 0);
    mpz_init_set_ui(last_val, 0);
    mpz_init_set_ui(next_calc_tmp, 0);

    start_60avg = time(NULL);
    start_tot = time(NULL);

    /*while(1)
    {
        stop_60avg = time(NULL);
        stop_tot = time(NULL);
        if(stop_60avg - start_60avg > 60)
        {
            mpz_set(next_calc_tmp, next_calc);
            mpz_sub(avg60, next_calc_tmp, avg60);
            mpz_tdiv_q_ui(avg60, avg60, 60);
            mpz_get_str(avg60_str, 10, avg60);
            mpz_set(avg60, next_calc_tmp);
            start_60avg = time(NULL);
        }

        if(list_get_size(param->client_list) > 0)
        {
            if(act_prev == 0)
            {
                start_act = time(NULL);
            }
            stop_act = time(NULL);
            act_time_tmp = act_time + (stop_act-start_act);
            act_prev = 1;
        }
        else
        {
            if(act_prev == 1)
            {
                act_time += (stop_act-start_act);
            }
            act_prev = 0;
        }
*/
        /*wmove(stat_win, 0,0);
        int2word(next_calc, conf.charset, conf.count_charset, crt_word);
        wprintw(stat_win, "Current word : %s\nSpeed : %s c/s\n", crt_word, avg60_str);
        wprintw(stat_win, "Total time : %d s\nActive time : %d s\n", stop_tot-start_tot, act_time_tmp);
*/
        /*for(i=0; i<stat_win_width; i++)
            wprintw(stat_win, "-");

        wrefresh(stat_win);

        wmove(cli_win, 0, 0);

        wprintw(cli_win, "Clients : %d ", list_get_size(param->client_list));
        attron(A_BOLD);



        for(i=0; i<list_get_size(param->client_list); i++)
        {
            cli_tmp = list_get(param->client_list, i);
            wprintw(cli_win, "Nb core : %d\n", cli_tmp->idp);
        }

        for(;i<cli_win_height; i++);
        {
            wprintw(cli_win, "\n");
        }

        wrefresh(cli_win);



        sleep(1);
    }
*/





    endwin();


}
