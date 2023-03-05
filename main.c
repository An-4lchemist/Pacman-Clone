#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<stdlib.h>
#include<time.h>

#define WIN_WIDTH 975
#define WIN_HEIGHT 775

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *surface;
SDL_Texture *main_screen_txt;
SDL_Texture *controls_txt;
SDL_Texture *instructions_txt;
SDL_Texture *texture_map;
SDL_Texture *texture_p_pellets;
SDL_Texture *txt_score;
SDL_Texture *txt_game_over;

SDL_Rect lives_rect;
SDL_Rect game_over_rect;

TTF_Font *font;
SDL_Color color = {255, 255, 255};

SDL_Event event;
Uint32 ticks, old_ticks, death_ticks, reset;

int a, b, random, expression, lives, score, fright_state, pellet_no;
char score_text[10];
int menu = 1, controls = 0, instructions = 0, game = 0;

struct Object {
    SDL_Texture *texture;
    SDL_Texture *death_texture;

    char *file;
    char *death_file;

    float SPEED;

    Uint32 sprite;
    SDL_Rect anim_0;
    SDL_Rect anim_left_1;
    SDL_Rect anim_left_2;
    SDL_Rect anim_right_1;
    SDL_Rect anim_right_2;
    SDL_Rect anim_up_1;
    SDL_Rect anim_up_2;
    SDL_Rect anim_down_1;
    SDL_Rect anim_down_2;

    SDL_Rect anim_death_0;
    SDL_Rect anim_death_1;
    SDL_Rect anim_death_2;
    SDL_Rect anim_death_3;
    SDL_Rect anim_death_4;
    SDL_Rect anim_death_5;
    SDL_Rect anim_death_6;
    SDL_Rect anim_death_7;
    SDL_Rect anim_death_8;
    SDL_Rect anim_death_9;
    SDL_Rect anim_death_10;
    SDL_Rect anim_death_11;

    int last_frame;

    int dead;
    Uint32 death_ticks;

    SDL_Rect png;
    SDL_Rect death_png;
    SDL_Rect box_png;

    SDL_Rect rectangle;

    SDL_Rect right;
    SDL_Rect left;
    SDL_Rect up;
    SDL_Rect down;

    SDL_bool coll_left;
    SDL_bool coll_right;
    SDL_bool coll_up;
    SDL_bool coll_down;
    SDL_bool wall_coll_1;
    SDL_bool ghost_coll_1;
    int wall_coll_2;

    int rel_time;

    int move;

    int old_move;
    int change_dir;

    int left_free;
    int right_free;
    int up_free;
    int down_free;

    float x_pos;
    float y_pos;
    float x_vel;
    float y_vel;
    float box_x_pos;
    float box_y_pos;
    float box_y_vel;
    float old_x_pos;
    float old_y_pos;

} player, ghost_1, ghost_2, ghost_3, ghost_4;
struct Object *ghosts_arr_addresses[4] = {&ghost_1, &ghost_2, &ghost_3, &ghost_4};

struct SDL_Rect walls[31][28];
SDL_Rect *walls_addresses[31][28];

struct SDL_Rect p_pellets[4];
SDL_Rect *p_pellets_addresses[4];

struct SDL_Rect pellets[31][28];
SDL_Rect *pellets_addresses[31][28];

int map_arr[31][28];

