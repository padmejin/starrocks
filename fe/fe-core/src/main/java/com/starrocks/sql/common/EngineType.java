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

package com.starrocks.sql.common;

import com.starrocks.common.Config;
import com.starrocks.common.RunMode;

public enum EngineType {
    OLAP,
    MYSQL,
    BROKER,
    ELASTICSEARCH,
    HIVE,
    ICEBERG,
    HUDI,
    JDBC,
    STARROCKS,
    FILE;

    public static EngineType defaultEngine() {
        if (Config.run_mode.equalsIgnoreCase(RunMode.SHARED_DATA.name())) {
            return STARROCKS;
        }
        return OLAP;
    }
}
