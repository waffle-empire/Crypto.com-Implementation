#pragma once
#include "common.hpp"
#include "util/logger.hpp"

namespace crypto_com_impl
{
	class Settings;
	inline Settings* g_settings{};

	class Settings {
		nlohmann::json default_options;
		nlohmann::json options;

		struct API
		{
			std::string key;
			std::string secret;
		};
		

	public:
		Settings()
		{
			g_settings = this;
		}

		~Settings()
		{
			g_settings = nullptr;
		}

		API api{};

		void from_json(const nlohmann::json& j)
		{
            this->api.key = j["api"]["key"];
			this->api.secret = j["api"]["secret"];
		}

		nlohmann::json to_json()
		{
			return nlohmann::json{
				{
					"api", {
                        { "key", this->api.key },
						{ "secret", this->api.secret },
					}
				}
			};
		}

		void attempt_save()
		{
			auto j = this->to_json();

			if (deep_compare(this->options, j, true))
				this->save();
		}

		bool load()
		{
			this->default_options = this->to_json();

			std::ifstream file(this->settings_location);
			if (!file.is_open())
			{
				this->write_default_config();

				file.open(this->settings_location);
			}

			try
			{
				file >> this->options;
			}
			catch (const std::exception&)
			{
				g_log->warning("SETTINGS", "Detected corrupt settings, writing default config...");

				this->write_default_config();

				return this->load();
			}

			bool should_save = this->deep_compare(this->options, this->default_options);

			this->from_json(this->options);

			if (should_save)
			{
				g_log->verbose("SETTINGS", "Updating settings...");
				save();
			}

			return true;
		}

	private:
		const char* settings_location = "./settings.json";

		bool deep_compare(nlohmann::json& current_settings, const nlohmann::json& default_settings, bool compare_value = false)
		{
			bool should_save = false;

			for (auto& e : default_settings.items())
			{
				const std::string& key = e.key();

				if (current_settings.count(key) == 0 || (compare_value && current_settings[key] != e.value()))
				{
					current_settings[key] = e.value();

					should_save = true;
				}
				else if (current_settings[key].is_structured() && e.value().is_structured())
				{
					if (deep_compare(current_settings[key], e.value(), compare_value))
						should_save = true;
				}
				else if (!current_settings[key].is_structured() && e.value().is_structured()) {
					current_settings[key] = e.value();

					should_save = true;
				}
			}

			return should_save;
		}

		bool save()
		{
			std::ofstream file(this->settings_location, std::ios::out | std::ios::trunc);
			file << this->to_json().dump(4);
			file.close();

			return true;
		}

		bool write_default_config()
		{
			std::ofstream file(this->settings_location, std::ios::out);
			file << this->to_json().dump(4);
			file.close();

			return true;
		}
	};
}