void init_rectangles(SDL_Rect *rect, int x, int y, int w, int h) {
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

void init_map() {
    int map[31][28] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                       {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
                       {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
                       {1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 3, 1},
                       {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
                       {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
                       {1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1},
                       {1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1},
                       {1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1},
                       {1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
                       {0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
                       {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
                       {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
                       {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
                       {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
                       {1, 3, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 3, 1},
                       {1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1},
                       {1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1},
                       {1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1},
                       {1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
                       {1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
                       {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 28; j++) {
            map_arr[i][j] = map[i][j];
        }
    }
    int k = 0;
    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 28; j++) {
            if (map_arr[i][j] == 1) {
                walls[i][j].x = j * 25;
                walls[i][j].y = i * 25;
                walls[i][j].w = 25;
                walls[i][j].h = 25;
                walls_addresses[i][j] = &walls[i][j];
            }

            if (map_arr[i][j] == 2) {
                pellets[i][j].x = j * 25 + 8;
                pellets[i][j].y = i * 25 + 8;
                pellets[i][j].w = 8;
                pellets[i][j].h = 8;
                pellets_addresses[i][j] = &pellets[i][j];
            }

            if (map_arr[i][j] == 3) {
                p_pellets[k].x = j * 25;
                p_pellets[k].y = i * 25;
                p_pellets[k].w = 25;
                p_pellets[k].h = 25;
                p_pellets_addresses[k] = &p_pellets[k];
                k++;
            }
        }
    }
}

void init_player(struct Object *player_x) {
    init_rectangles(&player_x->anim_0, 0, 96, 48, 48);
    init_rectangles(&player_x->anim_left_1, 0, 0, 48, 48);
    init_rectangles(&player_x->anim_left_2, 0, 48, 48, 48);
    init_rectangles(&player_x->anim_right_1, 48, 0, 48, 48);
    init_rectangles(&player_x->anim_right_2, 48, 48, 48, 48);
    init_rectangles(&player_x->anim_up_1, 96, 0, 48, 48);
    init_rectangles(&player_x->anim_up_2, 96, 48, 48, 48);
    init_rectangles(&player_x->anim_down_1, 144, 0, 48, 48);
    init_rectangles(&player_x->anim_down_2, 144, 48, 48, 48);

    init_rectangles(&player_x->png, 0, 0, 48, 48);
    init_rectangles(&player_x->death_png, 0, 0, 48, 38);
    init_rectangles(&player_x->rectangle, 0, 0, 24, 24);
    init_rectangles(&player_x->right, 0, 0, 10, 24);
    init_rectangles(&player_x->left, 0, 0, 10, 24);
    init_rectangles(&player_x->up, 0, 0, 24, 10);
    init_rectangles(&player_x->down, 0, 0, 24, 10);

    init_rectangles(&player_x->anim_death_0, 0, 0, 48, 38);
    init_rectangles(&player_x->anim_death_1, 48, 0, 48, 38);
    init_rectangles(&player_x->anim_death_2, 96, 0, 48, 38);
    init_rectangles(&player_x->anim_death_3, 0, 38, 48, 38);
    init_rectangles(&player_x->anim_death_4, 48, 38, 48, 38);
    init_rectangles(&player_x->anim_death_5, 96, 38, 48, 38);
    init_rectangles(&player_x->anim_death_6, 0, 76, 48, 38);
    init_rectangles(&player_x->anim_death_7, 48, 76, 48, 38);
    init_rectangles(&player_x->anim_death_8, 96, 76, 48, 38);
    init_rectangles(&player_x->anim_death_9, 0, 114, 48, 38);
    init_rectangles(&player_x->anim_death_10, 48, 114, 48, 38);
    init_rectangles(&player_x->anim_death_11, 96, 114, 48, 38);
}

void init_ghost(struct Object *ghost_x) {
    init_rectangles(&ghost_x->anim_left_1, 0, 0, 48, 48);
    init_rectangles(&ghost_x->anim_left_2, 0, 48, 48, 48);
    init_rectangles(&ghost_x->anim_right_1, 48, 0, 48, 48);
    init_rectangles(&ghost_x->anim_right_2, 48, 48, 48, 48);
    init_rectangles(&ghost_x->anim_up_1, 96, 0, 48, 48);
    init_rectangles(&ghost_x->anim_up_2, 96, 48, 48, 48);
    init_rectangles(&ghost_x->anim_down_1, 144, 0, 48, 48);
    init_rectangles(&ghost_x->anim_down_2, 144, 48, 48, 48);

    init_rectangles(&ghost_x->png, 13 * 25, 12 * 25, 48, 48);
    init_rectangles(&ghost_x->box_png, 11 * 25, 14 * 25, 48, 48);
    init_rectangles(&ghost_x->rectangle, 13 * 25, 12 * 25, 24, 24);
    init_rectangles(&ghost_x->right, 0, 0, 10, 24);
    init_rectangles(&ghost_x->left, 0, 0, 10, 24);
    init_rectangles(&ghost_x->up, 0, 0, 24, 10);
    init_rectangles(&ghost_x->down, 0, 0, 24, 10);

    init_rectangles(&ghost_x->anim_death_1, 0, 0, 48, 48);
    init_rectangles(&ghost_x->anim_death_2, 48, 0, 48, 48);
    init_rectangles(&ghost_x->anim_death_3, 0, 48, 48, 48);
    init_rectangles(&ghost_x->anim_death_4, 48, 48, 48, 48);
}

void init_objs() {
    player.x_pos = 14 * 25;
    player.y_pos = 23 * 25;
    player.rectangle.x = (int) player.x_pos;
    player.rectangle.y = (int) player.y_pos;
    player.move = 1;
    player.SPEED = 150;

    for (int i = 0; i < 4; i++) {
        ghosts_arr_addresses[i]->x_pos = 14 * 25;
        ghosts_arr_addresses[i]->y_pos = 11 * 25;
        ghosts_arr_addresses[i]->rectangle.x = (int) ghosts_arr_addresses[i]->x_pos;
        ghosts_arr_addresses[i]->rectangle.y = (int) ghosts_arr_addresses[i]->y_pos;
        random = rand() % 2;
        if (random) ghosts_arr_addresses[i]->move = 1;
        else ghosts_arr_addresses[i]->move = 2;
        ghosts_arr_addresses[i]->SPEED = 150;

        ghosts_arr_addresses[i]->box_x_pos = (float) (3 * i * i - 11 * i + 21) * 25;
        ghosts_arr_addresses[i]->box_y_pos = 13 * 25;
        ghosts_arr_addresses[i]->box_png.x = (int) ghosts_arr_addresses[i]->box_x_pos;
        ghosts_arr_addresses[i]->box_png.y = (int) ghosts_arr_addresses[i]->box_y_pos;
    }

    ghosts_arr_addresses[0]->rel_time = 0;
    ghosts_arr_addresses[1]->rel_time = 1 * 1000;
    ghosts_arr_addresses[2]->rel_time = 5 * 1000;
    ghosts_arr_addresses[3]->rel_time = 10 * 1000;

    if (player.dead) {
        for (int i = 0; i < 4; i++) {
            ghosts_arr_addresses[i]->rectangle.x = (int) ghosts_arr_addresses[i]->x_pos;
            ghosts_arr_addresses[i]->rectangle.y = (int) ghosts_arr_addresses[i]->y_pos;
        }
    }

    fright_state = 0;

}

void init_textures() {
    surface = IMG_Load("images/main_screen.png");
    main_screen_txt = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("images/instructions.png");
    instructions_txt = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("images/controls.png");
    controls_txt = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("images/map.png");
    texture_map = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    player.file = "images/pacman_sprites.png";

    surface = IMG_Load(player.file);
    player.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    ghost_1.file = "images/ghost_1_sprites.png";
    ghost_2.file = "images/ghost_2_sprites.png";
    ghost_3.file = "images/ghost_3_sprites.png";
    ghost_4.file = "images/ghost_4_sprites.png";

    for (int i = 0; i < 4; i++) {
        surface = IMG_Load(ghosts_arr_addresses[i]->file);
        ghosts_arr_addresses[i]->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        surface = IMG_Load("images/fright_ghosts_sprites.png");
        ghosts_arr_addresses[i]->death_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    player.death_file = "images/pacman_death.png";

    surface = IMG_Load(player.death_file);
    player.death_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("images/p_pellet.png");
    texture_p_pellets = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    font = TTF_OpenFont("fonts/arial.ttf", 40);

    surface = IMG_Load("images/game_over.png");
    txt_game_over = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    init_rectangles(&game_over_rect, 9 * 25 - 14, 11 * 25, 279, 25);
}

void init_win_ren() {
    window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED, 75, WIN_WIDTH, WIN_HEIGHT, 0);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    renderer = SDL_CreateRenderer(window, -1, render_flags);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    srand(time(0));
}

void input_event() {
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            exit(0);
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_a:
                case SDLK_LEFT:
                    player.move = 1;
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
                    player.move = 2;
                    break;
                case SDLK_w:
                case SDLK_UP:
                    player.move = 3;
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    player.move = 4;
                    break;
                case SDLK_q:
                    exit(0);
            }
            break;
    }
}

void disp_textures() {
    SDL_RenderCopy(renderer, texture_map, NULL, NULL);

    player.png.x = player.rectangle.x - 12;
    player.png.y = player.rectangle.y - 12;

    for (int i = 0; i < 4; i++) {
        ghosts_arr_addresses[i]->png.x = ghosts_arr_addresses[i]->rectangle.x - 12;
        ghosts_arr_addresses[i]->png.y = ghosts_arr_addresses[i]->rectangle.y - 12;
    }

    SDL_SetRenderDrawColor(renderer, 255, 218, 185, 255);
    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 28; j++) {
            if (map_arr[i][j] == 2) {
                SDL_RenderFillRect(renderer, pellets_addresses[i][j]);
            }
        }
    }

}

void disp_lives() {
    for (int i = 0; i < lives; i++) {
        init_rectangles(&lives_rect, 710 + 50 * i, 725, 48, 48);
        SDL_RenderCopy(renderer, player.texture, &player.anim_right_1, &lives_rect);
    }
}

void disp_score() {
    itoa(score, score_text, 10);
    surface = TTF_RenderText_Solid(font, score_text, color);
    txt_score = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(txt_score, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {725, 0, texW, texH};
    SDL_RenderCopy(renderer, txt_score, NULL, &dstrect);
}

void animate_player_x(struct Object *player_x) {
    player_x->sprite = (ticks / 120) % 3;

    if (player_x->rectangle.x <= 675 && player_x->rectangle.x > -25) {
        if (player_x->x_pos == player_x->old_x_pos && player_x->y_pos == player_x->old_y_pos) {
            switch (player_x->last_frame) {
                case 1:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_left_1, &player_x->png);
                    break;
                case 2:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_right_1, &player_x->png);
                    break;
                case 3:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_up_1, &player_x->png);
                    break;
                case 4:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_down_1, &player_x->png);
                    break;

            }
        } else if (player_x->x_vel < 0 && !player_x->y_vel) {
            player_x->last_frame = 1;
            switch (player_x->sprite) {
                case 0:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_0, &player_x->png);
                    break;
                case 1:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_left_1, &player_x->png);
                    break;
                case 2:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_left_2, &player_x->png);
                    break;

            }
        } else if (player_x->x_vel > 0 && !player_x->y_vel) {
            player_x->last_frame = 2;
            switch (player_x->sprite) {
                case 0:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_0, &player_x->png);
                    break;
                case 1:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_right_1, &player_x->png);
                    break;
                case 2:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_right_2, &player_x->png);
                    break;

            }
        } else if (!player_x->x_vel && player_x->y_vel < 0) {
            player_x->last_frame = 3;
            switch (player_x->sprite) {
                case 0:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_0, &player_x->png);
                    break;
                case 1:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_up_1, &player_x->png);
                    break;
                case 2:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_up_2, &player_x->png);
                    break;

            }
        } else if (!player_x->x_vel && player_x->y_vel > 0) {
            player_x->last_frame = 4;
            switch (player_x->sprite) {
                case 0:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_0, &player_x->png);
                    break;
                case 1:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_down_1, &player_x->png);
                    break;
                case 2:
                    SDL_RenderCopy(renderer, player_x->texture, &player_x->anim_down_2, &player_x->png);
                    break;

            }
        }
    }
}

