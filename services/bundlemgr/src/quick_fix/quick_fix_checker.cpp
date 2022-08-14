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

#include "quick_fix_checker.h"

#include "bundle_install_checker.h"
#include "bundle_util.h"
#include "patch_parser.h"

namespace OHOS {
namespace AppExecFwk {
size_t QuickFixChecker::QUICK_FIX_MAP_SIZE = 1;

ErrCode QuickFixChecker::CheckMultipleHapsSignInfo(
    const std::vector<std::string> &bundlePaths,
    std::vector<Security::Verify::HapVerifyResult> &hapVerifyRes)
{
    APP_LOGD("Check multiple haps signInfo");
    BundleInstallChecker checker;
    return checker.CheckMultipleHapsSignInfo(bundlePaths, hapVerifyRes);
}

ErrCode QuickFixChecker::ParseAppQuickFix(const std::string &patchPath, AppQuickFix &appQuickFix)
{
    if (patchPath.empty()) {
        return ERR_APPEXECFWK_QUICK_FIX_PARAM_ERROR;
    }
    PatchParser parse;
    return parse.ParsePatchInfo(patchPath, appQuickFix);
}

ErrCode QuickFixChecker::ParseAppQuickFixFiles(
    const std::vector<std::string> &filePaths,
    std::unordered_map<std::string, AppQuickFix> &appQuickFixs)
{
    APP_LOGD("Parse quick fix files start.");
    ErrCode result = ERR_OK;
    for (size_t index = 0; index < filePaths.size(); ++index) {
        AppQuickFix appQuickFix;
        result = ParseAppQuickFix(filePaths[index], appQuickFix);
        if (result != ERR_OK) {
            APP_LOGE("quick fix parse failed %{public}d", result);
            return result;
        }
        appQuickFixs.emplace(filePaths[index], appQuickFix);
    }
    APP_LOGD("Parse quick fix files end.");
    return result;
}

ErrCode QuickFixChecker::CheckAppQuickFixInfos(const std::unordered_map<std::string, AppQuickFix> &infos)
{
    APP_LOGD("Check quick fix files start.");
    if (infos.size() <= QUICK_FIX_MAP_SIZE) {
        return ERR_OK;
    }
    const AppQuickFix &appQuickFix = infos.begin()->second;
    for (const auto &info : infos) {
        if (appQuickFix.bundleName != info.second.bundleName) {
            return ERR_APPEXECFWK_QUICK_FIX_BUNDLE_NAME_NOT_SAME;
        }
        if (appQuickFix.versionCode != info.second.versionCode) {
            return ERR_APPEXECFWK_QUICK_FIX_VERSION_CODE_NOT_SAME;
        }
        if (appQuickFix.versionName != info.second.versionName) {
            return ERR_APPEXECFWK_QUICK_FIX_VERSION_NAME_NOT_SAME;
        }
        if (appQuickFix.deployingAppqfInfo.versionCode != info.second.deployingAppqfInfo.versionCode) {
            return ERR_APPEXECFWK_QUICK_FIX_PATCH_VERSION_CODE_NOT_SAME;
        }
        if (appQuickFix.deployingAppqfInfo.versionName != info.second.deployingAppqfInfo.versionName) {
            return ERR_APPEXECFWK_QUICK_FIX_PATCH_VERSION_NAME_NOT_SAME;
        }
        if (appQuickFix.deployingAppqfInfo.type != info.second.deployingAppqfInfo.type) {
            return ERR_APPEXECFWK_QUICK_FIX_PATCH_TYPE_NOT_SAME;
        }
    }
    APP_LOGD("Check quick fix files end.");
    return ERR_OK;
}

ErrCode QuickFixChecker::CheckWithInstalledBundle(const AppQuickFix &appQuickFix, const BundleInfo &bundleInfo)
{
    // check bundleName
    if (appQuickFix.bundleName != bundleInfo.name) {
        return ERR_APPEXECFWK_QUICK_FIX_BUNDLE_NAME_NOT_EXIST;
    }
    // check versionCode and versionName
    if (bundleInfo.versionCode != appQuickFix.versionCode) {
        return ERR_APPEXECFWK_QUICK_FIX_VERSION_CODE_NOT_SAME;
    }
    bool isDebug = bundleInfo.applicationInfo.debug &&
        (bundleInfo.applicationInfo.appProvisionType == Constants::APP_PROVISION_TYPE_DEBUG);
    APP_LOGD("application isDebug: %{public}d", isDebug);
    const auto &qfInfo = appQuickFix.deployingAppqfInfo;
    // hot reload does not require versionName and so files
    if (qfInfo.type == QuickFixType::HOT_RELOAD) {
        if (!isDebug) {
            return ERR_APPEXECFWK_QUICK_FIX_HOT_RELOAD_NOT_SUPPORT_RELEASE_BUNDLE;
        }
        if (bundleInfo.appqfInfo.type == QuickFixType::PATCH) {
            return ERR_APPEXECFWK_QUICK_FIX_PATCH_ALREADY_EXISTED;
        }
        if (qfInfo.versionCode <= bundleInfo.appqfInfo.versionCode) {
            APP_LOGE("qhf version code should be greater than the original");
            return ERR_APPEXECFWK_QUICK_FIX_VERSION_CODE_ERROR;
        }
        return ERR_OK;
    }
    if (isDebug && (bundleInfo.appqfInfo.type == QuickFixType::HOT_RELOAD)) {
        return ERR_APPEXECFWK_QUICK_FIX_HOT_RELOAD_ALREADY_EXISTED;
    }
    if (qfInfo.versionCode <= bundleInfo.appqfInfo.versionCode) {
        APP_LOGE("qhf version code should be greater than the original");
        return ERR_APPEXECFWK_QUICK_FIX_VERSION_CODE_ERROR;
    }
    if (bundleInfo.versionName != appQuickFix.versionName) {
        return ERR_APPEXECFWK_QUICK_FIX_VERSION_NAME_NOT_SAME;
    }
    if ((!qfInfo.cpuAbi.empty() && !bundleInfo.applicationInfo.cpuAbi.empty()) &&
        (qfInfo.cpuAbi != bundleInfo.applicationInfo.cpuAbi)) {
        APP_LOGE("qfInfo.cpuAbi: %{public}s, applicationInfo.cpuAbi: %{public}s", qfInfo.cpuAbi.c_str(),
            bundleInfo.applicationInfo.cpuAbi.c_str());
        return ERR_APPEXECFWK_QUICK_FIX_SO_INCOMPATIBLE;
    }
    if ((!qfInfo.nativeLibraryPath.empty() && !bundleInfo.applicationInfo.nativeLibraryPath.empty()) &&
        (qfInfo.nativeLibraryPath != bundleInfo.applicationInfo.nativeLibraryPath)) {
        APP_LOGE("qfInfo.nativeLibraryPath: %{public}s, applicationInfo.nativeLibraryPath: %{public}s",
            qfInfo.nativeLibraryPath.c_str(), bundleInfo.applicationInfo.nativeLibraryPath.c_str());
        return ERR_APPEXECFWK_QUICK_FIX_SO_INCOMPATIBLE;
    }
    return ERR_OK;
}

ErrCode QuickFixChecker::CheckModuleNameExist(const BundleInfo &bundleInfo,
    const std::unordered_map<std::string, AppQuickFix> &infos)
{
    for (const auto &info : infos) {
        if (info.second.deployingAppqfInfo.hqfInfos.empty()) {
            return ERR_APPEXECFWK_QUICK_FIX_MODULE_NAME_NOT_EXIST;
        }
        auto iter = std::find(bundleInfo.moduleNames.begin(), bundleInfo.moduleNames.end(),
            info.second.deployingAppqfInfo.hqfInfos[0].moduleName);
        if (iter == bundleInfo.moduleNames.end()) {
            APP_LOGE("error: moduleName %{public}s does not exist",
                info.second.deployingAppqfInfo.hqfInfos[0].moduleName.c_str());
            return ERR_APPEXECFWK_QUICK_FIX_MODULE_NAME_NOT_EXIST;
        }
    }
    return ERR_OK;
}

ErrCode QuickFixChecker::CheckSignatureInfo(const BundleInfo &bundleInfo,
    const Security::Verify::ProvisionInfo &provisionInfo)
{
    std::string quickFixAppId = bundleInfo.name + Constants::FILE_UNDERLINE + provisionInfo.appId;
    if ((bundleInfo.appId != quickFixAppId) ||
        (bundleInfo.applicationInfo.appPrivilegeLevel != provisionInfo.bundleInfo.apl)) {
            APP_LOGE("Quick fix signature info is different with installed bundle : %{public}s",
                bundleInfo.name.c_str());
            return ERR_APPEXECFWK_QUICK_FIX_SIGNATURE_INFO_NOT_SAME;
        }
    return ERR_OK;
}

ErrCode QuickFixChecker::CheckMultiNativeSo(
    std::unordered_map<std::string, AppQuickFix> &infos)
{
    if (infos.size() <= QUICK_FIX_MAP_SIZE) {
        return ERR_OK;
    }
    const AppqfInfo &appqfInfo = (infos.begin()->second).deployingAppqfInfo;
    std::string nativeLibraryPath = appqfInfo.nativeLibraryPath;
    std::string cpuAbi = appqfInfo.cpuAbi;
    for (const auto &info : infos) {
        const AppqfInfo &qfInfo = info.second.deployingAppqfInfo;
        if (qfInfo.nativeLibraryPath.empty()) {
            continue;
        }
        if (nativeLibraryPath.empty()) {
            nativeLibraryPath = qfInfo.nativeLibraryPath;
            cpuAbi = qfInfo.cpuAbi;
            continue;
        }
        if (!qfInfo.nativeLibraryPath.empty()) {
            if ((nativeLibraryPath != qfInfo.nativeLibraryPath) || (cpuAbi != qfInfo.cpuAbi)) {
                return ERR_APPEXECFWK_QUICK_FIX_SO_INCOMPATIBLE;
            }
        }
    }

    // Ensure the so is consistent in multiple haps
    if (!nativeLibraryPath.empty()) {
        for (auto &info : infos) {
            info.second.deployingAppqfInfo.nativeLibraryPath = nativeLibraryPath;
            info.second.deployingAppqfInfo.cpuAbi = cpuAbi;
        }
    }

    return ERR_OK;
}

std::string QuickFixChecker::GetAppDistributionType(const Security::Verify::AppDistType &type)
{
    std::unordered_map<Security::Verify::AppDistType, std::string> map = {
        { Security::Verify::AppDistType::NONE_TYPE, Constants::APP_DISTRIBUTION_TYPE_NONE },
        { Security::Verify::AppDistType::APP_GALLERY, Constants::APP_DISTRIBUTION_TYPE_APP_GALLERY },
        { Security::Verify::AppDistType::ENTERPRISE, Constants::APP_DISTRIBUTION_TYPE_ENTERPRISE },
        { Security::Verify::AppDistType::OS_INTEGRATION, Constants::APP_DISTRIBUTION_TYPE_OS_INTEGRATION },
        { Security::Verify::AppDistType::CROWDTESTING, Constants::APP_DISTRIBUTION_TYPE_CROWDTESTING },
    };
    auto typeIter = map.find(type);
    if (typeIter == map.end()) {
        APP_LOGE("wrong AppDistType");
        return Constants::APP_DISTRIBUTION_TYPE_NONE;
    }

    return typeIter->second;
}

std::string QuickFixChecker::GetAppProvisionType(const Security::Verify::ProvisionType &type)
{
    if (type == Security::Verify::ProvisionType::DEBUG) {
        return Constants::APP_PROVISION_TYPE_DEBUG;
    }

    return Constants::APP_PROVISION_TYPE_RELEASE;
}
} // AppExecFwk
} // OHOS
