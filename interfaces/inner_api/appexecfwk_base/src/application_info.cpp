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

#include "application_info.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "message_parcel.h"
#include "nlohmann/json.hpp"
#include "parcel_macro.h"
#include "string_ex.h"

#include "app_log_wrapper.h"
#include "json_util.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
const std::string APPLICATION_NAME = "name";
const std::string APPLICATION_BUNDLE_NAME = "bundleName";
const std::string APPLICATION_VERSION_CODE = "versionCode";
const std::string APPLICATION_VERSION_NAME = "versionName";
const std::string APPLICATION_MIN_COMPATIBLE_VERSION_CODE = "minCompatibleVersionCode";
const std::string APPLICATION_API_COMPATIBLE_VERSION = "apiCompatibleVersion";
const std::string APPLICATION_API_TARGET_VERSION = "apiTargetVersion";
const std::string APPLICATION_ICON_PATH = "iconPath";
const std::string APPLICATION_ICON_ID = "iconId";
const std::string APPLICATION_LABEL = "label";
const std::string APPLICATION_LABEL_ID = "labelId";
const std::string APPLICATION_DESCRIPTION = "description";
const std::string APPLICATION_DESCRIPTION_ID = "descriptionId";
const std::string APPLICATION_KEEP_ALIVE = "keepAlive";
const std::string APPLICATION_REMOVABLE = "removable";
const std::string APPLICATION_SINGLETON = "singleton";
const std::string APPLICATION_USER_DATA_CLEARABLE = "userDataClearable";
const std::string APPLICATION_IS_SYSTEM_APP = "isSystemApp";
const std::string APPLICATION_IS_LAUNCHER_APP = "isLauncherApp";
const std::string APPLICATION_IS_FREEINSTALL_APP = "isFreeInstallApp";
const std::string APPLICATION_CODE_PATH = "codePath";
const std::string APPLICATION_DATA_DIR = "dataDir";
const std::string APPLICATION_DATA_BASE_DIR = "dataBaseDir";
const std::string APPLICATION_CACHE_DIR = "cacheDir";
const std::string APPLICATION_ENTRY_DIR = "entryDir";
const std::string APPLICATION_API_RELEASETYPE = "apiReleaseType";
const std::string APPLICATION_DEBUG = "debug";
const std::string APPLICATION_DEVICE_ID = "deviceId";
const std::string APPLICATION_DISTRIBUTED_NOTIFICATION_ENABLED = "distributedNotificationEnabled";
const std::string APPLICATION_ENTITY_TYPE = "entityType";
const std::string APPLICATION_PROCESS = "process";
const std::string APPLICATION_SUPPORTED_MODES = "supportedModes";
const std::string APPLICATION_VENDOR = "vendor";
const std::string APPLICATION_ACCESSIBLE = "accessible";
const std::string APPLICATION_PRIVILEGE_LEVEL = "appPrivilegeLevel";
const std::string APPLICATION_ACCESSTOKEN_ID = "accessTokenId";
const std::string APPLICATION_ENABLED = "enabled";
const std::string APPLICATION_UID = "uid";
const std::string APPLICATION_PERMISSIONS = "permissions";
const std::string APPLICATION_MODULE_SOURCE_DIRS = "moduleSourceDirs";
const std::string APPLICATION_MODULE_INFOS = "moduleInfos";
const std::string APPLICATION_META_DATA_CONFIG_JSON = "metaData";
const std::string APPLICATION_META_DATA_MODULE_JSON = "metadata";
const std::string APPLICATION_FINGERPRINT = "fingerprint";
const std::string APPLICATION_ICON = "icon";
const std::string APPLICATION_FLAGS = "flags";
const std::string APPLICATION_ENTRY_MODULE_NAME = "entryModuleName";
const std::string APPLICATION_NATIVE_LIBRARY_PATH = "nativeLibraryPath";
const std::string APPLICATION_CPU_ABI = "cpuAbi";
const std::string APPLICATION_IS_COMPRESS_NATIVE_LIBS = "isCompressNativeLibs";
const std::string APPLICATION_SIGNATURE_KEY = "signatureKey";
const std::string APPLICATION_TARGETBUNDLELIST = "targetBundleList";
const std::string APPLICATION_APP_DISTRIBUTION_TYPE = "appDistributionType";
const std::string APPLICATION_APP_PROVISION_TYPE = "appProvisionType";
const std::string APPLICATION_ICON_RESOURCE = "iconResource";
const std::string APPLICATION_LABEL_RESOURCE = "labelResource";
const std::string APPLICATION_DESCRIPTION_RESOURCE = "descriptionResource";
const std::string APPLICATION_MULTI_PROJECTS = "multiProjects";
const std::string APPLICATION_CROWDTEST_DEADLINE = "crowdtestDeadline";
const std::string RESOURCE_BUNDLE_NAME = "bundleName";
const std::string RESOURCE_MODULE_NAME = "moduleName";
const std::string RESOURCE_ID = "id";
}

