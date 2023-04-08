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

#include "app_jump_interceptor_event_subscriber.h"

#include "app_log_wrapper.h"
#include "app_jump_interceptor_manager_rdb.h"
#include "want.h"

namespace OHOS {
namespace AppExecFwk {
AppJumpInterceptorEventSubscriber::AppJumpInterceptorEventSubscriber(
    const EventFwk::CommonEventSubscribeInfo &subscribeInfo, 
    const std::shared_ptr<IAppJumpInterceptorlManagerDb> &appJumpDb)
    : EventFwk::CommonEventSubscriber(subscribeInfo)
{
    appJumpDb_ = appJumpDb;
}

AppJumpInterceptorEventSubscriber::~AppJumpInterceptorEventSubscriber()
{
}

void AppJumpInterceptorEventSubscriber::OnReceiveEvent(const EventFwk::CommonEventData &eventData)
{
    const AAFwk::Want& want = eventData.GetWant();
    std::string action = want.GetAction();
    std::string bundleName = want.GetElement().GetBundleName();
    int32_t userId = want.GetIntParam("userId", -1);
    std::shared_ptr<IAppJumpInterceptorlManagerDb> db = appJumpDb_;
    if (action.empty() || eventHandler_ == nullptr || userId < 0) {
        APP_LOGE("%{public}s failed, empty action: %{public}s, or invalid event handler, userId:%d",
            __func__, action.c_str(), userId);
        return;
    }
    if (bundleName.empty() && action != EventFwk::CommonEventSupport::COMMON_EVENT_USER_SWITCHED) {
        APP_LOGE("%{public}s failed, invalid param, action: %{public}s, bundleName: %{public}s",
            __func__, action.c_str(), bundleName.c_str());
        return;
    }
    APP_LOGI("%{public}s, action:%{public}s.", __func__, action.c_str());
    std::weak_ptr<AppJumpInterceptorEventSubscriber> weakThis = shared_from_this();
    if (action == EventFwk::CommonEventSupport::COMMON_EVENT_PACKAGE_REMOVED) {
        auto task = [weakThis, bundleName, db, userId]() {
            APP_LOGI("bundle remove, bundleName: %{public}s", bundleName.c_str());
            std::shared_ptr<AppJumpInterceptorEventSubscriber> sharedThis = weakThis.lock();
            if (sharedThis) {
                APP_LOGI("start delete rule bundleName: %{public}s, userId:%d", bundleName.c_str(), userId);
                db->DeleteRuleByCallerBundleName(bundleName, userId);
                db->DeleteRuleByTargetBundleName(bundleName, userId);
            }
        };
        eventHandler_->PostTask(task);
    } else {
        APP_LOGW("%{public}s warnning, invalid action.", __func__);
    }
}
} // AppExecFwk
} // OHOS