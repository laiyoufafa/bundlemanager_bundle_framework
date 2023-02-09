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

#ifndef BUNDLE_MGR_H
#define BUNDLE_MGR_H
#include <vector>

#include "napi/native_api.h"
#include "napi/native_common.h"
#include "napi/native_node_api.h"

#include "application_info.h"
#include "bundle_death_recipient.h"
#include "bundle_mgr_interface.h"
#include "cleancache_callback.h"
#include "js_runtime_utils.h"
#ifdef BUNDLE_FRAMEWORK_GRAPHICS
#include "pixel_map.h"
#endif
#include "want.h"

namespace OHOS {
namespace AppExecFwk {
struct AsyncWorkData {
    explicit AsyncWorkData(napi_env napiEnv);
    virtual ~AsyncWorkData();
    napi_env env;
    napi_async_work asyncWork = nullptr;
    napi_deferred deferred = nullptr;
    napi_ref callback = nullptr;
};

struct QueryParameter {
    int flags;
    std::string userId;
};

struct BundleOptions {
    int32_t userId = Constants::UNSPECIFIED_USERID;
};

struct AsyncAbilityInfoCallbackInfo : public AsyncWorkData {
    explicit AsyncAbilityInfoCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    OHOS::AAFwk::Want want;
    int32_t flags = 0;
    int32_t userId = Constants::UNSPECIFIED_USERID;
    std::vector<OHOS::AppExecFwk::AbilityInfo> abilityInfos;
    bool ret = false;
    int32_t err = 0;
};

struct AsyncAbilityInfosCallbackInfo : public AsyncWorkData {
    explicit AsyncAbilityInfosCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    int32_t flags = 0;
    std::string bundleName;
    std::string abilityName;
    std::string moduleName = "";
    bool hasModuleName = false;
    OHOS::AppExecFwk::AbilityInfo abilityInfo;
    bool ret = false;
    int32_t err = 0;
    std::string message;
};

struct AsyncBundleInfoCallbackInfo : public AsyncWorkData {
    explicit AsyncBundleInfoCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    std::string param;
    int32_t flags = 0;
    OHOS::AppExecFwk::BundleInfo bundleInfo;
    bool ret = false;
    int32_t err = 0;
    std::string message;
    BundleOptions bundleOptions;
};

struct AsyncApplicationInfoCallbackInfo : public AsyncWorkData {
    explicit AsyncApplicationInfoCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    std::string bundleName;
    int32_t flags = 0;
    int32_t userId = Constants::UNSPECIFIED_USERID;
    OHOS::AppExecFwk::ApplicationInfo appInfo;
    bool ret = false;
    int32_t err = 0;
    std::string message;
};

struct AsyncPermissionDefCallbackInfo : public AsyncWorkData {
    explicit AsyncPermissionDefCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    std::string permissionName;
    OHOS::AppExecFwk::PermissionDef permissionDef;
    bool ret = false;
    int32_t err = 0;
    std::string message;
};

struct AsyncBundleInfosCallbackInfo : public AsyncWorkData {
    explicit AsyncBundleInfosCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    int32_t flags = 0;
    std::vector<OHOS::AppExecFwk::BundleInfo> bundleInfos;
    bool ret = false;
    int32_t err = 0;
    std::string message;
    int32_t userId = Constants::UNSPECIFIED_USERID;
};

struct AsyncApplicationInfosCallbackInfo : public AsyncWorkData {
    explicit AsyncApplicationInfosCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    int32_t flags = 0;
    int32_t userId = Constants::UNSPECIFIED_USERID;
    std::vector<OHOS::AppExecFwk::ApplicationInfo> appInfos;
    bool ret = false;
    int32_t err = 0;
    std::string message;
};

struct AsyncAbilityLabelCallbackInfo : public AsyncWorkData {
    explicit AsyncAbilityLabelCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    std::string bundleName;
    std::string className;
    std::string moduleName = "";
    bool hasModuleName = false;
    std::string abilityLabel;
    int32_t err = 0;
    std::string message;
};

struct InstallResult {
    std::string resultMsg;
    int32_t resultCode = 0;
};

struct AsyncInstallCallbackInfo : public AsyncWorkData {
    explicit AsyncInstallCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    std::vector<std::string> hapFiles;
    std::string bundleName;
    std::string param;
    OHOS::AppExecFwk::InstallParam installParam;
    InstallResult installResult;
    int32_t errCode = 0;
};

struct AsyncGetBundleInstallerCallbackInfo : public AsyncWorkData {
    explicit AsyncGetBundleInstallerCallbackInfo(napi_env env) : AsyncWorkData(env) {}
};

struct AsyncFormInfosCallbackInfo : public AsyncWorkData {
    explicit AsyncFormInfosCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    std::vector<OHOS::AppExecFwk::FormInfo> formInfos;
    bool ret = false;
};

struct AsyncFormInfosByModuleCallbackInfo : public AsyncWorkData {
    explicit AsyncFormInfosByModuleCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    std::string bundleName;
    std::string moduleName;
    std::vector<OHOS::AppExecFwk::FormInfo> formInfos;
    bool ret = false;
};

struct AsyncFormInfosByAppCallbackInfo : public AsyncWorkData {
    explicit AsyncFormInfosByAppCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    std::string bundleName;
    std::vector<OHOS::AppExecFwk::FormInfo> formInfos;
    bool ret = false;
};

struct AsyncLaunchWantForBundleCallbackInfo : public AsyncWorkData {
    explicit AsyncLaunchWantForBundleCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    std::string bundleName;
    OHOS::AAFwk::Want want;
    bool ret = false;
    int32_t err = 0;
};

struct AsyncGetBundleGidsCallbackInfo : public AsyncWorkData {
    explicit AsyncGetBundleGidsCallbackInfo(napi_env env) : AsyncWorkData(env) {}
    std::string bundleName;
    std::vector<int32_t> gids;
    int32_t err = 0;
    bool ret = false;
    std::string message;
};

struct AsyncGetNameByUidInfo : public AsyncWorkData {
    explicit AsyncGetNameByUidInfo(napi_env env) : AsyncWorkData(env) {}
    int32_t uid = 0;
    std::string bundleName;
    int32_t err = 0;
    bool ret = false;
};

struct AsyncHandleBundleContext : public AsyncWorkData {
    explicit AsyncHandleBundleContext(napi_env env) : AsyncWorkData(env) {}
    OHOS::sptr<CleanCacheCallback> cleanCacheCallback;
    std::string bundleName;
    std::string className;
    int32_t labelId = 0;
    int32_t iconId = 0;
    bool ret = false;
    int32_t err = 0;
};

struct EnabledInfo : public AsyncWorkData {
    explicit EnabledInfo(napi_env env) : AsyncWorkData(env) {}
    std::string bundleName;
    OHOS::AppExecFwk::AbilityInfo abilityInfo;
    bool isEnable = false;
    bool result = false;
    int32_t errCode = 0;
    std::string errMssage;
};

struct AsyncAbilityInfo : public AsyncWorkData {
    explicit AsyncAbilityInfo(napi_env env) : AsyncWorkData(env) {}
    std::string bundleName;
    std::string abilityName;
    std::string moduleName = "";
    bool hasModuleName = false;
#ifdef BUNDLE_FRAMEWORK_GRAPHICS
    std::shared_ptr<Media::PixelMap> pixelMap;
#endif
    int32_t errCode = 0;
    bool result = false;
    std::string errMssage;
};

struct Query {
    std::string bundleName_;
    std::string interfaceType_;
    int32_t flags_ = 0;
    int32_t userId_ = Constants::UNSPECIFIED_USERID;
    napi_env env_;
    Query(const std::string &bundleName, const std::string &interfaceType, int32_t flags, int32_t userId, napi_env env)
        : bundleName_(bundleName), interfaceType_(interfaceType), flags_(flags), userId_(userId), env_(env) {}

