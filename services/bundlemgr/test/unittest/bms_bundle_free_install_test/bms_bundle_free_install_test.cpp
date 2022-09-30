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

#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "appexecfwk_errors.h"
#include "bundle_info.h"
#include "bundle_mgr_service.h"
#include "bundle_mgr_proxy.h"
#include "bundle_pack_info.h"
#include "inner_bundle_info.h"
#include "installd/installd_service.h"
#include "installd_client.h"

using namespace testing::ext;
using namespace std::chrono_literals;
using namespace OHOS::AppExecFwk;

namespace OHOS {
namespace {
const std::string BUNDLE_NAME = "com.example.freeInstall";
const std::string BUNDLE_NAME_DEMO = "com.example.demo.freeInstall";
const std::string MODULE_NAME_TEST = "entry";
const std::string MODULE_NAME_NOT_EXIST = "notExist";
const std::string ABILITY_NAME_TEST = "MainAbility";
const int32_t USERID = 100;
const int32_t WAIT_TIME = 5; // init mocked bms
const int32_t UPGRADE_FLAG = 1;
const int32_t INVALID_USER_ID = -1;
const std::string EMPTY_STRING = "";
}  // namespace

class BmsBundleFreeInstallTest : public testing::Test {
public:
    BmsBundleFreeInstallTest();
    ~BmsBundleFreeInstallTest();
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    void AddInnerBundleInfo(const std::string bundleName);
    void UninstallBundleInfo(const std::string bundleName);
    BundlePackInfo CreateBundlePackInfo(const std::string &bundleName);
    const std::shared_ptr<BundleDataMgr> GetBundleDataMgr() const;
    void StartBundleService();

private:
    std::shared_ptr<BundleMgrService> bundleMgrService_ = DelayedSingleton<BundleMgrService>::GetInstance();
};

BmsBundleFreeInstallTest::BmsBundleFreeInstallTest()
{}

BmsBundleFreeInstallTest::~BmsBundleFreeInstallTest()
{}

void BmsBundleFreeInstallTest::SetUpTestCase()
{}

void BmsBundleFreeInstallTest::TearDownTestCase()
{}

void BmsBundleFreeInstallTest::SetUp()
{
    StartBundleService();
    auto dataMgr = GetBundleDataMgr();
    if (dataMgr != nullptr) {
        dataMgr->AddUserId(USERID);
    }
}

void BmsBundleFreeInstallTest::TearDown()
{}

void BmsBundleFreeInstallTest::AddInnerBundleInfo(const std::string bundleName)
{
    BundleInfo bundleInfo;
    bundleInfo.name = bundleName;

    ApplicationInfo application;
    application.name = bundleName;
    application.bundleName = bundleName;

    InnerBundleUserInfo userInfo;
    userInfo.bundleName = bundleName;
    userInfo.bundleUserInfo.userId = USERID;

    InnerModuleInfo moduleInfo;
    moduleInfo.moduleName = MODULE_NAME_TEST;
    moduleInfo.name = MODULE_NAME_TEST;
    moduleInfo.modulePackage = MODULE_NAME_TEST;

    std::map<std::string, InnerModuleInfo> innerModuleInfoMap;
    innerModuleInfoMap[MODULE_NAME_TEST] = moduleInfo;

    InnerBundleInfo innerBundleInfo;
    innerBundleInfo.SetBaseBundleInfo(bundleInfo);
    innerBundleInfo.SetBaseApplicationInfo(application);
    innerBundleInfo.AddInnerBundleUserInfo(userInfo);
    innerBundleInfo.SetBundlePackInfo(CreateBundlePackInfo(bundleName));
    innerBundleInfo.AddInnerModuleInfo(innerModuleInfoMap);

    auto dataMgr = GetBundleDataMgr();
    EXPECT_NE(dataMgr, nullptr);

    bool startRet = dataMgr->UpdateBundleInstallState(bundleName, InstallState::INSTALL_START);
    bool addRet = dataMgr->AddInnerBundleInfo(bundleName, innerBundleInfo);
    bool endRet = dataMgr->UpdateBundleInstallState(bundleName, InstallState::INSTALL_SUCCESS);

    EXPECT_TRUE(startRet);
    EXPECT_TRUE(addRet);
    EXPECT_TRUE(endRet);
}

BundlePackInfo BmsBundleFreeInstallTest::CreateBundlePackInfo(const std::string &bundleName)
{
    Packages packages;
    packages.name = bundleName;
    Summary summary;
    summary.app.bundleName = bundleName;
    PackageModule packageModule;
    packageModule.mainAbility = ABILITY_NAME_TEST;
    packageModule.distro.moduleName = MODULE_NAME_TEST;
    summary.modules.push_back(packageModule);

    BundlePackInfo packInfo;
    packInfo.packages.push_back(packages);
    packInfo.summary = summary;
    packInfo.SetValid(true);
    return packInfo;
}

void BmsBundleFreeInstallTest::UninstallBundleInfo(const std::string bundleName)
{
    auto dataMgr = GetBundleDataMgr();
    EXPECT_NE(dataMgr, nullptr);
    bool startRet = dataMgr->UpdateBundleInstallState(bundleName, InstallState::UNINSTALL_START);
    bool finishRet = dataMgr->UpdateBundleInstallState(bundleName, InstallState::UNINSTALL_SUCCESS);

    EXPECT_TRUE(startRet);
    EXPECT_TRUE(finishRet);
}

void BmsBundleFreeInstallTest::StartBundleService()
{
    if (!bundleMgrService_->IsServiceReady()) {
        bundleMgrService_->OnStart();
        std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME));
    }
}

