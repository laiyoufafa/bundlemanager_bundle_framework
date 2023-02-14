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

#include "bundle_info.h"

#include "json_util.h"
#include "parcel_macro.h"
#include "string_ex.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
const std::string BUNDLE_INFO_NAME = "name";
const std::string BUNDLE_INFO_LABEL = "label";
const std::string BUNDLE_INFO_DESCRIPTION = "description";
const std::string BUNDLE_INFO_VENDOR = "vendor";
const std::string BUNDLE_INFO_IS_KEEP_ALIVE = "isKeepAlive";
const std::string BUNDLE_INFO_SINGLETON = "singleton";
const std::string BUNDLE_INFO_IS_NATIVE_APP = "isNativeApp";
const std::string BUNDLE_INFO_IS_PREINSTALL_APP = "isPreInstallApp";
const std::string BUNDLE_INFO_IS_DIFFERENT_NAME = "isDifferentName";
const std::string BUNDLE_INFO_APPLICATION_INFO = "applicationInfo";
const std::string BUNDLE_INFO_ABILITY_INFOS = "abilityInfos";
const std::string BUNDLE_INFO_HAP_MODULE_INFOS = "hapModuleInfos";
const std::string BUNDLE_INFO_EXTENSION_ABILITY_INFOS = "extensionAbilityInfo";
const std::string BUNDLE_INFO_JOINT_USERID = "jointUserId";
const std::string BUNDLE_INFO_VERSION_CODE = "versionCode";
const std::string BUNDLE_INFO_MIN_COMPATIBLE_VERSION_CODE = "minCompatibleVersionCode";
const std::string BUNDLE_INFO_VERSION_NAME = "versionName";
const std::string BUNDLE_INFO_MIN_SDK_VERSION = "minSdkVersion";
const std::string BUNDLE_INFO_MAX_SDK_VERSION = "maxSdkVersion";
const std::string BUNDLE_INFO_MAIN_ENTRY = "mainEntry";
const std::string BUNDLE_INFO_CPU_ABI = "cpuAbi";
const std::string BUNDLE_INFO_APPID = "appId";
const std::string BUNDLE_INFO_COMPATIBLE_VERSION = "compatibleVersion";
const std::string BUNDLE_INFO_TARGET_VERSION = "targetVersion";
const std::string BUNDLE_INFO_RELEASE_TYPE = "releaseType";
const std::string BUNDLE_INFO_UID = "uid";
const std::string BUNDLE_INFO_GID = "gid";
const std::string BUNDLE_INFO_SEINFO = "seInfo";
const std::string BUNDLE_INFO_INSTALL_TIME = "installTime";
const std::string BUNDLE_INFO_UPDATE_TIME = "updateTime";
const std::string BUNDLE_INFO_ENTRY_MODULE_NAME = "entryModuleName";
const std::string BUNDLE_INFO_ENTRY_INSTALLATION_FREE = "entryInstallationFree";
const std::string BUNDLE_INFO_REQ_PERMISSIONS = "reqPermissions";
const std::string BUNDLE_INFO_REQ_PERMISSION_STATES = "reqPermissionStates";
const std::string BUNDLE_INFO_REQ_PERMISSION_DETAILS = "reqPermissionDetails";
const std::string BUNDLE_INFO_DEF_PERMISSIONS = "defPermissions";
const std::string BUNDLE_INFO_HAP_MODULE_NAMES = "hapModuleNames";
const std::string BUNDLE_INFO_MODULE_NAMES = "moduleNames";
const std::string BUNDLE_INFO_MODULE_PUBLIC_DIRS = "modulePublicDirs";
const std::string BUNDLE_INFO_MODULE_DIRS = "moduleDirs";
const std::string BUNDLE_INFO_MODULE_RES_PATHS = "moduleResPaths";
const std::string REQUESTPERMISSION_NAME = "name";
const std::string REQUESTPERMISSION_REASON = "reason";
const std::string REQUESTPERMISSION_REASON_ID = "reasonId";
const std::string REQUESTPERMISSION_USEDSCENE = "usedScene";
const std::string REQUESTPERMISSION_ABILITIES = "abilities";
const std::string REQUESTPERMISSION_ABILITY = "ability";
const std::string REQUESTPERMISSION_WHEN = "when";
const std::string SIGNATUREINFO_APPID = "appId";
const std::string SIGNATUREINFO_FINGERPRINT = "fingerprint";
const std::string BUNDLE_INFO_APP_INDEX = "appIndex";
const std::string BUNDLE_INFO_SIGNATURE_INFO = "signatureInfo";
}

