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

#ifndef FOUNDATION_APPEXECFWK_SERVICES_BUNDLEMGR_INCLUDE_INNER_BUNDLE_INFO_H
#define FOUNDATION_APPEXECFWK_SERVICES_BUNDLEMGR_INCLUDE_INNER_BUNDLE_INFO_H

#include "nocopyable.h"

#include "ability_info.h"
#include "bundle_constants.h"
#include "bundle_info.h"
#include "common_event_info.h"
#include "common_profile.h"
#include "extension_ability_info.h"
#include "form_info.h"
#include "hap_module_info.h"
#include "inner_bundle_user_info.h"
#include "json_util.h"
#include "shortcut_info.h"
#include "want.h"
#include "distributed_bundle_info.h"

namespace OHOS {
namespace AppExecFwk {
struct Distro {
    bool deliveryWithInstall = false;
    std::string moduleName;
    std::string moduleType;
    bool installationFree = false;
};

struct DefinePermission {
    std::string name;
    std::string grantMode = Profile::DEFINEPERMISSION_GRANT_MODE_DEFAULT_VALUE;
    std::string availableLevel = Profile::DEFINEPERMISSION_AVAILABLE_LEVEL_DEFAULT_VALUE;
    std::vector<std::string> availableScope;
    bool provisionEnable = true;
    bool distributedSceneEnable = false;
    std::string label;
    int32_t labelId = 0;
    std::string description;
    int32_t descriptionId = 0;
};

struct InnerModuleInfo {
    std::string modulePackage;
    std::string moduleName;
    std::string modulePath;
    std::string moduleDataDir;
    std::string moduleResPath;
    std::string label;
    int32_t labelId = 0;
    std::string description;
    int32_t descriptionId = 0;
    std::string mainAbility; // config.json : mainAbility; module.json : mainElement
    std::string entryAbilityKey; // skills contains "action.system.home" and "entity.system.home"
    std::string srcPath;
    bool isEntry = false;
    bool installationFree = false;
    // all user's value of isRemovable
    // key:userId
    // value:isRemovable true or flase
    std::map<std::string, bool> isRemovable;
    MetaData metaData;
    ModuleColorMode colorMode = ModuleColorMode::AUTO;
    Distro distro;
    std::vector<std::string> reqCapabilities;
    std::vector<DefinePermission> defPermissions;
    std::vector<std::string> abilityKeys;
    std::vector<std::string> skillKeys;
    // new version fields
    std::string pages;
    std::string process;
    std::string srcEntrance;
    std::string uiSyntax;
    std::string virtualMachine;
    bool isModuleJson = false;
    bool isStageBasedModel = false;
    std::vector<DefinePermission> definePermissions;
    std::vector<RequestPermission> requestPermissions;
    std::vector<std::string> deviceTypes;
    std::vector<std::string> extensionKeys;
    std::vector<std::string> extensionSkillKeys;
    std::vector<Metadata> metadata;
    int32_t upgradeFlag = 0;
    std::vector<std::string> dependencies;
};

struct SkillUri {
    std::string scheme;
    std::string host;
    std::string port;
    std::string path;
    std::string pathStartWith;
    std::string pathRegex;
    std::string type;
};

struct Skill {
public:
    std::vector<std::string> actions;
    std::vector<std::string> entities;
    std::vector<SkillUri> uris;
    bool Match(const OHOS::AAFwk::Want &want) const;
    bool MatchLauncher(const OHOS::AAFwk::Want &want) const;
private:
    bool MatchAction(const std::string &action) const;
    bool MatchEntities(const std::vector<std::string> &paramEntities) const;
    bool MatchUriAndType(const std::string &uriString, const std::string &type) const;
    bool MatchUri(const std::string &uriString, const SkillUri &skillUri) const;
    bool MatchType(const std::string &type, const std::string &skillUriType) const;
};

enum InstallExceptionStatus : int32_t {
    INSTALL_START = 1,
    INSTALL_FINISH,
    UPDATING_EXISTED_START,
    UPDATING_NEW_START,
    UPDATING_FINISH,
    UNINSTALL_BUNDLE_START,
    UNINSTALL_PACKAGE_START,
    UNKNOWN_STATUS,
};

struct InstallMark {
    std::string bundleName;
    std::string packageName;
    int32_t status = InstallExceptionStatus::UNKNOWN_STATUS;
};

struct SandboxAppPersistentInfo {
    uint32_t accessTokenId = 0;
    int32_t appIndex = 0;
    int32_t userId = Constants::INVALID_USERID;
};

class InnerBundleInfo {
public:
    enum class BundleStatus {
        ENABLED = 1,
        DISABLED,
    };