Metadata::Metadata(const std::string &paramName, const std::string &paramValue, const std::string &paramResource)
    : name(paramName), value(paramValue), resource(paramResource)
{
}

bool Metadata::ReadFromParcel(Parcel &parcel)
{
    name = Str16ToStr8(parcel.ReadString16());
    value = Str16ToStr8(parcel.ReadString16());
    resource = Str16ToStr8(parcel.ReadString16());
    return true;
}

bool Metadata::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(name));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(value));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(resource));
    return true;
}

Metadata *Metadata::Unmarshalling(Parcel &parcel)
{
    Metadata *metadata = new (std::nothrow) Metadata;
    if (metadata && !metadata->ReadFromParcel(parcel)) {
        APP_LOGE("read from parcel failed");
        delete metadata;
        metadata = nullptr;
    }
    return metadata;
}

CustomizeData::CustomizeData(std::string paramName, std::string paramValue, std::string paramExtra)
    :name(paramName), value(paramValue), extra(paramExtra)
{
}

bool CustomizeData::ReadFromParcel(Parcel &parcel)
{
    name = Str16ToStr8(parcel.ReadString16());
    value = Str16ToStr8(parcel.ReadString16());
    extra = Str16ToStr8(parcel.ReadString16());
    return true;
}

CustomizeData *CustomizeData::Unmarshalling(Parcel &parcel)
{
    CustomizeData *customizeData = new (std::nothrow) CustomizeData;
    if (customizeData && !customizeData->ReadFromParcel(parcel)) {
        APP_LOGE("read from parcel failed");
        delete customizeData;
        customizeData = nullptr;
    }
    return customizeData;
}

bool CustomizeData::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(name));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(value));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(extra));
    return true;
}

bool Resource::ReadFromParcel(Parcel &parcel)
{
    bundleName = Str16ToStr8(parcel.ReadString16());
    moduleName = Str16ToStr8(parcel.ReadString16());
    id = parcel.ReadInt32();
    return true;
}

bool Resource::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(bundleName));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(moduleName));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, id);
    return true;
}

Resource *Resource::Unmarshalling(Parcel &parcel)
{
    Resource *resource = new (std::nothrow) Resource;
    if (resource && !resource->ReadFromParcel(parcel)) {
        APP_LOGE("read from parcel failed");
        delete resource;
        resource = nullptr;
    }
    return resource;
}

bool ApplicationInfo::ReadMetaDataFromParcel(Parcel &parcel)
{
    int32_t metaDataSize = parcel.ReadInt32();
    for (int32_t i = 0; i < metaDataSize; i++) {
        std::string mouduleName = Str16ToStr8(parcel.ReadString16());
        int32_t customizeDataSize = parcel.ReadInt32();
        std::vector<CustomizeData> customizeDatas;
        metaData[mouduleName] = customizeDatas;
        for (int j = 0; j < customizeDataSize; j++) {
            std::unique_ptr<CustomizeData> customizeData(parcel.ReadParcelable<CustomizeData>());
            if (!customizeData) {
                APP_LOGE("ReadParcelable<CustomizeData> failed");
                return false;
            }
            metaData[mouduleName].emplace_back(*customizeData);
        }
    }
    return true;
}