bool RequestPermissionUsedScene::ReadFromParcel(Parcel &parcel)
{
    int32_t size;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, size);
    for (int32_t i = 0; i < size; i++) {
        abilities.emplace_back(Str16ToStr8(parcel.ReadString16()));
    }
    when = Str16ToStr8(parcel.ReadString16());
    return true;
}

bool RequestPermissionUsedScene::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, abilities.size());
    for (auto &ability : abilities) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(ability));
    }
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(when));
    return true;
}

RequestPermissionUsedScene *RequestPermissionUsedScene::Unmarshalling(Parcel &parcel)
{
    RequestPermissionUsedScene *info = new (std::nothrow) RequestPermissionUsedScene();
    if (info && !info->ReadFromParcel(parcel)) {
        APP_LOGW("read from parcel failed");
        delete info;
        info = nullptr;
    }
    return info;
}

bool RequestPermission::ReadFromParcel(Parcel &parcel)
{
    name = Str16ToStr8(parcel.ReadString16());
    reason = Str16ToStr8(parcel.ReadString16());
    reasonId = parcel.ReadInt32();
    std::unique_ptr<RequestPermissionUsedScene> scene(parcel.ReadParcelable<RequestPermissionUsedScene>());
    if (!scene) {
        APP_LOGE("ReadParcelable<RequestPermissionUsedScene> failed");
        return false;
    }
    usedScene = *scene;
    return true;
}

bool RequestPermission::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(name));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(reason));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, reasonId);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &usedScene);
    return true;
}

RequestPermission *RequestPermission::Unmarshalling(Parcel &parcel)
{
    RequestPermission *info = new (std::nothrow) RequestPermission();
    if (info && !info->ReadFromParcel(parcel)) {
        APP_LOGW("read from parcel failed");
        delete info;
        info = nullptr;
    }
    return info;
}

bool SignatureInfo::ReadFromParcel(Parcel &parcel)
{
    appId = Str16ToStr8(parcel.ReadString16());
    fingerprint = Str16ToStr8(parcel.ReadString16());
    return true;
}

bool SignatureInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(appId));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(fingerprint));
    return true;
}

SignatureInfo *SignatureInfo::Unmarshalling(Parcel &parcel)
{
    SignatureInfo *info = new (std::nothrow) SignatureInfo();
    if (info && !info->ReadFromParcel(parcel)) {
        APP_LOGW("read from parcel failed");
        delete info;
        info = nullptr;
    }
    return info;
}

