/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "hap_module_info.h"

#include "json_util.h"
#include "message_parcel.h"
#include "nlohmann/json.hpp"
#include "parcel_macro.h"
#include "string_ex.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
const std::string HAP_MODULE_INFO_NAME = "name";
const std::string HAP_MODULE_INFO_MODULE_NAME = "moduleName";
const std::string HAP_MODULE_INFO_DESCRIPTION = "description";
const std::string HAP_MODULE_INFO_DESCRIPTION_ID = "descriptionId";
const std::string HAP_MODULE_INFO_ICON_PATH = "iconPath";
const std::string HAP_MODULE_INFO_LABEL = "label";
const std::string HAP_MODULE_INFO_LABEL_ID = "labelId";
const std::string HAP_MODULE_INFO_BACKGROUND_IMG = "backgroundImg";
const std::string HAP_MODULE_INFO_MAIN_ABILITY = "mainAbility";
const std::string HAP_MODULE_INFO_SRC_PATH = "srcPath";
const std::string HAP_MODULE_INFO_SUPPORTED_MODES = "supportedModes";
const std::string HAP_MODULE_INFO_REQ_CAPABILITIES = "reqCapabilities";
const std::string HAP_MODULE_INFO_DEVICE_TYPES = "deviceTypes";
const std::string HAP_MODULE_INFO_ABILITY_INFOS = "abilityInfos";
const std::string HAP_MODULE_INFO_COLOR_MODE = "colorMode";
const std::string HAP_MODULE_INFO_BUNDLE_NAME = "bundleName";
const std::string HAP_MODULE_INFO_MAIN_ELEMENTNAME = "mainElementName";
const std::string HAP_MODULE_INFO_PAGES = "pages";
const std::string HAP_MODULE_INFO_PROCESS = "process";
const std::string HAP_MODULE_INFO_RESOURCE_PATH = "resourcePath";
const std::string HAP_MODULE_INFO_SRC_ENTRANCE = "srcEntrance";
const std::string HAP_MODULE_INFO_UI_SYNTAX = "uiSyntax";
const std::string HAP_MODULE_INFO_VIRTUAL_MACHINE = "virtualMachine";
const std::string HAP_MODULE_INFO_DELIVERY_WITH_INSTALL = "deliveryWithInstall";
const std::string HAP_MODULE_INFO_INSTALLATION_FREE = "installationFree";
const std::string HAP_MODULE_INFO_IS_MODULE_JSON = "isModuleJson";
const std::string HAP_MODULE_INFO_IS_STAGE_BASED_MODEL = "isStageBasedModel";
const std::string HAP_MODULE_INFO_IS_REMOVABLE = "isRemovable";
const std::string HAP_MODULE_INFO_MODULE_TYPE = "moduleType";
const std::string HAP_MODULE_INFO_EXTENSION_INFOS = "extensionInfos";
const std::string HAP_MODULE_INFO_META_DATA = "metadata";
const std::string HAP_MODULE_INFO_DEPENDENCIES = "dependencies";
const std::string HAP_MODULE_INFO_UPGRADE_FLAG = "upgradeFlag";
}

bool HapModuleInfo::ReadFromParcel(Parcel &parcel)
{
    MessageParcel *messageParcel = reinterpret_cast<MessageParcel *>(&parcel);
    if (!messageParcel) {
        APP_LOGE("Type conversion failed");
        return false;
    }
    uint32_t length = messageParcel->ReadUint32();
    if (length == 0) {
        APP_LOGE("Invalid data length");
        return false;
    }
    const char *data = reinterpret_cast<const char *>(messageParcel->ReadRawData(length));
    if (!data) {
        APP_LOGE("Fail to read raw data, length = %{public}d", length);
        return false;
    }
    nlohmann::json jsonObject = nlohmann::json::parse(data, nullptr, false);
    if (jsonObject.is_discarded()) {
        APP_LOGE("failed to parse HapModuleInfo");
        return false;
    }
    *this = jsonObject.get<HapModuleInfo>();
    return true;
}

