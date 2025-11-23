#include "render/bullets.hpp"

// NOTE: this needs pooling at some point
void drawBullet(Texture2D texture, Vector2 origin, Vector2 direction,
                float speed) {
    static Vector2 _speed = {.x = speed, .y = speed};
    DrawTexture(texture, origin.x + _speed.x, origin.y + _speed.y, WHITE);
    _speed.x += direction.x * speed;
    _speed.y += direction.y * speed;
}