const std::shared_ptr<BundleDataMgr> BmsBundleFreeInstallTest::GetBundleDataMgr() const
{
    return bundleMgrService_->GetDataMgr();
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0001
 * Function: IsModuleRemovable
 * @tc.name: test IsModuleRemovable
 * @tc.require: issueI5MZ7R
 * @tc.desc: bundleName and moduleName is empty
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0001, Function | SmallTest | Level0)
{
    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        bool isRemovable = false;
        ErrCode ret = bundleMgr->IsModuleRemovable("", "", isRemovable);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_PARAM_ERROR);
        EXPECT_FALSE(isRemovable);
    }
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0002
 * Function: IsModuleRemovable
 * @tc.name: test IsModuleRemovable
 * @tc.require: issueI5MZ7R
 * @tc.desc: bundleName does not exist
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0002, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        bool isRemovable = false;
        ErrCode ret = bundleMgr->IsModuleRemovable(BUNDLE_NAME_DEMO, MODULE_NAME_TEST, isRemovable);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_BUNDLE_NOT_EXIST);
        EXPECT_FALSE(isRemovable);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0003
 * Function: IsModuleRemovable
 * @tc.name: test IsModuleRemovable
 * @tc.require: issueI5MZ7R
 * @tc.desc: moduleName does not exist
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0003, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        bool isRemovable = false;
        ErrCode ret = bundleMgr->IsModuleRemovable(BUNDLE_NAME, MODULE_NAME_NOT_EXIST, isRemovable);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_MODULE_NOT_EXIST);
        EXPECT_FALSE(isRemovable);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0004
 * Function: IsModuleRemovable
 * @tc.name: test IsModuleRemovable
 * @tc.require: issueI5MZ7R
 * @tc.desc: bundleName and moduleName both exist
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0004, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        bool isRemovable = false;
        ErrCode ret = bundleMgr->IsModuleRemovable(BUNDLE_NAME, MODULE_NAME_TEST, isRemovable);
        EXPECT_EQ(ret, ERR_OK);
        EXPECT_FALSE(isRemovable);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0005
 * Function: SetModuleRemovable
 * @tc.name: test SetModuleRemovable
 * @tc.require: issueI5MZ7R
 * @tc.desc: bundleName and moduleName both exist
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0005, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        bool result = bundleMgr->SetModuleRemovable(BUNDLE_NAME, MODULE_NAME_TEST, true);
        EXPECT_TRUE(result);
        bool isRemovable = false;
        ErrCode ret = bundleMgr->IsModuleRemovable(BUNDLE_NAME, MODULE_NAME_TEST, isRemovable);
        EXPECT_EQ(ret, ERR_OK);
        EXPECT_TRUE(isRemovable);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0006
 * Function: SetModuleUpgradeFlag
 * @tc.name: test SetModuleUpgradeFlag
 * @tc.require: issueI5MZ7R
 * @tc.desc: bundleName and moduleName are empty
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0006, Function | SmallTest | Level0)
{
    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        ErrCode ret = bundleMgr->SetModuleUpgradeFlag("", "", 0);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_PARAM_ERROR);
    }
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0007
 * Function: SetModuleUpgradeFlag
 * @tc.name: test SetModuleUpgradeFlag
 * @tc.require: issueI5MZ7R
 * @tc.desc: bundleName not exist
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0007, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        ErrCode ret = bundleMgr->SetModuleUpgradeFlag(BUNDLE_NAME_DEMO, MODULE_NAME_TEST, 0);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_BUNDLE_NOT_EXIST);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0008
 * Function: SetModuleUpgradeFlag
 * @tc.name: test SetModuleUpgradeFlag
 * @tc.require: issueI5MZ7R
 * @tc.desc: moduleName not exist
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0008, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        ErrCode ret = bundleMgr->SetModuleUpgradeFlag(BUNDLE_NAME, MODULE_NAME_NOT_EXIST, 0);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_MODULE_NOT_EXIST);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0009
 * Function: SetModuleUpgradeFlag
 * @tc.name: test SetModuleUpgradeFlag
 * @tc.require: issueI5MZ7R
 * @tc.desc: bundleName and moduleName both exist
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0009, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        ErrCode ret = bundleMgr->SetModuleUpgradeFlag(BUNDLE_NAME, MODULE_NAME_TEST, UPGRADE_FLAG);
        EXPECT_EQ(ret, ERR_OK);
        bool flag = bundleMgr->GetModuleUpgradeFlag(BUNDLE_NAME, MODULE_NAME_TEST);
        EXPECT_TRUE(flag);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0010
 * Function: GetModuleUpgradeFlag
 * @tc.name: test GetModuleUpgradeFlag
 * @tc.require: issueI5MZ7R
 * @tc.desc: bundleName and moduleName are empty
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0010, Function | SmallTest | Level0)
{
    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        bool flag = bundleMgr->GetModuleUpgradeFlag("", "");
        EXPECT_FALSE(flag);
    }
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0011
 * Function: GetModuleUpgradeFlag
 * @tc.name: test GetModuleUpgradeFlag
 * @tc.require: issueI5MZ7R
 * @tc.desc: bundleName not exist
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0011, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        bool flag = bundleMgr->GetModuleUpgradeFlag(BUNDLE_NAME_DEMO, MODULE_NAME_TEST);
        EXPECT_FALSE(flag);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0012
 * Function: GetModuleUpgradeFlag
 * @tc.name: test GetModuleUpgradeFlag
 * @tc.require: issueI5MZ7R
 * @tc.desc: moduleName not exist
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0012, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        bool flag = bundleMgr->GetModuleUpgradeFlag(BUNDLE_NAME, MODULE_NAME_NOT_EXIST);
        EXPECT_FALSE(flag);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0013
 * Function: GetBundlePackInfo
 * @tc.name: test GetBundlePackInfo
 * @tc.require: issueI5MZ7R
 * @tc.desc: bundleName not exist
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0013, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        BundlePackInfo packInfo;
        ErrCode ret = bundleMgr->GetBundlePackInfo(BUNDLE_NAME_DEMO,
            BundlePackFlag::GET_PACK_INFO_ALL, packInfo, USERID);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_BUNDLE_NOT_EXIST);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0014
 * Function: GetBundlePackInfo
 * @tc.name: test GetBundlePackInfo
 * @tc.require: issueI5MZ7R
 * @tc.desc: userId not exist
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0014, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        BundlePackInfo packInfo;
        ErrCode ret = bundleMgr->GetBundlePackInfo(BUNDLE_NAME,
            BundlePackFlag::GET_PACK_INFO_ALL, packInfo, INVALID_USER_ID);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_INVALID_USER_ID);
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0015
 * Function: GetBundlePackInfo
 * @tc.name: test GetBundlePackInfo
 * @tc.require: issueI5MZ7R
 * @tc.desc: test bundle pack flag GET_PACK_INFO_ALL
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0015, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        BundlePackInfo packInfo;
        ErrCode ret = bundleMgr->GetBundlePackInfo(BUNDLE_NAME,
            BundlePackFlag::GET_PACK_INFO_ALL, packInfo, USERID);
        EXPECT_EQ(ret, ERR_OK);
        EXPECT_FALSE(packInfo.packages.empty());
        if (!packInfo.packages.empty()) {
            EXPECT_EQ(packInfo.packages[0].name, BUNDLE_NAME);
        }

        EXPECT_EQ(packInfo.summary.app.bundleName, BUNDLE_NAME);
        EXPECT_FALSE(packInfo.summary.modules.empty());
        if (!packInfo.summary.modules.empty()) {
            EXPECT_EQ(packInfo.summary.modules[0].mainAbility, ABILITY_NAME_TEST);
            EXPECT_EQ(packInfo.summary.modules[0].distro.moduleName, MODULE_NAME_TEST);
        }
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0016
 * Function: GetBundlePackInfo
 * @tc.name: test GetBundlePackInfo
 * @tc.require: issueI5MZ7R
 * @tc.desc: test bundle pack flag GET_PACKAGES
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0016, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        BundlePackInfo packInfo;
        ErrCode ret  = bundleMgr->GetBundlePackInfo(BUNDLE_NAME,
            BundlePackFlag::GET_PACKAGES, packInfo, USERID);
        EXPECT_EQ(ret, ERR_OK);
        // GET_PACKAGES: include packages, not include summary
        EXPECT_FALSE(packInfo.packages.empty());
        if (!packInfo.packages.empty()) {
            EXPECT_EQ(packInfo.packages[0].name, BUNDLE_NAME);
        }
        EXPECT_EQ(packInfo.summary.app.bundleName, EMPTY_STRING);
        EXPECT_TRUE(packInfo.summary.modules.empty());
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0017
 * Function: GetBundlePackInfo
 * @tc.name: test GetBundlePackInfo
 * @tc.require: issueI5MZ7R
 * @tc.desc: test bundle pack flag GET_BUNDLE_SUMMARY
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0017, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        BundlePackInfo packInfo;
        ErrCode ret = bundleMgr->GetBundlePackInfo(BUNDLE_NAME,
            BundlePackFlag::GET_BUNDLE_SUMMARY, packInfo, USERID);
        EXPECT_EQ(ret, ERR_OK);
        // GET_PACKAGES: include summary, not include package
        EXPECT_TRUE(packInfo.packages.empty());

        EXPECT_FALSE(packInfo.summary.modules.empty());
        EXPECT_EQ(packInfo.summary.app.bundleName, BUNDLE_NAME);
        if (!packInfo.summary.modules.empty()) {
            EXPECT_EQ(packInfo.summary.modules[0].mainAbility, ABILITY_NAME_TEST);
            EXPECT_EQ(packInfo.summary.modules[0].distro.moduleName, MODULE_NAME_TEST);
        }
    }

    UninstallBundleInfo(BUNDLE_NAME);
}

