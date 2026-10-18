#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

#include <algorithm>
#include <deque>
#include <utility>

using namespace geode::prelude;

class $modify(DynPlayer, PlayerObject) {
    struct Fields {
        std::deque<std::pair<uint64_t, bool>> queue;
    };

    static inline uint64_t s_tick = 0;

    static bool activo() {
        return Mod::get()->getSettingValue<bool>("enabled");
    }

    static uint64_t ventana() {
        auto w = Mod::get()->getSettingValue<int64_t>("window");
        return (uint64_t)std::clamp<int64_t>(w, 0, 16);
    }

    bool pushButton(PlayerButton btn) override {
        if (btn != PlayerButton::Jump || !activo() || ventana() == 0) {
            return PlayerObject::pushButton(btn);
        }
        m_fields->queue.emplace_back(s_tick + ventana(), true);
        return true;
    }

    bool releaseButton(PlayerButton btn) override {
        if (btn != PlayerButton::Jump || !activo() || ventana() == 0) {
            return PlayerObject::releaseButton(btn);
        }
        m_fields->queue.emplace_back(s_tick + ventana(), false);
        return true;
    }

    void update(float dt) override {
        if (m_gameLayer && m_gameLayer->m_player1 == this) {
            ++s_tick;
        }

        auto f = m_fields.self();

        if (m_isDead) {
            f->queue.clear();
        } else {
            while (!f->queue.empty() && f->queue.front().first <= s_tick) {
                const bool pulsacion = f->queue.front().second;
                f->queue.pop_front();
                if (pulsacion) (void)PlayerObject::pushButton(PlayerButton::Jump);
                else           (void)PlayerObject::releaseButton(PlayerButton::Jump);
            }
        }

        PlayerObject::update(dt);
    }
};