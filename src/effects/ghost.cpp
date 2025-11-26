#include <deque>
#include <raylib.h>

// Snapshot of the sprite
struct GhostSnapshot {
    Rectangle sprite_rec;
    Vector2 position;
    float rotation;
    double lifetime;
};

struct GhostSystem {
    std::deque<GhostSnapshot> snapshot_deque;
    float spawn_timer;
    float lifetime_snapshot;
    float timer_accumulator;
    Texture2D spritesheet;
    Color color_start;
    Color color_end;
};

GhostSystem _ghost_sys;

void initGhostSystem(Texture2D tex, Color start, Color end) {
    _ghost_sys = {.snapshot_deque = {},
                  .spawn_timer = 0.1f,
                  .lifetime_snapshot = 0.5f,
                  .timer_accumulator = 0.0f,
                  .spritesheet = tex,
                  .color_start = start,
                  .color_end = end};
}

void updateGhosts(float dt) {
    _ghost_sys.timer_accumulator += dt;
    std::deque<GhostSnapshot> &deque = _ghost_sys.snapshot_deque;
    for (GhostSnapshot &g : deque) {
        g.lifetime -= dt;
    }
    while (!deque.empty() && deque.begin()->lifetime <= 0) {
        deque.pop_front();
    }
}

void addGhost(Vector2 pos, Rectangle rec, float rot) {
    if (_ghost_sys.timer_accumulator < _ghost_sys.spawn_timer) {
        return;
    }
    _ghost_sys.timer_accumulator = 0.0f;
    GhostSnapshot g = {.sprite_rec = rec,
                       .position = pos,
                       .rotation = rot,
                       .lifetime = _ghost_sys.lifetime_snapshot};
    _ghost_sys.snapshot_deque.push_back(g);
}

void drawGhost(GhostSnapshot &g, Color tint) {
    Rectangle dest_rec = {g.position.x, g.position.y, (float)g.sprite_rec.width,
                          (float)g.sprite_rec.height};

    Vector2 origin = {0, 0};

    DrawTexturePro(_ghost_sys.spritesheet, g.sprite_rec, dest_rec, origin,
                   g.rotation, tint);
}

void drawGhosts() {
    for (GhostSnapshot g : _ghost_sys.snapshot_deque) {
        float factor = 1.0f - (g.lifetime / _ghost_sys.lifetime_snapshot);
        Color tint =
            ColorLerp(_ghost_sys.color_start, _ghost_sys.color_end, factor);
        drawGhost(g, tint);
    }
}
