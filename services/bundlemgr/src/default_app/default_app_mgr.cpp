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

#include "default_app_mgr.h"

#include "bundle_data_mgr.h"
#include "bundle_mgr_service.h"
#include "bundle_permission_mgr.h"
#include "ipc_skeleton.h"
#include "string_ex.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
    constexpr int32_t INITIAL_USER_ID = -1;
    const std::string PERMISSION_GET_DEFAULT_APPLICATION = "ohos.permission.GET_DEFAULT_APPLICATION";
    const std::string PERMISSION_SET_DEFAULT_APPLICATION = "ohos.permission.SET_DEFAULT_APPLICATION";
    const std::string BROWSER = "BROWSER";
    const std::string IMAGE = "IMAGE";
    const std::string AUDIO = "AUDIO";
    const std::string VIDEO = "VIDEO";
}

DefaultAppMgr& DefaultAppMgr::GetInstance()
{
    static DefaultAppMgr defaultAppMgr;
    return defaultAppMgr;
}

DefaultAppMgr::DefaultAppMgr()
{
    APP_LOGD("create DefaultAppMgr.");
    InitSupportAppTypes();
}

DefaultAppMgr::~DefaultAppMgr()
{
    APP_LOGD("destroy DefaultAppMgr.");
}

bool DefaultAppMgr::IsDefaultApplication(int32_t userId, const std::string& type) const
{
    bool ret = VerifyUserIdAndType(userId, type);
    if (!ret) {
        APP_LOGE("VerifyUserIdAndType failed.");
        return false;
    }
    std::shared_ptr<BundleDataMgr> dataMgr = DelayedSingleton<BundleMgrService>::GetInstance()->GetDataMgr();
    if (dataMgr == nullptr) {
        APP_LOGE("get BundleDataMgr failed.");
        return false;
    }
    // get bundle name via calling uid
    APP_LOGD("callingUid : %{public}d", IPCSkeleton::GetCallingUid());
    std::string callingBundleName;
    ret = dataMgr->GetBundleNameForUid(IPCSkeleton::GetCallingUid(), callingBundleName);
    if (!ret) {
        APP_LOGE("GetBundleNameForUid failed.");
        return false;
    }
    APP_LOGD("callingBundleName : %{public}s", callingBundleName.c_str());
    Element element;
    ret = defaultAppDb_->GetDefaultApplicationInfo(userId, type, element);
    if (!ret) {
        APP_LOGE("GetDefaultApplicationInfo failed.");
        return false;
    }
    ret = IsElementValid(userId, type, element);
    if (!ret) {
        APP_LOGE("Element is invalid.");
        return false;
    }
    return element.bundleName == callingBundleName;
}

bool DefaultAppMgr::GetDefaultApplication(int32_t userId, const std::string& type, BundleInfo& bundleInfo) const
{
    bool ret = VerifyUserIdAndType(userId, type);
    if (!ret) {
        APP_LOGE("VerifyUserIdAndType failed.");
        return false;
    }
    if (!BundlePermissionMgr::VerifyCallingPermission(PERMISSION_GET_DEFAULT_APPLICATION)) {
        APP_LOGE("verify permission ohos.permission.GET_DEFAULT_APPLICATION failed.");
        return false;
    }

    if (IsAppType(type)) {
        return GetAppTypeInfo(userId, type, bundleInfo);
    } else if (IsFileType(type)) {
        return GetFileTypeInfo(userId, type, bundleInfo);
    } else {
        APP_LOGE("invalid type, not app type or file type.");
        return false;
    }
}

bool DefaultAppMgr::SetDefaultApplication(int32_t userId, const std::string& type, const Element& element) const
{
    bool ret = VerifyUserIdAndType(userId, type);
    if (!ret) {
        APP_LOGE("VerifyUserIdAndType failed.");
        return false;
    }
    if (!BundlePermissionMgr::VerifyCallingPermission(PERMISSION_SET_DEFAULT_APPLICATION)) {
        APP_LOGE("verify permission ohos.permission.SET_DEFAULT_APPLICATION failed.");
        return false;
    }
    // clear default app
    ret = IsElementEmpty(element);
    if (ret) {
        APP_LOGD("clear default app.");
        ret = defaultAppDb_->DeleteDefaultApplicationInfo(userId, type);
        if (!ret) {
            APP_LOGE("DeleteDefaultApplicationInfo failed.");
            return false;
        }
        APP_LOGD("SetDefaultApplication success.");
        return true;
    }
    ret = IsElementValid(userId, type, element);
    if (!ret) {
        APP_LOGE("Element is invalid.");
        return false;
    }
    ret = defaultAppDb_->SetDefaultApplicationInfo(userId, type, element);
    if (!ret) {
        APP_LOGE("SetDefaultApplicationInfo failed.");
        return false;
    }
    APP_LOGD("SetDefaultApplication success.");
    return true;
}