bool ApplicationInfo::ReadFromParcel(Parcel &parcel)
{
    name = Str16ToStr8(parcel.ReadString16());
    bundleName = Str16ToStr8(parcel.ReadString16());
    versionCode = parcel.ReadUint32();
    versionName = Str16ToStr8(parcel.ReadString16());
    minCompatibleVersionCode = parcel.ReadInt32();
    apiCompatibleVersion = parcel.ReadUint32();
    apiTargetVersion = parcel.ReadInt32();
    crowdtestDeadline = parcel.ReadInt64();

    iconPath = Str16ToStr8(parcel.ReadString16());
    iconId = parcel.ReadInt32();
    std::unique_ptr<Resource> iconResourcePtr(parcel.ReadParcelable<Resource>());
    if (!iconResourcePtr) {
        APP_LOGE("icon ReadParcelable<Resource> failed");
        return false;
    }
    iconResource = *iconResourcePtr;

    label = Str16ToStr8(parcel.ReadString16());
    labelId = parcel.ReadInt32();
    std::unique_ptr<Resource> labelResourcePtr(parcel.ReadParcelable<Resource>());
    if (!labelResourcePtr) {
        APP_LOGE("label ReadParcelable<Resource> failed");
        return false;
    }
    labelResource = *labelResourcePtr;

    description = Str16ToStr8(parcel.ReadString16());
    descriptionId = parcel.ReadInt32();
    std::unique_ptr<Resource> descriptionResourcePtr(parcel.ReadParcelable<Resource>());
    if (!descriptionResourcePtr) {
        APP_LOGE("description ReadParcelable<Resource> failed");
        return false;
    }
    descriptionResource = *descriptionResourcePtr;

    keepAlive = parcel.ReadBool();
    removable = parcel.ReadBool();
    singleton = parcel.ReadBool();
    userDataClearable = parcel.ReadBool();
    accessible = parcel.ReadBool();
    isSystemApp = parcel.ReadBool();
    isLauncherApp = parcel.ReadBool();
    isFreeInstallApp = parcel.ReadBool();
    
    codePath = Str16ToStr8(parcel.ReadString16());
    dataDir = Str16ToStr8(parcel.ReadString16());
    dataBaseDir = Str16ToStr8(parcel.ReadString16());
    cacheDir = Str16ToStr8(parcel.ReadString16());
    entryDir = Str16ToStr8(parcel.ReadString16());

    apiReleaseType = Str16ToStr8(parcel.ReadString16());
    debug = parcel.ReadBool();
    deviceId = Str16ToStr8(parcel.ReadString16());
    distributedNotificationEnabled = parcel.ReadBool();
    entityType = Str16ToStr8(parcel.ReadString16());
    process = Str16ToStr8(parcel.ReadString16());
    supportedModes = parcel.ReadInt32();
    vendor = Str16ToStr8(parcel.ReadString16());
    appPrivilegeLevel = Str16ToStr8(parcel.ReadString16());
    appDistributionType = Str16ToStr8(parcel.ReadString16());
    appProvisionType = Str16ToStr8(parcel.ReadString16());
    accessTokenId = parcel.ReadUint32();
    enabled = parcel.ReadBool();
    uid = parcel.ReadInt32();
    nativeLibraryPath = Str16ToStr8(parcel.ReadString16());
    cpuAbi = Str16ToStr8(parcel.ReadString16());
    
    int32_t permissionsSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, permissionsSize);
    for (auto i = 0; i < permissionsSize; i++) {
        permissions.emplace_back(Str16ToStr8(parcel.ReadString16()));
    }

    int32_t moduleSourceDirsSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, moduleSourceDirsSize);
    for (auto i = 0; i < moduleSourceDirsSize; i++) {
        moduleSourceDirs.emplace_back(Str16ToStr8(parcel.ReadString16()));
    }

    int32_t moduleInfosSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, moduleInfosSize);
    for (auto i = 0; i < moduleInfosSize; i++) {
        std::unique_ptr<ModuleInfo> moduleInfo(parcel.ReadParcelable<ModuleInfo>());
        if (!moduleInfo) {
            APP_LOGE("ReadParcelable<ModuleInfo> failed");
            return false;
        }
        moduleInfos.emplace_back(*moduleInfo);
    }

    int32_t metaDataSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, metaDataSize);
    for (int32_t i = 0; i < metaDataSize; ++i) {
        std::string key = Str16ToStr8(parcel.ReadString16());
        int32_t customizeDataSize = parcel.ReadInt32();
        for (int n = 0; n < customizeDataSize; ++n) {
            std::unique_ptr<CustomizeData> customizeData(parcel.ReadParcelable<CustomizeData>());
            if (!customizeData) {
                APP_LOGE("ReadParcelable<CustomizeData> failed");
                return false;
            }
            metaData[key].emplace_back(*customizeData);
        }
    }

    int32_t metadataSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, metadataSize);
    for (int32_t i = 0; i < metadataSize; ++i) {
        std::string key = Str16ToStr8(parcel.ReadString16());
        int32_t metaSize = parcel.ReadInt32();
        for (int n = 0; n < metaSize; ++n) {
            std::unique_ptr<Metadata> meta(parcel.ReadParcelable<Metadata>());
            if (!meta) {
                APP_LOGE("ReadParcelable<Metadata> failed");
                return false;
            }
            metadata[key].emplace_back(*meta);
        }
    }

    int32_t targetBundleListSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, targetBundleListSize);
    for (auto i = 0; i < targetBundleListSize; i++) {
        targetBundleList.emplace_back(Str16ToStr8(parcel.ReadString16()));
    }

    fingerprint = Str16ToStr8(parcel.ReadString16());
    icon = Str16ToStr8(parcel.ReadString16());
    flags = parcel.ReadInt32();
    entryModuleName = Str16ToStr8(parcel.ReadString16());
    isCompressNativeLibs = parcel.ReadBool();
    signatureKey = Str16ToStr8(parcel.ReadString16());
    multiProjects = parcel.ReadBool();
    return true;
}