void animate_death_player_x(struct Object *player_x) {
    random = 0;
    player.png.x = -48;
    player.png.y = -48;
    player.death_png.x = player.rectangle.x - 12;
    player.death_png.y = player.rectangle.y - 7;
    ticks = SDL_GetTicks() - death_ticks;
    player.sprite = (ticks / 120) % 21;
    switch (player_x->sprite) {
        case 0:
            SDL_RenderCopy(renderer, player_x->death_texture, &player_x->anim_death_0, &player_x->death_png);
            break;
        case 1:
            SDL_RenderCopy(renderer, player_x->death_texture, &player_x->anim_death_1, &player_x->death_png);
            break;
        case 2:
            SDL_RenderCopy(renderer, player_x->death_texture, &player_x->anim_death_2, &player_x->death_png);
            break;
        case 3:
            SDL_RenderCopy(renderer, player_x->death_texture, &player_x->anim_death_3, &player_x->death_png);
            break;
        case 4:
            SDL_RenderCopy(renderer, player_x->death_texture, &player_x->anim_death_4, &player_x->death_png);
            break;
        case 5:
            SDL_RenderCopy(renderer, player_x->death_texture, &player_x->anim_death_5, &player_x->death_png);
            break;
        case 6:
            SDL_RenderCopy(renderer, player_x->death_texture, &player_x->anim_death_6, &player_x->death_png);
            break;
        case 7:
            SDL_RenderCopy(renderer, player_x->death_texture, &player_x->anim_death_7, &player_x->death_png);
            break;
        case 8:
            SDL_RenderCopy(renderer, player_x->death_texture, &player_x->anim_death_8, &player_x->death_png);
            break;
        case 9:
            SDL_RenderCopy(renderer, player_x->death_texture, &player_x->anim_death_9, &player_x->death_png);
            break;
        case 10:
            SDL_RenderCopy(renderer, player_x->death_texture, &player_x->anim_death_10, &player_x->death_png);
            break;
        case 11:
        case 12:
            SDL_RenderCopy(renderer, player_x->death_texture, &player_x->anim_death_11, &player_x->death_png);
            break;
        case 20:
            random = 1;
            break;
    }
}

