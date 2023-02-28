/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FOUNDATION_APPEXECFWK_SERVICES_SR_INCLUDE_SERVICE_ROUTER_UTIL_H
#define FOUNDATION_APPEXECFWK_SERVICES_SR_INCLUDE_SERVICE_ROUTER_UTIL_H

#include <string>
#include <vector>

#include "bundle_info.h"
#include "service_info.h"
#include "bundle_constants.h"
#include "inner_service_info.h"
#include "string_ex.h"
#include "sr_constants.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
    static std::unordered_map<std::string, ExtensionServiceType> SERVICE_TYPE_MAP = {
        {"share", ExtensionServiceType::SHARE}};
}
class BundleInfoResolveUtil {
public:
static bool ResolveBundleInfo(const BundleInfo &bundleInfo, std::vector<IntentInfo> &intentInfos,
    std::vector<ServiceInfo> &serviceInfos, const AppInfo &appInfo)
{
    if (bundleInfo.name.empty()) {
        APP_LOGE("ConvertBundleInfo, bundleInfo invalid");
        return false;
    }
    ResolveAbilityInfos(bundleInfo.abilityInfos, intentInfos, appInfo);
    ResolveExtAbilityInfos(bundleInfo.extensionInfos, intentInfos, serviceInfos, appInfo);
    if (intentInfos.empty() && serviceInfos.empty()) {
        APP_LOGI("ResolveBundleInfo, not support, bundleName: %{public}s", bundleInfo.name.c_str());
        return false;
    }
    return true;
}

static ExtensionServiceType findExtensionServiceType(const std::string serviceType)
{
    if (serviceType.empty()) {
        return ExtensionServiceType::UNSPECIFIED;
    }

    auto item = SERVICE_TYPE_MAP.find(LowerStr(serviceType));
    if (item != SERVICE_TYPE_MAP.end()) {
        return item->second;
    }
    return ExtensionServiceType::UNSPECIFIED;
}

private:
static void ResolveAbilityInfos(const std::vector<AbilityInfo> &abilityInfos, std::vector<IntentInfo> &intentInfos
    , const AppInfo appInfo)
{
    if (abilityInfos.empty()) {
        return;
    }
    for (const auto &abilityInfo : abilityInfos) {
        ConvertAbilityToIntents(abilityInfo, intentInfos, appInfo);
    }
}

static void ResolveExtAbilityInfos(const std::vector<ExtensionAbilityInfo> &extensionInfos,
    std::vector<IntentInfo> &intentInfos, std::vector<ServiceInfo> &serviceInfos, const AppInfo appInfo)
{
    if (extensionInfos.empty()) {
        return;
    }
    for (const auto &extensionInfo : extensionInfos) {
        ConvertExtAbilityToIntents(extensionInfo, intentInfos, appInfo);
        ConvertExtAbilityToService(extensionInfo, serviceInfos, appInfo);
    }
}

static void ConvertAbilityToIntents(const AbilityInfo &abilityInfo, std::vector<IntentInfo> &intentInfos,
    const AppInfo appInfo)
{
    std::string supportIntent = GetAbilityMetadataValue(abilityInfo, SrConstants::METADATA_SUPPORTINTENT_KEY);
    if (supportIntent.empty()) {
        return;
    }
    std::vector<std::string> intentNames;
    SplitStr(supportIntent, SrConstants::MUTIL_SPLIT_KEY, intentNames);
    for (std::string &name : intentNames) {
        IntentInfo intentInfo;
        intentInfo.intentName = name;
        intentInfo.abilityName = abilityInfo.name;
        intentInfo.moduleName = abilityInfo.moduleName;
        intentInfo.bundleName = abilityInfo.bundleName;
        intentInfo.componentType = ComponentType::UI_ABILITY;
        intentInfo.appInfo = appInfo;
        intentInfos.emplace_back(intentInfo);
        APP_LOGI("AbilityToIntents, bundle: %{public}s ,ability: %{public}s, intentName: %{public}s",
            abilityInfo.bundleName.c_str(), abilityInfo.name.c_str(), name.c_str());
    }
}

static void ConvertExtAbilityToIntents(const ExtensionAbilityInfo &extAbilityInfo,
    std::vector<IntentInfo> &intentInfos, const AppInfo appInfo)
{
    if (extAbilityInfo.type != ExtensionAbilityType::FORM && extAbilityInfo.type != ExtensionAbilityType::UI) {
        return;
    }
    std::string supportIntent = GetExtAbilityMetadataValue(extAbilityInfo, SrConstants::METADATA_SUPPORTINTENT_KEY);
    if (supportIntent.empty()) {
        return;
    }
    std::vector<std::string> intents;
    SplitStr(supportIntent, SrConstants::MUTIL_SPLIT_KEY, intents);
    for (std::string &intentAndCard : intents) {
        IntentInfo intentInfo;
        intentInfo.abilityName = extAbilityInfo.name;
        intentInfo.moduleName = extAbilityInfo.moduleName;
        intentInfo.bundleName = extAbilityInfo.bundleName;
        intentInfo.appInfo = appInfo;
        if (extAbilityInfo.type == ExtensionAbilityType::UI) {
            intentInfo.intentName = intentAndCard;
            intentInfo.componentType = ComponentType::UI_EXTENSION;
            intentInfos.emplace_back(intentInfo);
            APP_LOGI("UIExtToIntents, bundle: %{public}s, abilityName: %{public}s, intentName: %{public}s",
                extAbilityInfo.bundleName.c_str(), extAbilityInfo.name.c_str(), intentAndCard.c_str());
        } else {
            std::vector<std::string> intentNameAndCardName;
            SplitStr(intentAndCard, SrConstants::FORM_INTENTCARD_SPLIT_KEY, intentNameAndCardName);
            if (intentNameAndCardName.size() == SrConstants::FORM_INTENTCARD_SPLIT_SIZE) {
                intentInfo.intentName = intentNameAndCardName[0];
                intentInfo.cardName = intentNameAndCardName[1];
                intentInfo.componentType = ComponentType::FORM;
                intentInfos.emplace_back(intentInfo);
                APP_LOGI("FormToIntents, bundle: %{public}s, abilityName: %{public}s, intentName: %{public}s",
                    extAbilityInfo.bundleName.c_str(), extAbilityInfo.name.c_str(), intentInfo.intentName.c_str());
            } else {
                APP_LOGW("FormToIntents invalid supportIntent");
            }
        }
    }
}

static void ConvertExtAbilityToService(const ExtensionAbilityInfo &extAbilityInfo,
    std::vector<ServiceInfo> &serviceInfos, const AppInfo appInfo)
{
    if (extAbilityInfo.type != ExtensionAbilityType::UI) {
        return;
    }
    std::string serviceType = GetExtAbilityMetadataValue(extAbilityInfo, SrConstants::METADATA_SERVICE_TYPE_KEY);
    APP_LOGI("ToService, abilityName: %{public}s, serviceType: %{public}s",
        extAbilityInfo.name.c_str(), serviceType.c_str());
    auto item = SERVICE_TYPE_MAP.find(LowerStr(serviceType));
    ExtensionServiceType type = findExtensionServiceType(serviceType);
    if (type != ExtensionServiceType::UNSPECIFIED) {
        ServiceInfo serviceInfo;
        serviceInfo.appInfo = appInfo;
        serviceInfo.abilityName = extAbilityInfo.name;
        serviceInfo.moduleName = extAbilityInfo.moduleName;
        serviceInfo.bundleName = extAbilityInfo.bundleName;
        serviceInfo.serviceType = item->second;
        serviceInfo.iconId = extAbilityInfo.iconId;
        serviceInfo.labelId = extAbilityInfo.labelId;
        serviceInfo.descriptionId = extAbilityInfo.descriptionId;
        serviceInfo.permissions = extAbilityInfo.permissions;
        serviceInfos.emplace_back(serviceInfo);
    }
}

static std::string GetAbilityMetadataValue(const AbilityInfo &abilityInfo, const std::string &name)
{
    if (abilityInfo.metadata.empty()) {
        return Constants::EMPTY_STRING;
    }
    for (auto &metadata : abilityInfo.metadata) {
        if (name == metadata.name && !metadata.value.empty()) {
            return metadata.value;
        }
    }
    return Constants::EMPTY_STRING;
}

static std::string GetExtAbilityMetadataValue(const ExtensionAbilityInfo &extAbilityInfo, const std::string &name)
{
    if (extAbilityInfo.metadata.empty()) {
        return Constants::EMPTY_STRING;
    }
    for (auto &metadata : extAbilityInfo.metadata) {
        if (name == metadata.name && !metadata.value.empty()) {
            return metadata.value;
        }
    }
    return Constants::EMPTY_STRING;
}
}; // namespace ServiceRouterUtil
} // namespace AppExecFwk
} // namespace OHOS
#endif // FOUNDATION_APPEXECFWK_SERVICES_SR_INCLUDE_SERVICE_ROUTER_UTIL_H