/**
 * @tc.number: BmsBundleFreeInstallTest_0018
 * Function: GetBundlePackInfo
 * @tc.name: test GetBundlePackInfo
 * @tc.require: issueI5MZ7R
 * @tc.desc: test bundle pack flag GET_MODULE_SUMMARY
 */
HWTEST_F(BmsBundleFreeInstallTest, BmsBundleFreeInstallTest_0018, Function | SmallTest | Level0)
{
    AddInnerBundleInfo(BUNDLE_NAME);

    auto bundleMgr = GetBundleDataMgr();
    if (bundleMgr != nullptr) {
        BundlePackInfo packInfo;
        ErrCode ret = bundleMgr->GetBundlePackInfo(BUNDLE_NAME,
            BundlePackFlag::GET_MODULE_SUMMARY, packInfo, USERID);
        EXPECT_EQ(ret, ERR_OK);
        // GET_PACKAGES: include summary.modules, not include packages and summary.app
        EXPECT_TRUE(packInfo.packages.empty());
        EXPECT_EQ(packInfo.summary.app.bundleName, EMPTY_STRING);
        EXPECT_EQ(packInfo.summary.app.bundleName, EMPTY_STRING);

        EXPECT_FALSE(packInfo.summary.modules.empty());
        if (!packInfo.summary.modules.empty()) {
            EXPECT_EQ(packInfo.summary.modules[0].mainAbility, ABILITY_NAME_TEST);
            EXPECT_EQ(packInfo.summary.modules[0].distro.moduleName, MODULE_NAME_TEST);
        }
    }

    UninstallBundleInfo(BUNDLE_NAME);
}
} // OHOS