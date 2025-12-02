#pragma once
#include <chrono>
#include <deque>

namespace gl {

    /**
     * @brief gl::governor — a framerate governor using std::chrono::milliseconds.
     *
     * Supports sleep-based and busy-wait pacing, with FPS averaging and debugging.
     */
    class governor {
    public:
        governor();

        /// Set target frames per second (0 = uncapped)
        void setFPS(float fps);

        /// Enable or disable debug output
        void setDebug(bool enabled);

        /// Set number of frames to average for FPS measurement
        void setAverageSampleCount(size_t count);

        /// Perform a regulated frame cycle (uses sleep-based pacing)
        void update();

        /// Sleep-based pacing — uses std::this_thread::sleep_for
        void sleep();

        /// Busy-wait pacing — actively waits until target time reached
        void busy();

        void busyMin();

        /// Returns delta time between frames in milliseconds
        double getDeltaTimeMS() const;

        /// Returns instantaneous FPS (1 / delta)
        double getCurrentFPS() const;

        /// Returns average FPS over recent frames
        double getAverageFPS() const;

        /// Reset internal timers
        void reset();

    private:
        using clock = std::chrono::steady_clock;
        using time_point = std::chrono::time_point<clock>;
        using milliseconds = std::chrono::milliseconds;

        float m_targetFPS;
        milliseconds m_targetFrameDuration;
        time_point m_lastFrameTime;
        milliseconds m_deltaTime;
        bool m_firstCall;
        bool m_debugEnabled;

        // Average FPS tracking
        std::deque<double> m_recentFPS;
        size_t m_maxSamples;
    };

} // namespace gl
