#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <QtGlobal>
namespace GameConfig {
    namespace Field {
        inline constexpr int CELL_SIZE = 20;
        inline constexpr int WIDTH = 30;
        inline constexpr int HEIGHT = 30;
    }
    namespace Snake {
        inline constexpr int INITIAL_SIZE = 4;
        inline constexpr int PART_SIZE = 10;
    }
    namespace Timing {
        inline constexpr int INITIAL_INTERVAL_MS = 150;
    }
    namespace Food {
        inline constexpr int REWARD = 10;
    }
}

#endif // GAMESETTINGS_H