HapModuleInfo *HapModuleInfo::Unmarshalling(Parcel &parcel)
{
    HapModuleInfo *info = new (std::nothrow) HapModuleInfo();
    if (info && !info->ReadFromParcel(parcel)) {
        APP_LOGW("read from parcel failed");
        delete info;
        info = nullptr;
    }
    return info;
}

bool HapModuleInfo::Marshalling(Parcel &parcel) const
{
    MessageParcel *messageParcel = reinterpret_cast<MessageParcel *>(&parcel);
    if (!messageParcel) {
        APP_LOGE("Type conversion failed");
        return false;
    }
    nlohmann::json json = *this;
    std::string str = json.dump();
    if (!messageParcel->WriteUint32(str.size() + 1)) {
        APP_LOGE("Failed to write data size");
        return false;
    }
    if (!messageParcel->WriteRawData(str.c_str(), str.size() + 1)) {
        APP_LOGE("Failed to write data");
        return false;
    }
    return true;
}

void to_json(nlohmann::json &jsonObject, const HapModuleInfo &hapModuleInfo)
{
    jsonObject = nlohmann::json {
        {HAP_MODULE_INFO_NAME, hapModuleInfo.name},
        {HAP_MODULE_INFO_MODULE_NAME, hapModuleInfo.moduleName},
        {HAP_MODULE_INFO_DESCRIPTION, hapModuleInfo.description},
        {HAP_MODULE_INFO_DESCRIPTION_ID, hapModuleInfo.descriptionId},
        {HAP_MODULE_INFO_ICON_PATH, hapModuleInfo.iconPath},
        {HAP_MODULE_INFO_LABEL, hapModuleInfo.label},
        {HAP_MODULE_INFO_LABEL_ID, hapModuleInfo.labelId},
        {HAP_MODULE_INFO_BACKGROUND_IMG, hapModuleInfo.backgroundImg},
        {HAP_MODULE_INFO_MAIN_ABILITY, hapModuleInfo.mainAbility},
        {HAP_MODULE_INFO_SRC_PATH, hapModuleInfo.srcPath},
        {HAP_MODULE_INFO_SUPPORTED_MODES, hapModuleInfo.supportedModes},
        {HAP_MODULE_INFO_REQ_CAPABILITIES, hapModuleInfo.reqCapabilities},
        {HAP_MODULE_INFO_DEVICE_TYPES, hapModuleInfo.deviceTypes},
        {HAP_MODULE_INFO_ABILITY_INFOS, hapModuleInfo.abilityInfos},
        {HAP_MODULE_INFO_COLOR_MODE, hapModuleInfo.colorMode},
        {HAP_MODULE_INFO_BUNDLE_NAME, hapModuleInfo.bundleName},
        {HAP_MODULE_INFO_MAIN_ELEMENTNAME, hapModuleInfo.mainElementName},
        {HAP_MODULE_INFO_PAGES, hapModuleInfo.pages},
        {HAP_MODULE_INFO_PROCESS, hapModuleInfo.process},
        {HAP_MODULE_INFO_RESOURCE_PATH, hapModuleInfo.resourcePath},
        {HAP_MODULE_INFO_SRC_ENTRANCE, hapModuleInfo.srcEntrance},
        {HAP_MODULE_INFO_UI_SYNTAX, hapModuleInfo.uiSyntax},
        {HAP_MODULE_INFO_VIRTUAL_MACHINE, hapModuleInfo.virtualMachine},
        {HAP_MODULE_INFO_DELIVERY_WITH_INSTALL, hapModuleInfo.deliveryWithInstall},
        {HAP_MODULE_INFO_INSTALLATION_FREE, hapModuleInfo.installationFree},
        {HAP_MODULE_INFO_IS_MODULE_JSON, hapModuleInfo.isModuleJson},
        {HAP_MODULE_INFO_IS_STAGE_BASED_MODEL, hapModuleInfo.isStageBasedModel},
        {HAP_MODULE_INFO_IS_REMOVABLE, hapModuleInfo.isRemovable},
        {HAP_MODULE_INFO_UPGRADE_FLAG, hapModuleInfo.upgradeFlag},
        {HAP_MODULE_INFO_MODULE_TYPE, hapModuleInfo.moduleType},
        {HAP_MODULE_INFO_EXTENSION_INFOS, hapModuleInfo.extensionInfos},
        {HAP_MODULE_INFO_META_DATA, hapModuleInfo.metadata},
        {HAP_MODULE_INFO_DEPENDENCIES, hapModuleInfo.dependencies}
    };
}

