// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include <string>
#include <vector>
#include "json.h"
#include "../TerminalSettingsModel/JsonUtils.h"

// M(icrosoft) T(erminal) P(ersistence) M(odel)
#define MTPM_ROOT_FIELDS(XX) \
    XX(tabs, std::vector<Tab>)

// M(icrosoft) T(erminal) P(ersistence) M(odel)
#define MTPM_TAB_FIELDS(XX) \
    XX(profile, std::wstring)

#define MTPM_MODELS(XX) \
    XX(Root, MTPM_ROOT_FIELDS) \
    XX(Tab, MTPM_TAB_FIELDS)

namespace Microsoft::Terminal::Persistence
{
    namespace Model
    {
#define MTPM_XX(NAME, GENERATOR) struct NAME;
        MTPM_MODELS(MTPM_XX)
#undef MTPM_XX

#define MTPM_XX_SUB(NAME, TYPE) TYPE NAME;
#define MTPM_XX(NAME, GENERATOR)   \
        struct NAME {              \
            GENERATOR(MTPM_XX_SUB) \
        };
        MTPM_MODELS(MTPM_XX)
#undef MTPM_XX
#undef MTPM_XX_SUB
    }
}

namespace Microsoft::Terminal::Settings::Model::JsonUtils
{
#define MTPM_XX_GET(NAME, TYPE) GetValueForKey(json, #NAME, val.NAME);
#define MTPM_XX_SET(NAME, TYPE) SetValueForKey(json, #NAME, val.NAME);
#define MTPM_XX(NAME, GENERATOR)                                               \
        template<>                                                             \
        class ConversionTrait<::Microsoft::Terminal::Persistence::Model::NAME> \
        {                                                                      \
            using Model = ::Microsoft::Terminal::Persistence::Model::NAME;     \
                                                                               \
        public:                                                                \
            Model FromJson(const Json::Value& json) const                      \
            {                                                                  \
                Model val;                                                     \
                GENERATOR(MTPM_XX_GET)                                         \
                return val;                                                    \
            }                                                                  \
                                                                               \
            bool CanConvert(const Json::Value&) const                          \
            {                                                                  \
                return true;                                                   \
            }                                                                  \
                                                                               \
            Json::Value ToJson(const Model& val) const                         \
            {                                                                  \
                using namespace ::Microsoft::Terminal::Persistence::Model;     \
                Json::Value json;                                              \
                GENERATOR(MTPM_XX_SET)                                         \
                return json;                                                   \
            }                                                                  \
                                                                               \
            std::string TypeDescription() const                                \
            {                                                                  \
                return #NAME;                                                  \
            }                                                                  \
        };
        MTPM_MODELS(MTPM_XX)
#undef MTPM_XX
#undef MTPM_XX_SET
#undef MTPM_XX_GET
}
