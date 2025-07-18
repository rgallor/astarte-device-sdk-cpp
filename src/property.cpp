// (C) Copyright 2025, SECO Mind Srl
//
// SPDX-License-Identifier: Apache-2.0

#include "astarte_device_sdk/property.hpp"

#include <optional>
#include <sstream>
#include <string>

#include "astarte_device_sdk/data.hpp"

namespace AstarteDeviceSdk {

AstartePropertyIndividual::AstartePropertyIndividual(const std::optional<AstarteData> &data)
    : data_(data) {}

auto AstartePropertyIndividual::get_value() const -> const std::optional<AstarteData> & {
  return data_;
}

auto AstartePropertyIndividual::operator==(const AstartePropertyIndividual &other) const -> bool {
  return this->get_value() == other.get_value();
}
auto AstartePropertyIndividual::operator!=(const AstartePropertyIndividual &other) const -> bool {
  return this->get_value() != other.get_value();
}

}  // namespace AstarteDeviceSdk