bool BundleInfo::ReadFromParcel(Parcel &parcel)
{
    name = Str16ToStr8(parcel.ReadString16());
    versionCode = parcel.ReadUint32();
    versionName = Str16ToStr8(parcel.ReadString16());
    minCompatibleVersionCode = parcel.ReadUint32();
    compatibleVersion = parcel.ReadUint32();
    targetVersion = parcel.ReadUint32();
    isKeepAlive = parcel.ReadBool();
    singleton = parcel.ReadBool();
    isPreInstallApp = parcel.ReadBool();

    vendor = Str16ToStr8(parcel.ReadString16());
    releaseType = Str16ToStr8(parcel.ReadString16());
    isNativeApp = parcel.ReadBool();

    mainEntry = Str16ToStr8(parcel.ReadString16());
    entryModuleName = Str16ToStr8(parcel.ReadString16());
    entryInstallationFree = parcel.ReadBool();

    appId = Str16ToStr8(parcel.ReadString16());
    uid = parcel.ReadInt32();
    gid = parcel.ReadInt32();
    installTime = parcel.ReadInt64();
    updateTime = parcel.ReadInt64();
    std::unique_ptr<ApplicationInfo> appInfo(parcel.ReadParcelable<ApplicationInfo>());
    if (!appInfo) {
        APP_LOGE("ReadParcelable<ApplicationInfo> failed");
        return false;
    }
    applicationInfo = *appInfo;

    int32_t abilityInfosSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, abilityInfosSize);
    for (auto i = 0; i < abilityInfosSize; i++) {
        std::unique_ptr<AbilityInfo> abilityInfo(parcel.ReadParcelable<AbilityInfo>());
        if (!abilityInfo) {
            APP_LOGE("ReadParcelable<AbilityInfo> failed");
            return false;
        }
        abilityInfos.emplace_back(*abilityInfo);
    }

    int32_t extensionInfosSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, extensionInfosSize);
    for (auto i = 0; i < extensionInfosSize; i++) {
        std::unique_ptr<ExtensionAbilityInfo> extensionAbilityInfo(parcel.ReadParcelable<ExtensionAbilityInfo>());
        if (!extensionAbilityInfo) {
            APP_LOGE("ReadParcelable<ExtensionAbilityInfo> failed");
            return false;
        }
        extensionInfos.emplace_back(*extensionAbilityInfo);
    }

    int32_t hapModuleInfosSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, hapModuleInfosSize);
    for (auto i = 0; i < hapModuleInfosSize; i++) {
        std::unique_ptr<HapModuleInfo> hapModuleInfo(parcel.ReadParcelable<HapModuleInfo>());
        if (!hapModuleInfo) {
            APP_LOGE("ReadParcelable<HapModuleInfo> failed");
            return false;
        }
        hapModuleInfos.emplace_back(*hapModuleInfo);
    }

    int32_t hapModuleNamesSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, hapModuleNamesSize);
    for (auto i = 0; i < hapModuleNamesSize; i++) {
        hapModuleNames.emplace_back(Str16ToStr8(parcel.ReadString16()));
    }

    int32_t moduleNamesSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, moduleNamesSize);
    for (auto i = 0; i < moduleNamesSize; i++) {
        moduleNames.emplace_back(Str16ToStr8(parcel.ReadString16()));
    }

    int32_t modulePublicDirsSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, modulePublicDirsSize);
    for (auto i = 0; i < modulePublicDirsSize; i++) {
        modulePublicDirs.emplace_back(Str16ToStr8(parcel.ReadString16()));
    }

    int32_t moduleDirsSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, moduleDirsSize);
    for (auto i = 0; i < moduleDirsSize; i++) {
        moduleDirs.emplace_back(Str16ToStr8(parcel.ReadString16()));
    }

    int32_t moduleResPathsSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, moduleResPathsSize);
    for (auto i = 0; i < moduleResPathsSize; i++) {
        moduleResPaths.emplace_back(Str16ToStr8(parcel.ReadString16()));
    }

    int32_t reqPermissionsSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, reqPermissionsSize);
    for (auto i = 0; i < reqPermissionsSize; i++) {
        reqPermissions.emplace_back(Str16ToStr8(parcel.ReadString16()));
    }

    int32_t defPermissionsSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, defPermissionsSize);
    for (auto i = 0; i < defPermissionsSize; i++) {
        defPermissions.emplace_back(Str16ToStr8(parcel.ReadString16()));
    }

    int32_t reqPermissionStatesSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, reqPermissionStatesSize);
    for (auto i = 0; i < reqPermissionStatesSize; i++) {
        reqPermissionStates.emplace_back(parcel.ReadInt32());
    }

    int32_t reqPermissionDetailsSize;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, reqPermissionDetailsSize);
    for (auto i = 0; i < reqPermissionDetailsSize; i++) {
        std::unique_ptr<RequestPermission> requestPermission(parcel.ReadParcelable<RequestPermission>());
        if (!requestPermission) {
            APP_LOGE("ReadParcelable<RequestPermission> failed");
            return false;
        }
        reqPermissionDetails.emplace_back(*requestPermission);
    }

    cpuAbi = Str16ToStr8(parcel.ReadString16());
    seInfo = Str16ToStr8(parcel.ReadString16());
    label = Str16ToStr8(parcel.ReadString16());
    description = Str16ToStr8(parcel.ReadString16());
    jointUserId = Str16ToStr8(parcel.ReadString16());
    minSdkVersion = parcel.ReadInt32();
    maxSdkVersion = parcel.ReadInt32();
    isDifferentName = parcel.ReadBool();
    appIndex = parcel.ReadInt32();

    std::unique_ptr<SignatureInfo> sigInfo(parcel.ReadParcelable<SignatureInfo>());
    if (!sigInfo) {
        APP_LOGE("ReadParcelable<SignatureInfo> failed");
        return false;
    }
    signatureInfo = *sigInfo;
    return true;
}