ApplicationInfo *ApplicationInfo::Unmarshalling(Parcel &parcel)
{
    ApplicationInfo *info = new (std::nothrow) ApplicationInfo();
    if (info && !info->ReadFromParcel(parcel)) {
        APP_LOGW("read from parcel failed");
        delete info;
        info = nullptr;
    }
    return info;
}

bool ApplicationInfo::Marshalling(Parcel &parcel) const
{
    APP_LOGD("ApplicationInfo::Marshalling called");
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(name));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(bundleName));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, versionCode);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(versionName));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, minCompatibleVersionCode);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, apiCompatibleVersion);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, apiTargetVersion);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int64, parcel, crowdtestDeadline);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(iconPath));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, iconId);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &iconResource);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(label));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, labelId);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &labelResource);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(description));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, descriptionId);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &descriptionResource);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, keepAlive);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, removable);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, singleton);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, userDataClearable);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, accessible);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, isSystemApp);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, isLauncherApp);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, isFreeInstallApp);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(codePath));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(dataDir));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(dataBaseDir));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(cacheDir));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(entryDir));

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(apiReleaseType));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, debug);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(deviceId));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, distributedNotificationEnabled);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(entityType));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(process));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, supportedModes);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(vendor));

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(appPrivilegeLevel));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(appDistributionType));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(appProvisionType));

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, accessTokenId);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, enabled);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, uid);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(nativeLibraryPath));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(cpuAbi));

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, permissions.size());
    for (auto &permission : permissions) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(permission));
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, moduleSourceDirs.size());
    for (auto &moduleSourceDir : moduleSourceDirs) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(moduleSourceDir));
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, moduleInfos.size());
    for (auto &moduleInfo : moduleInfos) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &moduleInfo);
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, metaData.size());
    for (auto &item : metaData) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(item.first));
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, item.second.size());
        for (auto &customizeData : item.second) {
            WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &customizeData);
        }
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, metadata.size());
    for (auto &item : metadata) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(item.first));
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, item.second.size());
        for (auto &meta : item.second) {
            WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &meta);
        }
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, targetBundleList.size());
    for (auto &targetBundle : targetBundleList) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(targetBundle));
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(fingerprint));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(icon));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, flags);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(entryModuleName));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, isCompressNativeLibs);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(signatureKey));

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, multiProjects);
    return true;
}

