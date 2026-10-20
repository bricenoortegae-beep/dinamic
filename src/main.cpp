class $modify(DynPlayer, PlayerObject) {
    struct Fields {
        std::deque<std::pair<uint64_t, bool>> queue;
        uint64_t tick = 0;
    };

    static bool activo() {
        return Mod::get()->getSettingValue<bool>("enabled");
    }

    static uint64_t ventana() {
        auto w = Mod::get()->getSettingValue<int64_t>("window");
        return (uint64_t)std::clamp<int64_t>(w, 0, 16);
    }

    bool pushButton(PlayerButton btn) {
        if (btn != PlayerButton::Jump || !activo() || ventana() == 0) {
            return PlayerObject::pushButton(btn);
        }
        auto f = m_fields.self();
        f->queue.emplace_back(f->tick + ventana(), true);
        return true;
    }

    bool releaseButton(PlayerButton btn) {
        if (btn != PlayerButton::Jump || !activo() || ventana() == 0) {
            return PlayerObject::releaseButton(btn);
        }
        auto f = m_fields.self();
        f->queue.emplace_back(f->tick + ventana(), false);
        return true;
    }

    void update(float dt) override {
        auto f = m_fields.self();
        ++f->tick;

        if (m_isDead) {
            f->queue.clear();
        } else {
            // si desactivas el mod o pones ventana 0, suelta todo lo pendiente
            bool vaciar = !activo() || ventana() == 0;
            while (!f->queue.empty() && (vaciar || f->queue.front().first <= f->tick)) {
                const bool pulsacion = f->queue.front().second;
                f->queue.pop_front();
                if (pulsacion) (void)PlayerObject::pushButton(PlayerButton::Jump);
                else           (void)PlayerObject::releaseButton(PlayerButton::Jump);
            }
        }

        PlayerObject::update(dt);
    }
};