bool BundleInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(name));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, versionCode);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(versionName));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, minCompatibleVersionCode);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, compatibleVersion);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint32, parcel, targetVersion);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, isKeepAlive);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, singleton);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, isPreInstallApp);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(vendor));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(releaseType));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, isNativeApp);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(mainEntry));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(entryModuleName));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, entryInstallationFree);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(appId));

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, uid);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, gid);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int64, parcel, installTime);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int64, parcel, updateTime);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &applicationInfo);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, abilityInfos.size());
    for (auto &abilityInfo : abilityInfos) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &abilityInfo);
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, extensionInfos.size());
    for (auto &extensionInfo : extensionInfos) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &extensionInfo);
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, hapModuleInfos.size());
    for (auto &hapModuleInfo : hapModuleInfos) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &hapModuleInfo);
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, hapModuleNames.size());
    for (auto &hapModuleName : hapModuleNames) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(hapModuleName));
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, moduleNames.size());
    for (auto &moduleName : moduleNames) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(moduleName));
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, modulePublicDirs.size());
    for (auto &modulePublicDir : modulePublicDirs) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(modulePublicDir));
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, moduleDirs.size());
    for (auto &moduleDir : moduleDirs) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(moduleDir));
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, moduleResPaths.size());
    for (auto &moduleResPath : moduleResPaths) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(moduleResPath));
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, reqPermissions.size());
    for (auto &reqPermission : reqPermissions) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(reqPermission));
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, defPermissions.size());
    for (auto &defPermission : defPermissions) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(defPermission));
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, reqPermissionStates.size());
    for (auto &reqPermissionState : reqPermissionStates) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, reqPermissionState);
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, reqPermissionDetails.size());
    for (auto &reqPermissionDetail : reqPermissionDetails) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &reqPermissionDetail);
    }

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(cpuAbi));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(seInfo));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(label));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(description));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String16, parcel, Str8ToStr16(jointUserId));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, minSdkVersion);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, maxSdkVersion);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, isDifferentName);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, appIndex);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &signatureInfo);
    return true;
}

BundleInfo *BundleInfo::Unmarshalling(Parcel &parcel)
{
    BundleInfo *info = new (std::nothrow) BundleInfo();
    if (info && !info->ReadFromParcel(parcel)) {
        APP_LOGW("read from parcel failed");
        delete info;
        info = nullptr;
    }
    return info;
}

void to_json(nlohmann::json &jsonObject, const RequestPermissionUsedScene &usedScene)
{
    jsonObject = nlohmann::json {
        {REQUESTPERMISSION_ABILITIES, usedScene.abilities},
        {REQUESTPERMISSION_WHEN, usedScene.when}
    };
}

