/**
 * @author Ryan Benasutti, WPI
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "okapi/device/motor/abstractMotor.hpp"

namespace okapi {
AbstractMotor::AbstractMotor(const uint8_t port, const bool reverse,
                             const motor_encoder_units_e_t encoder_units,
                             const motor_gearset_e_t gearset)
  : pros::Motor::Motor(port, gearset, reverse, encoder_units) {
}

AbstractMotor::~AbstractMotor() = default;
} // namespace okapi
