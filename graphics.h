#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "globals.h"

void draw_text(Text &text) {
    Vector2 dimensions = MeasureTextEx(*text.font, text.str.c_str(), text.size * screen_scale, text.spacing);
    Vector2 pos = {
        (screen_size.x * text.position.x) - (0.5f * dimensions.x),
        (screen_size.y * text.position.y) - (0.5f * dimensions.y)
    };

    DrawTextEx(*text.font, text.str.c_str(), pos, dimensions.y, text.spacing, text.color);
}

void derive_graphics_metrics_from_loaded_level() {
    screen_size.x  = static_cast<float>(GetScreenWidth());
    screen_size.y = static_cast<float>(GetScreenHeight());

    cell_size = std::min(
        screen_size.x / static_cast<float>(current_level.columns),
        screen_size.y / static_cast<float>(current_level.rows)
    ) * CELL_SCALE;
    screen_scale = std::min(
        screen_size.x,
        screen_size.y
    ) / SCREEN_SCALE_DIVISOR;
    float level_width  = static_cast<float>(current_level.columns) * cell_size;
    float level_height = static_cast<float>(current_level.rows)    * cell_size;
    shift_to_center.x = (screen_size.x - level_width) * 0.5f;
    shift_to_center.y = (screen_size.y - level_height) * 0.5f;
}

void draw_menu() {
    ClearBackground(BLACK);
    draw_text(game_title);
    draw_text(game_subtitle);
}

void draw_game_over_screen() {
    Text score = {
        "Your Score : " + std::to_string(player_score),
{ 0.50f, 0.85f },
    };
    ClearBackground(BLACK);
    draw_text(game_over_text);
    draw_text(game_over_subtitle);
    draw_text(score);

}

void draw_game_overlay() {
    Text score = {
        "Score " + std::to_string(player_score),
        { 0.50f, 0.05f },
        48.0f
    };
    Text score_shadow = {
        "Score " + std::to_string(player_score),
        { 0.503f, 0.055f },
        48.0f,
        GRAY
    };

    draw_text(score_shadow);
    draw_text(score);
}

void draw_level() {
    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {

            Vector2 pos = {
                    shift_to_center.x + static_cast<float>(column) * cell_size,
                    shift_to_center.y + static_cast<float>(row) * cell_size
            };

            char cell = current_level.data[row * current_level.columns + column];
            // The first image layer
            switch (cell) {
                case AIR:
                case PLAYER:
                case COIN:
                case GEM:
                case ENEMIE:
                case EXIT:
                case MASHROOM:
                    draw_image(air_image, pos, cell_size);
                    break;
                case WALL:
                    draw_image(wall_image, pos, cell_size);
                    break;
            }
            // The second image layer
            switch (cell) {
                case COIN:
                    draw_sprite(coin_sprite, pos, cell_size);
                    break;
                case EXIT:
                    draw_image(exit_image, pos, cell_size);
                    break;
                case GEM:
                    draw_sprite(gem_sprite, pos, cell_size);
                    break;
                case ENEMIE:
                    draw_sprite(enemie_sprite, pos, cell_size);
                    break;
                case MASHROOM:
                    draw_image(mashroom_image, pos, cell_size);
                    break;
                default:
                    break;
            }
        }
    }

    draw_player();
}

void draw_player() {
    Vector2 pos = {
        shift_to_center.x + player_pos.x * cell_size,
        shift_to_center.y + player_pos.y * cell_size
    };

    draw_sprite(player_sprite, pos, cell_size);
}

void draw_pause_menu() {
    draw_text(game_paused);
}

void create_victory_menu_background() {
    for (auto &ball : victory_balls) {
        ball.x = rand_up_to(screen_size.x);
        ball.y = rand_up_to(screen_size.y);
        ball.dx = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED);
        ball.dx *= screen_scale;
        if (abs(ball.dx) < 0.1f) ball.dx = 1.0f;
        ball.dy = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED);
        ball.dy *= screen_scale;
        if (abs(ball.dy) < 0.1f) ball.dy = 1.0f;
        ball.radius = rand_from_to(VICTORY_BALL_MIN_RADIUS, VICTORY_BALL_MAX_RADIUS);
        ball.radius *= screen_scale;
    }

    ClearBackground(BLACK);
    EndDrawing();
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
    BeginDrawing();
}

void animate_victory_menu_background() {
    for (auto &ball : victory_balls) {
        ball.x += ball.dx * cos(0.01f) - ball.dy * sin(0.01f); // Circular motion
        ball.y += ball.dx * sin(0.01f) + ball.dy * cos(0.01f);

        // Wrap around the screen edges
        if (ball.x < 0) ball.x = screen_size.x;
        if (ball.x > screen_size.x) ball.x = 0;
        if (ball.y < 0) ball.y = screen_size.y;
        if (ball.y > screen_size.y) ball.y = 0;
    }
}

void draw_victory_menu_background() {
    for (auto &ball : victory_balls) {
        // Randomize colors for each shape
        Color shape_color = {
            static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(rand() % 255),
            200
        };

        // Alternate between different shapes
        int shape_type = rand() % 3; // 0 = circle, 1 = rectangle, 2 = triangle

        if (shape_type == 0) {
            DrawCircleV({ ball.x, ball.y }, ball.radius, shape_color);
        } else if (shape_type == 1) {
            DrawRectangle(
                static_cast<int>(ball.x - ball.radius),
                static_cast<int>(ball.y - ball.radius),
                static_cast<int>(ball.radius * 2),
                static_cast<int>(ball.radius * 2),
                shape_color
            );
        } else if (shape_type == 2) {
            Vector2 point1 = { ball.x, ball.y - ball.radius };
            Vector2 point2 = { ball.x - ball.radius, ball.y + ball.radius };
            Vector2 point3 = { ball.x + ball.radius, ball.y + ball.radius };
            DrawTriangle(point1, point2, point3, shape_color);
        }
    }
}

void draw_victory_menu() {
    ClearBackground(BLACK);
    DrawRectangle(
        0, 0,
        static_cast<int>(screen_size.x), static_cast<int>(screen_size.y),
        { 0, 0, 0, VICTORY_BALL_TRAIL_TRANSPARENCY }
    );

    animate_victory_menu_background();
    draw_victory_menu_background();

    draw_text(victory_title);
    draw_text(victory_subtitle);
}

#endif // GRAPHICS_H