void to_json(nlohmann::json &jsonObject, const RequestPermission &requestPermission)
{
    jsonObject = nlohmann::json {
        {REQUESTPERMISSION_NAME, requestPermission.name},
        {REQUESTPERMISSION_REASON, requestPermission.reason},
        {REQUESTPERMISSION_REASON_ID, requestPermission.reasonId},
        {REQUESTPERMISSION_USEDSCENE, requestPermission.usedScene}
    };
}

void to_json(nlohmann::json &jsonObject, const SignatureInfo &signatureInfo)
{
    jsonObject = nlohmann::json {
        {SIGNATUREINFO_APPID, signatureInfo.appId},
        {SIGNATUREINFO_FINGERPRINT, signatureInfo.fingerprint}
    };
}

void from_json(const nlohmann::json &jsonObject, RequestPermissionUsedScene &usedScene)
{
    const auto &jsonObjectEnd = jsonObject.end();
    int32_t parseResult = ERR_OK;
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        REQUESTPERMISSION_ABILITIES,
        usedScene.abilities,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        REQUESTPERMISSION_ABILITY,
        usedScene.abilities,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        REQUESTPERMISSION_WHEN,
        usedScene.when,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    if (parseResult != ERR_OK) {
        APP_LOGE("read RequestPermissionUsedScene from database error, error code : %{public}d", parseResult);
    }
}