void animate_ghost_x(struct Object *ghost_x) {
    if (!fright_state) {
        ghost_x->sprite = (ticks / 120) % 2;
        if (ghost_x->rectangle.x <= 675 && ghost_x->rectangle.x > -25) {
            switch (ghost_x->move) {
                case 1:
                    switch (ghost_x->sprite) {
                        case 0:
                            SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_left_1, &ghost_x->png);
                            break;
                        case 1:
                            SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_left_2, &ghost_x->png);
                            break;
                    }
                    break;
                case 2:
                    switch (ghost_x->sprite) {
                        case 0:
                            SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_right_1, &ghost_x->png);
                            break;
                        case 1:
                            SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_right_2, &ghost_x->png);
                            break;
                    }
                    break;
                case 3:
                    switch (ghost_x->sprite) {
                        case 0:
                            SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_up_1, &ghost_x->png);
                            break;
                        case 1:
                            SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_up_2, &ghost_x->png);
                            break;
                    }
                    break;
                case 4:
                    switch (ghost_x->sprite) {
                        case 0:
                            SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_down_1, &ghost_x->png);
                            break;
                        case 1:
                            SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_down_2, &ghost_x->png);
                            break;
                    }
                    break;
            }
        }
    }
    if (fright_state) {
        if (ghost_x->rectangle.x <= 675 && ghost_x->rectangle.x > -25) {
            if (ticks < old_ticks + 7000) {
                ghost_x->sprite = (ticks / 120) % 2;
                switch (ghost_x->sprite) {
                    case 0:
                        SDL_RenderCopy(renderer, ghost_x->death_texture, &ghost_x->anim_death_1, &ghost_x->png);
                        break;
                    case 1:
                        SDL_RenderCopy(renderer, ghost_x->death_texture, &ghost_x->anim_death_2, &ghost_x->png);
                        break;
                }
            } else if (ticks > old_ticks + 7000) {
                ghost_x->sprite = (ticks / 120) % 4;
                switch (ghost_x->sprite) {
                    case 0:
                        SDL_RenderCopy(renderer, ghost_x->death_texture, &ghost_x->anim_death_1, &ghost_x->png);
                        break;
                    case 1:
                        SDL_RenderCopy(renderer, ghost_x->death_texture, &ghost_x->anim_death_2, &ghost_x->png);
                        break;
                    case 2:
                        SDL_RenderCopy(renderer, ghost_x->death_texture, &ghost_x->anim_death_3, &ghost_x->png);
                        break;
                    case 3:
                        SDL_RenderCopy(renderer, ghost_x->death_texture, &ghost_x->anim_death_4, &ghost_x->png);
                        break;
                }
            }
        }
    }
}

