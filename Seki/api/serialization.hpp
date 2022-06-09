#ifndef SERIALIZATION_H
#define SERIALIZATION_H

namespace api {
  struct GameInfo {
  std::string id;
  std::vector<std::string> players_joined;
  std::string game_name;
  int event_clock;
  std::string state;
  bool is_ready;
  bool is_finished;
  std::string winner;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameInfo, id, players_joined, game_name,
      event_clock, state, is_ready, is_finished, winner)
  };
}

namespace nlohmann {
  template <>
  struct adl_serializer<core::GameState> {
    static void to_json(nlohmann::json &json, const core::GameState &state) {
      auto current_player = state.get_is_r() ? "R" : "C";
      auto field_width = state.get_field().get_shape()[1];
      auto field_height = state.get_field().get_shape()[0];

      std::vector<int> v;
      v.reserve(field_width * field_height);

      for (int y = 0; y < field_height; ++y) {
        for (int x = 0; x < field_width; ++x) {
          v.push_back(state.get_field().get(x, y));
        }
      }

      auto type = api::detail::match<core::SekiType, std::string, 2>(
        {core::SekiType::SEKI, core::SekiType::DSEKI},
        {"seki", "dseki"},
        state.get_type());
      
      if (!type) {
        throw std::runtime_error("Invalid game type");
      }

      auto passtype = api::detail::match<core::PassType, std::string, 4>(
        {
          core::PassType::R_PASS, core::PassType::C_PASS,
          core::PassType::ANY_PASS, core::PassType::NO_PASS,
        },
        {"R", "C", "RC", ""},
        state.get_passtype());

      if (!passtype) {
        throw std::runtime_error("Invalid pass type");
      }

      int passcount = state.get_pass_count();

      json = nlohmann::json({
        {"CurrentPlayer", current_player},
        {"Width", field_width},
        {"Height", field_height},
        {"FlattenedField", v},
        {"Type", *type},
        {"Pp", *passtype},
        {"PassCount", passcount},
      });
    }

    static core::GameState from_json(const nlohmann::json &json) {
      auto current_player = json.at("CurrentPlayer").get<std::string>();
      bool is_r = current_player == "R";
      if (current_player != "R" && current_player != "C") {
        throw std::runtime_error("Invalid player specifided");
      }

      auto width = json.at("Width").get<int>();
      auto height = json.at("Height").get<int>();
      if (width < 1 || height < 1
        || static_cast<size_t>(width) > core::max_field_dim
        || static_cast<size_t>(height) > core::max_field_dim) {
        throw std::runtime_error("Invalid field size");
      }

      auto uwidth = static_cast<size_t>(width);
      auto uheight = static_cast<size_t>(height);

      auto flat_field_data = json.at("FlattenedField").get<std::vector<int>>();
      if (flat_field_data.size() != uwidth * uheight) {
        throw std::runtime_error("Field size mismatch");
      }

      std::vector<std::vector<int>> field_data(uheight, std::vector<int>(uwidth, 0));

      unsigned long long sum = 0;

      for(size_t y = 0; y < uheight; ++y) {
        for(size_t x = 0; x < uwidth; ++x) {
          auto value = flat_field_data[x + y * uwidth];
          if (value < 0) {
            throw std::runtime_error("Negative items are not allowed");
          }
          sum += (unsigned int)value;
          field_data[y][x] = value;
        }
      }

      if (sum > 20) {
        throw std::runtime_error("Sum is too big");
      }

      auto type = api::detail::match<std::string, core::SekiType, 2>(
        {"seki", "dseki"},
        {core::SekiType::SEKI, core::SekiType::DSEKI},
        json.at("Type").get<std::string>());
      
      if (!type) {
        throw std::runtime_error("Invalid game type");
      }

      auto pass = api::detail::match<std::string, core::PassType, 4>(
        {"R", "C", "RC", ""},
        {
          core::PassType::R_PASS, core::PassType::C_PASS,
          core::PassType::ANY_PASS, core::PassType::NO_PASS,
        },
        json.at("Pp").get<std::string>());

      if (!pass) {
        throw std::runtime_error("Invalid pass type");
      }

      auto result = core::GameState(
        core::Field(field_data),
        is_r,
        1,
        *type,
        *pass);

      auto passcount = api::detail::get_key_or_default<int>(json, "PassCount", 0);
      if (passcount >= 0 && passcount <= 2) {
        result.set_pass_count(passcount);
        return result;
      }
      throw std::runtime_error("Pass count must be in [0, 2]");
    }
  };

  template <>
  struct adl_serializer<core::Move> {
    static core::Move from_json(const nlohmann::json &json) {
      int x = json.at("X").get<int>();
      int y = json.at("Y").get<int>();
      auto result = core::Move(0, x, y);
      result.is_pass = json.at("IsPass").get<bool>();
      return result;
    }

    static void to_json(nlohmann::json &json, const core::Move &m) {
      json = nlohmann::json({
        {"X", m.x},
        {"Y", m.y},
        {"IsPass", m.is_pass},
        {"Value", m.value},
      });
    }
  };

}

#endif // SERIALIZATION