void from_json(const nlohmann::json &jsonObject, HapModuleInfo &hapModuleInfo)
{
    const auto &jsonObjectEnd = jsonObject.end();
    int32_t parseResult = ERR_OK;
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_NAME,
        hapModuleInfo.name,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_MODULE_NAME,
        hapModuleInfo.moduleName,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_DESCRIPTION,
        hapModuleInfo.description,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_DESCRIPTION_ID,
        hapModuleInfo.descriptionId,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_ICON_PATH,
        hapModuleInfo.iconPath,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_LABEL,
        hapModuleInfo.label,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_LABEL_ID,
        hapModuleInfo.labelId,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_BACKGROUND_IMG,
        hapModuleInfo.backgroundImg,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_MAIN_ABILITY,
        hapModuleInfo.mainAbility,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_SRC_PATH,
        hapModuleInfo.srcPath,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_SUPPORTED_MODES,
        hapModuleInfo.supportedModes,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_REQ_CAPABILITIES,
        hapModuleInfo.reqCapabilities,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_DEVICE_TYPES,
        hapModuleInfo.deviceTypes,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::vector<AbilityInfo>>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_ABILITY_INFOS,
        hapModuleInfo.abilityInfos,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::OBJECT);
    GetValueIfFindKey<ModuleColorMode>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_COLOR_MODE,
        hapModuleInfo.colorMode,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_BUNDLE_NAME,
        hapModuleInfo.bundleName,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_MAIN_ELEMENTNAME,
        hapModuleInfo.mainElementName,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_PAGES,
        hapModuleInfo.pages,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_PROCESS,
        hapModuleInfo.process,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_RESOURCE_PATH,
        hapModuleInfo.resourcePath,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_SRC_ENTRANCE,
        hapModuleInfo.srcEntrance,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_UI_SYNTAX,
        hapModuleInfo.uiSyntax,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_VIRTUAL_MACHINE,
        hapModuleInfo.virtualMachine,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_DELIVERY_WITH_INSTALL,
        hapModuleInfo.deliveryWithInstall,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_INSTALLATION_FREE,
        hapModuleInfo.installationFree,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_IS_MODULE_JSON,
        hapModuleInfo.isModuleJson,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_IS_STAGE_BASED_MODEL,
        hapModuleInfo.isStageBasedModel,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::map<std::string, bool>>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_IS_REMOVABLE,
        hapModuleInfo.isRemovable,
        JsonType::OBJECT,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int32_t>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_UPGRADE_FLAG,
        hapModuleInfo.upgradeFlag,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<ModuleType>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_MODULE_TYPE,
        hapModuleInfo.moduleType,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::vector<ExtensionAbilityInfo>>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_EXTENSION_INFOS,
        hapModuleInfo.extensionInfos,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::OBJECT);
    GetValueIfFindKey<std::vector<Metadata>>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_META_DATA,
        hapModuleInfo.metadata,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::OBJECT);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        HAP_MODULE_INFO_DEPENDENCIES,
        hapModuleInfo.dependencies,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
}
}  // namespace AppExecFwk
}  // namespace OHOS
