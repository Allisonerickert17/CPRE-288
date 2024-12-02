

/**
 * main.c
 */
#include "music.h"
#include "open_interface.h"
int main(void)
{

    oi_t* sensor = oi_alloc();
    oi_init(sensor);

    load_songs();

    oi_play_song(0);


    oi_free(sensor);
    return 0;
}