void from_json(const nlohmann::json &jsonObject, RequestPermission &requestPermission)
{
    const auto &jsonObjectEnd = jsonObject.end();
    int32_t parseResult = ERR_OK;
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        REQUESTPERMISSION_NAME,
        requestPermission.name,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        REQUESTPERMISSION_REASON,
        requestPermission.reason,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int32_t>(jsonObject,
        jsonObjectEnd,
        REQUESTPERMISSION_REASON_ID,
        requestPermission.reasonId,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<RequestPermissionUsedScene>(jsonObject,
        jsonObjectEnd,
        REQUESTPERMISSION_USEDSCENE,
        requestPermission.usedScene,
        JsonType::OBJECT,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    if (parseResult != ERR_OK) {
        APP_LOGE("read RequestPermission from database error, error code : %{public}d", parseResult);
    }
}

void from_json(const nlohmann::json &jsonObject, SignatureInfo &signatureInfo)
{
    const auto &jsonObjectEnd = jsonObject.end();
    int32_t parseResult = ERR_OK;
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        SIGNATUREINFO_APPID,
        signatureInfo.appId,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        SIGNATUREINFO_FINGERPRINT,
        signatureInfo.fingerprint,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    if (parseResult != ERR_OK) {
        APP_LOGE("read SignatureInfo from database error, error code : %{public}d", parseResult);
    }
}

void to_json(nlohmann::json &jsonObject, const BundleInfo &bundleInfo)
{
    jsonObject = nlohmann::json {
        {BUNDLE_INFO_NAME, bundleInfo.name},
        {BUNDLE_INFO_LABEL, bundleInfo.label},
        {BUNDLE_INFO_DESCRIPTION, bundleInfo.description},
        {BUNDLE_INFO_VENDOR, bundleInfo.vendor},
        {BUNDLE_INFO_IS_KEEP_ALIVE, bundleInfo.isKeepAlive},
        {BUNDLE_INFO_IS_NATIVE_APP, bundleInfo.isNativeApp},
        {BUNDLE_INFO_IS_PREINSTALL_APP, bundleInfo.isPreInstallApp},
        {BUNDLE_INFO_IS_DIFFERENT_NAME, bundleInfo.isDifferentName},
        {BUNDLE_INFO_APPLICATION_INFO, bundleInfo.applicationInfo},
        {BUNDLE_INFO_ABILITY_INFOS, bundleInfo.abilityInfos},
        {BUNDLE_INFO_HAP_MODULE_INFOS, bundleInfo.hapModuleInfos},
        {BUNDLE_INFO_EXTENSION_ABILITY_INFOS, bundleInfo.extensionInfos},
        {BUNDLE_INFO_JOINT_USERID, bundleInfo.jointUserId},
        {BUNDLE_INFO_VERSION_CODE, bundleInfo.versionCode},
        {BUNDLE_INFO_MIN_COMPATIBLE_VERSION_CODE, bundleInfo.minCompatibleVersionCode},
        {BUNDLE_INFO_VERSION_NAME, bundleInfo.versionName},
        {BUNDLE_INFO_MIN_SDK_VERSION, bundleInfo.minSdkVersion},
        {BUNDLE_INFO_MAX_SDK_VERSION, bundleInfo.maxSdkVersion},
        {BUNDLE_INFO_MAIN_ENTRY, bundleInfo.mainEntry},
        {BUNDLE_INFO_CPU_ABI, bundleInfo.cpuAbi},
        {BUNDLE_INFO_APPID, bundleInfo.appId},
        {BUNDLE_INFO_COMPATIBLE_VERSION, bundleInfo.compatibleVersion},
        {BUNDLE_INFO_TARGET_VERSION, bundleInfo.targetVersion},
        {BUNDLE_INFO_RELEASE_TYPE, bundleInfo.releaseType},
        {BUNDLE_INFO_UID, bundleInfo.uid},
        {BUNDLE_INFO_GID, bundleInfo.gid},
        {BUNDLE_INFO_SEINFO, bundleInfo.seInfo},
        {BUNDLE_INFO_INSTALL_TIME, bundleInfo.installTime},
        {BUNDLE_INFO_UPDATE_TIME, bundleInfo.updateTime},
        {BUNDLE_INFO_ENTRY_MODULE_NAME, bundleInfo.entryModuleName},
        {BUNDLE_INFO_ENTRY_INSTALLATION_FREE, bundleInfo.entryInstallationFree},
        {BUNDLE_INFO_REQ_PERMISSIONS, bundleInfo.reqPermissions},
        {BUNDLE_INFO_REQ_PERMISSION_STATES, bundleInfo.reqPermissionStates},
        {BUNDLE_INFO_REQ_PERMISSION_DETAILS, bundleInfo.reqPermissionDetails},
        {BUNDLE_INFO_DEF_PERMISSIONS, bundleInfo.defPermissions},
        {BUNDLE_INFO_HAP_MODULE_NAMES, bundleInfo.hapModuleNames},
        {BUNDLE_INFO_MODULE_NAMES, bundleInfo.moduleNames},
        {BUNDLE_INFO_MODULE_PUBLIC_DIRS, bundleInfo.modulePublicDirs},
        {BUNDLE_INFO_MODULE_DIRS, bundleInfo.moduleDirs},
        {BUNDLE_INFO_MODULE_RES_PATHS, bundleInfo.moduleResPaths},
        {BUNDLE_INFO_SINGLETON, bundleInfo.singleton},
        {BUNDLE_INFO_APP_INDEX, bundleInfo.appIndex},
        {BUNDLE_INFO_SIGNATURE_INFO, bundleInfo.signatureInfo},
    };
}

void from_json(const nlohmann::json &jsonObject, BundleInfo &bundleInfo)
{
    const auto &jsonObjectEnd = jsonObject.end();
    int32_t parseResult = ERR_OK;
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_NAME,
        bundleInfo.name,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_LABEL,
        bundleInfo.label,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_DESCRIPTION,
        bundleInfo.description,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_VENDOR,
        bundleInfo.vendor,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_IS_KEEP_ALIVE,
        bundleInfo.isKeepAlive,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_IS_NATIVE_APP,
        bundleInfo.isNativeApp,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_IS_PREINSTALL_APP,
        bundleInfo.isPreInstallApp,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_IS_DIFFERENT_NAME,
        bundleInfo.isDifferentName,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<ApplicationInfo>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_APPLICATION_INFO,
        bundleInfo.applicationInfo,
        JsonType::OBJECT,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::vector<AbilityInfo>>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_ABILITY_INFOS,
        bundleInfo.abilityInfos,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::OBJECT);
    GetValueIfFindKey<std::vector<HapModuleInfo>>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_HAP_MODULE_INFOS,
        bundleInfo.hapModuleInfos,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::OBJECT);
    GetValueIfFindKey<uint32_t>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_VERSION_CODE,
        bundleInfo.versionCode,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<uint32_t>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_MIN_COMPATIBLE_VERSION_CODE,
        bundleInfo.minCompatibleVersionCode,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_VERSION_NAME,
        bundleInfo.versionName,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_JOINT_USERID,
        bundleInfo.jointUserId,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int32_t>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_MIN_SDK_VERSION,
        bundleInfo.minSdkVersion,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int32_t>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_MAX_SDK_VERSION,
        bundleInfo.maxSdkVersion,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_MAIN_ENTRY,
        bundleInfo.mainEntry,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_CPU_ABI,
        bundleInfo.cpuAbi,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_APPID,
        bundleInfo.appId,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_COMPATIBLE_VERSION,
        bundleInfo.compatibleVersion,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<uint32_t>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_TARGET_VERSION,
        bundleInfo.targetVersion,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_RELEASE_TYPE,
        bundleInfo.releaseType,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_UID,
        bundleInfo.uid,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_GID,
        bundleInfo.gid,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_SEINFO,
        bundleInfo.seInfo,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int64_t>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_INSTALL_TIME,
        bundleInfo.installTime,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<int64_t>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_UPDATE_TIME,
        bundleInfo.updateTime,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::string>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_ENTRY_MODULE_NAME,
        bundleInfo.entryModuleName,
        JsonType::STRING,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_ENTRY_INSTALLATION_FREE,
        bundleInfo.entryInstallationFree,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_REQ_PERMISSIONS,
        bundleInfo.reqPermissions,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::vector<int32_t>>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_REQ_PERMISSION_STATES,
        bundleInfo.reqPermissionStates,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::NUMBER);
    GetValueIfFindKey<std::vector<RequestPermission>>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_REQ_PERMISSION_DETAILS,
        bundleInfo.reqPermissionDetails,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::OBJECT);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_DEF_PERMISSIONS,
        bundleInfo.defPermissions,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_HAP_MODULE_NAMES,
        bundleInfo.hapModuleNames,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_MODULE_NAMES,
        bundleInfo.moduleNames,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_MODULE_PUBLIC_DIRS,
        bundleInfo.modulePublicDirs,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_MODULE_DIRS,
        bundleInfo.moduleDirs,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<std::vector<std::string>>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_MODULE_RES_PATHS,
        bundleInfo.moduleResPaths,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::STRING);
    GetValueIfFindKey<bool>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_SINGLETON,
        bundleInfo.singleton,
        JsonType::BOOLEAN,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<std::vector<ExtensionAbilityInfo>>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_EXTENSION_ABILITY_INFOS,
        bundleInfo.extensionInfos,
        JsonType::ARRAY,
        false,
        parseResult,
        ArrayType::OBJECT);
    GetValueIfFindKey<int32_t>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_APP_INDEX,
        bundleInfo.appIndex,
        JsonType::NUMBER,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    GetValueIfFindKey<SignatureInfo>(jsonObject,
        jsonObjectEnd,
        BUNDLE_INFO_SIGNATURE_INFO,
        bundleInfo.signatureInfo,
        JsonType::OBJECT,
        false,
        parseResult,
        ArrayType::NOT_ARRAY);
    if (parseResult != ERR_OK) {
        APP_LOGE("BundleInfo from_json error, error code : %{public}d", parseResult);
    }
}
}  // namespace AppExecFwk
}  // namespace OHOS