    InnerBundleInfo();
    ~InnerBundleInfo();
    /**
     * @brief Transform the InnerBundleInfo object to json.
     * @param jsonObject Indicates the obtained json object.
     * @return
     */
    void ToJson(nlohmann::json &jsonObject) const;
    /**
     * @brief Transform the json object to InnerBundleInfo object.
     * @param jsonObject Indicates the obtained json object.
     * @return Returns 0 if the json object parsed successfully; returns error code otherwise.
     */
    int32_t FromJson(const nlohmann::json &jsonObject);
    /**
     * @brief Add module info to old InnerBundleInfo object.
     * @param newInfo Indicates the new InnerBundleInfo object.
     * @return Returns true if the module successfully added; returns false otherwise.
     */
    bool AddModuleInfo(const InnerBundleInfo &newInfo);
    /**
     * @brief Update module info to old InnerBundleInfo object.
     * @param newInfo Indicates the new InnerBundleInfo object.
     * @return
     */
    void UpdateModuleInfo(const InnerBundleInfo &newInfo);
    /**
     * @brief Remove module info from InnerBundleInfo object.
     * @param modulePackage Indicates the module package to be remove.
     * @return
     */
    void RemoveModuleInfo(const std::string &modulePackage);
    /**
     * @brief Find hap module info by module package.
     * @param modulePackage Indicates the module package.
     * @param userId Indicates the user ID.
     * @return Returns the HapModuleInfo object if find it; returns null otherwise.
     */
    std::optional<HapModuleInfo> FindHapModuleInfo(
        const std::string &modulePackage, int32_t userId = Constants::UNSPECIFIED_USERID) const;
    /**
     * @brief Find abilityInfo by bundle name and ability name.
     * @param bundleName Indicates the bundle name.
     * @param moduleName Indicates the module name
     * @param abilityName Indicates the ability name.
     * @param userId Indicates the user ID.
     * @return Returns the AbilityInfo object if find it; returns null otherwise.
     */
    std::optional<AbilityInfo> FindAbilityInfo(const std::string &bundleName,
        const std::string &moduleName, const std::string &abilityName,
        int32_t userId = Constants::UNSPECIFIED_USERID) const;
    /**
     * @brief Find abilityInfo of list by bundle name.
     * @param bundleName Indicates the bundle name.
     * @param userId Indicates the user ID.
     * @return Returns the AbilityInfo of list if find it; returns null otherwise.
     */
    std::optional<std::vector<AbilityInfo>> FindAbilityInfos(
        const std::string &bundleName, int32_t userId = Constants::UNSPECIFIED_USERID) const;
    /**
     * @brief Find extensionInfo by bundle name and extension name.
     * @param bundleName Indicates the bundle name.
     * @param moduleName Indicates the module name.
     * @param extensionName Indicates the extension name
     * @return Returns the ExtensionAbilityInfo object if find it; returns null otherwise.
     */
    std::optional<ExtensionAbilityInfo> FindExtensionInfo(
        const std::string &bundleName, const std::string &moduleName, const std::string &extensionName) const;
    /**
     * @brief Find extensionInfos by bundle name.
     * @param bundleName Indicates the bundle name.
     * @return Returns the ExtensionAbilityInfo array if find it; returns null otherwise.
     */
    std::optional<std::vector<ExtensionAbilityInfo>> FindExtensionInfos(const std::string &bundleName) const;
    /**
     * @brief Find abilityInfo of list for clone by bundle name and ability name.
     * @param bundleName Indicates the bundle name.
     * @param abilityName Indicates the ability name
     * @param userId Indicates the user ID.
     * @return Returns the AbilityInfo of list if find it; returns null otherwise.
     */
    void FindAbilityInfosForClone(const std::string &bundleName,
        const std::string &abilityName, int32_t userId, std::vector<AbilityInfo> &abilitys);
    /**
     * @brief Transform the InnerBundleInfo object to string.
     * @return Returns the string object
     */
    std::string ToString() const;
    /**
     * @brief Add ability infos to old InnerBundleInfo object.
     * @param abilityInfos Indicates the AbilityInfo object to be add.
     * @return
     */
    void AddModuleAbilityInfo(const std::map<std::string, AbilityInfo> &abilityInfos)
    {
        for (const auto &ability : abilityInfos) {
            baseAbilityInfos_.try_emplace(ability.first, ability.second);
        }
    }