void ApplicationInfo::Dump(std::string prefix, int fd)
{
    APP_LOGI("called dump ApplicationInfo");
    if (fd < 0) {
        APP_LOGE("dump ApplicationInfo fd error");
        return;
    }
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) {
        APP_LOGE("dump ApplicationInfo fcntl error, errno : %{public}d", errno);
        return;
    }
    uint uflags = static_cast<uint>(flags);
    uflags &= O_ACCMODE;
    if ((uflags == O_WRONLY) || (uflags == O_RDWR)) {
        nlohmann::json jsonObject = *this;
        std::string result;
        result.append(prefix);
        result.append(jsonObject.dump(Constants::DUMP_INDENT));
        int ret = TEMP_FAILURE_RETRY(write(fd, result.c_str(), result.size()));
        if (ret < 0) {
            APP_LOGE("dump ApplicationInfo write error, errno : %{public}d", errno);
        }
    }
    return;
}

void to_json(nlohmann::json &jsonObject, const Resource &resource)
{
    jsonObject = nlohmann::json {
        {RESOURCE_BUNDLE_NAME, resource.bundleName},
        {RESOURCE_MODULE_NAME, resource.moduleName},
        {RESOURCE_ID, resource.id}
    };
}

void from_json(const nlohmann::json &jsonObject, Resource &resource)
{
    const auto &jsonObjectEnd = jsonObject.end();
    int32_t parseResult = ERR_OK;
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        RESOURCE_BUNDLE_NAME,
        resource.bundleName,
        JsonType::STRING,
        true,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        RESOURCE_MODULE_NAME,
        resource.moduleName,
        JsonType::STRING,
        true,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int32_t>(jsonObject,
        jsonObjectEnd,
        RESOURCE_ID,
        resource.id,
        JsonType::NUMBER,
        true,
        parseResult,
        ArrayType::NOT_ARRAY);
    if (parseResult != ERR_OK) {
        APP_LOGE("read Resource from database error, error code : %{public}d", parseResult);
    }
}

