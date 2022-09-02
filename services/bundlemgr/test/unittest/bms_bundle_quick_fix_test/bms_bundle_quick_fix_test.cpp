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

#define private public

#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "appexecfwk_errors.h"
#include "bundle_info.h"
#include "bundle_mgr_service.h"
#include "inner_app_quick_fix.h"
#include "inner_bundle_info.h"
#include "installd/installd_service.h"
#include "installd_client.h"
#include "json_constants.h"
#include "mock_quick_fix_callback.h"
#include "mock_status_receiver.h"
#include "quick_fix_data_mgr.h"
#include "quick_fix_deployer.h"
#include "quick_fix/patch_extractor.h"
#include "quick_fix/patch_profile.h"
#include "quick_fix_checker.h"

using namespace testing::ext;
using namespace std::chrono_literals;
using namespace OHOS;
using namespace OHOS::AppExecFwk;
using namespace OHOS::Security;

namespace OHOS {
namespace {
const std::string BUNDLE_NAME = "com.example.bmsaccesstoken1";
const std::string BUNDLE_NAME_DEMO = "com.example.demo.bmsaccesstoken1";
const std::string HAP_FILE_PATH1 = "/data/test/resource/bms/quick_fix/bmsAccessTokentest1.hap";
const std::string HAP_FILE_PATH2 = "/data/test/resource/bms/quick_fix/bmsAccessTokentest3.hap";
const std::string HQF_FILE_PATH1 = "/data/test/resource/bms/quick_fix/bmsAccessTokentest1.hqf";
const int32_t USERID = 100;
const int32_t WAIT_TIME = 5; // init mocked bms
const std::string QUICK_FIX_ABI = "arms";
const std::string QUICK_FIX_SO_PATH = "libs/arms";
const uint32_t QUICK_FIX_VERSION_CODE = 1;
const uint32_t BUNDLE_VERSION_CODE = 1;
const std::string QUICK_FIX_VERSION_NAME = "1.0";
const std::string BUNDLE_VERSION_NAME = "1.0";
const std::string PROVISION_TYPE_DEBUG = "debug";
const std::string PROVISION_TYPE_RELEASE = "release";

const nlohmann::json PATCH_JSON = R"(
    {
        "app" : {
            "bundleName" : "com.example.bmsaccesstoken1",
            "versionCode" : 1,
            "versionName" : "1.0",
            "patchVersionCode" : 1,
            "patchVersionName" : "1.0"
        },
        "module" : {
            "name" : "entry",
            "type" : "patch",
            "deviceTypes" : [
                "phone",
                "tablet"
            ],
            "originalModuleHash" : "11223344556677889900"
        }
    }
)"_json;
std::string BUNDLE_PATCH_PROFILE_APP_KEY_BUNDLE_NAME = "bundleName";
std::string BUNDLE_PATCH_PROFILE_APP_KEY_VERSION_CODE = "versionCode";
std::string BUNDLE_PATCH_PROFILE_APP_KEY_VERSION_NAME = "versionName";
std::string BUNDLE_PATCH_PROFILE_APP_KEY_PATCH_VERSION_CODE = "patchVersionCode";
std::string BUNDLE_PATCH_PROFILE_APP_KEY_PATCH_VERSION_NAME = "patchVersionName";
std::string BUNDLE_PATCH_PROFILE_MODULE_KEY_NAME = "name";
std::string BUNDLE_PATCH_PROFILE_MODULE_KEY_TYPE = "type";
std::string BUNDLE_PATCH_PROFILE_MODULE_KEY_DEVICE_TYPES = "deviceTypes";
std::string BUNDLE_PATCH_PROFILE_MODULE_KEY_ORIGINAL_MODULE_HASH = "originalModuleHash";
std::string BUNDLE_PATCH_PROFILE_KEY_APP = "app";
std::string BUNDLE_PATCH_PROFILE_KEY_MODULE = "module";
}  // namespace

class BmsBundleQuickFixTest : public testing::Test {
public:
    BmsBundleQuickFixTest();
    ~BmsBundleQuickFixTest();
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    ErrCode InstallBundle(const std::string &bundlePath) const;
    ErrCode UpdateBundle(const std::string &bundlePath) const;
    ErrCode UnInstallBundle(const std::string &bundleName) const;
    void AddInnerBundleInfo(const std::string bundleName, const std::string provisionType = PROVISION_TYPE_RELEASE,
        QuickFixType type = QuickFixType::PATCH);
    void UninstallBundleInfo(const std::string bundleName);
    const std::shared_ptr<BundleDataMgr> GetBundleDataMgr() const;
    const std::shared_ptr<QuickFixDeployer> GetQuickFixDeployer();
    const std::shared_ptr<QuickFixDataMgr> GetQuickFixDataMgr() const;
    AppQuickFix CreateAppQuickFix(const nlohmann::json &object);
    void StartInstalldService() const;
    void StartBundleService();
    sptr<IQuickFixManager> GetQuickFixManagerProxy();

private:
    std::shared_ptr<InstalldService> installdService_ = std::make_shared<InstalldService>();
    std::shared_ptr<BundleMgrService> bundleMgrService_ = DelayedSingleton<BundleMgrService>::GetInstance();
    std::shared_ptr<QuickFixDeployer> deployer_ = nullptr;
    std::shared_ptr<QuickFixDataMgr> quickFixDataMgr_ = DelayedSingleton<QuickFixDataMgr>::GetInstance();
};