    void AddModuleExtensionInfos(const std::map<std::string, ExtensionAbilityInfo> &extensionInfos)
    {
        for (const auto &extensionInfo : extensionInfos) {
            baseExtensionInfos_.try_emplace(extensionInfo.first, extensionInfo.second);
        }
    }
    /**
     * @brief Add skill infos to old InnerBundleInfo object.
     * @param skillInfos Indicates the Skill object to be add.
     * @return
     */
    void AddModuleSkillInfo(const std::map<std::string, std::vector<Skill>> &skillInfos)
    {
        for (const auto &skills : skillInfos) {
            skillInfos_.try_emplace(skills.first, skills.second);
        }
    }
    void AddModuleExtensionSkillInfos(const std::map<std::string, std::vector<Skill>> &extensionSkillInfos)
    {
        for (const auto &skills : extensionSkillInfos) {
            extensionSkillInfos_.try_emplace(skills.first, skills.second);
        }
    }
    /**
     * @brief Add form infos to old InnerBundleInfo object.
     * @param formInfos Indicates the Forms object to be add.
     * @return
     */
    void AddModuleFormInfo(const std::map<std::string, std::vector<FormInfo>> &formInfos)
    {
        for (const auto &forms : formInfos) {
            formInfos_.try_emplace(forms.first, forms.second);
        }
    }
    /**
     * @brief Add common events to old InnerBundleInfo object.
     * @param commonEvents Indicates the Common Event object to be add.
     * @return
     */
    void AddModuleCommonEvent(const std::map<std::string, CommonEventInfo> &commonEvents)
    {
        for (const auto &commonEvent : commonEvents) {
            commonEvents_.try_emplace(commonEvent.first, commonEvent.second);
        }
    }
    /**
     * @brief Add shortcut infos to old InnerBundleInfo object.
     * @param shortcutInfos Indicates the Shortcut object to be add.
     * @return
     */
    void AddModuleShortcutInfo(const std::map<std::string, ShortcutInfo> &shortcutInfos)
    {
        for (const auto &shortcut : shortcutInfos) {
            shortcutInfos_.try_emplace(shortcut.first, shortcut.second);
        }
    }
    /**
     * @brief Add innerModuleInfos to old InnerBundleInfo object.
     * @param innerModuleInfos Indicates the InnerModuleInfo object to be add.
     * @return
     */
    void AddInnerModuleInfo(const std::map<std::string, InnerModuleInfo> &innerModuleInfos)
    {
        for (const auto &info : innerModuleInfos) {
            innerModuleInfos_.try_emplace(info.first, info.second);
        }
    }
    /**
     * @brief Get application name.
     * @return Return application name
     */
    std::string GetApplicationName() const
    {
        return baseApplicationInfo_.name;
    }
    /**
     * @brief Set bundle status.
     * @param status Indicates the BundleStatus object to set.
     * @return
     */
    void SetBundleStatus(const BundleStatus &status)
    {
        bundleStatus_ = status;
    }
    /**
     * @brief Get bundle status.
     * @return Return the BundleStatus object
     */
    BundleStatus GetBundleStatus() const
    {
        return bundleStatus_;
    }
    /**
     * @brief Set bundle install time.
     * @param time Indicates the install time to set.
     * @param userId Indicates the user ID.
     * @return
     */
    void SetBundleInstallTime(
        const int64_t time, int32_t userId = Constants::UNSPECIFIED_USERID);
    /**
     * @brief Get bundle install time.
     * @param userId Indicates the user ID.
     * @return Return the bundle install time.
     */
    int64_t GetBundleInstallTime(int32_t userId = Constants::UNSPECIFIED_USERID) const
    {
        InnerBundleUserInfo innerBundleUserInfo;
        if (!GetInnerBundleUserInfo(userId, innerBundleUserInfo)) {
            APP_LOGE("can not find userId %{public}d when GetBundleInstallTime", userId);
            return -1;
        }
        return innerBundleUserInfo.installTime;
    }
    /**
     * @brief Set bundle update time.
     * @param time Indicates the update time to set.
     * @param userId Indicates the user ID.
     * @return
     */
    void SetBundleUpdateTime(const int64_t time, int32_t userId = Constants::UNSPECIFIED_USERID);
    /**
     * @brief Get bundle update time.
     * @param userId Indicates the user ID.
     * @return Return the bundle update time.
     */
    int64_t GetBundleUpdateTime(int32_t userId = Constants::UNSPECIFIED_USERID) const
    {
        InnerBundleUserInfo innerBundleUserInfo;
        if (!GetInnerBundleUserInfo(userId, innerBundleUserInfo)) {
            APP_LOGE("can not find userId %{public}d when GetBundleUpdateTime", userId);
            return -1;
        }
        return innerBundleUserInfo.updateTime;
    }
    /**
     * @brief Get bundle name.
     * @return Return bundle name
     */
    const std::string GetBundleName() const
    {
        return baseApplicationInfo_.bundleName;
    }
    /**
     * @brief Get baseBundleInfo.
     * @return Return the BundleInfo object.
     */
    BundleInfo GetBaseBundleInfo() const
    {
        return baseBundleInfo_;
    }
    /**
     * @brief Set baseBundleInfo.
     * @param bundleInfo Indicates the BundleInfo object.
     */
    void SetBaseBundleInfo(const BundleInfo &bundleInfo)
    {
        baseBundleInfo_ = bundleInfo;
    }
    /**
     * @brief Update baseBundleInfo.
     * @param bundleInfo Indicates the new BundleInfo object.
     * @return
     */
    void UpdateBaseBundleInfo(const BundleInfo &bundleInfo, bool isEntry);
    /**
     * @brief Get baseApplicationInfo.
     * @return Return the ApplicationInfo object.
     */
    ApplicationInfo GetBaseApplicationInfo() const
    {
        return baseApplicationInfo_;
    }
    /**
     * @brief Set baseApplicationInfo.
     * @param applicationInfo Indicates the ApplicationInfo object.
     */
    void SetBaseApplicationInfo(const ApplicationInfo &applicationInfo)
    {
        baseApplicationInfo_ = applicationInfo;
    }
    /**
     * @brief Update baseApplicationInfo.
     * @param applicationInfo Indicates the ApplicationInfo object.
     */
    void UpdateBaseApplicationInfo(const ApplicationInfo &applicationInfo);
    /**
     * @brief Get application enabled.
     * @param userId Indicates the user ID.
     * @return Return whether the application is enabled.
     */
    bool GetApplicationEnabled(int32_t userId = Constants::UNSPECIFIED_USERID) const
    {
        InnerBundleUserInfo innerBundleUserInfo;
        if (!GetInnerBundleUserInfo(userId, innerBundleUserInfo)) {
            APP_LOGE("can not find userId %{public}d when GetApplicationEnabled", userId);
            return false;
        }

        return innerBundleUserInfo.bundleUserInfo.enabled;
    }
    /**
     * @brief Set application enabled.
     * @param userId Indicates the user ID.
     * @return Return whether the application is enabled.
     */
    void SetApplicationEnabled(bool enabled, int32_t userId = Constants::UNSPECIFIED_USERID);
    /**
     * @brief Get application code path.
     * @return Return the string object.
     */
    const std::string GetAppCodePath() const
    {
        return baseApplicationInfo_.codePath;
    }
    /**
     * @brief Set application code path.
     * @param codePath Indicates the code path to be set.
     */
    void SetAppCodePath(const std::string codePath)
    {
        baseApplicationInfo_.codePath = codePath;
    }
    /**
     * @brief Insert innerModuleInfos.
     * @param modulePackage Indicates the modulePackage object as key.
     * @param innerModuleInfo Indicates the InnerModuleInfo object as value.
     */
    void InsertInnerModuleInfo(const std::string &modulePackage, const InnerModuleInfo &innerModuleInfo)
    {
        innerModuleInfos_.try_emplace(modulePackage, innerModuleInfo);
    }
    /**
     * @brief Insert AbilityInfo.
     * @param key bundleName.moduleName.abilityName
     * @param abilityInfo value.
     */
    void InsertAbilitiesInfo(const std::string &key, const AbilityInfo &abilityInfo)
    {
        baseAbilityInfos_.emplace(key, abilityInfo);
    }
    /**
     * @brief Insert ExtensionAbilityInfo.
     * @param key bundleName.moduleName.extensionName
     * @param extensionInfo value.
     */
    void InsertExtensionInfo(const std::string &key, const ExtensionAbilityInfo &extensionInfo)
    {
        baseExtensionInfos_.emplace(key, extensionInfo);
    }
    /**
     * @brief Insert ability skillInfos.
     * @param key bundleName.moduleName.abilityName
     * @param skills ability skills.
     */
    void InsertSkillInfo(const std::string &key, const std::vector<Skill> &skills)
    {
        skillInfos_.emplace(key, skills);
    }
    /**
     * @brief Insert extension skillInfos.
     * @param key bundleName.moduleName.extensionName
     * @param skills extension skills.
     */
    void InsertExtensionSkillInfo(const std::string &key, const std::vector<Skill> &skills)
    {
        extensionSkillInfos_.emplace(key, skills);
    }
    /**
     * @brief Find AbilityInfo object by Uri.
     * @param abilityUri Indicates the ability uri.
     * @param userId Indicates the user ID.
     * @return Returns the AbilityInfo object if find it; returns null otherwise.
     */
    std::optional<AbilityInfo> FindAbilityInfoByUri(const std::string &abilityUri) const
    {
        APP_LOGI("Uri is %{public}s", abilityUri.c_str());
        for (const auto &ability : baseAbilityInfos_) {
            auto abilityInfo = ability.second;
            if (abilityInfo.uri.size() < Constants::DATA_ABILITY_URI_PREFIX.size()) {
                continue;
            }

            auto configUri = abilityInfo.uri.substr(Constants::DATA_ABILITY_URI_PREFIX.size());
            APP_LOGI("configUri is %{public}s", configUri.c_str());
            if (configUri == abilityUri) {
                return abilityInfo;
            }
        }
        return std::nullopt;
    }