void animate_ghost_box(struct Object *ghost_x) {
    if (ghost_x->box_png.y == 14 * 25) {
        ghost_x->box_y_vel = -80;
    } else if (ghost_x->box_png.y == 13 * 25) {
        ghost_x->box_y_vel = 80;
    }

    if (ghost_1.dead && !ghost_2.dead) {
        ghost_1.box_x_pos = ghost_2.box_x_pos;
    } else if (ghost_1.dead && !ghost_3.dead) {
        ghost_1.box_x_pos = ghost_3.box_x_pos;
    } else if (ghost_1.dead && !ghost_4.dead) {
        ghost_1.box_x_pos = ghost_4.box_x_pos;
    }

    ghost_x->box_y_pos += ghost_x->box_y_vel / 60;
    ghost_x->box_png.y = (int) ghost_x->box_y_pos;
    ghost_x->box_png.x = (int) ghost_x->box_x_pos;

    ghost_x->sprite = (ticks / 120) % 2;

    if (ghost_x->box_y_vel < 0) {
        ghost_x->last_frame = 0;
        switch (ghost_x->sprite) {
            case 0:
                SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_up_1, &ghost_x->box_png);
                break;
            case 1:
                SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_up_2, &ghost_x->box_png);
                break;
        }
    }
    if (ghost_x->box_y_vel > 0) {
        ghost_x->last_frame = 1;
        switch (ghost_x->sprite) {
            case 0:
                SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_down_1, &ghost_x->box_png);
                break;
            case 1:
                SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_down_2, &ghost_x->box_png);
                break;
        }
    }
    if (ghost_x->box_y_vel == 0) {
        if (!ghost_x->last_frame) {
            switch (ghost_x->sprite) {
                case 0:
                    SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_up_1, &ghost_x->box_png);
                    break;
                case 1:
                    SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_up_2, &ghost_x->box_png);
                    break;
            }
        }
        if (!ghost_x->last_frame) {
            switch (ghost_x->sprite) {
                case 0:
                    SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_down_1, &ghost_x->box_png);
                    break;
                case 1:
                    SDL_RenderCopy(renderer, ghost_x->texture, &ghost_x->anim_down_2, &ghost_x->box_png);
                    break;
            }
        }
    }

}

void animate_p_pellets() {
    random = (int) (ticks / 120) % 2;
    switch (random) {
        case 0:
            for (int i = 0; i < 4; i++) {
                SDL_RenderCopy(renderer, texture_p_pellets, NULL, p_pellets_addresses[i]);
            }
            break;
        case 1:
            break;
    }
}

void create_field_obj(struct Object *obj) {
    obj->left.x = obj->rectangle.x - obj->left.w;
    obj->left.y = obj->rectangle.y;
    obj->right.x = obj->rectangle.x + obj->rectangle.w;
    obj->right.y = obj->rectangle.y;
    obj->up.x = obj->rectangle.x;
    obj->up.y = obj->rectangle.y - obj->up.h;
    obj->down.x = obj->rectangle.x;
    obj->down.y = obj->rectangle.y + obj->rectangle.h;
}

