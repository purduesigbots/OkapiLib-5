/**
 * @author Ryan Benasutti, WPI
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _OKAPI_BUTTON_HPP_
#define _OKAPI_BUTTON_HPP_

#include "api.h"

namespace okapi {
class Button {
  public:
  virtual ~Button();

  /**
   * Return whether the button is current pressed.
   **/
  virtual bool isPressed() = 0;

  /**
   * Return whether the state of the button changed since the last time this method was
   * called.
   **/
  virtual bool changed() = 0;

  /**
   * Return whether the state of the button changed to being pressed since the last time this method
   * was called.
   **/
  virtual bool changedToPressed() = 0;

  /**
   * Return whether the state of the button to being not pressed changed since the last time this
   * method was called.
   **/
  virtual bool changedToReleased() = 0;
};
} // namespace okapi

#endif
