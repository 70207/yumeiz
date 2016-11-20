##yumeiz

yumeiz is a http server, it can deal with file request and dynamic busi.

you can use it just like the code below:
```c
int main(int argc, char *const *argv)
{
    yumei_signal_init();
    yumei_listen_init();
    yumei_event_main_init();

    yumei_listen_append_c( "0.0.0.0", 80, 1 );


    yumei_update_timer();

    yumei_event_create_events();
    yumei_event_create_threads();


    while(1)
    {
        yumei_update_timer();
        usleep(1000);
    }

    return 0;
    

}
```