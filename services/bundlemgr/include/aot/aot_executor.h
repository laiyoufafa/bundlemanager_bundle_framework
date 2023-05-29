/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef FOUNDATION_BUNDLE_FRAMEWORK_AOT_AOT_EXECUTOR
#define FOUNDATION_BUNDLE_FRAMEWORK_AOT_AOT_EXECUTOR

#include <mutex>

#include "aot/aot_args.h"
#include "appexecfwk_errors.h"
#include "nocopyable.h"

namespace OHOS {
namespace AppExecFwk {
class AOTExecutor final {
public:
    static AOTExecutor& GetInstance();
    void ExecuteAOT(const AOTArgs &aotArgs, ErrCode &ret) const;
private:
    AOTExecutor() = default;
    ~AOTExecutor() = default;
    DISALLOW_COPY_AND_MOVE(AOTExecutor);

    std::string DecToHex(uint32_t decimal) const;
    bool CheckArgs(const AOTArgs &aotArgs) const;
    bool GetAbcFileInfo(const std::string &hapPath, uint32_t &offset, uint32_t &length) const;
    ErrCode PrepareArgs(const AOTArgs &aotArgs, AOTArgs &completeArgs) const;
    void ExecuteInChildProcess(const AOTArgs &aotArgs) const;
    void ExecuteInParentProcess(pid_t childPid, ErrCode &ret) const;
private:
    mutable std::mutex mutex_;
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // FOUNDATION_BUNDLE_FRAMEWORK_AOT_AOT_EXECUTOR