void to_json(nlohmann::json &jsonObject, const ApplicationInfo &applicationInfo)
{
    jsonObject = nlohmann::json {
        {APPLICATION_NAME, applicationInfo.name},
        {APPLICATION_BUNDLE_NAME, applicationInfo.bundleName},
        {APPLICATION_VERSION_CODE, applicationInfo.versionCode},
        {APPLICATION_VERSION_NAME, applicationInfo.versionName},
        {APPLICATION_MIN_COMPATIBLE_VERSION_CODE, applicationInfo.minCompatibleVersionCode},
        {APPLICATION_API_COMPATIBLE_VERSION, applicationInfo.apiCompatibleVersion},
        {APPLICATION_API_TARGET_VERSION, applicationInfo.apiTargetVersion},
        {APPLICATION_ICON_PATH, applicationInfo.iconPath},
        {APPLICATION_ICON_ID, applicationInfo.iconId},
        {APPLICATION_LABEL, applicationInfo.label},
        {APPLICATION_LABEL_ID, applicationInfo.labelId},
        {APPLICATION_DESCRIPTION, applicationInfo.description},
        {APPLICATION_DESCRIPTION_ID, applicationInfo.descriptionId},
        {APPLICATION_KEEP_ALIVE, applicationInfo.keepAlive},
        {APPLICATION_REMOVABLE, applicationInfo.removable},
        {APPLICATION_SINGLETON, applicationInfo.singleton},
        {APPLICATION_USER_DATA_CLEARABLE, applicationInfo.userDataClearable},
        {APPLICATION_ACCESSIBLE, applicationInfo.accessible},
        {APPLICATION_IS_SYSTEM_APP, applicationInfo.isSystemApp},
        {APPLICATION_IS_LAUNCHER_APP, applicationInfo.isLauncherApp},
        {APPLICATION_IS_FREEINSTALL_APP, applicationInfo.isFreeInstallApp},
        {APPLICATION_CODE_PATH, applicationInfo.codePath},
        {APPLICATION_DATA_DIR, applicationInfo.dataDir},
        {APPLICATION_DATA_BASE_DIR, applicationInfo.dataBaseDir},
        {APPLICATION_CACHE_DIR, applicationInfo.cacheDir},
        {APPLICATION_ENTRY_DIR, applicationInfo.entryDir},
        {APPLICATION_API_RELEASETYPE, applicationInfo.apiReleaseType},
        {APPLICATION_DEBUG, applicationInfo.debug},
        {APPLICATION_DEVICE_ID, applicationInfo.deviceId},
        {APPLICATION_DISTRIBUTED_NOTIFICATION_ENABLED, applicationInfo.distributedNotificationEnabled},
        {APPLICATION_ENTITY_TYPE, applicationInfo.entityType},
        {APPLICATION_PROCESS, applicationInfo.process},
        {APPLICATION_SUPPORTED_MODES, applicationInfo.supportedModes},
        {APPLICATION_VENDOR, applicationInfo.vendor},
        {APPLICATION_PRIVILEGE_LEVEL, applicationInfo.appPrivilegeLevel},
        {APPLICATION_ACCESSTOKEN_ID, applicationInfo.accessTokenId},
        {APPLICATION_ENABLED, applicationInfo.enabled},
        {APPLICATION_UID, applicationInfo.uid},
        {APPLICATION_PERMISSIONS, applicationInfo.permissions},
        {APPLICATION_MODULE_SOURCE_DIRS, applicationInfo.moduleSourceDirs},
        {APPLICATION_MODULE_INFOS, applicationInfo.moduleInfos},
        {APPLICATION_META_DATA_CONFIG_JSON, applicationInfo.metaData},
        {APPLICATION_META_DATA_MODULE_JSON, applicationInfo.metadata},
        {APPLICATION_FINGERPRINT, applicationInfo.fingerprint},
        {APPLICATION_ICON, applicationInfo.icon},
        {APPLICATION_FLAGS, applicationInfo.flags},
        {APPLICATION_ENTRY_MODULE_NAME, applicationInfo.entryModuleName},
        {APPLICATION_NATIVE_LIBRARY_PATH, applicationInfo.nativeLibraryPath},
        {APPLICATION_CPU_ABI, applicationInfo.cpuAbi},
        {APPLICATION_IS_COMPRESS_NATIVE_LIBS, applicationInfo.isCompressNativeLibs},
        {APPLICATION_SIGNATURE_KEY, applicationInfo.signatureKey},
        {APPLICATION_TARGETBUNDLELIST, applicationInfo.targetBundleList},
        {APPLICATION_APP_DISTRIBUTION_TYPE, applicationInfo.appDistributionType},
        {APPLICATION_APP_PROVISION_TYPE, applicationInfo.appProvisionType},
        {APPLICATION_ICON_RESOURCE, applicationInfo.iconResource},
        {APPLICATION_LABEL_RESOURCE, applicationInfo.labelResource},
        {APPLICATION_DESCRIPTION_RESOURCE, applicationInfo.descriptionResource},
        {APPLICATION_MULTI_PROJECTS, applicationInfo.multiProjects},
        {APPLICATION_CROWDTEST_DEADLINE, applicationInfo.crowdtestDeadline},
    };
}

