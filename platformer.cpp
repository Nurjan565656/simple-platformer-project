#include "raylib.h"

#include "globals.h"
#include "level.h"
#include "player.h"
#include "graphics.h"
#include "assets.h"
#include "utilities.h"

void update_game() {
    game_frame++;

    // TODO

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        move_player_horizontally(MOVEMENT_SPEED);
    }

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        move_player_horizontally(-MOVEMENT_SPEED);
    }

    // Calculating collisions to decide whether the player is allowed to jump: don't want them to suction cup to the ceiling or jump midair
    is_player_on_ground = is_colliding({player_pos.x, player_pos.y + 0.1f}, WALL);
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && is_player_on_ground) {
        player_y_velocity = -JUMP_STRENGTH;
    }

    update_player();
}

void draw_game() {
    // TODO

    ClearBackground(BLACK);
    draw_level();
    draw_game_overlay();
}
void reset_game() {
    level_index = 0;
    player_score = 0;
    load_level(0);
}

void update_menu() {
    if (IsKeyPressed(KEY_ENTER)) {
        game_state = PLAYING;
    }
}

void update_paused() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        game_state = PLAYING;
    }
}

void update_victory() {
    if (IsKeyPressed(KEY_ENTER)) {
        game_state = MAIN_MENU;
    }
}

void update_gameplay() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        game_state = PAUSED;
    }

    update_game();
}

int main() {
    InitWindow(1024, 480, "Platformer");
    SetTargetFPS(60);
    SetExitKey(0);

    load_fonts();
    load_images();
    load_sounds();
    load_level();

    while (!WindowShouldClose()) {
        BeginDrawing();

        if (game_state == PLAYING) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = PAUSED;
            }

            update_game();
            draw_game();
        }
        else if (game_state == PAUSED) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = PLAYING;
            }

            draw_pause_menu();
        }
        else if (game_state == MAIN_MENU) {
            if (IsKeyPressed(KEY_ENTER)) {
                game_state = PLAYING;
            }

            draw_menu();
        }
        else if (game_state == VICTORY) {
            if (IsKeyPressed(KEY_ENTER)) {
                reset_game();
                game_state = MAIN_MENU;
            }

            draw_victory_menu();
        }
        else if (game_state == GAME_OVER) {
            if (IsKeyPressed(KEY_ENTER)) {
                reset_game();
                game_state = MAIN_MENU;
            }

            draw_game_over_screen();
        }

        EndDrawing();
    }

    unload_level();
    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