void find_direction_obj(struct Object *obj) {
    obj->left_free = 1;
    a = (int) (obj->left.x / 25);
    b = (int) (obj->left.y / 25);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (map_arr[b + i][a + j] == 1) {
                obj->coll_left = SDL_HasIntersection(&obj->left, walls_addresses[b + i][a + j]);
                if (obj->coll_left) obj->left_free = 0;
            }
        }
    }

    obj->right_free = 1;
    a = (int) (obj->right.x / 25);
    b = (int) (obj->right.y / 25);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (map_arr[b + i][a + j] == 1) {
                obj->coll_right = SDL_HasIntersection(&obj->right, walls_addresses[b + i][a + j]);
                if (obj->coll_right) obj->right_free = 0;
            }
        }
    }

    obj->up_free = 1;
    a = (int) (obj->up.x / 25);
    b = (int) (obj->up.y / 25);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (map_arr[b + i][a + j] == 1) {
                obj->coll_up = SDL_HasIntersection(&obj->up, walls_addresses[b + i][a + j]);
                if (obj->coll_up || obj->rectangle.x < 0 || obj->rectangle.x > 700 - obj->rectangle.w)
                    obj->up_free = 0;
            }
        }
    }
    obj->down_free = 1;
    a = (int) (obj->down.x / 25);
    b = (int) (obj->down.y / 25);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (map_arr[b + i][a + j] == 1) {
                obj->coll_down = SDL_HasIntersection(&obj->down, walls_addresses[b + i][a + j]);
                if (obj->coll_down || obj->rectangle.x < 0 || obj->rectangle.x > 700 - obj->rectangle.w)
                    obj->down_free = 0;
            }
        }
    }
}

void change_ghost_x_direction(struct Object *ghost_x) {
    ghost_x->change_dir = 0;
    if (((ghost_x->x_pos == ghost_x->old_x_pos) && (ghost_x->y_pos == ghost_x->old_y_pos))
        || (ghost_x->left_free + ghost_x->right_free + ghost_x->up_free + ghost_x->down_free >= 3)) {
        ghost_x->change_dir = 1;
    }

    if (ghost_x->change_dir) {
        ghost_x->old_move = ghost_x->move;
        expression =
                8 * ghost_x->left_free + 4 * ghost_x->right_free + 2 * ghost_x->up_free + 1 * ghost_x->down_free;

        switch (expression) {
            case 5:
                random = rand() % 2;
                if (random == 0) ghost_x->move = 2;
                else if (random == 1) ghost_x->move = 4;
                break;
            case 6:
                random = rand() % 2;
                if (random == 0) ghost_x->move = 2;
                else if (random == 1) ghost_x->move = 3;
                break;
            case 7:
                random = rand() % 3;
                if (random == 0) ghost_x->move = 2;
                else if (random == 1) ghost_x->move = 3;
                else if (random == 2) ghost_x->move = 4;
                break;
            case 9:
                random = rand() % 2;
                if (random == 0) ghost_x->move = 1;
                else if (random == 1) ghost_x->move = 4;
                break;
            case 10:
                random = rand() % 2;
                if (random == 0) ghost_x->move = 1;
                else if (random == 1) ghost_x->move = 3;
                break;
            case 11:
                random = rand() % 3;
                if (random == 0) ghost_x->move = 1;
                else if (random == 1) ghost_x->move = 3;
                else if (random == 2) ghost_x->move = 4;
                break;
            case 13:
                random = rand() % 3;
                if (random == 0) ghost_x->move = 1;
                else if (random == 1) ghost_x->move = 2;
                else if (random == 2) ghost_x->move = 4;
                break;
            case 14:
                random = rand() % 3;
                if (random == 0) ghost_x->move = 1;
                else if (random == 1) ghost_x->move = 2;
                else if (random == 2) ghost_x->move = 3;
                break;
            case 15:
                random = rand() % 4;
                if (random == 0) ghost_x->move = 1;
                else if (random == 1) ghost_x->move = 2;
                else if (random == 2) ghost_x->move = 3;
                else if (random == 3) ghost_x->move = 4;
                break;
        }
        random = ghost_x->move + ghost_x->old_move;
        if (random == 3 || random == 7) {
            ghost_x->move = ghost_x->old_move;
        }
    }
}

void move_obj(struct Object *obj) {
    switch (obj->move) {
        case 1:
            if (obj->left_free) {
                obj->x_vel = -obj->SPEED;
                obj->y_vel = 0;
            }
            break;
        case 2:
            if (obj->right_free) {
                obj->x_vel = obj->SPEED;
                obj->y_vel = 0;
            }
            break;
        case 3:
            if (obj->up_free) {
                obj->y_vel = -obj->SPEED;
                obj->x_vel = 0;
            }
            break;
        case 4:
            if (obj->down_free) {
                obj->y_vel = obj->SPEED;
                obj->x_vel = 0;
            }
            break;
        default:
            obj->move = 0;
    }

    obj->old_x_pos = obj->x_pos;
    obj->old_y_pos = obj->y_pos;

    obj->x_pos += obj->x_vel / 60;
    obj->y_pos += obj->y_vel / 60;

    obj->rectangle.x = (int) obj->x_pos;
    obj->rectangle.y = (int) obj->y_pos;
}