    bool operator==(const Query &query) const
    {
        return bundleName_ == query.bundleName_ && interfaceType_ == query.interfaceType_ &&
            flags_ == query.flags_ && userId_ == query.userId_ && env_ == query.env_;
    }
};

struct QueryHash  {
    size_t operator()(const Query &query) const
    {
        return std::hash<std::string>()(query.bundleName_) ^ std::hash<std::string>()(query.interfaceType_) ^
            std::hash<int32_t>()(query.flags_) ^ std::hash<int32_t>()(query.userId_);
    }
};

class BundleMgrDeathRecipient : public IRemoteObject::DeathRecipient {
    virtual void OnRemoteDied([[maybe_unused]] const wptr<IRemoteObject>& remote) override;
};

extern thread_local napi_ref g_classBundleInstaller;

napi_value WrapVoidToJS(napi_env env);
napi_value GetApplicationInfos(napi_env env, napi_callback_info info);
napi_value QueryAbilityInfos(napi_env env, napi_callback_info info);
napi_value GetBundleInfos(napi_env env, napi_callback_info info);
napi_value GetPermissionDef(napi_env env, napi_callback_info info);
napi_value GetAllFormsInfo(napi_env env, napi_callback_info info);
napi_value GetFormsInfoByApp(napi_env env, napi_callback_info info);
napi_value GetFormsInfoByModule(napi_env env, napi_callback_info info);
napi_value GetShortcutInfos(napi_env env, napi_callback_info info);
napi_value UnregisterPermissionsChanged(napi_env env, napi_callback_info info);
napi_value ClearBundleCache(napi_env env, napi_callback_info info);
napi_value SetApplicationEnabled(napi_env env, napi_callback_info info);
napi_value SetAbilityEnabled(napi_env env, napi_callback_info info);
napi_value GetBundleGids(napi_env env, napi_callback_info info);
bool UnwrapAbilityInfo(napi_env env, napi_value param, OHOS::AppExecFwk::AbilityInfo& abilityInfo);

NativeValue *CreateAbilityTypeObject(NativeEngine *engine);
NativeValue *CreateAbilitySubTypeObject(NativeEngine *engine);
NativeValue *CreateDisplayOrientationObject(NativeEngine *engine);
NativeValue *CreateLaunchModeObject(NativeEngine *engine);
NativeValue *CreateColorModeObject(NativeEngine *engine);
NativeValue *CreateGrantStatusObject(NativeEngine *engine);
NativeValue *CreateModuleRemoveFlagObject(NativeEngine *engine);
NativeValue *CreateSignatureCompareResultObject(NativeEngine *engine);
NativeValue *CreateShortcutExistenceObject(NativeEngine *engine);
NativeValue *CreateQueryShortCutFlagObject(NativeEngine *engine);
NativeValue *CreateInstallErrorCodeObject(NativeEngine *engine);
NativeValue *CreateBundleFlagObject(NativeEngine *engine);
class JsBundleMgr {
public:
    JsBundleMgr() = default;
    ~JsBundleMgr() = default;

