// SPDX-License-Identifier: BSD-2-Clause

// This code is part of the sfizz library and is licensed under a BSD 2-clause
// license. You should have receive a LICENSE.md file along with the code.
// If not, contact the sfizz maintainers at https://github.com/sfztools/sfizz

#pragma once
#include "Config.h"
#include "utility/NumericId.h"
#include "utility/LeakDetector.h"
#include <utility>
#include <string>
#include <vector>
#include <bitset>
#include <memory>

namespace sfz {
struct Region;
class MidiState;

struct Layer {
public:
    Layer(std::unique_ptr<Region> region, const MidiState& midiState);
    Layer(Region* region, const MidiState& midiState);
    ~Layer();

    Layer(const Layer&) = delete;
    Layer(Layer&&) = delete;
    Layer& operator=(const Layer&) = delete;
    Layer& operator=(Layer&&) = delete;

    /**
     * @brief Get the region that this layer operates on.
     */
    const Region& getRegion() const noexcept { return *region_; }
    /**
     * @brief Get the region that this layer operates on.
     */
    Region& getRegion() noexcept { return *region_; }

    /**
     * @brief Reset the activations to their initial states.
     */
    void initializeActivations();

    /**
     * @brief Given the current midi state, is the region switched on?
     *
     * @return true
     * @return false
     */
    bool isSwitchedOn() const noexcept;
    /**
     * @brief Register a new note on event. The region may be switched on or off using keys so
     * this function updates the keyswitches state.
     *
     * @param noteNumber
     * @param velocity
     * @param randValue a random value between 0 and 1 used to randomize a bit the region activations
     *                  and vary the samples
     * @return true if the region should trigger on this event.
     * @return false
     */
    bool registerNoteOn(int noteNumber, float velocity, float randValue) noexcept;
    /**
     * @brief Register a new note off event. The region may be switched on or off using keys so
     * this function updates the keyswitches state.
     *
     * @param noteNumber
     * @param velocity
     * @param randValue a random value between 0 and 1 used to randomize a bit the region activations
     *                  and vary the samples
     * @return true if the region should trigger on this event.
     * @return false
     */
    bool registerNoteOff(int noteNumber, float velocity, float randValue) noexcept;
    /**
     * @brief Register a new CC event. The region may be switched on or off using CCs so
     * this function checks if it indeeds need to activate or not.
     *
     * @param ccNumber
     * @param ccValue
     * @return true if the region should trigger on this event
     * @return false
     */
    bool registerCC(int ccNumber, float ccValue) noexcept;
    /**
     * @brief Register a new pitch wheel event.
     *
     * @param pitch
     */
    void registerPitchWheel(float pitch) noexcept;
    /**
     * @brief Register a new aftertouch event.
     *
     * @param aftertouch
     */
    void registerAftertouch(float aftertouch) noexcept;
    /**
     * @brief Register tempo
     *
     * @param secondsPerQuarter
     */
    void registerTempo(float secondsPerQuarter) noexcept;

    Region* const region_ {};
    bool regionOwned_ = false;

    // Started notes
    bool sustainPressed_ { false };
    bool sostenutoPressed_ { false };
    std::vector<std::pair<int, float>> delayedSustainReleases_;
    std::vector<std::pair<int, float>> delayedSostenutoReleases_;
    void delaySustainRelease(int noteNumber, float velocity) noexcept;
    void delaySostenutoRelease(int noteNumber, float velocity) noexcept;
    void storeSostenutoNotes() noexcept;
    void removeFromSostenutoReleases(int noteNumber) noexcept;
    bool isNoteSustained(int noteNumber) const noexcept;
    bool isNoteSostenutoed(int noteNumber) const noexcept;

    const MidiState& midiState_;
    bool keySwitched_ {};
    bool previousKeySwitched_ {};
    bool sequenceSwitched_ {};
    bool pitchSwitched_ {};
    bool bpmSwitched_ {};
    bool aftertouchSwitched_ {};
    std::bitset<config::numCCs> ccSwitched_;

    int sequenceCounter_ { 0 };

    LEAK_DETECTOR(Layer);
};

} // namespace sfz