void collision_wall_obj(struct Object *obj) {
    obj->wall_coll_2 = 0;
    a = (int) (obj->rectangle.x / 25);
    b = (int) (obj->rectangle.y / 25);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (map_arr[b + i][a + j] == 1) {
                obj->wall_coll_1 = SDL_HasIntersection(&obj->rectangle, walls_addresses[b + i][a + j]);
                if (obj->wall_coll_1) obj->wall_coll_2 = 1;
            }
        }
    }

    if (obj->wall_coll_2) {
        obj->x_pos = obj->old_x_pos;
        obj->y_pos = obj->old_y_pos;
        obj->rectangle.x = (int) obj->x_pos;
        obj->rectangle.y = (int) obj->y_pos;
    }
}

void collision_player_ghosts() {
    if (!fright_state) {
        player.dead = 0;
        for (int i = 0; i < 4; i++) {
            if (player.rectangle.x > 14 && player.rectangle.x < 700 - 14) {
                if (ticks > ghosts_arr_addresses[i]->rel_time) {
                    player.ghost_coll_1 = SDL_HasIntersection(&player.rectangle,
                                                              &ghosts_arr_addresses[i]->rectangle);
                    if (player.ghost_coll_1) player.dead = 1;
                }
            }
        }

        if (player.dead) {
            old_ticks = ticks;
        }
    }
    if (fright_state) {
        for (int i = 0; i < 4; i++) {
            if (player.rectangle.x > 14 && player.rectangle.x < 700 - 14) {
                if (ticks > ghosts_arr_addresses[i]->rel_time) {
                    if (SDL_HasIntersection(&player.rectangle, &ghosts_arr_addresses[i]->rectangle)) {
                        ghosts_arr_addresses[i]->dead = 1;
                        ghosts_arr_addresses[i]->death_ticks = ticks;
                        ghosts_arr_addresses[i]->rectangle.x = -50;
                        ghosts_arr_addresses[i]->rectangle.y = -50;
                        score += 100;
                    }
                }
            }
        }
    }
}

void collision_player_pellets() {
    a = (int) (player.rectangle.x / 25);
    b = (int) (player.rectangle.y / 25);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (map_arr[b + i][a + j] == 2) {
                if (SDL_HasIntersection(&player.rectangle, pellets_addresses[b + i][a + j])) {
                    pellets_addresses[b + i][a + j]->x = -25;
                    pellets_addresses[b + i][a + j]->y = -25;
                    score += 10;
                    pellet_no--;
                    if (!pellet_no) old_ticks = SDL_GetTicks();
                }
            }
        }
    }

}

void collision_player_p_pellets() {
    for (int i = 0; i < 4; i++) {
        if (SDL_HasIntersection(&player.rectangle, p_pellets_addresses[i]) && !fright_state) {
            fright_state = 1;
            p_pellets[i].x = -25;
            p_pellets[i].y = -25;
            old_ticks = ticks;
            score += 30;
        }
    }

    if (fright_state) {
        if (ticks > old_ticks + 10000)fright_state = 0;
    }
}

void revive_dead_ghosts() {
    for (int i = 0; i < 4; i++) {
        if (ticks > ghosts_arr_addresses[i]->rel_time) {
            if (ghosts_arr_addresses[i]->dead) {
                if (ticks > ghosts_arr_addresses[i]->death_ticks + 3000) ghosts_arr_addresses[i]->dead = 0;
                animate_ghost_box(ghosts_arr_addresses[i]);
                ghosts_arr_addresses[i]->x_pos = 14 * 25;
                ghosts_arr_addresses[i]->y_pos = 11 * 25;
                ghosts_arr_addresses[i]->rectangle.x = (int) ghosts_arr_addresses[i]->x_pos;
                ghosts_arr_addresses[i]->rectangle.y = (int) ghosts_arr_addresses[i]->y_pos;
                random = rand() % 2;
                if (random) ghosts_arr_addresses[i]->move = 1;
                else ghosts_arr_addresses[i]->move = 2;
            }
        }
    }
}

void freeze_objs() {
    player.x_pos = player.old_x_pos;
    player.y_pos = player.old_y_pos;
    player.move = 0;

    for (int i = 0; i < 4; i++) {
        if (ticks < ghosts_arr_addresses[i]->rel_time) {
            ghosts_arr_addresses[i]->box_y_vel = 0;
        }
        if (ticks > ghosts_arr_addresses[i]->rel_time) {
            ghosts_arr_addresses[i]->x_pos = ghosts_arr_addresses[i]->old_x_pos;
            ghosts_arr_addresses[i]->y_pos = ghosts_arr_addresses[i]->old_y_pos;
        }
    }
}