void from_json(const nlohmann::json &jsonObject, ApplicationInfo &applicationInfo)
{
    const auto &jsonObjectEnd = jsonObject.end();
    int32_t parseResult = ERR_OK;
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_NAME,
        applicationInfo.name,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_BUNDLE_NAME,
        applicationInfo.bundleName,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<uint32_t>(jsonObject,
        jsonObjectEnd,
        APPLICATION_VERSION_CODE,
        applicationInfo.versionCode,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_VERSION_NAME,
        applicationInfo.versionName,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int32_t>(jsonObject,
        jsonObjectEnd,
        APPLICATION_MIN_COMPATIBLE_VERSION_CODE,
        applicationInfo.minCompatibleVersionCode,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int32_t>(jsonObject,
        jsonObjectEnd,
        APPLICATION_API_COMPATIBLE_VERSION,
        applicationInfo.apiCompatibleVersion,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int32_t>(jsonObject,
        jsonObjectEnd,
        APPLICATION_API_TARGET_VERSION,
        applicationInfo.apiTargetVersion,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_ICON_PATH,
        applicationInfo.iconPath,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int32_t>(jsonObject,
        jsonObjectEnd,
        APPLICATION_ICON_ID,
        applicationInfo.iconId,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_LABEL,
        applicationInfo.label,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int32_t>(jsonObject,
        jsonObjectEnd,
        APPLICATION_LABEL_ID,
        applicationInfo.labelId,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_DESCRIPTION,
        applicationInfo.description,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int32_t>(jsonObject,
        jsonObjectEnd,
        APPLICATION_DESCRIPTION_ID,
        applicationInfo.descriptionId,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_KEEP_ALIVE,
        applicationInfo.keepAlive,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_REMOVABLE,
        applicationInfo.removable,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_SINGLETON,
        applicationInfo.singleton,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_USER_DATA_CLEARABLE,
        applicationInfo.userDataClearable,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_ACCESSIBLE,
        applicationInfo.accessible,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_IS_SYSTEM_APP,
        applicationInfo.isSystemApp,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_IS_LAUNCHER_APP,
        applicationInfo.isLauncherApp,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_IS_FREEINSTALL_APP,
        applicationInfo.isFreeInstallApp,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_CODE_PATH,
        applicationInfo.codePath,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_DATA_DIR,
        applicationInfo.dataDir,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_DATA_BASE_DIR,
        applicationInfo.dataBaseDir,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_CACHE_DIR,
        applicationInfo.cacheDir,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_ENTRY_DIR,
        applicationInfo.entryDir,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_API_RELEASETYPE,
        applicationInfo.apiReleaseType,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_DEBUG,
        applicationInfo.debug,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_DEVICE_ID,
        applicationInfo.deviceId,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_DISTRIBUTED_NOTIFICATION_ENABLED,
        applicationInfo.distributedNotificationEnabled,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_ENTITY_TYPE,
        applicationInfo.entityType,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_PROCESS,
        applicationInfo.process,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int>(jsonObject,
        jsonObjectEnd,
        APPLICATION_SUPPORTED_MODES,
        applicationInfo.supportedModes,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_VENDOR,
        applicationInfo.vendor,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_PRIVILEGE_LEVEL,
        applicationInfo.appPrivilegeLevel,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<uint32_t>(jsonObject,
        jsonObjectEnd,
        APPLICATION_ACCESSTOKEN_ID,
        applicationInfo.accessTokenId,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_ENABLED,
        applicationInfo.enabled,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int>(jsonObject,
        jsonObjectEnd,
        APPLICATION_UID,
        applicationInfo.uid,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        APPLICATION_PERMISSIONS,
        applicationInfo.permissions,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        APPLICATION_MODULE_SOURCE_DIRS,
        applicationInfo.moduleSourceDirs,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::vector<ModuleInfo>>(jsonObject,
        jsonObjectEnd,
        APPLICATION_MODULE_INFOS,
        applicationInfo.moduleInfos,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::OBJECT);
    GetValueIfFindKey<std::map<std::string, std::vector<CustomizeData>>>(jsonObject,
        jsonObjectEnd,
        APPLICATION_META_DATA_CONFIG_JSON,
        applicationInfo.metaData,
        JsonType::OBJECT,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::map<std::string, std::vector<Metadata>>>(jsonObject,
        jsonObjectEnd,
        APPLICATION_META_DATA_MODULE_JSON,
        applicationInfo.metadata,
        JsonType::OBJECT,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_FINGERPRINT,
        applicationInfo.fingerprint,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_ICON,
        applicationInfo.icon,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int>(jsonObject,
        jsonObjectEnd,
        APPLICATION_FLAGS,
        applicationInfo.flags,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_ENTRY_MODULE_NAME,
        applicationInfo.entryModuleName,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_NATIVE_LIBRARY_PATH,
        applicationInfo.nativeLibraryPath,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_CPU_ABI,
        applicationInfo.cpuAbi,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_IS_COMPRESS_NATIVE_LIBS,
        applicationInfo.isCompressNativeLibs,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_SIGNATURE_KEY,
        applicationInfo.signatureKey,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        APPLICATION_TARGETBUNDLELIST,
        applicationInfo.targetBundleList,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_APP_DISTRIBUTION_TYPE,
        applicationInfo.appDistributionType,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        APPLICATION_APP_PROVISION_TYPE,
        applicationInfo.appProvisionType,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<Resource>(jsonObject,
        jsonObjectEnd,
        APPLICATION_ICON_RESOURCE,
        applicationInfo.iconResource,
        JsonType::OBJECT,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<Resource>(jsonObject,
        jsonObjectEnd,
        APPLICATION_LABEL_RESOURCE,
        applicationInfo.labelResource,
        JsonType::OBJECT,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<Resource>(jsonObject,
        jsonObjectEnd,
        APPLICATION_DESCRIPTION_RESOURCE,
        applicationInfo.descriptionResource,
        JsonType::OBJECT,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        APPLICATION_MULTI_PROJECTS,
        applicationInfo.multiProjects,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int64_t>(jsonObject,
        jsonObjectEnd,
        APPLICATION_CROWDTEST_DEADLINE,
        applicationInfo.crowdtestDeadline,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    if (parseResult != ERR_OK) {
        APP_LOGE("from_json error, error code : %{public}d", parseResult);
    }
}

void ApplicationInfo::ConvertToCompatibleApplicationInfo(CompatibleApplicationInfo& compatibleApplicationInfo) const
{
    APP_LOGD("ApplicationInfo::ConvertToCompatibleApplicationInfo called");
    compatibleApplicationInfo.name = name;
    compatibleApplicationInfo.icon = icon;
    compatibleApplicationInfo.label = label;
    compatibleApplicationInfo.description = description;
    compatibleApplicationInfo.cpuAbi = cpuAbi;
    compatibleApplicationInfo.process = process;
    compatibleApplicationInfo.systemApp = isSystemApp;
    compatibleApplicationInfo.isCompressNativeLibs = isCompressNativeLibs;
    compatibleApplicationInfo.iconId = iconId;
    compatibleApplicationInfo.labelId = labelId;
    compatibleApplicationInfo.descriptionId = descriptionId;
    compatibleApplicationInfo.permissions = permissions;
    compatibleApplicationInfo.moduleInfos = moduleInfos;
    compatibleApplicationInfo.supportedModes = supportedModes;
    compatibleApplicationInfo.enabled = enabled;
    compatibleApplicationInfo.debug = debug;
}
}  // namespace AppExecFwk
}  // namespace OHOS