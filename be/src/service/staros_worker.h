// Copyright 2021-present StarRocks, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef USE_STAROS

#include <starlet.h>

#include <shared_mutex>
#include <unordered_map>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "common/status.h"
#include "fslib/configuration.h"
#include "fslib/file_system.h"

namespace starrocks {

// TODO: find a better place to put this function
// Convert absl::Status to starrocks::Status
Status to_status(absl::Status absl_status);

class StarOSWorker : public staros::starlet::Worker {
public:
    using ServiceId = staros::starlet::ServiceId;
    using WorkerId = staros::starlet::WorkerId;
    using ShardId = staros::starlet::ShardId;
    using ShardInfo = staros::starlet::ShardInfo;
    using WorkerInfo = staros::starlet::WorkerInfo;
    using FileSystem = staros::starlet::fslib::FileSystem;
    using Configuration = staros::starlet::fslib::Configuration;

    StarOSWorker() = default;

    ~StarOSWorker() override = default;

    absl::Status add_shard(const ShardInfo& shard) override;

    absl::Status remove_shard(const ShardId shard) override;

    absl::StatusOr<WorkerInfo> worker_info() const override;

    absl::Status update_worker_info(const WorkerInfo& info) override;

    absl::StatusOr<ShardInfo> get_shard_info(ShardId id) const override;

    std::vector<ShardInfo> shards() const override;

    // `conf`: a k-v map, provides additional information about the filesystem configuration
    absl::StatusOr<std::shared_ptr<FileSystem>> get_shard_filesystem(ShardId id, const Configuration& conf);

private:
    struct ShardInfoDetails {
        ShardInfo shard_info;
        std::shared_ptr<FileSystem> fs;

        ShardInfoDetails(const ShardInfo& info) : shard_info(info) {}
    };

private:
    mutable std::shared_mutex _mtx;
    std::unordered_map<ShardId, ShardInfoDetails> _shards;
};

extern std::shared_ptr<StarOSWorker> g_worker;
void init_staros_worker();
void shutdown_staros_worker();

} // namespace starrocks
#endif // USE_STAROS