void tunnel_obj(struct Object *obj) {
    if (obj->x_pos < -(float) obj->rectangle.w) {
        obj->x_pos = (float) 700 + (float) obj->rectangle.w;
        obj->y_vel = 0;
    }
    if (obj->x_pos > 700.0 + obj->rectangle.w) {
        obj->x_pos = -(float) obj->rectangle.w;
        obj->y_vel = 0;
    }
}

void clear_everything() {
    SDL_DestroyTexture(texture_map);
    SDL_DestroyTexture(main_screen_txt);
    SDL_DestroyTexture(instructions_txt);
    SDL_DestroyTexture(controls_txt);
    SDL_DestroyTexture(texture_p_pellets);
    SDL_DestroyTexture(txt_score);
    SDL_DestroyTexture(txt_game_over);
    SDL_DestroyTexture(player.texture);
    SDL_DestroyTexture(player.death_texture);
    TTF_CloseFont(font);
    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(ghosts_arr_addresses[i]->texture);
        SDL_DestroyTexture(ghosts_arr_addresses[i]->death_texture);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    init_win_ren();

    init_player(&player);

    for (int i = 0; i < 4; i++) {
        init_ghost(ghosts_arr_addresses[i]);
    }

    init_objs();

    init_textures();

    label1:
    init_map();

    label:
    while (menu) {
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_c:
                        controls = 1;
                        menu = 0;
                        break;
                    case SDLK_i:
                        instructions = 1;
                        menu = 0;
                        break;
                    case SDLK_SPACE:
                        game = 1;
                        menu = 0;
                        lives = 4;
                        score = 0;
                        pellet_no = 240;
                        goto label1;
                    case SDLK_ESCAPE:
                        exit(0);
                }
                break;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, main_screen_txt, NULL, NULL);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }

    while (controls) {
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        controls = 0;
                        menu = 1;
                        goto label;
                }
                break;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, controls_txt, NULL, NULL);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }

    while (instructions) {
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        instructions = 0;
                        menu = 1;
                        goto label;
                }
                break;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, instructions_txt, NULL, NULL);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }

    while (game) {

        ticks = SDL_GetTicks() - reset;

        input_event();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        disp_textures();

        disp_score();

        if (lives && pellet_no) {
            animate_p_pellets();
            disp_lives();

            if (!player.dead) {
                animate_player_x(&player);
                create_field_obj(&player);
                find_direction_obj(&player);
                move_obj(&player);
                collision_wall_obj(&player);
                tunnel_obj(&player);

                for (int i = 0; i < 4; i++) {
                    if (ticks < ghosts_arr_addresses[i]->rel_time) {
                        animate_ghost_box(ghosts_arr_addresses[i]);
                    }
                    if (ticks > ghosts_arr_addresses[i]->rel_time && !ghosts_arr_addresses[i]->dead) {
                        animate_ghost_x(ghosts_arr_addresses[i]);
                        create_field_obj(ghosts_arr_addresses[i]);
                        find_direction_obj(ghosts_arr_addresses[i]);
                        change_ghost_x_direction(ghosts_arr_addresses[i]);
                        move_obj(ghosts_arr_addresses[i]);
                        collision_wall_obj(ghosts_arr_addresses[i]);
                        tunnel_obj(ghosts_arr_addresses[i]);
                    }
                }

                collision_player_ghosts();

                collision_player_p_pellets();
                collision_player_pellets();

                revive_dead_ghosts();
            }
            if (player.dead) {
                if (ticks < old_ticks + 2000) {
                    freeze_objs();
                    animate_player_x(&player);
                    create_field_obj(&player);
                    for (int i = 0; i < 4; i++) {
                        if (ticks < ghosts_arr_addresses[i]->rel_time) {
                            animate_ghost_box(ghosts_arr_addresses[i]);
                        }
                        if (ticks > ghosts_arr_addresses[i]->rel_time) {
                            animate_ghost_x(ghosts_arr_addresses[i]);
                            create_field_obj(ghosts_arr_addresses[i]);
                        }
                    }
                    random = 0;
                    player.sprite = 0;
                    death_ticks = SDL_GetTicks();
                }
                if (ticks > old_ticks + 2000) {
                    animate_death_player_x(&player);
                }
                if (random) {
                    reset = SDL_GetTicks();
                    init_objs();
                    disp_textures();
                    for (int i = 0; i < 4; i++) {
                        create_field_obj(ghosts_arr_addresses[i]);
                    }
                    lives--;
                    if (lives == 0) old_ticks = SDL_GetTicks();
                    player.dead = 0;
                    death_ticks = 0;
                }
            }
        }
        if (!lives || !pellet_no) {
            SDL_RenderCopy(renderer, txt_game_over, NULL, &game_over_rect);
            ticks = SDL_GetTicks();
            if (ticks > old_ticks + 5000) {
                game = 0;
                menu = 1;
                goto label;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }

    clear_everything();
    return 0;
}