    bool FindExtensionAbilityInfoByUri(const std::string &uri, ExtensionAbilityInfo &extensionAbilityInfo) const
    {
        for (const auto &item : baseExtensionInfos_) {
            if (uri == item.second.uri) {
                extensionAbilityInfo = item.second;
                APP_LOGD("find target extension, bundleName : %{public}s, moduleName : %{public}s, name : %{public}s",
                    extensionAbilityInfo.bundleName.c_str(), extensionAbilityInfo.moduleName.c_str(),
                    extensionAbilityInfo.name.c_str());
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Find AbilityInfo object by Uri.
     * @param abilityUri Indicates the ability uri.
     * @param userId Indicates the user ID.
     * @return Returns the AbilityInfo object if find it; returns null otherwise.
     */
    void FindAbilityInfosByUri(const std::string &abilityUri,
        std::vector<AbilityInfo> &abilityInfos,  int32_t userId = Constants::UNSPECIFIED_USERID)
    {
        APP_LOGI("Uri is %{public}s", abilityUri.c_str());
        for (auto &ability : baseAbilityInfos_) {
            auto abilityInfo = ability.second;
            if (abilityInfo.uri.size() < Constants::DATA_ABILITY_URI_PREFIX.size()) {
                continue;
            }

            auto configUri = abilityInfo.uri.substr(Constants::DATA_ABILITY_URI_PREFIX.size());
            APP_LOGI("configUri is %{public}s", configUri.c_str());
            if (configUri == abilityUri) {
                GetApplicationInfo(
                    ApplicationFlag::GET_APPLICATION_INFO_WITH_PERMISSION, userId, abilityInfo.applicationInfo);
                abilityInfos.emplace_back(abilityInfo);
            }
        }
        return;
    }
    /**
     * @brief Get all ability names in application.
     * @return Returns ability names.
     */
    auto GetAbilityNames() const
    {
        std::vector<std::string> abilityNames;
        for (auto &ability : baseAbilityInfos_) {
            abilityNames.emplace_back(ability.second.name);
        }
        return abilityNames;
    }
    /**
     * @brief Get version code in application.
     * @return Returns version code.
     */
    uint32_t GetVersionCode() const
    {
        return baseBundleInfo_.versionCode;
    }
    /**
     * @brief Get version name in application.
     * @return Returns version name.
     */
    std::string GetVersionName() const
    {
        return baseBundleInfo_.versionName;
    }
    /**
     * @brief Get vendor in application.
     * @return Returns vendor.
     */
    std::string GetVendor() const
    {
        return baseBundleInfo_.vendor;
    }
    /**
     * @brief Get comparible version in application.
     * @return Returns comparible version.
     */
    uint32_t GetCompatibleVersion() const
    {
        return baseBundleInfo_.compatibleVersion;
    }
    /**
     * @brief Get target version in application.
     * @return Returns target version.
     */
    uint32_t GetTargetVersion() const
    {
        return baseBundleInfo_.targetVersion;
    }
    /**
     * @brief Get release type in application.
     * @return Returns release type.
     */
    std::string GetReleaseType() const
    {
        return baseBundleInfo_.releaseType;
    }
    /**
     * @brief Get minCompatibleVersionCode in base bundleInfo.
     * @return Returns release type.
     */
    uint32_t GetMinCompatibleVersionCode() const
    {
        return baseBundleInfo_.minCompatibleVersionCode;
    }
    /**
     * @brief Get install mark in application.
     * @return Returns install mark.
     */
    void SetInstallMark(const std::string &bundleName, const std::string &packageName,
        const InstallExceptionStatus &status)
    {
        mark_.bundleName = bundleName;
        mark_.packageName = packageName;
        mark_.status = status;
    }
    /**
     * @brief Get install mark in application.
     * @return Returns install mark.
     */
    InstallMark GetInstallMark() const
    {
        return mark_;
    }
    /**
     * @brief Set application base data dir.
     * @param baseDataDir Indicates the dir to be set.
     */
    void SetBaseDataDir(const std::string &baseDataDir)
    {
        baseDataDir_ = baseDataDir;
    }
    /**
     * @brief Get application base data dir.
     * @return Return the string object.
     */
    std::string GetBaseDataDir() const
    {
        return baseDataDir_;
    }
    /**
     * @brief Get application data dir.
     * @return Return the string object.
     */
    std::string GetAppDataDir() const
    {
        return baseApplicationInfo_.dataDir;
    }
    /**
     * @brief Set application data dir.
     * @param dataDir Indicates the data Dir to be set.
     */
    void SetAppDataDir(std::string dataDir)
    {
        baseApplicationInfo_.dataDir = dataDir;
    }
    /**
     * @brief Set application data base dir.
     * @param dataBaseDir Indicates the data base Dir to be set.
     */
    void SetAppDataBaseDir(std::string dataBaseDir)
    {
        baseApplicationInfo_.dataBaseDir = dataBaseDir;
    }
    /**
     * @brief Set application cache dir.
     * @param cacheDir Indicates the cache Dir to be set.
     */
    void SetAppCacheDir(std::string cacheDir)
    {
        baseApplicationInfo_.cacheDir = cacheDir;
    }
    /**
     * @brief Set application uid.
     * @param uid Indicates the uid to be set.
     */
    void SetUid(int uid) {}
    /**
     * @brief Get application uid.
     * @param userId Indicates the user ID.
     * @return Returns the uid.
     */
    int GetUid(int32_t userId = Constants::UNSPECIFIED_USERID) const
    {
        InnerBundleUserInfo innerBundleUserInfo;
        if (!GetInnerBundleUserInfo(userId, innerBundleUserInfo)) {
            return Constants::INVALID_UID;
        }

        return innerBundleUserInfo.uid;
    }
    /**
     * @brief Get application gid.
     * @param userId Indicates the user ID.
     * @return Returns the gid.
     */
    int GetGid(int32_t userId = Constants::UNSPECIFIED_USERID) const
    {
        InnerBundleUserInfo innerBundleUserInfo;
        if (!GetInnerBundleUserInfo(userId, innerBundleUserInfo)) {
            return Constants::INVALID_GID;
        }

        if (innerBundleUserInfo.gids.empty()) {
            return Constants::INVALID_GID;
        }

        return innerBundleUserInfo.gids[0];
    }
    /**
     * @brief Set application gid.
     * @param gid Indicates the gid to be set.
     */
    void SetGid(int gid) {}
    /**
     * @brief Get application AppType.
     * @return Returns the AppType.
     */
    Constants::AppType GetAppType() const
    {
        return appType_;
    }
    /**
     * @brief Set application AppType.
     * @param gid Indicates the AppType to be set.
     */
    void SetAppType(Constants::AppType appType)
    {
        appType_ = appType;
        if (appType_ == Constants::AppType::SYSTEM_APP) {
            baseApplicationInfo_.isSystemApp = true;
        } else {
            baseApplicationInfo_.isSystemApp = false;
        }
    }
    /**
     * @brief Get application user id.
     * @return Returns the user id.
     */
    int GetUserId() const
    {
        return userId_;
    }
    /**
     * @brief Set application user id.
     * @param gid Indicates the user id to be set.
     */
    void SetUserId(int userId)
    {
        userId_ = userId;
    }

    // only used in install progress with newInfo
    std::string GetCurrentModulePackage() const
    {
        return currentPackage_;
    }
    void SetCurrentModulePackage(const std::string &modulePackage)
    {
        currentPackage_ = modulePackage;
    }
    void AddModuleSrcDir(const std::string &moduleSrcDir)
    {
        if (innerModuleInfos_.count(currentPackage_) == 1) {
            innerModuleInfos_.at(currentPackage_).modulePath = moduleSrcDir;
        }
    }
    void AddModuleDataDir(const std::string &moduleDataDir)
    {
        if (innerModuleInfos_.count(currentPackage_) == 1) {
            innerModuleInfos_.at(currentPackage_).moduleDataDir = moduleDataDir;
        }
    }
    void AddModuleResPath(const std::string &moduleSrcDir)
    {
        if (innerModuleInfos_.count(currentPackage_) == 1) {
            std::string moduleResPath;
            if (isNewVersion_) {
                moduleResPath = moduleSrcDir + Constants::PATH_SEPARATOR + Constants::RESOURCES_INDEX;
            } else {
                moduleResPath = moduleSrcDir + Constants::PATH_SEPARATOR + Constants::ASSETS_DIR +
                    Constants::PATH_SEPARATOR +innerModuleInfos_.at(currentPackage_).distro.moduleName +
                    Constants::PATH_SEPARATOR + Constants::RESOURCES_INDEX;
            }

            innerModuleInfos_.at(currentPackage_).moduleResPath = moduleResPath;
            for (auto &abilityInfo : baseAbilityInfos_) {
                abilityInfo.second.resourcePath = moduleResPath;
            }
            for (auto &extensionInfo : baseExtensionInfos_) {
                extensionInfo.second.resourcePath = moduleResPath;
            }
        }
    }

    std::vector<DefinePermission> GetDefPermissions() const
    {
        std::vector<DefinePermission> defPermissions;
        if (innerModuleInfos_.count(currentPackage_) == 1) {
            defPermissions = innerModuleInfos_.at(currentPackage_).defPermissions;
        }
        return defPermissions;
    }

    std::vector<DefinePermission> GetDefinePermissions() const
    {
        std::vector<DefinePermission> definePermissions;
        if (innerModuleInfos_.count(currentPackage_) == 1) {
            definePermissions = innerModuleInfos_.at(currentPackage_).definePermissions;
        }
        return definePermissions;
    }

    std::vector<RequestPermission> GetRequestPermissions() const
    {
        std::vector<RequestPermission> requestPermissions;
        if (innerModuleInfos_.count(currentPackage_) == 1) {
            requestPermissions = innerModuleInfos_.at(currentPackage_).requestPermissions;
        }
        return requestPermissions;
    }

    std::vector<DefinePermission> GetAllDefinePermissions() const;

    std::vector<RequestPermission> GetAllRequestPermissions() const;

    bool FindModule(std::string modulePackage) const
    {
        return (innerModuleInfos_.find(modulePackage) != innerModuleInfos_.end());
    }

    void SetIsKeepAlive(bool isKeepAlive)
    {
        baseBundleInfo_.isKeepAlive = isKeepAlive;
    }

    bool GetIsKeepAlive() const
    {
        return baseBundleInfo_.isKeepAlive;
    }

    void SetIsNativeApp(bool isNativeApp)
    {
        baseBundleInfo_.isNativeApp = isNativeApp;
    }

    bool GetIsNativeApp() const
    {
        return baseBundleInfo_.isNativeApp;
    }

    void SetApplicationInfoUid()
    {
        baseApplicationInfo_.uid = uid_;
    }

    int GetApplicationInfoUid() const
    {
        return baseApplicationInfo_.uid;
    }

    void SetIsCloned(bool isClone)
    {
        baseApplicationInfo_.isCloned = isClone;
    }

    bool GetisCloned() const
    {
        return baseApplicationInfo_.isCloned;
    }

    void SetNewBundleName(std::string bundlename)
    {
        std::string strUid = std::to_string(uid_);
        newBundleName_ = bundlename + '#' + strUid;
        APP_LOGI("set clone newBundleName_ %{public}s", newBundleName_.c_str());
    }

    std::string GetDBKeyBundleName() const
    {
        if (!baseApplicationInfo_.isCloned) {
            return baseApplicationInfo_.bundleName;
        }
        return newBundleName_;
    }

    void SetIsLauncherApp(bool isLauncher)
    {
        baseApplicationInfo_.isLauncherApp = isLauncher;
    }

    bool GetIsLauncherApp() const
    {
        return baseApplicationInfo_.isLauncherApp;
    }

    void SetIsFreeInstallApp(bool isFreeInstall)
    {
        baseApplicationInfo_.isFreeInstallApp = isFreeInstall;
    }

    bool GetIsFreeInstallApp() const
    {
        return baseApplicationInfo_.isFreeInstallApp;
    }

    std::string GetMainAbility() const;

    void GetMainAbilityInfo(AbilityInfo &abilityInfo) const;

    std::string GetModuleDir(std::string modulePackage) const
    {
        if (innerModuleInfos_.find(modulePackage) != innerModuleInfos_.end()) {
            return innerModuleInfos_.at(modulePackage).modulePath;
        }
        return Constants::EMPTY_STRING;
    }

    std::string GetModuleDataDir(std::string modulePackage) const
    {
        if (innerModuleInfos_.find(modulePackage) != innerModuleInfos_.end()) {
            return innerModuleInfos_.at(modulePackage).moduleDataDir;
        }
        return Constants::EMPTY_STRING;
    }

    bool IsDisabled() const
    {
        return (bundleStatus_ == BundleStatus::DISABLED);
    }

    bool IsEnabled() const
    {
        return (bundleStatus_ == BundleStatus::ENABLED);
    }

    bool IsOnlyModule(const std::string &modulePackage)
    {
        if ((innerModuleInfos_.size() == 1) && (innerModuleInfos_.count(modulePackage) == 1)) {
            return true;
        }
        return false;
    }

    void SetProvisionId(const std::string &provisionId)
    {
        baseBundleInfo_.appId = baseBundleInfo_.name + Constants::FILE_UNDERLINE + provisionId;
    }

    std::string GetProvisionId() const
    {
        if (!baseBundleInfo_.appId.empty()) {
            return baseBundleInfo_.appId.substr(baseBundleInfo_.name.size() + 1);
        }
        return "";
    }

    void SetAppFeature(const std::string &appFeature)
    {
        appFeature_ = appFeature;
    }

    std::string GetAppFeature() const
    {
        return appFeature_;
    }

    void SetAppPrivilegeLevel(const std::string &appPrivilegeLevel)
    {
        if (appPrivilegeLevel.empty()) {
            return;
        }
        baseApplicationInfo_.appPrivilegeLevel = appPrivilegeLevel;
    }

    std::string GetAppPrivilegeLevel() const
    {
        return baseApplicationInfo_.appPrivilegeLevel;
    }

    bool HasEntry() const;

    /**
     * @brief Insert formInfo.
     * @param keyName Indicates object as key.
     * @param formInfos Indicates the formInfo object as value.
     */
    void InsertFormInfos(const std::string &keyName, const std::vector<FormInfo> &formInfos)
    {
        formInfos_.emplace(keyName, formInfos);
    }
    /**
     * @brief Insert commonEvent.
     * @param keyName Indicates object as key.
     * @param commonEvents Indicates the common event object as value.
     */
    void InsertCommonEvents(const std::string &keyName, const CommonEventInfo &commonEvents)
    {
        commonEvents_.emplace(keyName, commonEvents);
    }
    /**
     * @brief Insert shortcutInfos.
     * @param keyName Indicates object as key.
     * @param shortcutInfos Indicates the shortcutInfos object as value.
     */
    void InsertShortcutInfos(const std::string &keyName, const ShortcutInfo &shortcutInfos)
    {
        shortcutInfos_.emplace(keyName, shortcutInfos);
    }
    // use for new Info in updating progress
    void RestoreFromOldInfo(const InnerBundleInfo &oldInfo)
    {
        SetAppCodePath(oldInfo.GetAppCodePath());
        SetBaseDataDir(oldInfo.GetBaseDataDir());
        SetUid(oldInfo.GetUid());
        SetGid(oldInfo.GetGid());
    }
    void RestoreModuleInfo(const InnerBundleInfo &oldInfo)
    {
        if (oldInfo.FindModule(currentPackage_)) {
            innerModuleInfos_.at(currentPackage_).moduleDataDir = oldInfo.GetModuleDataDir(currentPackage_);
        }
    }
    /**
     * @brief Set ability enabled.
     * @param bundleName Indicates the bundleName.
     * @param moduleName Indicates the moduleName.
     * @param abilityName Indicates the abilityName.
     * @param isEnabled Indicates the ability enabled.
     * @param userId Indicates the user id.
     * @return Return whether the application is enabled.
     */
    bool SetAbilityEnabled(
        const std::string &bundleName, const std::string &moduleName, const std::string &abilityName,
        bool isEnabled, int32_t userId);
    /**
     * @brief Set the Application Need Recover object
     * @param moduleName Indicates the module name of the application.
     * @param upgradeFlag Indicates the module is need update or not.
     * @return Return true if set data successfully.
     */
    bool SetModuleUpgradeFlag(std::string moduleName, int32_t upgradeFlag);

    /**
     * @brief Get the Application Need Recover object
     * @param moduleName Indicates the module name of the application.
     * @return upgradeFlag type,NOT_UPGRADE means not need to be upgraded,SINGLE_UPGRADE means
     *         single module need to be upgraded,RELATION_UPGRADE means relation module need to be upgraded.
     */
    int32_t GetModuleUpgradeFlag(std::string moduleName) const;

    /**
     * @brief Obtains configuration information about an application.
     * @param flags Indicates the flag used to specify information contained
     *             in the ApplicationInfo object that will be returned.
     * @param userId Indicates the user ID.
     * @param appInfo Indicates the obtained ApplicationInfo object.
     */
    void GetApplicationInfo(int32_t flags, int32_t userId, ApplicationInfo &appInfo) const;
    /**
     * @brief Obtains configuration information about an bundle.
     * @param flags Indicates the flag used to specify information contained in the BundleInfo that will be returned.
     * @param bundleInfos Indicates all of the obtained BundleInfo objects.
     * @param userId Indicates the user ID.
     */
    void GetBundleInfo(int32_t flags, BundleInfo &bundleInfo, int32_t userId = Constants::UNSPECIFIED_USERID) const;
    /**
     * @brief Check if special metadata is in the application.
     * @param metaData Indicates the special metaData.
     * @param bundleInfos Returns true if the metadata in application; returns false otherwise.
     */
    bool CheckSpecialMetaData(const std::string &metaData) const;
    /**
     * @brief Obtains the FormInfo objects provided by all applications on the device.
     * @param moduleName Indicates the module name of the application.
     * @param formInfos List of FormInfo objects if obtained;
     */
    void GetFormsInfoByModule(const std::string &moduleName, std::vector<FormInfo> &formInfos) const;
    /**
     * @brief Obtains the FormInfo objects provided by a specified application on the device.
     * @param formInfos List of FormInfo objects if obtained;
     */
    void GetFormsInfoByApp(std::vector<FormInfo> &formInfos) const;
    /**
     * @brief Obtains the ShortcutInfo objects provided by a specified application on the device.
     * @param shortcutInfos List of ShortcutInfo objects if obtained.
     */
    void GetShortcutInfos(std::vector<ShortcutInfo> &shortcutInfos) const;
    /**
     * @brief Obtains the common event objects provided by a specified application on the device.
     * @param commonEvents List of common event objects if obtained.
     */
    void GetCommonEvents(const std::string &eventKey, std::vector<CommonEventInfo> &commonEvents) const;


    std::optional<InnerModuleInfo> GetInnerModuleInfoByModuleName(const std::string &moduleName) const;

    void GetModuleNames(std::vector<std::string> &moduleNames) const;
    /**
     * @brief Fetch all innerModuleInfos, can be modify.
     */
    std::map<std::string, InnerModuleInfo> &FetchInnerModuleInfos()
    {
        return innerModuleInfos_;
    }
    /**
     * @brief Fetch all abilityInfos, can be modify.
     */
    std::map<std::string, AbilityInfo> &FetchAbilityInfos()
    {
        return baseAbilityInfos_;
    }
    /**
     * @brief Obtains all abilityInfos.
     */
    const std::map<std::string, AbilityInfo> &GetInnerAbilityInfos() const
    {
        return baseAbilityInfos_;
    }
    /**
     * @brief Obtains all skillInfos.
     */
    const std::map<std::string, std::vector<Skill>> &GetInnerSkillInfos() const
    {
        return skillInfos_;
    }
    /**
     * @brief Obtains all extensionAbilityInfos.
     */
    const std::map<std::string, ExtensionAbilityInfo> &GetInnerExtensionInfos() const
    {
        return baseExtensionInfos_;
    }
    /**
     * @brief Obtains all extensionSkillInfos.
     */
    const std::map<std::string, std::vector<Skill>> &GetExtensionSkillInfos() const
    {
        return  extensionSkillInfos_;
    }
    /**
     * @brief Get the bundle is whether removable.
     * @return Return whether the bundle is removable.
     */
    bool IsRemovable() const
    {
        return baseApplicationInfo_.removable;
    }
    void SetIsPreInstallApp(bool isPreInstallApp)
    {
        baseBundleInfo_.isPreInstallApp = isPreInstallApp;
    }
    bool IsPreInstallApp() const
    {
        return baseBundleInfo_.isPreInstallApp;
    }
    /**
     * @brief Get whether the bundle is a system app.
     * @return Return whether the bundle is a system app.
     */
    bool IsSystemApp() const
    {
        return baseApplicationInfo_.isSystemApp;
    }
    /**
     * @brief Get all InnerBundleUserInfo.
     * @return Return about all userinfo under the app.
     */
    const std::map<std::string, InnerBundleUserInfo>& GetInnerBundleUserInfos() const
    {
        return innerBundleUserInfos_;
    }
    /**
     * @brief Set userId to remove userinfo.
     * @param userId Indicates the userId to set.
     */
    void RemoveInnerBundleUserInfo(int32_t userId);
    /**
     * @brief Set userId to add userinfo.
     * @param userId Indicates the userInfo to set.
     */
    void AddInnerBundleUserInfo(const InnerBundleUserInfo& userInfo);
    /**
     * @brief Set userId to add userinfo.
     * @param userId Indicates the userInfo to set.
     * @param userInfo Indicates the userInfo to get.
     * @return Return whether the user information is obtained successfully.
     */
    bool GetInnerBundleUserInfo(int32_t userId, InnerBundleUserInfo& userInfo) const;
    /**
     * @brief  Check whether the user exists.
     * @param userId Indicates the userInfo to set.
     * @return Return whether the user exists..
     */
    bool HasInnerBundleUserInfo(int32_t userId) const;
    /**
     * @brief  Check whether onlyCreateBundleUser.
     * @return Return onlyCreateBundleUser.
     */
    bool IsOnlyCreateBundleUser() const
    {
        return onlyCreateBundleUser_;
    }
    /**
     * @brief Set onlyCreateBundleUser.
     * @param onlyCreateBundleUser Indicates the onlyCreateBundleUser.
     */
    void SetOnlyCreateBundleUser(bool onlyCreateBundleUser)
    {
        onlyCreateBundleUser_ = onlyCreateBundleUser;
    }
    /**
     * @brief Check whether isSingleton.
     * @return Return isSingleton.
     */
    bool IsSingleton() const
    {
        return baseApplicationInfo_.singleton;
    }
    /**
     * @brief Get response userId.
     * @param userId Indicates the request userId..
     * @return Return response userId.
     */
    int32_t GetResponseUserId(int32_t requestUserId) const;

    std::vector<std::string> GetModuleNameVec()
    {
        std::vector<std::string> moduleVec;
        for (const auto &it : innerModuleInfos_) {
            moduleVec.emplace_back(it.first);
        }
        return moduleVec;
    }

    uint32_t GetAccessTokenId(const int32_t userId) const
    {
        InnerBundleUserInfo userInfo;
        if (GetInnerBundleUserInfo(userId, userInfo)) {
            return userInfo.accessTokenId;
        }
        return 0;
    }

    void SetAccessTokenId(uint32_t accessToken, const int32_t userId);

    void SetIsNewVersion(bool flag)
    {
        isNewVersion_ = flag;
    }

    bool GetIsNewVersion() const
    {
        return isNewVersion_;
    }

    void SetAllowedAcls(const std::vector<std::string> &allowedAcls)
    {
        allowedAcls_.clear();
        for (const auto &acl : allowedAcls) {
            allowedAcls_.emplace_back(acl);
        }
    }

    std::vector<std::string> GetAllowedAcls() const
    {
        return allowedAcls_;
    }
    /**
     * @brief ability is enabled.
     * @param abilityInfo Indicates the abilityInfo.
     * @param userId Indicates the user Id.
     * @return Return set ability enabled result.
     */
    bool IsAbilityEnabled(const AbilityInfo &abilityInfo, int32_t userId) const;

    bool IsAccessible() const
    {
        return baseApplicationInfo_.accessible;
    }

    std::vector<std::string> GetDependentModuleNames(const std::string &moduleName) const;

    std::vector<std::string> GetAllDependentModuleNames(const std::string &moduleName) const;

    void GetUriPrefixList(std::vector<std::string> &uriPrefixList, const std::string &excludeModule = "") const;
    void GetUriPrefixList(std::vector<std::string> &uriPrefixList, int32_t userId,
        const std::string &excludeModule = "") const;
    /**
     * @brief Whether bundle of userId should be removed.
     * @param userId Indicates the userId.
     * @return Return get bundle isRemoved result
     */
    bool IsBundleRemovable(int32_t userId) const;
    /**
     * @brief Whether module of userId is exist.
     * @param moduleName Indicates the moduleName.
     * @param userId Indicates the userId.
     * @return Return get module exist result.
     */
    bool IsUserExistModule(const std::string &moduleName, int32_t userId) const;
    /**
     * @brief whether userId's module should be removed.
     * @param moduleName Indicates the moduleName.
     * @param userId Indicates the userId.
     * @return Return get module isRemoved result.
     */
    bool IsModuleRemovable(const std::string &moduleName, int32_t userId) const;
    /**
     * @brief Add module removable info
     * @param info Indicates the innerModuleInfo of module.
     * @param stringUserId Indicates the string userId add to isRmovable map.
     * @param isEnable Indicates the value of enable module is removed.
     * @return Return add module isRemovable info result.
     */
    bool AddModuleRemovableInfo(InnerModuleInfo &info, const std::string &stringUserId, bool isEnable) const;
    /**
     * @brief Set userId's module value of isRemoved.
     * @param moduleName Indicates the moduleName.
     * @param isEnable Indicates the module isRemovable is enable.
     * @param userId Indicates the userId.
     * @return Return set module isRemoved result.
     */
    bool SetModuleRemovable(const std::string &moduleName, bool isEnable, int32_t userId);
    /**
     * @brief Delete userId isRemoved info from module.
     * @param moduleName Indicates the moduleName.
     * @param userId Indicates the userId.
     * @return
     */
    void DeleteModuleRemovable(const std::string &moduleName, int32_t userId);
    /**
     * @brief Delete removable info.
     * @param info Indicates the innerModuleInfo of module.
     * @param stringUserId Indicates the string userId of isRmovable map.
     * @return
     */
    void DeleteModuleRemovableInfo(InnerModuleInfo &info, const std::string &stringUserId);

    void SetEntryInstallationFree(bool installationFree)
    {
        baseBundleInfo_.entryInstallationFree = installationFree;
    }

    bool GetEntryInstallationFree() const
    {
        return baseBundleInfo_.entryInstallationFree;
    }

    void SetBundlePackInfo(const BundlePackInfo &bundlePackInfo)
    {
        bundlePackInfo_ = bundlePackInfo;
    }

    BundlePackInfo GetBundlePackInfo() const
    {
        return bundlePackInfo_;
    }

    void SetAppIndex(int32_t appIndex)
    {
        appIndex_ = appIndex;
    }

    int32_t GetAppIndex() const
    {
        return appIndex_;
    }

    void SetIsSandbox(bool isSandbox)
    {
        isSandboxApp_ = isSandbox;
    }

    bool GetIsSandbox() const
    {
        return isSandboxApp_;
    }

    void CleanInnerBundleUserInfos()
    {
        innerBundleUserInfos_.clear();
    }

    std::vector<SandboxAppPersistentInfo> GetSandboxPersistentInfo() const
    {
        return sandboxPersistentInfo_;
    }

    void AddSandboxPersistentInfo(const SandboxAppPersistentInfo& info)
    {
        auto it = std::find_if(sandboxPersistentInfo_.begin(), sandboxPersistentInfo_.end(), [&info](
            const auto &sandboxInfo) {
                return sandboxInfo.appIndex == info.appIndex;
            });

        if (it != sandboxPersistentInfo_.end()) {
            sandboxPersistentInfo_.erase(it);
        }
        sandboxPersistentInfo_.emplace_back(info);
    }

    void RemoveSandboxPersistentInfo(const SandboxAppPersistentInfo& info)
    {
        auto it = std::find_if(sandboxPersistentInfo_.begin(), sandboxPersistentInfo_.end(), [&info](
            const auto &sandboxInfo) {
                return sandboxInfo.appIndex == info.appIndex;
            });

        if (it == sandboxPersistentInfo_.end()) {
            return;
        }
        sandboxPersistentInfo_.erase(it);
    }

    void ClearSandboxPersistentInfo()
    {
        sandboxPersistentInfo_.clear();
    }

private:
    void GetBundleWithAbilities(
        int32_t flags, BundleInfo &bundleInfo, int32_t userId = Constants::UNSPECIFIED_USERID) const;
    void GetBundeleWithExtension(
        int32_t flags, BundleInfo &bundleInfo, int32_t userId = Constants::UNSPECIFIED_USERID) const;
    void BuildDefaultUserInfo();
    void RemoveDuplicateName(std::vector<std::string> &name) const;

    // using for get
    Constants::AppType appType_ = Constants::AppType::THIRD_PARTY_APP;
    int uid_ = Constants::INVALID_UID;
    int gid_ = Constants::INVALID_GID;
    int userId_ = Constants::DEFAULT_USERID;
    std::string baseDataDir_;
    BundleStatus bundleStatus_ = BundleStatus::ENABLED;
    ApplicationInfo baseApplicationInfo_;
    BundleInfo baseBundleInfo_;  // applicationInfo and abilityInfo empty
    std::string appFeature_;
    std::vector<std::string> allowedAcls_;
    InstallMark mark_;
    int32_t appIndex_ = Constants::INITIAL_APP_INDEX;
    bool isSandboxApp_ = false;

    // only using for install or update progress, doesn't need to save to database
    std::string currentPackage_;
    std::string newBundleName_;
    // Auxiliary property, which is used when the application
    // has been installed when the user is created.
    bool onlyCreateBundleUser_ = false;

    std::map<std::string, InnerModuleInfo> innerModuleInfos_;

    std::map<std::string, std::vector<FormInfo>> formInfos_;
    std::map<std::string, CommonEventInfo> commonEvents_;
    std::map<std::string, ShortcutInfo> shortcutInfos_;

    std::map<std::string, AbilityInfo> baseAbilityInfos_;
    std::map<std::string, std::vector<Skill>> skillInfos_;

    std::map<std::string, InnerBundleUserInfo> innerBundleUserInfos_;

    BundlePackInfo bundlePackInfo_;
    // new version fields
    bool isNewVersion_ = false;
    int32_t upgradeFlag_ = 0;
    std::map<std::string, ExtensionAbilityInfo> baseExtensionInfos_;
    std::map<std::string, std::vector<Skill>> extensionSkillInfos_;

    // SandBox App Persistent Info
    std::vector<SandboxAppPersistentInfo> sandboxPersistentInfo_;
};

void from_json(const nlohmann::json &jsonObject, InnerModuleInfo &info);
void from_json(const nlohmann::json &jsonObject, SkillUri &uri);
void from_json(const nlohmann::json &jsonObject, Skill &skill);
void from_json(const nlohmann::json &jsonObject, Distro &distro);
void from_json(const nlohmann::json &jsonObject, InstallMark &installMark);
void from_json(const nlohmann::json &jsonObject, DefinePermission &definePermission);
void from_json(const nlohmann::json &jsonObject, SandboxAppPersistentInfo &sandboxPersistentInfo);
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // FOUNDATION_APPEXECFWK_SERVICES_BUNDLEMGR_INCLUDE_INNER_BUNDLE_INFO_H