BmsBundleQuickFixTest::BmsBundleQuickFixTest()
{}

BmsBundleQuickFixTest::~BmsBundleQuickFixTest()
{}

void BmsBundleQuickFixTest::SetUpTestCase()
{}

void BmsBundleQuickFixTest::TearDownTestCase()
{}

void BmsBundleQuickFixTest::SetUp()
{
    StartInstalldService();
    StartBundleService();
    auto dataMgr = GetBundleDataMgr();
    if (dataMgr != nullptr) {
        dataMgr->AddUserId(USERID);
    }
}

void BmsBundleQuickFixTest::TearDown()
{}

ErrCode BmsBundleQuickFixTest::InstallBundle(const std::string &bundlePath) const
{
    if (!bundleMgrService_) {
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    auto installer = bundleMgrService_->GetBundleInstaller();
    if (!installer) {
        EXPECT_FALSE(true) << "the installer is nullptr";
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    sptr<MockStatusReceiver> receiver = new (std::nothrow) MockStatusReceiver();
    if (!receiver) {
        EXPECT_FALSE(true) << "the receiver is nullptr";
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    InstallParam installParam;
    installParam.installFlag = InstallFlag::NORMAL;
    installParam.userId = USERID;
    bool result = installer->Install(bundlePath, installParam, receiver);
    EXPECT_TRUE(result);
    return receiver->GetResultCode();
}

ErrCode BmsBundleQuickFixTest::UpdateBundle(const std::string &bundlePath) const
{
    if (!bundleMgrService_) {
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    auto installer = bundleMgrService_->GetBundleInstaller();
    if (!installer) {
        EXPECT_FALSE(true) << "the installer is nullptr";
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    sptr<MockStatusReceiver> receiver = new (std::nothrow) MockStatusReceiver();
    if (!receiver) {
        EXPECT_FALSE(true) << "the receiver is nullptr";
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    InstallParam installParam;
    installParam.installFlag = InstallFlag::REPLACE_EXISTING;
    installParam.userId = USERID;
    bool result = installer->Install(bundlePath, installParam, receiver);
    EXPECT_TRUE(result);
    return receiver->GetResultCode();
}

ErrCode BmsBundleQuickFixTest::UnInstallBundle(const std::string &bundleName) const
{
    if (!bundleMgrService_) {
        return ERR_APPEXECFWK_UNINSTALL_BUNDLE_MGR_SERVICE_ERROR;
    }
    auto installer = bundleMgrService_->GetBundleInstaller();
    if (!installer) {
        EXPECT_FALSE(true) << "the installer is nullptr";
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    sptr<MockStatusReceiver> receiver = new (std::nothrow) MockStatusReceiver();
    if (!receiver) {
        EXPECT_FALSE(true) << "the receiver is nullptr";
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    InstallParam installParam;
    installParam.installFlag = InstallFlag::NORMAL;
    installParam.userId = USERID;
    bool result = installer->Uninstall(bundleName, installParam, receiver);
    EXPECT_TRUE(result);
    return receiver->GetResultCode();
}

void BmsBundleQuickFixTest::AddInnerBundleInfo(const std::string bundleName,
    const std::string provisionType,
    QuickFixType type)
{
    BundleInfo bundleInfo;
    bundleInfo.name = bundleName;
    AppqfInfo deployedAppqfInfo;
    deployedAppqfInfo.versionCode = QUICK_FIX_VERSION_CODE;
    deployedAppqfInfo.versionName =  QUICK_FIX_VERSION_NAME;
    deployedAppqfInfo.cpuAbi = QUICK_FIX_ABI;
    deployedAppqfInfo.nativeLibraryPath = QUICK_FIX_SO_PATH;
    deployedAppqfInfo.type = type;
    bundleInfo.versionCode = BUNDLE_VERSION_CODE;
    bundleInfo.versionName = BUNDLE_VERSION_NAME;

    ApplicationInfo applicationInfo;
    applicationInfo.name = bundleName;
    if (provisionType == PROVISION_TYPE_DEBUG) {
        applicationInfo.debug = true;
        applicationInfo.appProvisionType = PROVISION_TYPE_DEBUG;
    }
    applicationInfo.appQuickFix.deployedAppqfInfo = deployedAppqfInfo;
    applicationInfo.nativeLibraryPath = QUICK_FIX_SO_PATH;
    InnerBundleUserInfo userInfo;
    userInfo.bundleName = bundleName;
    userInfo.bundleUserInfo.userId = USERID;

    InnerBundleInfo innerBundleInfo;
    innerBundleInfo.SetBaseBundleInfo(bundleInfo);
    innerBundleInfo.SetBaseApplicationInfo(applicationInfo);
    innerBundleInfo.AddInnerBundleUserInfo(userInfo);

    auto dataMgr = GetBundleDataMgr();
    EXPECT_NE(dataMgr, nullptr);

    bool startRet = dataMgr->UpdateBundleInstallState(bundleName, InstallState::INSTALL_START);
    bool addRet = dataMgr->AddInnerBundleInfo(bundleName, innerBundleInfo);
    bool endRet = dataMgr->UpdateBundleInstallState(bundleName, InstallState::INSTALL_SUCCESS);

    EXPECT_TRUE(startRet);
    EXPECT_TRUE(addRet);
    EXPECT_TRUE(endRet);
}

void BmsBundleQuickFixTest::UninstallBundleInfo(const std::string bundleName)
{
    auto dataMgr = GetBundleDataMgr();
    EXPECT_NE(dataMgr, nullptr);
    bool startRet = dataMgr->UpdateBundleInstallState(bundleName, InstallState::UNINSTALL_START);
    bool finishRet = dataMgr->UpdateBundleInstallState(bundleName, InstallState::UNINSTALL_SUCCESS);

    EXPECT_TRUE(startRet);
    EXPECT_TRUE(finishRet);
}

void BmsBundleQuickFixTest::StartInstalldService() const
{
    if (!installdService_->IsServiceReady()) {
        installdService_->Start();
    }
}

void BmsBundleQuickFixTest::StartBundleService()
{
    if (!bundleMgrService_->IsServiceReady()) {
        bundleMgrService_->OnStart();
        std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME));
    }
}

const std::shared_ptr<BundleDataMgr> BmsBundleQuickFixTest::GetBundleDataMgr() const
{
    return bundleMgrService_->GetDataMgr();
}

const std::shared_ptr<QuickFixDeployer> BmsBundleQuickFixTest::GetQuickFixDeployer()
{
    if (deployer_ == nullptr) {
        std::vector<std::string> path;
        deployer_ = std::make_shared<QuickFixDeployer>(path);
    }
    return deployer_;
}

const std::shared_ptr<QuickFixDataMgr> BmsBundleQuickFixTest::GetQuickFixDataMgr() const
{
    return quickFixDataMgr_;
}

AppQuickFix BmsBundleQuickFixTest::CreateAppQuickFix(const nlohmann::json &object)
{
    std::ostringstream profileFileBuffer;
    profileFileBuffer << object.dump();
    PatchProfile patchProfile;
    PatchExtractor patchExtractor("");
    AppQuickFix appQuickFix;
    ErrCode ret = patchProfile.TransformTo(profileFileBuffer, patchExtractor, appQuickFix);
    EXPECT_EQ(ret, ERR_OK);
    return appQuickFix;
}

sptr<IQuickFixManager> BmsBundleQuickFixTest::GetQuickFixManagerProxy()
{
    auto systemAbilityManager = OHOS::SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (systemAbilityManager == nullptr) {
        APP_LOGE("GetSystemAbilityManager failed.");
        return nullptr;
    }
    auto bundleMgrSa = systemAbilityManager->GetSystemAbility(OHOS::BUNDLE_MGR_SERVICE_SYS_ABILITY_ID);
    if (bundleMgrSa == nullptr) {
        APP_LOGE("GetSystemAbility failed.");
        return nullptr;
    }
    auto bundleMgr = OHOS::iface_cast<IBundleMgr>(bundleMgrSa);
    if (bundleMgr == nullptr) {
        APP_LOGE("iface_cast failed.");
        return nullptr;
    }
    return bundleMgr->GetQuickFixManagerProxy();
}
/**
 * @tc.number: BmsBundleQuickFixTest_0001
 * Function: CheckAppQuickFixInfos
 * @tc.name: test QuickFixChecker
 * @tc.require: issueI5N7AD
 * @tc.desc: 1. check bundle name not same
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0001, Function | SmallTest | Level0)
{
    // parse patch.json
    std::unordered_map<std::string, AppQuickFix> infos;
    nlohmann::json object = PATCH_JSON;
    AppQuickFix appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_1", appQuickFix);

    object[BUNDLE_PATCH_PROFILE_KEY_APP][BUNDLE_PATCH_PROFILE_APP_KEY_BUNDLE_NAME] = "wrong_name";
    appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_2", appQuickFix);

    QuickFixChecker checker;
    ErrCode ret = checker.CheckAppQuickFixInfos(infos);
    EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_BUNDLE_NAME_NOT_SAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0002
 * Function: CheckAppQuickFixInfos
 * @tc.name: test QuickFixChecker
 * @tc.require: issueI5N7AD
 * @tc.desc: 1. check bundle version code not same
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0002, Function | SmallTest | Level0)
{
    // parse patch.json
    std::unordered_map<std::string, AppQuickFix> infos;
    nlohmann::json object = PATCH_JSON;
    AppQuickFix appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_1", appQuickFix);

    object[BUNDLE_PATCH_PROFILE_KEY_APP][BUNDLE_PATCH_PROFILE_APP_KEY_VERSION_CODE] = 20000;
    object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_NAME] = "feature";
    appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_2", appQuickFix);

    QuickFixChecker checker;
    ErrCode ret = checker.CheckAppQuickFixInfos(infos);
    EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_VERSION_CODE_NOT_SAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0003
 * Function: CheckAppQuickFixInfos
 * @tc.name: test QuickFixChecker
 * @tc.require: issueI5N7AD
 * @tc.desc: 1. check bundle version name not same
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0003, Function | SmallTest | Level0)
{
    // parse patch.json
    std::unordered_map<std::string, AppQuickFix> infos;
    nlohmann::json object = PATCH_JSON;
    AppQuickFix appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_1", appQuickFix);

    object[BUNDLE_PATCH_PROFILE_KEY_APP][BUNDLE_PATCH_PROFILE_APP_KEY_VERSION_NAME] = "2.0.0";
    object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_NAME] = "feature";
    appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_2", appQuickFix);

    QuickFixChecker checker;
    ErrCode ret = checker.CheckAppQuickFixInfos(infos);
    EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_VERSION_NAME_NOT_SAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0004
 * Function: CheckAppQuickFixInfos
 * @tc.name: test QuickFixChecker
 * @tc.require: issueI5N7AD
 * @tc.desc: 1. check patch version code not same
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0004, Function | SmallTest | Level0)
{
    // parse patch.json
    std::unordered_map<std::string, AppQuickFix> infos;
    nlohmann::json object = PATCH_JSON;
    AppQuickFix appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_1", appQuickFix);

    object[BUNDLE_PATCH_PROFILE_KEY_APP][BUNDLE_PATCH_PROFILE_APP_KEY_PATCH_VERSION_CODE] = 200000;
    object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_NAME] = "feature";
    appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_2", appQuickFix);

    QuickFixChecker checker;
    ErrCode ret = checker.CheckAppQuickFixInfos(infos);
    EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_PATCH_VERSION_CODE_NOT_SAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0005
 * Function: CheckAppQuickFixInfos
 * @tc.name: test QuickFixChecker
 * @tc.require: issueI5N7AD
 * @tc.desc: 1. check patch version code not same
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0005, Function | SmallTest | Level0)
{
    // parse patch.json
    std::unordered_map<std::string, AppQuickFix> infos;
    nlohmann::json object = PATCH_JSON;
    AppQuickFix appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_1", appQuickFix);

    object[BUNDLE_PATCH_PROFILE_KEY_APP][BUNDLE_PATCH_PROFILE_APP_KEY_PATCH_VERSION_NAME] = "2.0.0";
    object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_NAME] = "feature";
    appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_2", appQuickFix);

    QuickFixChecker checker;
    ErrCode ret = checker.CheckAppQuickFixInfos(infos);
    EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_PATCH_VERSION_NAME_NOT_SAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0006
 * Function: CheckAppQuickFixInfos
 * @tc.name: test QuickFixChecker
 * @tc.require: issueI5N7AD
 * @tc.desc: 1. check patch version code not same
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0006, Function | SmallTest | Level0)
{
    // parse patch.json
    std::unordered_map<std::string, AppQuickFix> infos;
    nlohmann::json object = PATCH_JSON;
    AppQuickFix appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_1", appQuickFix);

    object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_TYPE] = "hotreload";
    object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_NAME] = "feature";
    appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_2", appQuickFix);

    QuickFixChecker checker;
    ErrCode ret = checker.CheckAppQuickFixInfos(infos);
    EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_PATCH_TYPE_NOT_SAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0007
 * Function: CheckAppQuickFixInfos
 * @tc.name: test QuickFixChecker
 * @tc.require: issueI5N7AD
 * @tc.desc: 1. check module not same
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0007, Function | SmallTest | Level0)
{
    // parse patch.json
    std::unordered_map<std::string, AppQuickFix> infos;
    nlohmann::json object = PATCH_JSON;
    AppQuickFix appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_1", appQuickFix);

    object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_NAME] = "entry";
    appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_2", appQuickFix);

    QuickFixChecker checker;
    ErrCode ret = checker.CheckAppQuickFixInfos(infos);
    EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_MODULE_NAME_SAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0008
 * Function: CheckAppQuickFixInfos
 * @tc.name: test QuickFixChecker
 * @tc.require: issueI5N7AD
 * @tc.desc: 1. check module not same
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0008, Function | SmallTest | Level0)
{
    // parse patch.json
    std::unordered_map<std::string, AppQuickFix> infos;
    nlohmann::json object = PATCH_JSON;
    AppQuickFix appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_1", appQuickFix);

    object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_NAME] = "feature";
    appQuickFix = CreateAppQuickFix(object);
    infos.emplace("appQuickFix_2", appQuickFix);

    QuickFixChecker checker;
    ErrCode ret = checker.CheckAppQuickFixInfos(infos);
    EXPECT_EQ(ret, ERR_OK);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0009
 * Function: Query inner app quick fix
 * @tc.name: test QuickFixDataMgr
 * @tc.require: issueI5N7AD
 * @tc.desc: 1. check module not same
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0009, Function | SmallTest | Level0)
{
    // parse patch.json
    nlohmann::json object = PATCH_JSON;
    AppQuickFix appQuickFix = CreateAppQuickFix(object);
    QuickFixMark mark;
    mark.bundleName = appQuickFix.bundleName;
    mark.status = QuickFixStatus::DEPLOY_START;
    InnerAppQuickFix innerAppQuickFix(appQuickFix, mark);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    ErrCode ret = ERR_OK;
    if (deployer != nullptr) {
        ret = deployer->SaveAppQuickFix(innerAppQuickFix);
        EXPECT_EQ(ret, ERR_OK);
        auto quickFixMgr = GetQuickFixDataMgr();
        EXPECT_FALSE(quickFixMgr == nullptr);
        InnerAppQuickFix tempInnerAppQuickFix;
        bool query = quickFixMgr->QueryInnerAppQuickFix(appQuickFix.bundleName, tempInnerAppQuickFix);
        EXPECT_EQ(query, true);
        EXPECT_EQ(tempInnerAppQuickFix.GetAppQuickFix().bundleName, appQuickFix.bundleName);
        EXPECT_EQ(tempInnerAppQuickFix.GetAppQuickFix().versionCode, appQuickFix.versionCode);
        query = quickFixMgr->DeleteInnerAppQuickFix(appQuickFix.bundleName);
        EXPECT_EQ(query, true);
    }
}

/**
 * @tc.number: BmsBundleQuickFixTest_0010
 * Function: Query DeployQuickFixResult
 * @tc.name: test ToDeployQuickFixResult
 * @tc.require: issueI5N7AD
 * @tc.desc: parse and check ToDeployQuickFixResult
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0010, Function | SmallTest | Level0)
{
    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        deployer->ToDeployQuickFixResult(appQuickFix);
        DeployQuickFixResult result = deployer->GetDeployQuickFixResult();
        EXPECT_EQ(result.bundleName, appQuickFix.bundleName);
        EXPECT_EQ(result.bundleVersionCode, appQuickFix.versionCode);
        EXPECT_EQ(result.patchVersionCode, appQuickFix.deployingAppqfInfo.versionCode);
        EXPECT_EQ(result.type, appQuickFix.deployingAppqfInfo.type);
    }
}

/**
 * @tc.number: BmsBundleQuickFixTest_0011
 * Function: GetBundleInfo
 * @tc.name: test GetBundleInfo
 * @tc.require: issueI5N7AD
 * @tc.desc: GetBundleInfo
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0011, Function | SmallTest | Level0)
{
    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        BundleInfo bundleInfo;
        ErrCode ret = deployer->GetBundleInfo(appQuickFix.bundleName, bundleInfo);
        EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_BUNDLE_NAME_NOT_EXIST);
    }
}

/**
 * @tc.number: BmsBundleQuickFixTest_0012
 * Function: GetBundleInfo
 * @tc.name: test GetBundleInfo
 * @tc.require: issueI5N7AD
 * @tc.desc: GetBundleInfo
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0012, Function | SmallTest | Level0)
{
    ErrCode installResult = InstallBundle(HAP_FILE_PATH1);
    EXPECT_EQ(installResult, ERR_OK);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        BundleInfo bundleInfo;
        ErrCode ret = deployer->GetBundleInfo(appQuickFix.bundleName, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
    }

    ErrCode unInstallResult = UnInstallBundle(BUNDLE_NAME);
    EXPECT_EQ(unInstallResult, ERR_OK);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0013
 * Function: GetBundleInfo
 * @tc.name: test GetBundleInfo
 * @tc.require: issueI5N7AD
 * @tc.desc: GetBundleInfo
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0013, Function | SmallTest | Level0)
{
    ErrCode installResult = InstallBundle(HAP_FILE_PATH1);
    EXPECT_EQ(installResult, ERR_OK);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        BundleInfo bundleInfo;
        ErrCode ret = deployer->GetBundleInfo(appQuickFix.bundleName, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
    }

    ErrCode unInstallResult = UnInstallBundle(BUNDLE_NAME);
    EXPECT_EQ(unInstallResult, ERR_OK);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0014
 * Function: CheckCommonWithInstalledBundle
 * @tc.name: test CheckCommonWithInstalledBundle
 * @tc.require: issueI5MZ5L
 * @tc.desc: CheckCommonWithInstalledBundle
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0014, Function | SmallTest | Level0)
{
    ErrCode installResult = InstallBundle(HAP_FILE_PATH1);
    EXPECT_EQ(installResult, ERR_OK);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        BundleInfo bundleInfo;
        ErrCode ret = deployer->GetBundleInfo(appQuickFix.bundleName, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
        QuickFixChecker checker;
        ret = checker.CheckCommonWithInstalledBundle(appQuickFix, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
    }

    ErrCode unInstallResult = UnInstallBundle(BUNDLE_NAME);
    EXPECT_EQ(unInstallResult, ERR_OK);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0015
 * Function: CheckCommonWithInstalledBundle
 * @tc.name: test CheckCommonWithInstalledBundle
 * @tc.require: issueI5MZ5L
 * @tc.desc: CheckCommonWithInstalledBundle
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0015, Function | SmallTest | Level0)
{
    ErrCode installResult = InstallBundle(HAP_FILE_PATH1);
    EXPECT_EQ(installResult, ERR_OK);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        object[BUNDLE_PATCH_PROFILE_KEY_APP][BUNDLE_PATCH_PROFILE_APP_KEY_VERSION_CODE] = 200005;
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        BundleInfo bundleInfo;
        ErrCode ret = deployer->GetBundleInfo(appQuickFix.bundleName, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
        QuickFixChecker checker;
        ret = checker.CheckCommonWithInstalledBundle(appQuickFix, bundleInfo);
        EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_VERSION_CODE_NOT_SAME);
    }

    ErrCode unInstallResult = UnInstallBundle(BUNDLE_NAME);
    EXPECT_EQ(unInstallResult, ERR_OK);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0016
 * Function: CheckCommonWithInstalledBundle
 * @tc.name: test CheckCommonWithInstalledBundle
 * @tc.require: issueI5MZ5D
 * @tc.desc: CheckCommonWithInstalledBundle
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0016, Function | SmallTest | Level0)
{
    ErrCode installResult = InstallBundle(HAP_FILE_PATH1);
    EXPECT_EQ(installResult, ERR_OK);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        BundleInfo bundleInfo;
        ErrCode ret = deployer->GetBundleInfo(appQuickFix.bundleName, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
        bundleInfo.applicationInfo.appQuickFix.deployedAppqfInfo.versionCode = 2;
        QuickFixChecker checker;
        ret = checker.CheckCommonWithInstalledBundle(appQuickFix, bundleInfo);
        EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_VERSION_CODE_ERROR);
    }

    ErrCode unInstallResult = UnInstallBundle(BUNDLE_NAME);
    EXPECT_EQ(unInstallResult, ERR_OK);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0017
 * Function: CheckCommonWithInstalledBundle
 * @tc.name: test CheckHotReloadWithInstalledBundle
 * @tc.require: issueI5MZ5D
 * @tc.desc: CheckCommonWithInstalledBundle
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0017, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        object[BUNDLE_PATCH_PROFILE_KEY_APP][BUNDLE_PATCH_PROFILE_APP_KEY_PATCH_VERSION_CODE] = 2;
        object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_TYPE] = "hotreload";
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        BundleInfo bundleInfo;
        ErrCode ret = deployer->GetBundleInfo(appQuickFix.bundleName, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
        QuickFixChecker checker;
        ret = checker.CheckHotReloadWithInstalledBundle(appQuickFix, bundleInfo);
        EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_HOT_RELOAD_NOT_SUPPORT_RELEASE_BUNDLE);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0018
 * Function: CheckCommonWithInstalledBundle
 * @tc.name: test CheckHotReloadWithInstalledBundle
 * @tc.require: issueI5MZ7R
 * @tc.desc: CheckCommonWithInstalledBundle
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0018, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME, PROVISION_TYPE_DEBUG);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        object[BUNDLE_PATCH_PROFILE_KEY_APP][BUNDLE_PATCH_PROFILE_APP_KEY_PATCH_VERSION_CODE] = 2;
        object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_TYPE] = "hotreload";
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        BundleInfo bundleInfo;
        ErrCode ret = deployer->GetBundleInfo(appQuickFix.bundleName, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
        QuickFixChecker checker;
        ret = checker.CheckHotReloadWithInstalledBundle(appQuickFix, bundleInfo);
        EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_PATCH_ALREADY_EXISTED);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0019
 * Function: CheckCommonWithInstalledBundle
 * @tc.name: test CheckHotReloadWithInstalledBundle
 * @tc.require: issueI5MZ7R
 * @tc.desc: CheckCommonWithInstalledBundle
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0019, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME, PROVISION_TYPE_DEBUG, QuickFixType::HOT_RELOAD);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        object[BUNDLE_PATCH_PROFILE_KEY_APP][BUNDLE_PATCH_PROFILE_APP_KEY_PATCH_VERSION_CODE] = 2;
        object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_TYPE] = "hotreload";
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        BundleInfo bundleInfo;
        ErrCode ret = deployer->GetBundleInfo(appQuickFix.bundleName, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
        QuickFixChecker checker;
        ret = checker.CheckHotReloadWithInstalledBundle(appQuickFix, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0020
 * Function: CheckCommonWithInstalledBundle
 * @tc.name: test CheckPatchWithInstalledBundle
 * @tc.require: issueI5N7AD
 * @tc.desc: CheckCommonWithInstalledBundle
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0020, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME, PROVISION_TYPE_DEBUG, QuickFixType::HOT_RELOAD);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        object[BUNDLE_PATCH_PROFILE_KEY_APP][BUNDLE_PATCH_PROFILE_APP_KEY_PATCH_VERSION_CODE] = 2;
        object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_TYPE] = "patch";
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        BundleInfo bundleInfo;
        ErrCode ret = deployer->GetBundleInfo(appQuickFix.bundleName, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
        Security::Verify::ProvisionInfo provisionInfo;
        QuickFixChecker checker;
        ret = checker.CheckPatchWithInstalledBundle(appQuickFix, bundleInfo, provisionInfo);
        EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_HOT_RELOAD_ALREADY_EXISTED);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0021
 * Function: CheckCommonWithInstalledBundle
 * @tc.name: test CheckPatchWithInstalledBundle
 * @tc.require: issueI5N7AD
 * @tc.desc: CheckCommonWithInstalledBundle
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0021, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        object[BUNDLE_PATCH_PROFILE_KEY_APP][BUNDLE_PATCH_PROFILE_APP_KEY_PATCH_VERSION_CODE] = 2;
        object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_TYPE] = "patch";
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        appQuickFix.deployingAppqfInfo.nativeLibraryPath = "libs/armeabi-v7a";
        BundleInfo bundleInfo;
        ErrCode ret = deployer->GetBundleInfo(appQuickFix.bundleName, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
        Security::Verify::ProvisionInfo provisionInfo;
        QuickFixChecker checker;
        ret = checker.CheckPatchWithInstalledBundle(appQuickFix, bundleInfo, provisionInfo);
        EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_SO_INCOMPATIBLE);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0022
 * Function: CheckCommonWithInstalledBundle
 * @tc.name: test CheckPatchWithInstalledBundle
 * @tc.require: issueI5N7AD
 * @tc.desc: CheckPatchWithInstalledBundle, signature info not same
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0022, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        object[BUNDLE_PATCH_PROFILE_KEY_APP][BUNDLE_PATCH_PROFILE_APP_KEY_PATCH_VERSION_CODE] = 2;
        object[BUNDLE_PATCH_PROFILE_KEY_MODULE][BUNDLE_PATCH_PROFILE_MODULE_KEY_TYPE] = "patch";
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        appQuickFix.deployingAppqfInfo.nativeLibraryPath = QUICK_FIX_SO_PATH;
        BundleInfo bundleInfo;
        ErrCode ret = deployer->GetBundleInfo(appQuickFix.bundleName, bundleInfo);
        EXPECT_EQ(ret, ERR_OK);
        Security::Verify::ProvisionInfo provisionInfo;
        QuickFixChecker checker;
        ret = checker.CheckPatchWithInstalledBundle(appQuickFix, bundleInfo, provisionInfo);
        EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_SIGNATURE_INFO_NOT_SAME);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0023
 * Function: SaveInnerAppQuickFix
 * @tc.name: test update
 * @tc.require: issueI5N7AD
 * @tc.desc: update hap, app quick fix info will be deleted
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0023, Function | SmallTest | Level0)
{
    ErrCode installResult = InstallBundle(HAP_FILE_PATH1);
    EXPECT_EQ(installResult, ERR_OK);

    auto quickFixMgr = GetQuickFixDataMgr();
    EXPECT_FALSE(quickFixMgr == nullptr);
    if (quickFixMgr != nullptr) {
        nlohmann::json object = PATCH_JSON;
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        QuickFixMark mark;
        mark.bundleName = appQuickFix.bundleName;
        mark.status = QuickFixStatus::DEPLOY_END;
        InnerAppQuickFix innerAppQuickFix(appQuickFix, mark);
        bool ret = quickFixMgr->SaveInnerAppQuickFix(innerAppQuickFix);
        EXPECT_TRUE(ret);
        ErrCode updateResult = UpdateBundle(HAP_FILE_PATH1);
        EXPECT_EQ(updateResult, ERR_OK);
        ret = quickFixMgr->QueryInnerAppQuickFix(appQuickFix.bundleName, innerAppQuickFix);
        EXPECT_FALSE(ret);
    }

    ErrCode unInstallResult = UnInstallBundle(BUNDLE_NAME);
    EXPECT_EQ(unInstallResult, ERR_OK);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0024
 * Function: SaveInnerAppQuickFix
 * @tc.name: test add new module
 * @tc.require: issueI5N7AD
 * @tc.desc: update hap, app quick fix info will not be deleted
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0024, Function | SmallTest | Level0)
{
    ErrCode installResult = InstallBundle(HAP_FILE_PATH1);
    EXPECT_EQ(installResult, ERR_OK);

    auto quickFixMgr = GetQuickFixDataMgr();
    EXPECT_FALSE(quickFixMgr == nullptr);
    if (quickFixMgr != nullptr) {
        nlohmann::json object = PATCH_JSON;
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        QuickFixMark mark;
        mark.bundleName = appQuickFix.bundleName;
        mark.status = QuickFixStatus::DEPLOY_END;
        InnerAppQuickFix innerAppQuickFix(appQuickFix, mark);
        bool ret = quickFixMgr->SaveInnerAppQuickFix(innerAppQuickFix);
        EXPECT_TRUE(ret);
        ErrCode installResult = InstallBundle(HAP_FILE_PATH2);
        EXPECT_EQ(installResult, ERR_OK);
        ret = quickFixMgr->QueryInnerAppQuickFix(appQuickFix.bundleName, innerAppQuickFix);
        EXPECT_TRUE(ret);
    }

    ErrCode unInstallResult = UnInstallBundle(BUNDLE_NAME);
    EXPECT_EQ(unInstallResult, ERR_OK);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0025
 * Function: SaveInnerAppQuickFix
 * @tc.name: test uninstall
 * @tc.require: issueI5N7AD
 * @tc.desc: uninstall hap, app quick fix info will be deleted
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0025, Function | SmallTest | Level0)
{
    ErrCode installResult = InstallBundle(HAP_FILE_PATH1);
    EXPECT_EQ(installResult, ERR_OK);

    auto quickFixMgr = GetQuickFixDataMgr();
    EXPECT_FALSE(quickFixMgr == nullptr);
    if (quickFixMgr != nullptr) {
        nlohmann::json object = PATCH_JSON;
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        QuickFixMark mark;
        mark.bundleName = appQuickFix.bundleName;
        mark.status = QuickFixStatus::DEPLOY_END;
        InnerAppQuickFix innerAppQuickFix(appQuickFix, mark);
        bool ret = quickFixMgr->SaveInnerAppQuickFix(innerAppQuickFix);
        EXPECT_TRUE(ret);
    }
    ErrCode unInstallResult = UnInstallBundle(BUNDLE_NAME);
    EXPECT_EQ(unInstallResult, ERR_OK);
    if (quickFixMgr != nullptr) {
        InnerAppQuickFix innerAppQuickFix;
        bool ret = quickFixMgr->QueryInnerAppQuickFix(BUNDLE_NAME, innerAppQuickFix);
        EXPECT_FALSE(ret);
    }
}

/**
 * @tc.number: BmsBundleQuickFixTest_0026
 * Function: GetDeployQuickFixResult
 * @tc.name: test GetDeployQuickFixResult
 * @tc.require: issueI5N7AD
 * @tc.desc: deploy hqf, GetDeployQuickFixResult.
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0026, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto deployer = GetQuickFixDeployer();
    EXPECT_FALSE(deployer == nullptr);
    if (deployer != nullptr) {
        // parse patch.json
        nlohmann::json object = PATCH_JSON;
        AppQuickFix appQuickFix = CreateAppQuickFix(object);
        deployer->ToDeployQuickFixResult(appQuickFix);
        DeployQuickFixResult result = deployer->GetDeployQuickFixResult();
        EXPECT_EQ(result.bundleName, appQuickFix.bundleName);
        EXPECT_EQ(result.bundleVersionCode, appQuickFix.versionCode);
        EXPECT_EQ(result.patchVersionCode, appQuickFix.deployingAppqfInfo.versionCode);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0027
 * Function: DeployQuickFix
 * @tc.name: test DeployQuickFix
 * @tc.require: issueI5N7AD
 * @tc.desc: empty path, DeployQuickFix.
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0027, Function | SmallTest | Level0)
{
    auto quickFixProxy= GetQuickFixManagerProxy();
    EXPECT_NE(quickFixProxy, nullptr) << "the quickFixProxy is nullptr";
    sptr<MockQuickFixCallback> callback = new (std::nothrow) MockQuickFixCallback();
    EXPECT_NE(callback, nullptr) << "the callback is nullptr";
    std::vector<std::string> path;
    ErrCode ret = quickFixProxy->DeployQuickFix(path, callback);
    EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_PARAM_ERROR);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0028
 * Function: DeployQuickFix
 * @tc.name: test DeployQuickFix
 * @tc.require: issueI5N7AD
 * @tc.desc: not hqf file, DeployQuickFix.
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0028, Function | SmallTest | Level0)
{
    auto quickFixProxy= GetQuickFixManagerProxy();
    EXPECT_NE(quickFixProxy, nullptr) << "the quickFixProxy is nullptr";
    sptr<MockQuickFixCallback> callback = new (std::nothrow) MockQuickFixCallback();
    EXPECT_NE(callback, nullptr) << "the callback is nullptr";
    std::vector<std::string> path {HAP_FILE_PATH1};
    ErrCode ret = quickFixProxy->DeployQuickFix(path, callback);
    EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_PARAM_ERROR);
}

/**
 * @tc.number: BmsBundleQuickFixTest_0029
 * Function: DeployQuickFix
 * @tc.name: test DeployQuickFix
 * @tc.require: issueI5N7AD
 * @tc.desc: not exist hqf file, DeployQuickFix.
 */
HWTEST_F(BmsBundleQuickFixTest, BmsBundleQuickFixTest_0029, Function | SmallTest | Level0)
{
    auto quickFixProxy= GetQuickFixManagerProxy();
    EXPECT_NE(quickFixProxy, nullptr) << "the quickFixProxy is nullptr";
    sptr<MockQuickFixCallback> callback = new (std::nothrow) MockQuickFixCallback();
    EXPECT_NE(callback, nullptr) << "the callback is nullptr";
    std::vector<std::string> path {HQF_FILE_PATH1};
    ErrCode ret = quickFixProxy->DeployQuickFix(path, callback);
    EXPECT_EQ(ret, ERR_BUNDLEMANAGER_QUICK_FIX_REAL_PATH_FAILED);
}
} // OHOS