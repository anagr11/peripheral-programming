#include "governor.hpp"
#include <thread>
#include <iostream>

namespace gl {

    governor::governor()
        : m_targetFPS(60.0f),
        m_targetFrameDuration(milliseconds(1000 / 60)),
        m_deltaTime(milliseconds(0)),
        m_firstCall(true),
        m_debugEnabled(false),
        m_maxSamples(60) {
    }

    void governor::setFPS(float fps) {
        if (fps <= 0.0f) {
            m_targetFPS = 0.0f;
            m_targetFrameDuration = milliseconds(0);
            if (m_debugEnabled)
                std::cout << "[governor] Uncapped framerate" << std::endl;
            return;
        }

        m_targetFPS = fps;
        m_targetFrameDuration = milliseconds(static_cast<int>(1000.0f / fps));

        if (m_debugEnabled)
            std::cout << "[governor] Target FPS set to " << m_targetFPS
            << " (" << m_targetFrameDuration.count() << " ms/frame)" << std::endl;
    }

    void governor::setDebug(bool enabled) {
        m_debugEnabled = enabled;
        if (enabled)
            std::cout << "[governor] Debug mode enabled" << std::endl;
    }

    void governor::setAverageSampleCount(size_t count) {
        m_maxSamples = (count == 0) ? 1 : count;
        m_recentFPS.clear();
        if (m_debugEnabled)
            std::cout << "[governor] Average FPS sample count = " << m_maxSamples << std::endl;
    }

    void governor::update() {
        // Default to sleep-based pacing for most uses
        sleep();
    }

    void governor::sleep() {
        const auto now = clock::now();

        if (m_firstCall) {
            m_lastFrameTime = now;
            m_firstCall = false;
            m_deltaTime = milliseconds(0);
            if (m_debugEnabled)
                std::cout << "[governor] First frame initialized" << std::endl;
            return;
        }

        const auto elapsed = std::chrono::duration_cast<milliseconds>(now - m_lastFrameTime);
        m_deltaTime = elapsed;

        if (m_targetFPS > 0.0f && elapsed < m_targetFrameDuration) {
            const auto remaining = m_targetFrameDuration - elapsed;
            std::this_thread::sleep_for(remaining);
            const auto afterSleep = clock::now();
            m_deltaTime = std::chrono::duration_cast<milliseconds>(afterSleep - m_lastFrameTime);
        }

        m_lastFrameTime = clock::now();

        // Record FPS sample
        const double fps = getCurrentFPS();
        if (fps > 0.0) {
            m_recentFPS.push_back(fps);
            if (m_recentFPS.size() > m_maxSamples)
                m_recentFPS.pop_front();
        }

        if (m_debugEnabled) {
            std::cout << "[governor:sleep] dt = " << m_deltaTime.count()
                << " ms | FPS: " << fps
                << " | Avg: " << getAverageFPS()
                << std::endl;
        }
    }

    void governor::busyMin() {
        if(m_firstCall) {
            busy();
            return;
        }

        busy();
        std::this_thread::sleep_for(milliseconds(1));
    }

    void governor::busy() {
        const auto now = clock::now();

        if (m_firstCall) {
            m_lastFrameTime = now;
            m_firstCall = false;
            m_deltaTime = milliseconds(0);
            if (m_debugEnabled)
                std::cout << "[governor] First frame initialized (busy)" << std::endl;
            return;
        }

        const auto frameStart = m_lastFrameTime;
        const auto targetEnd = frameStart + m_targetFrameDuration;

        while (clock::now() < targetEnd) {
            // Active waiting (spin loop)
            std::this_thread::yield(); // be nice to the CPU scheduler
        }

        const auto nowAfterWait = clock::now();
        m_deltaTime = std::chrono::duration_cast<milliseconds>(nowAfterWait - m_lastFrameTime);
        m_lastFrameTime = nowAfterWait;

        const double fps = getCurrentFPS();
        if (fps > 0.0) {
            m_recentFPS.push_back(fps);
            if (m_recentFPS.size() > m_maxSamples)
                m_recentFPS.pop_front();
        }

        if (m_debugEnabled) {
            std::cout << "[governor:busy] dt = " << m_deltaTime.count()
                << " ms | FPS: " << fps
                << " | Avg: " << getAverageFPS()
                << std::endl;
        }
    }

    double governor::getDeltaTimeMS() const {
        return static_cast<double>(m_deltaTime.count());
    }

    double governor::getCurrentFPS() const {
        const auto ms = m_deltaTime.count();
        if (ms <= 0.0)
            return 0.0;
        return 1000.0 / static_cast<double>(ms);
    }

    double governor::getAverageFPS() const {
        if (m_recentFPS.empty())
            return 0.0;
        double sum = 0.0;
        for (auto f : m_recentFPS)
            sum += f;
        return sum / static_cast<double>(m_recentFPS.size());
    }

    void governor::reset() {
        m_firstCall = true;
        m_deltaTime = milliseconds(0);
        m_lastFrameTime = clock::now();
        m_recentFPS.clear();

        if (m_debugEnabled)
            std::cout << "[governor] Timer reset" << std::endl;
    }

} // namespace gl
