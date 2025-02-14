#pragma once
#include "token.hpp"
#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>

namespace pog {

    class Environment {
    public:
        Environment() = default;
        explicit Environment(std::shared_ptr<Environment> enclosing)
            : enclosing(std::move(enclosing)) {}

        void define(const std::string& name, std::any value);
        void assign(const Token& name, std::any value);
        std::any get(const Token& name);

    private:
        std::shared_ptr<Environment> enclosing;
        std::unordered_map<std::string, std::any> values;
    };

} // namespace pog