bool DefaultAppMgr::ResetDefaultApplication(int32_t userId, const std::string& type) const
{
    bool ret = VerifyUserIdAndType(userId, type);
    if (!ret) {
        APP_LOGE("VerifyUserIdAndType failed.");
        return false;
    }
    if (!BundlePermissionMgr::VerifyCallingPermission(PERMISSION_SET_DEFAULT_APPLICATION)) {
        APP_LOGE("verify permission ohos.permission.SET_DEFAULT_APPLICATION failed.");
        return false;
    }
    Element element;
    ret = defaultAppDb_->GetDefaultApplicationInfo(INITIAL_USER_ID, type, element);
    if (!ret) {
        APP_LOGD("directly delete default info.");
        return defaultAppDb_->DeleteDefaultApplicationInfo(userId, type);
    }
    ret = IsElementValid(userId, type, element);
    if (!ret) {
        APP_LOGE("Element is invalid.");
        return false;
    }
    ret = defaultAppDb_->SetDefaultApplicationInfo(userId, type, element);
    if (!ret) {
        APP_LOGE("SetDefaultApplicationInfo failed.");
        return false;
    }
    APP_LOGD("ResetDefaultApplication success.");
    return true;
}

bool DefaultAppMgr::GetAppTypeInfo(int32_t userId, const std::string& type, BundleInfo& bundleInfo) const
{
    Element element;
    bool ret = defaultAppDb_->GetDefaultApplicationInfo(userId, type, element);
    if (!ret) {
        APP_LOGE("GetAppTypeInfo failed.");
        return false;
    }
    ret = GetBundleInfo(userId, type, element, bundleInfo);
    if (!ret) {
        APP_LOGE("GetBundleInfo failed.");
        return false;
    }
    APP_LOGD("GetAppTypeInfo success.");
    return true;
}

bool DefaultAppMgr::GetFileTypeInfo(int32_t userId, const std::string& type, BundleInfo& bundleInfo) const
{
    std::map<std::string, Element> infos;
    bool ret = defaultAppDb_->GetDefaultApplicationInfos(userId, infos);
    if (!ret) {
        APP_LOGE("GetDefaultApplicationInfos failed.");
        return false;
    }
    std::map<std::string, Element> defaultAppTypeInfos;
    std::map<std::string, Element> defaultFileTypeInfos;
    for (const auto& item : infos) {
        if (IsAppType(item.first)) {
            defaultAppTypeInfos.emplace(item.first, item.second);
        }
        if (IsFileType(item.first)) {
            defaultFileTypeInfos.emplace(item.first, item.second);
        }
    }
    // match default app type
    for (const auto& item : defaultAppTypeInfos) {
        if (GetBundleInfo(userId, type, item.second, bundleInfo)) {
            APP_LOGD("match default app type success.");
            return true;
        }
    }
    // match default file type
    for (const auto& item : defaultFileTypeInfos) {
        if (item.first == type && GetBundleInfo(userId, type, item.second, bundleInfo)) {
            APP_LOGD("match default file type success.");
            return true;
        }
    }
    APP_LOGE("GetFileTypeInfo failed.");
    return false;
}

bool DefaultAppMgr::IsElementValid(int32_t userId, const std::string& type, const Element& element) const
{
    bool ret = VerifyElementFormat(element);
    if (!ret) {
        APP_LOGE("VerifyElementFormat failed.");
        return false;
    }
    std::shared_ptr<BundleDataMgr> dataMgr = DelayedSingleton<BundleMgrService>::GetInstance()->GetDataMgr();
    if (dataMgr == nullptr) {
        APP_LOGE("get BundleDataMgr failed.");
        return false;
    }
    AbilityInfo abilityInfo;
    ExtensionAbilityInfo extensionInfo;
    std::vector<Skill> skills;
    // verify if element exists
    ret = dataMgr->QueryInfoAndSkillsByElement(userId, element, abilityInfo, extensionInfo, skills);
    if (!ret) {
        APP_LOGE("QueryInfoAndSkillsByElement failed.");
        return false;
    }
    // match type and skills
    ret = IsMatch(type, skills);
    if (!ret) {
        APP_LOGE("type and skills not match.");
        return false;
    }
    APP_LOGD("Element is valid.");
    return true;
}

