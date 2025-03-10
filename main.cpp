#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "rng.h"

int width = 100;
int height = 100;
int red_random = 1;

int map[10][10] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,1,0,ra(0,4),1,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,1},
    {1,0,ra(0,1),0,ra(0,1),1,0,0,1,1},
    {1,0,0,0,0,ra(0,4),0,0,1,1},
    {1,0,ra(0,1),0,0,1,0,0,1,1},
    {1,0,1,1,0,ra(0,1),0,0,1,1},
    {1,0,0,ra(0,1),0,1,0,0,1,1},
    {1,0,1,1,0,1,0,0,1,1},
    {1,1,1,1,1,1,1,1,1,1}
}; //randomized map

int main()
{
    al_init();
    al_init_image_addon();
    al_init_acodec_addon();

    al_install_keyboard();
    al_install_audio();

    al_reserve_samples(8);

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(width, height);
    al_set_display_flag(disp, ALLEGRO_MAXIMIZED, true);
    //ahhhh so many
    ALLEGRO_BITMAP* carpet = al_load_bitmap("carpet.png");
    ALLEGRO_BITMAP* wall = al_load_bitmap("wall.png");
    ALLEGRO_BITMAP* player = al_load_bitmap("front.png");
    ALLEGRO_BITMAP* menu = al_load_bitmap("menu.png");
    ALLEGRO_BITMAP* door = al_load_bitmap("door.png");
    ALLEGRO_BITMAP* painting = al_load_bitmap("painting1.png");
    ALLEGRO_BITMAP* painting2 = al_load_bitmap("painting2.png");
    ALLEGRO_BITMAP* painting3 = al_load_bitmap("painting3.png");
    ALLEGRO_BITMAP* redface = al_load_bitmap("blank.png");
    ALLEGRO_SAMPLE* dooropen = al_load_sample("dooropen.wav");
    ALLEGRO_SAMPLE* paintchange = al_load_sample("paintchange.wav");
    ALLEGRO_SAMPLE* start = al_load_sample("gamestart.wav");

    int playerX = 80;
    int playerY = 10;

    int redX = 10;
    int redY = 80;

    int health = 100;

    bool playing = false;

    ALLEGRO_BITMAP* textures[5] = {
        carpet,
        wall,
        painting,
        painting2,
        painting3
    };

    int texture_ran = ra(1, 15);

    if (texture_ran == 7)
    {
        textures[0] = al_load_bitmap("carpetblue.png");
        textures[1] = al_load_bitmap("walldark.png");
    }
    else if (texture_ran == 8)
    {
        textures[1] = al_load_bitmap("chaos.png");
    }
    else if (texture_ran == 9)
    {
        textures[0] = al_load_bitmap("concrete.png");
        textures[1] = al_load_bitmap("concrete2.png");
    }
    else if (texture_ran == 10)
    {
        textures[0] = al_load_bitmap("darkness.png");
        textures[1] = al_load_bitmap("lightness.png");
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);

        //monster controls
        if (red_random == 10 && playing == true)
        {
            if (playerX == redX && playerY == redY && health > 0)
            {
                health -= 5;
            }
            int randomX = ra(1, 8);
            int randomY = ra(1, 8);
            redX = (randomX * 10);
            redY = (randomY * 10);
            al_play_sample(paintchange, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
            if (map[randomX][randomY] == 1)
            {
                map[randomX][randomY] = 4;
            }
            else if (map[randomX][randomY] == 2)
            {
                map[randomX][randomY] = 3;
            }
            else if (map[randomX][randomY] == 4)
            {
                map[randomX][randomY] = 2;
            }
            else if (map[randomX][randomY] == 3)
            {
                map[randomX][randomY] = 1;
            }
        }

        //player & game controls
        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            // game logic goes here.
            redraw = true;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                done = true;
            if (playing == true)
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_W && map[(playerX / 10)][((playerY / 10) - 1)] == 0)
                {
                    playerY -= 10;
                    player = al_load_bitmap("back.png");
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_S && map[(playerX / 10)][((playerY / 10) + 1)] == 0)
                {
                    playerY += 10;
                    player = al_load_bitmap("front.png");
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_D && map[((playerX / 10) + 1)][(playerY / 10)] == 0)
                {
                    playerX += 10;
                    player = al_load_bitmap("right.png");
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_A && map[((playerX / 10) - 1)][(playerY / 10)] == 0)
                {
                    playerX -= 10;
                    player = al_load_bitmap("left.png");
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_E && playerX == 10 && playerY == 80)
                {
                    red_random = 1;
                    redface = al_load_bitmap("blank.png");
                    al_play_sample(dooropen, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
                    red_random = ra(1, 10);

                    if (red_random == 10)
                    {
                        redface = al_load_bitmap("redface.png");
                    }

                    for (int i = 0; i < 9; i++)
                    {
                        map[0][i] = 1;
                        map[9][i] = 1;
                        map[i][0] = 1;
                        map[i][9] = 1;
                        map[1][4] = ra(0, 4); //painting
                        map[3][2] = ra(0, 1);
                        map[3][4] = ra(0, 1);
                        map[4][5] = ra(0, 4); //painting
                        map[5][2] = ra(0, 1);
                        map[6][5] = ra(0, 1);
                        map[7][3] = ra(0, 1);
                        playerX = 80;
                        playerY = 10;
                    }
                }
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_P && playing == false)
            {
                menu = al_load_bitmap("blank.png");
                al_play_sample(start, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
                playing = true;
            }
            break;
        }
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        if (health <= 0 && playing == true)
        {
            playing = false;
            menu = al_load_bitmap("youdied.png");
        }

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            for (int y = 0; y < (height / 10); y++)
            {
                for (int x = 0; x < (width / 10); x++)
                {
                    al_draw_bitmap(textures[(map[x][y])], (x * 10), (y * 10), 0); //dont load twice i guess
                }
            }
            al_draw_bitmap(door, 80, 10, 0);
            al_draw_bitmap(door, 10, 80, 0);
            al_draw_bitmap(player, playerX, playerY, 0);
            al_draw_bitmap(redface, redX, redY, 0);
            al_draw_bitmap(menu, 0, 0, 0);
            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_sample(dooropen);
    al_destroy_sample(paintchange);
    al_destroy_sample(start);
    al_destroy_bitmap(menu);
    al_destroy_bitmap(carpet);
    al_destroy_bitmap(wall);
    al_destroy_bitmap(painting);
    al_destroy_bitmap(painting2);
    al_destroy_bitmap(painting3);
    al_destroy_bitmap(redface);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}