// include/server.hpp
#ifndef SERVER_HPP
#define SERVER_HPP

#include <nlohmann/json.hpp>
#include <httplib.h>
#include <cstdlib>
#include <string>
#include <ctime>

inline bool reportar(const std::string &mensaje)
{
    try
    {
        std::time_t t = std::time(nullptr);
        std::string str_time = std::ctime(&t);
        str_time.pop_back();

        std::string carga = "```\nTIME: " + str_time + "\n\n" + mensaje + "\n```";

        nlohmann::json discord_payload = {
            {"content", carga},
            {"username", std::getenv("ID_PC")}};

        if (discord_payload["username"].is_null())
        {
            return false;
        }

        httplib::Client client("https://discord.com");

        std::string webhook_url = std::getenv("WEBHOOK_URL");
        if (webhook_url.empty())
        {
            webhook_url = "/api/webhooks/yourWebhookID/yourWebhookToken";
        }

        webhook_url = "https://discord.com" + webhook_url;

        auto response = client.Post(webhook_url.c_str(), discord_payload.dump(), "application/json");

        if (response && response->status == 200)
        {
            return true;
        }

        return false;
    }
    catch (const std::exception &)
    {
        return false;
    }
}

#endif