bool DefaultAppMgr::GetBundleInfo(int32_t userId, const std::string& type, const Element& element,
    BundleInfo& bundleInfo) const
{
    APP_LOGD("begin to GetBundleInfo.");
    bool ret = VerifyElementFormat(element);
    if (!ret) {
        APP_LOGE("VerifyElementFormat failed.");
        return false;
    }
    std::shared_ptr<BundleDataMgr> dataMgr = DelayedSingleton<BundleMgrService>::GetInstance()->GetDataMgr();
    if (dataMgr == nullptr) {
        APP_LOGE("get BundleDataMgr failed.");
        return false;
    }
    AbilityInfo abilityInfo;
    ExtensionAbilityInfo extensionInfo;
    std::vector<Skill> skills;
    // verify if element exists
    ret = dataMgr->QueryInfoAndSkillsByElement(userId, element, abilityInfo, extensionInfo, skills);
    if (!ret) {
        APP_LOGE("QueryInfoAndSkillsByElement failed.");
        return false;
    }
    // match type and skills
    ret = IsMatch(type, skills);
    if (!ret) {
        APP_LOGE("type and skills not match.");
        return false;
    }
    ret = dataMgr->GetBundleInfo(element.bundleName, GET_BUNDLE_DEFAULT, bundleInfo, userId);
    if (!ret) {
        APP_LOGE("GetBundleInfo failed.");
        return false;
    }
    bool isAbility = !element.abilityName.empty();
    if (isAbility) {
        bundleInfo.abilityInfos.emplace_back(abilityInfo);
    } else {
        bundleInfo.extensionInfos.emplace_back(extensionInfo);
    }
    APP_LOGD("GetBundleInfo success.");
    return true;
}

bool DefaultAppMgr::IsMatch(const std::string& type, const std::vector<Skill>& skills) const
{
    if (IsAppType(type)) {
        return MatchAppType(type, skills);
    } else if (IsFileType(type)) {
        return MatchFileType(type, skills);
    } else {
        APP_LOGE("invalid type.");
        return false;
    }
}

bool DefaultAppMgr::MatchAppType(const std::string& type, const std::vector<Skill>& skills) const
{
    APP_LOGE("begin to match app type, type : %{public}s.", type.c_str());
    if (type == BROWSER) {
        return IsBrowserSkillsValid(skills);
    } else if (type == IMAGE) {
        return IsImageSkillsValid(skills);
    } else if (type == AUDIO) {
        return IsAudioSkillsValid(skills);
    } else if (type == VIDEO) {
        return IsVideoSkillsValid(skills);
    } else {
        return false;
    }
}

bool DefaultAppMgr::IsBrowserSkillsValid(const std::vector<Skill>& skills) const
{
    for (const Skill& skill : skills) {
        auto item = std::find(skill.actions.cbegin(), skill.actions.cend(), "ohos.want.action.viewData");
        if (item == skill.actions.cend()) {
            continue;
        }
        item = std::find(skill.entities.cbegin(), skill.entities.cend(), "entity.system.browsable");
        if (item == skill.entities.cend()) {
            continue;
        }
        for (const SkillUri& skillUri : skill.uris) {
            if (skillUri.scheme == "http" || skillUri.scheme == "https") {
                APP_LOGD("browser skills is valid.");
                return true;
            }
        }
    }
    APP_LOGE("browser skills is invalid.");
    return false;
}

bool DefaultAppMgr::IsImageSkillsValid(const std::vector<Skill>& skills) const
{
    for (const Skill& skill : skills) {
        auto item = std::find(skill.actions.cbegin(), skill.actions.cend(), "ohos.want.action.viewData");
        if (item == skill.actions.cend()) {
            continue;
        }
        for (const SkillUri& skillUri : skill.uris) {
            if (skill.MatchType("image/*", skillUri.type)) {
                APP_LOGD("image skills is valid.");
                return true;
            }
        }
    }
    APP_LOGE("image skills is invalid.");
    return false;
}

