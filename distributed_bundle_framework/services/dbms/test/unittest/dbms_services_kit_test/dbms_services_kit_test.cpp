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
#include "distributed_ability_info.h"
#include "distributed_bms.h"
#include "distributed_bms_interface.h"
#include "distributed_bms_proxy.h"
#include "distributed_bundle_info.h"
#include "distributed_module_info.h"
#include "element_name.h"
#include "image_compress.h"
#include "json_util.h"

using namespace testing::ext;
using namespace std::chrono_literals;
using namespace OHOS::AppExecFwk;

namespace OHOS {
namespace {
const std::string WRONG_BUNDLE_NAME = "wrong";
const std::string WRONG_ABILITY_NAME = "wrong";
const std::string BUNDLE_NAME = "com.ohos.launcher";
const std::string MODULE_NAME = "launcher_settings";
const std::string ABILITY_NAME = "com.ohos.launcher.settings.MainAbility";
const std::string DEVICE_ID = "1111";
const std::string INVALID_NAME = "invalid";
const std::string HAP_FILE_PATH =
    "/data/app/el1/bundle/public/com.example.test/entry.hap";
int32_t USERID = 100;
}  // namespace

class DbmsServicesKitTest : public testing::Test {
public:
    DbmsServicesKitTest();
    ~DbmsServicesKitTest();
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    std::shared_ptr<DistributedBms> GetDistributedBms();
    std::shared_ptr<DistributedBmsProxy> GetDistributedBmsProxy();
    std::shared_ptr<DistributedDataStorage> GetDistributedDataStorage();
    void StartInstalldService() const;
    void StartBundleService();
private:
    std::shared_ptr<DistributedBms> distributedBms_ = nullptr;
    std::shared_ptr<DistributedBmsProxy> distributedBmsProxy_ = nullptr;
    std::shared_ptr<DistributedDataStorage> distributedDataStorage_ = nullptr;
};

DbmsServicesKitTest::DbmsServicesKitTest()
{}

DbmsServicesKitTest::~DbmsServicesKitTest()
{}

void DbmsServicesKitTest::SetUpTestCase()
{}

void DbmsServicesKitTest::TearDownTestCase()
{}

void DbmsServicesKitTest::SetUp()
{}

void DbmsServicesKitTest::TearDown()
{}

std::shared_ptr<DistributedBms> DbmsServicesKitTest::GetDistributedBms()
{
    if (distributedBms_ == nullptr) {
        distributedBms_ = DelayedSingleton<DistributedBms>::GetInstance();
    }
    return distributedBms_;
}

std::shared_ptr<DistributedBmsProxy> DbmsServicesKitTest::GetDistributedBmsProxy()
{
    if (distributedBmsProxy_ == nullptr) {
        distributedBmsProxy_ = std::make_shared<DistributedBmsProxy>(nullptr);
    }
    return distributedBmsProxy_;
}

std::shared_ptr<DistributedDataStorage> DbmsServicesKitTest::GetDistributedDataStorage()
{
    if (distributedDataStorage_ == nullptr) {
        distributedDataStorage_ =
            DelayedSingleton<DistributedDataStorage>::GetInstance();
    }
    return distributedDataStorage_;
}
/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetRemoteAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test ElementName empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0001, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        ElementName name;
        RemoteAbilityInfo info;
        auto ret = distributedBms->GetRemoteAbilityInfo(name, info);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_DEVICE_ID_NOT_EXIST);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetRemoteAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test ElementName empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0002, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        ElementName name;
        RemoteAbilityInfo info;
        auto ret = distributedBms->GetRemoteAbilityInfo(name, "", info);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_DEVICE_ID_NOT_EXIST);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetRemoteAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test ElementName empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0003, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        ElementName name;
        RemoteAbilityInfo info;
        auto ret = distributedBms->GetRemoteAbilityInfo(name, "", info);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_DEVICE_ID_NOT_EXIST);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetRemoteAbilityInfos
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test ElementName empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0004, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        std::vector<ElementName> name;
        std::vector<RemoteAbilityInfo> info;
        auto ret = distributedBms->GetRemoteAbilityInfos(name, info);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_PARAM_ERROR);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetRemoteAbilityInfos
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test ElementName empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0005, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        std::vector<ElementName> name;
        std::vector<RemoteAbilityInfo> info;
        auto ret = distributedBms->GetRemoteAbilityInfos(name, "", info);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_PARAM_ERROR);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test bundleName empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0006, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        ElementName name;
        name.SetBundleName(WRONG_BUNDLE_NAME);
        name.SetAbilityName(ABILITY_NAME);
        RemoteAbilityInfo info;
        auto ret = distributedBms->GetAbilityInfo(name, info);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_BUNDLE_NOT_EXIST);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test abilityName empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0007, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        ElementName name;
        name.SetBundleName(BUNDLE_NAME);
        name.SetAbilityName(WRONG_ABILITY_NAME);
        RemoteAbilityInfo info;
        auto ret = distributedBms->GetAbilityInfo(name, info);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_ABILITY_NOT_EXIST);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test bundleName and abilityName both exist
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0008, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        ElementName name;
        name.SetBundleName(BUNDLE_NAME);
        name.SetAbilityName(ABILITY_NAME);
        RemoteAbilityInfo info;
        auto ret = distributedBms->GetAbilityInfo(name, info);
        EXPECT_EQ(ret, ERR_OK);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test wrong abilityName
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0009, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        ElementName name;
        name.SetBundleName(BUNDLE_NAME);
        name.SetModuleName(MODULE_NAME);
        name.SetAbilityName(WRONG_ABILITY_NAME);
        RemoteAbilityInfo info;
        auto ret = distributedBms->GetAbilityInfo(name, info);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_ABILITY_NOT_EXIST);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test ElementName empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0010, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        std::vector<ElementName> name;
        std::vector<RemoteAbilityInfo> info;
        auto ret = distributedBms->GetAbilityInfos(name, info);
        EXPECT_EQ(ret, ERR_OK);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test abilityName empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0011, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        std::vector<ElementName> names;
        ElementName name;
        name.SetBundleName(BUNDLE_NAME);
        name.SetModuleName(MODULE_NAME);
        name.SetAbilityName(WRONG_ABILITY_NAME);
        names.push_back(name);
        std::vector<RemoteAbilityInfo> infos;
        auto ret = distributedBms->GetAbilityInfos(names, infos);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_ABILITY_NOT_EXIST);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test abilityName empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0012, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        std::vector<ElementName> names;
        ElementName name;
        name.SetBundleName(BUNDLE_NAME);
        name.SetAbilityName(ABILITY_NAME);
        names.push_back(name);
        std::vector<RemoteAbilityInfo> infos;
        auto ret = distributedBms->GetAbilityInfos(names, infos);
        EXPECT_EQ(ret, ERR_OK);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test bundleName empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0013, Function | SmallTest | Level0)
{
    auto distributedBmsProxy = GetDistributedBmsProxy();
    EXPECT_NE(distributedBmsProxy, nullptr);
    if (distributedBmsProxy != nullptr) {
        ElementName name;
        RemoteAbilityInfo info;
        auto ret = distributedBmsProxy->GetRemoteAbilityInfo(name, "", info);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_BUNDLE_NOT_EXIST);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test abilityName empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0014, Function | SmallTest | Level0)
{
    auto distributedBmsProxy = GetDistributedBmsProxy();
    EXPECT_NE(distributedBmsProxy, nullptr);
    if (distributedBmsProxy != nullptr) {
        ElementName name;
        name.SetBundleName(BUNDLE_NAME);
        RemoteAbilityInfo info;
        auto ret = distributedBmsProxy->GetRemoteAbilityInfo(name, "", info);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_ABILITY_NOT_EXIST);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test deviceID empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0015, Function | SmallTest | Level0)
{
    auto distributedBmsProxy = GetDistributedBmsProxy();
    EXPECT_NE(distributedBmsProxy, nullptr);
    if (distributedBmsProxy != nullptr) {
        ElementName name;
        name.SetBundleName(BUNDLE_NAME);
        name.SetAbilityName(ABILITY_NAME);
        RemoteAbilityInfo info;
        auto ret = distributedBmsProxy->GetRemoteAbilityInfo(name, "", info);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_DEVICE_ID_NOT_EXIST);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test ElementName not empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0016, Function | SmallTest | Level0)
{
    auto distributedBmsProxy = GetDistributedBmsProxy();
    EXPECT_NE(distributedBmsProxy, nullptr);
    if (distributedBmsProxy != nullptr) {
        ElementName name;
        name.SetBundleName(BUNDLE_NAME);
        name.SetAbilityName(ABILITY_NAME);
        name.SetDeviceID(DEVICE_ID);
        RemoteAbilityInfo info;
        auto ret = distributedBmsProxy->GetRemoteAbilityInfo(name, "", info);
        EXPECT_EQ(ret, ERR_APPEXECFWK_FAILED_GET_REMOTE_PROXY);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test ElementNames empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0017, Function | SmallTest | Level0)
{
    auto distributedBmsProxy = GetDistributedBmsProxy();
    EXPECT_NE(distributedBmsProxy, nullptr);
    if (distributedBmsProxy != nullptr) {
        std::vector<ElementName> names;
        std::vector<RemoteAbilityInfo> infos;
        auto ret = distributedBmsProxy->GetRemoteAbilityInfos(names, infos);
        EXPECT_EQ(ret, ERR_APPEXECFWK_FAILED_GET_REMOTE_PROXY);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test ElementNames not empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0018, Function | SmallTest | Level0)
{
    auto distributedBmsProxy = GetDistributedBmsProxy();
    EXPECT_NE(distributedBmsProxy, nullptr);
    if (distributedBmsProxy != nullptr) {
        std::vector<ElementName> names;
        ElementName name_1;
        name_1.SetBundleName(BUNDLE_NAME);
        name_1.SetAbilityName(ABILITY_NAME);
        name_1.SetDeviceID(DEVICE_ID);
        names.push_back(name_1);

        ElementName name_2;
        name_2.SetBundleName(BUNDLE_NAME);
        name_2.SetAbilityName(ABILITY_NAME);
        names.push_back(name_2);

        std::vector<RemoteAbilityInfo> infos;
        auto ret = distributedBmsProxy->GetRemoteAbilityInfos(names, infos);
        EXPECT_EQ(ret, ERR_BUNDLE_MANAGER_DEVICE_ID_NOT_EXIST);
    }
}

/**
 * @tc.number: DbmsServicesKitTest_0019
 * @tc.name: test DistributedAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test to_json and from_json
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0019, Function | SmallTest | Level0)
{
    DistributedAbilityInfo distributedAbilityInfo;
    distributedAbilityInfo.abilityName = "abilityName";
    nlohmann::json jsonObject;
    to_json(jsonObject, distributedAbilityInfo);
    DistributedAbilityInfo result;
    from_json(jsonObject, result);
    EXPECT_EQ(result.abilityName, "abilityName");
}

/**
 * @tc.number: DbmsServicesKitTest_0020
 * @tc.name: test DistributedAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test Dump
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0020, Function | SmallTest | Level0)
{
    DistributedAbilityInfo distributedAbilityInfo;
    std::string path = "/data/test/abilityInfo.txt";
    std::ofstream file(path);
    file.close();
    int fd = 8;
    std::string prefix = "[ability]";
    distributedAbilityInfo.Dump(prefix, fd);
    long length = lseek(fd, 0, SEEK_END);
    EXPECT_GT(length, 0);
}

/**
 * @tc.number: DbmsServicesKitTest_0021
 * @tc.name: test DistributedBundleInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test FromJsonString and ToString
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0021, Function | SmallTest | Level0)
{
    DistributedBundleInfo distributedBundleInfo;
    std::string value = distributedBundleInfo.ToString();
    std::string jsonString;
    auto res = distributedBundleInfo.FromJsonString(value);
    EXPECT_EQ(res, true);
}

/**
 * @tc.number: DbmsServicesKitTest_0022
 * @tc.name: test DistributedBundleInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test Marshalling and Unmarshalling
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0022, Function | SmallTest | Level0)
{
    DistributedBundleInfo distributedBundleInfo;
    distributedBundleInfo.version = 2;
    distributedBundleInfo.bundleName = "bundleName";
    distributedBundleInfo.versionCode = 1;
    distributedBundleInfo.versionName = "versionName";
    distributedBundleInfo.minCompatibleVersion = 1;
    distributedBundleInfo.targetVersionCode = 1;
    distributedBundleInfo.compatibleVersionCode = 1;
    distributedBundleInfo.appId = "appId";
    distributedBundleInfo.enabled = false;

    Parcel parcel;
    bool ret = distributedBundleInfo.Marshalling(parcel);
    EXPECT_TRUE(ret);
    auto unmarshalledResult = DistributedBundleInfo::Unmarshalling(parcel);
    EXPECT_EQ(unmarshalledResult->version, 2);
    EXPECT_EQ(unmarshalledResult->bundleName, "bundleName");
    EXPECT_EQ(unmarshalledResult->versionCode, 1);
    EXPECT_EQ(unmarshalledResult->versionName, "versionName");
    EXPECT_EQ(unmarshalledResult->minCompatibleVersion, 1);
    EXPECT_EQ(unmarshalledResult->targetVersionCode, 1);
    EXPECT_EQ(unmarshalledResult->compatibleVersionCode, 1);
    EXPECT_EQ(unmarshalledResult->appId, "appId");
    EXPECT_EQ(unmarshalledResult->enabled, false);
}

/**
 * @tc.number: DbmsServicesKitTest_0023
 * @tc.name: test DistributedModuleInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test to_json and from_json
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0023, Function | SmallTest | Level0)
{
    DistributedModuleInfo distributedModuleInfo;
    distributedModuleInfo.moduleName = "moduleName";
    nlohmann::json jsonObject;
    to_json(jsonObject, distributedModuleInfo);
    DistributedModuleInfo result;
    from_json(jsonObject, result);
    EXPECT_EQ(result.moduleName, "moduleName");
}

/**
 * @tc.number: DbmsServicesKitTest_0024
 * @tc.name: test DistributedModuleInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test Dump
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0024, Function | SmallTest | Level0)
{
    DistributedModuleInfo distributedModuleInfo;
    std::string path = "/data/test/abilityInfo.txt";
    std::ofstream file(path);
    file.close();
    int fd = 8;
    std::string prefix = "[ability]";
    distributedModuleInfo.Dump(prefix, fd);
    long length = lseek(fd, 0, SEEK_END);
    EXPECT_GT(length, 0);
}

/**
 * @tc.number: DbmsServicesKitTest_0025
 * @tc.name: test SummaryAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test Marshalling and Unmarshalling
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0025, Function | SmallTest | Level0)
{
    SummaryAbilityInfo summaryAbilityInfo;
    summaryAbilityInfo.bundleName = "bundleName";
    summaryAbilityInfo.moduleName = "moduleName";
    summaryAbilityInfo.abilityName = "abilityName";
    summaryAbilityInfo.logoUrl = "logoUrl";
    summaryAbilityInfo.label = "label";
    summaryAbilityInfo.deviceType.push_back("deviceType");
    summaryAbilityInfo.rpcId.push_back("rpcId");

    Parcel parcel;
    bool ret = summaryAbilityInfo.Marshalling(parcel);
    EXPECT_TRUE(ret);
    auto unmarshalledResult = SummaryAbilityInfo::Unmarshalling(parcel);
    EXPECT_EQ(unmarshalledResult->bundleName, "bundleName");
    EXPECT_EQ(unmarshalledResult->moduleName, "moduleName");
    EXPECT_EQ(unmarshalledResult->abilityName, "abilityName");
    EXPECT_EQ(unmarshalledResult->logoUrl, "logoUrl");
    EXPECT_EQ(unmarshalledResult->label, "label");
    EXPECT_EQ(unmarshalledResult->deviceType.size(), 1);
    EXPECT_EQ(unmarshalledResult->rpcId.size(), 1);
}

/**
 * @tc.number: DbmsServicesKitTest_0026
 * @tc.name: test SummaryAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test to_json and from_json
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0026, Function | SmallTest | Level0)
{
    SummaryAbilityInfo summaryAbilityInfo;
    summaryAbilityInfo.abilityName = "abilityName";
    nlohmann::json jsonObject;
    to_json(jsonObject, summaryAbilityInfo);
    SummaryAbilityInfo result;
    from_json(jsonObject, result);
    EXPECT_EQ(result.abilityName, "abilityName");
}

/**
 * @tc.number: DbmsServicesKitTest_0027
 * @tc.name: test RpcIdResult
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test to_json and from_json
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0027, Function | SmallTest | Level0)
{
    RpcIdResult rpcIdResult;
    rpcIdResult.version = "version";
    rpcIdResult.transactId = "transactId";
    rpcIdResult.retCode = 1;
    rpcIdResult.resultMsg = "resultMsg";
    nlohmann::json jsonObject;
    to_json(jsonObject, rpcIdResult);
    RpcIdResult result;
    from_json(jsonObject, result);
    EXPECT_EQ(result.version, "version");
    EXPECT_EQ(result.transactId, "transactId");
    EXPECT_EQ(result.retCode, 1);
    EXPECT_EQ(result.resultMsg, "resultMsg");
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test OnStart
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test distributedSub_ not empty
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0028, Function | SmallTest | Level0)
{
    auto distributedBms = GetDistributedBms();
    EXPECT_NE(distributedBms, nullptr);
    if (distributedBms != nullptr) {
        distributedBms->OnStart();
        EXPECT_NE(nullptr, distributedBms->distributedSub_);
        distributedBms->OnStop();
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test SaveStorageDistributeInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test GetStorageDistributeInfo failed by invalid bundle name
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0029, Function | SmallTest | Level0)
{
    auto distributedDataStorage = GetDistributedDataStorage();
    EXPECT_NE(distributedDataStorage, nullptr);
    if (distributedDataStorage != nullptr) {
        DistributedBundleInfo info;
        distributedDataStorage->SaveStorageDistributeInfo(INVALID_NAME, USERID);
        bool res = distributedDataStorage->GetStorageDistributeInfo("123", INVALID_NAME, info);
        EXPECT_EQ(res, false);
        distributedDataStorage->UpdateDistributedData(USERID);
        distributedDataStorage->DeleteStorageDistributeInfo(INVALID_NAME, USERID);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetAbilityInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test GetStorageDistributeInfo failed by invalid user id
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0030, Function | SmallTest | Level0)
{
    auto distributedDataStorage = GetDistributedDataStorage();
    EXPECT_NE(distributedDataStorage, nullptr);
    if (distributedDataStorage != nullptr) {
        DistributedBundleInfo info;
        distributedDataStorage->SaveStorageDistributeInfo(BUNDLE_NAME, Constants::INVALID_USERID);
        bool res = distributedDataStorage->GetStorageDistributeInfo("123", BUNDLE_NAME, info);
        EXPECT_EQ(res, false);
        distributedDataStorage->UpdateDistributedData(Constants::INVALID_USERID);
        distributedDataStorage->DeleteStorageDistributeInfo(BUNDLE_NAME, Constants::INVALID_USERID);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test IsPathValid
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test path is not valid
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0031, Function | SmallTest | Level0)
{
    std::unique_ptr<ImageCompress> imageCompress = std::make_unique<ImageCompress>();
    EXPECT_NE(imageCompress, nullptr);
    if (imageCompress != nullptr) {
        bool res = imageCompress->IsPathValid(HAP_FILE_PATH);
        EXPECT_EQ(res, false);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetImageType
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test get image type
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0032, Function | SmallTest | Level0)
{
    std::unique_ptr<ImageCompress> imageCompress = std::make_unique<ImageCompress>();
    EXPECT_NE(imageCompress, nullptr);
    if (imageCompress != nullptr) {
        std::unique_ptr<uint8_t[]> fileData;
        constexpr size_t fileLength = 7;
        ImageType res = imageCompress->GetImageType(fileData, fileLength);
        EXPECT_EQ(res, ImageType::WORNG_TYPE);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetImageTypeString
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. get image type failed
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0033, Function | SmallTest | Level0)
{
    std::unique_ptr<ImageCompress> imageCompress = std::make_unique<ImageCompress>();
    EXPECT_NE(imageCompress, nullptr);
    if (imageCompress != nullptr) {
        std::unique_ptr<uint8_t[]> fileData;
        constexpr size_t fileLength = 7;
        std::string imageType;
        bool res = imageCompress->GetImageTypeString(fileData, fileLength, imageType);
        EXPECT_FALSE(res);
    }
}

/**
 * @tc.number: DbmsServicesKitTest
 * @tc.name: test GetImageFileInfo
 * @tc.require: issueI5MZ8V
 * @tc.desc: 1. system running normally
 *           2. test get image file info failed
 */
HWTEST_F(DbmsServicesKitTest, DbmsServicesKitTest_0034, Function | SmallTest | Level0)
{
    std::unique_ptr<ImageCompress> imageCompress = std::make_unique<ImageCompress>();
    EXPECT_NE(imageCompress, nullptr);
    if (imageCompress != nullptr) {
        std::unique_ptr<uint8_t[]> fileContent;
        int64_t fileLength;
        bool res = imageCompress->
            GetImageFileInfo(
                HAP_FILE_PATH, fileContent, fileLength);
        EXPECT_EQ(res, false);
    }
}
} // OHOS