    struct JsAbilityInfo {
        std::string bundleName;
        std::string abilityName;
        std::string moduleName = "";
        bool hasModuleName = false;
        OHOS::AppExecFwk::AbilityInfo abilityInfo;
        bool ret = false;
    };

    struct JsNameForUid {
        std::string bundleName;
        int32_t uid;
        bool ret = false;
    };

    struct JsAbilityLabel {
        std::string bundleName;
        std::string className;
        std::string moduleName = "";
        bool hasModuleName = false;
        std::string abilityLabel;
    };

    struct JsAbilityIcon {
        std::string bundleName;
        std::string abilityName;
        std::string moduleName = "";
        bool hasModuleName = false;
    };

    struct JsGetPermissionDef {
        std::string permissionName;
        OHOS::AppExecFwk::PermissionDef permissionDef;
        bool ret = false;
    };

    struct JsQueryAbilityInfo {
        std::vector<AbilityInfo> abilityInfos;
        bool ret = false;
        bool getCache = false;
    };
    static void Finalizer(NativeEngine *engine, void *data, void *hint);
    static NativeValue* GetAllApplicationInfo(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* GetApplicationInfo(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* GetBundleArchiveInfo(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* GetLaunchWantForBundle(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* IsAbilityEnabled(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* IsApplicationEnabled(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* GetBundleInfo(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* GetAbilityIcon(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* GetNameForUid(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* GetAbilityInfo(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* GetAbilityLabel(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* SetAbilityEnabled(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* SetApplicationEnabled(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* QueryAbilityInfos(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* GetAllBundleInfo(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* GetBundleInstaller(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* GetPermissionDef(NativeEngine *engine, NativeCallbackInfo *info);
    std::string errMessage_;

private:
    NativeValue* OnGetAllApplicationInfo(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnGetApplicationInfo(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnGetBundleArchiveInfo(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnGetLaunchWantForBundle(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnIsAbilityEnabled(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnIsApplicationEnabled(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnGetBundleInfo(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnGetAbilityIcon(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnGetNameForUid(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnGetAbilityInfo(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnGetAbilityLabel(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnSetAbilityEnabled(NativeEngine &engine, const NativeCallbackInfo &info);
    NativeValue* OnSetApplicationEnabled(NativeEngine &engine, const NativeCallbackInfo &info);
    NativeValue* OnQueryAbilityInfos(NativeEngine &engine, NativeCallbackInfo &info);
    static int32_t InitGetAbilityIcon(NativeEngine &engine, NativeCallbackInfo &info, NativeValue *&lastParam,
        std::string &errMessage, std::shared_ptr<JsAbilityIcon> abilityIcon);
    static int32_t InitGetAbilityLabel(NativeEngine &engine, NativeCallbackInfo &info, NativeValue *&lastParam,
        std::string &errMessage, std::shared_ptr<JsAbilityLabel> abilityLabel);
    NativeValue* OnGetAllBundleInfo(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnGetBundleInstaller(NativeEngine &engine, const NativeCallbackInfo &info);
    NativeValue* OnGetPermissionDef(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* CreateCustomizeMetaDatas(
        NativeEngine &engine, const std::map<std::string, std::vector<CustomizeData>> &metaData);
    NativeValue* CreateInnerMetaDatas(
        NativeEngine &engine, const std::map<std::string, std::vector<Metadata>> &metaData);
    NativeValue* CreateInnerMetaDatas(NativeEngine &engine, const std::vector<Metadata> &metaData);
    NativeValue* CreateCustomizeMetaData(NativeEngine &engine, const CustomizeData &customizeData);
    NativeValue* CreateInnerMetaData(NativeEngine &engine, const Metadata &metadata);
    NativeValue* CreateResource(NativeEngine &engine, const Resource &resource);
    NativeValue* CreateModuleInfos(NativeEngine &engine, const std::vector<ModuleInfo> &moduleInfos);
    NativeValue* CreateModuleInfo(NativeEngine &engine, const ModuleInfo &modInfo);
    NativeValue* CreateAppInfo(NativeEngine &engine, const ApplicationInfo &appInfo);
    static int32_t InitGetAbilityInfo(NativeEngine &engine, NativeCallbackInfo &info,
        NativeValue *&lastParam, std::string &errMessage, std::shared_ptr<JsAbilityInfo> abilityInfo);
    NativeValue* CreateAbilityInfo(NativeEngine &engine,  const AbilityInfo &abilityInfo);
    NativeValue* CreateMetaData(NativeEngine &engine, const MetaData &metaData);
    NativeValue* CreateUsedScene(NativeEngine &engine, const RequestPermissionUsedScene &usedScene);
    NativeValue* CreateAppInfos(NativeEngine &engine, const std::vector<ApplicationInfo> &appInfos);
    NativeValue* CreateBundleInfos(NativeEngine &engine, const std::vector<BundleInfo> &bundleInfos);
    NativeValue* CreateBundleInfo(NativeEngine &engine, const BundleInfo &bundleInfo);
    NativeValue* CreateAbilityInfos(NativeEngine &engine, const std::vector<AbilityInfo> &abilityInfos);
    NativeValue* CreateHapModuleInfos(NativeEngine &engine, const std::vector<HapModuleInfo> &hapModuleInfos);
    NativeValue* CreateHapModuleInfo(NativeEngine &engine, const HapModuleInfo &hapModuleInfo);
    NativeValue* CreateRequestPermissions(NativeEngine &engine,
        const std::vector<RequestPermission> &requestPermissions);
    NativeValue* CreateRequestPermission(NativeEngine &engine, const RequestPermission &requestPermission);
    NativeValue* CreateWant(NativeEngine &engine, const OHOS::AAFwk::Want &want);
    NativeValue* CreatePermissionDef(NativeEngine &engine, const PermissionDef &permissionDef);
    static NativeValue* UnwarpQueryAbilityInfoParams(NativeEngine &engine, NativeCallbackInfo &info, int32_t &userId,
        int32_t &errCode);
    static bool UnwarpUserIdThreeParams(NativeEngine &engine, NativeCallbackInfo &info, int32_t &userId);
    static bool UnwarpUserIdFourParams(NativeEngine &engine, NativeCallbackInfo &info, int32_t &userId);
    static bool UnwarpUserIdFiveParams(NativeEngine &engine, NativeCallbackInfo &info, int32_t &userId);
    static bool UnwarpBundleOptionsParams(NativeEngine &engine, NativeCallbackInfo &info,
        BundleOptions &options, bool &unwarpBundleOptionsParamsResult);
    NativeValue* JsBundleInstallInit(NativeEngine &engine);
    NativeValue* CreateBundlePackInfo(NativeEngine &engine, const int32_t &flags, const BundlePackInfo &bundlePackInfo);
    NativeValue* CreatePackages(NativeEngine &engine, const BundlePackInfo &bundlePackInfo);
    NativeValue* CreateSummary(NativeEngine &engine, const BundlePackInfo &bundlePackInfo);
    NativeValue* CreateSummaryApp(NativeEngine &engine, const BundlePackInfo &bundlePackInfo);
    NativeValue* CreateSummaryModules(NativeEngine &engine, const BundlePackInfo &bundlePackInfo);
    NativeValue* CreateSummaryModule(NativeEngine &engine, const PackageModule &moduleInfo);
    NativeValue* CreateSummaryAppVersion(NativeEngine &engine, const BundlePackInfo &bundlePackInfo);
    NativeValue* CreateModulesApiVersion(NativeEngine &engine, const OHOS::AppExecFwk::PackageModule &module);
    NativeValue* CreateDistro(NativeEngine &engine, const OHOS::AppExecFwk::PackageModule &module);
    NativeValue* CreateAbilities(NativeEngine &engine, const OHOS::AppExecFwk::PackageModule &module);
    NativeValue* CreateAbility(NativeEngine &engine, const ModuleAbilityInfo &ability);
    NativeValue* CreateFormsInfos(NativeEngine &engine, const std::vector<OHOS::AppExecFwk::AbilityFormInfo> &forms);
    NativeValue* CreateFormsInfo(NativeEngine &engine, const AbilityFormInfo &form);
    NativeValue* CreateExtensionAbilities(NativeEngine &engine, const OHOS::AppExecFwk::PackageModule &module);
    NativeValue* CreateExtensionAbility(NativeEngine &engine, const ExtensionAbilities &extensionAbility);
};

class JsBundleInstall {
public:
    JsBundleInstall() = default;
    ~JsBundleInstall() = default;
    struct BundleInstallResult {
        int32_t resCode = 0;
        std::string resMessage;
    };
    static void Finalizer(NativeEngine *engine, void *data, void *hint);
    static NativeValue* Install(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* Recover(NativeEngine *engine, NativeCallbackInfo *info);
    static NativeValue* Uninstall(NativeEngine *engine, NativeCallbackInfo *info);
private:
    NativeValue* OnInstall(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnRecover(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* OnUninstall(NativeEngine &engine, NativeCallbackInfo &info);
    NativeValue* CreateInstallStatus(NativeEngine &engine,
        const std::shared_ptr<BundleInstallResult> bundleInstallResult);
    bool GetStringsValue(NativeEngine &engine, NativeValue *object, std::vector<std::string> &strList);
    static bool GetInstallParamValue(NativeEngine &engine, NativeValue *object, InstallParam &installParam);
    static void ConvertInstallResult(std::shared_ptr<BundleInstallResult> installResult);
};

}  // namespace AppExecFwk
}  // namespace OHOS
#endif /* BUNDLE_MGR_H */