bool DefaultAppMgr::IsAudioSkillsValid(const std::vector<Skill>& skills) const
{
    for (const Skill& skill : skills) {
        auto item = std::find(skill.actions.cbegin(), skill.actions.cend(), "ohos.want.action.viewData");
        if (item == skill.actions.cend()) {
            continue;
        }
        for (const SkillUri& skillUri : skill.uris) {
            if (skill.MatchType("audio/*", skillUri.type)) {
                APP_LOGD("audio skills is valid.");
                return true;
            }
        }
    }
    APP_LOGE("audio skills is invalid.");
    return false;
}

bool DefaultAppMgr::IsVideoSkillsValid(const std::vector<Skill>& skills) const
{
    for (const Skill& skill : skills) {
        auto item = std::find(skill.actions.cbegin(), skill.actions.cend(), "ohos.want.action.viewData");
        if (item == skill.actions.cend()) {
            continue;
        }
        for (const SkillUri& skillUri : skill.uris) {
            if (skill.MatchType("video/*", skillUri.type)) {
                APP_LOGD("video skills is valid.");
                return true;
            }
        }
    }
    APP_LOGE("video skills is invalid.");
    return false;
}

bool DefaultAppMgr::MatchFileType(const std::string& type, const std::vector<Skill>& skills) const
{
    APP_LOGE("begin to match file type, type : %{public}s.", type.c_str());
    for (const Skill& skill : skills) {
        for (const SkillUri& skillUri : skill.uris) {
            if (skill.MatchType(type, skillUri.type)) {
                APP_LOGE("match file type success.");
                return true;
            }
        }
    }
    APP_LOGE("match file type failed.");
    return false;
}

void DefaultAppMgr::InitSupportAppTypes()
{
    supportAppTypes.insert(BROWSER);
    supportAppTypes.insert(IMAGE);
    supportAppTypes.insert(AUDIO);
    supportAppTypes.insert(VIDEO);
}

bool DefaultAppMgr::IsTypeValid(const std::string& type) const
{
    return IsAppType(type) || IsFileType(type);
}

bool DefaultAppMgr::IsAppType(const std::string& type) const
{
    if (type.empty()) {
        return false;
    }
    return supportAppTypes.find(type) != supportAppTypes.end();
}

bool DefaultAppMgr::IsFileType(const std::string& type) const
{
    // valid fileType format : type/subType
    if (type.empty() || type.find("*") != type.npos) {
        return false;
    }
    std::vector<std::string> vector;
    SplitStr(type, "/", vector, false, false);
    if (vector.size() == 2 && !vector[0].empty() && !vector[1].empty()) {
        return true;
    }
    return false;
}

bool DefaultAppMgr::IsUserIdExist(int32_t userId) const
{
    std::shared_ptr<BundleDataMgr> dataMgr = DelayedSingleton<BundleMgrService>::GetInstance()->GetDataMgr();
    if (dataMgr == nullptr) {
        APP_LOGE("get BundleDataMgr failed.");
        return false;
    }
    return dataMgr->HasUserId(userId);
}

bool DefaultAppMgr::VerifyUserIdAndType(int32_t userId, const std::string& type) const
{
    bool ret = IsUserIdExist(userId);
    if (!ret) {
        APP_LOGE("userId doesn't exist.");
        return false;
    }
    ret = IsTypeValid(type);
    if (!ret) {
        APP_LOGE("invalid type, not app type or file type.");
        return false;
    }
    return true;
}

bool DefaultAppMgr::IsElementEmpty(const Element& element) const
{
    return element.bundleName.empty() && element.moduleName.empty()
        && element.abilityName.empty() && element.extensionName.empty();
}

bool DefaultAppMgr::VerifyElementFormat(const Element& element) const
{
    const std::string& bundleName = element.bundleName;
    const std::string& moduleName = element.moduleName;
    const std::string& abilityName = element.abilityName;
    const std::string& extensionName = element.extensionName;
    if (bundleName.empty()) {
        APP_LOGE("bundleName empty, bad Element format.");
        return false;
    }
    if (moduleName.empty()) {
        APP_LOGE("moduleName empty, bad Element format.");
        return false;
    }
    if (abilityName.empty() && extensionName.empty()) {
        APP_LOGE("abilityName and extensionName both empty, bad Element format.");
        return false;
    }
    if (!abilityName.empty() && !extensionName.empty()) {
        APP_LOGE("abilityName and extensionName both non-empty, bad Element format.");
        return false;
    }
    return true;
}
}
}