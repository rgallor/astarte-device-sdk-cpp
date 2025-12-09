// (C) Copyright 2025, SECO Mind Srl
//
// SPDX-License-Identifier: Apache-2.0

#include <spdlog/spdlog.h>

#include <cstdlib>
#include <iostream>
#include <thread>

#include "astarte_device_sdk/data.hpp"
#include "astarte_device_sdk/device_grpc.hpp"
#include "astarte_device_sdk/formatter.hpp"
#include "astarte_device_sdk/msg.hpp"

using AstarteDeviceSdk::AstarteData;
using AstarteDeviceSdk::AstarteDatastreamIndividual;
using AstarteDeviceSdk::AstarteDatastreamObject;
using AstarteDeviceSdk::AstarteDeviceGRPC;
using AstarteDeviceSdk::AstarteMessage;
using AstarteDeviceSdk::AstartePropertyIndividual;

void reception_handler(std::shared_ptr<AstarteDeviceGRPC> msghub_client) {
  while (true) {
    auto incoming = msghub_client->poll_incoming(std::chrono::milliseconds(100));
    if (incoming.has_value()) {
      AstarteMessage msg(incoming.value());
      spdlog::info("Received message.");
      spdlog::info("Interface name: {}", msg.get_interface());
      spdlog::info("Path: {}", msg.get_path());

      // this removes the "/request/" substring in the path and keeps only the objectPath value
      auto object_path = msg.get_path().substr(9);
      spdlog::info("objectPath: {}", object_path);

      if (msg.is_datastream()) {
        if (msg.is_individual()) {
          spdlog::info("Type: individual datastream");
          const auto& data(msg.into<AstarteDatastreamIndividual>());
          spdlog::info("Value: {}", data);
        } else {
          spdlog::info("Type: object datastream");
          const auto& data(msg.into<AstarteDatastreamObject>());
          spdlog::info("Value: {}", data);

          // access every endpoint and its respective value
          for (const auto& [endpoint, value] : data.get_raw_data()) {
            spdlog::info("endpoint {} has value: {}", endpoint, value);
          }
        }
      } else {
        spdlog::info("Type: individual property");
        const auto& data(msg.into<AstartePropertyIndividual>());
        spdlog::info("Value: {}", data);
      }
    }
  }
}

int main(int argc, char** argv) {
  spdlog::set_level(spdlog::level::debug);
  std::string server_addr = "localhost:50051";
  std::string node_id("aa04dade-9401-4c37-8c6a-d8da15b083ae");
  std::shared_ptr<AstarteDeviceGRPC> msghub_client =
      std::make_shared<AstarteDeviceGRPC>(server_addr, node_id);

  std::filesystem::path server_property_interface_file_path =
      "samples/longpath/interfaces/org.astarte-platform.cpp.examples.AggregateLongPath.json";
  msghub_client->add_interface_from_json(server_property_interface_file_path);

  msghub_client->connect();

  do {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  } while (!msghub_client->is_connected(std::chrono::milliseconds(100)));

  // Start a reception thread for the Astarte device
  auto reception_thread = std::thread(reception_handler, msghub_client);

  // Wait to receive some messages
  std::this_thread::sleep_for(std::chrono::seconds(20));

  msghub_client->disconnect();

  std::this_thread::sleep_for(std::chrono::seconds(3));
  std::exit(EXIT_SUCCESS);
}
