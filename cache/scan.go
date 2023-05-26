//
// Copyright 2018-2020 Solus Project <copyright@getsol.us>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

package cache

import (
	"os"
	"path/filepath"

	"github.com/getsolus/aa-lsm-hook/config"
	"github.com/getsolus/aa-lsm-hook/profiles"
)

// Scan finds all of the currently cached profiles
func Scan() (profiles.ProfMap, error) {
	profs := make(profiles.ProfMap)
	err := profs.AddProfiles(config.AppArmorCache)
	return profs, err
}

// ScanDirs returns all the currently cached directories.
func ScanDirs() ([]string, error) {
	entries, err := os.ReadDir(config.AppArmorCache)
	if err != nil {
		return nil, err
	}

	dirs := make([]string, 0, len(entries))
	for _, entry := range entries {
		if entry.IsDir() {
			dirs = append(dirs, filepath.Join(config.AppArmorCache, entry.Name()))
		}
	}

	return dirs, nil
}
