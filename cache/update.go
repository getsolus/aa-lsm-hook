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
	"github.com/getsolus/aa-lsm-hook/config"
	"github.com/getsolus/aa-lsm-hook/profiles"
)

// Update rebuilds all of the profiles in the cache and cleans up the cache
func Update() error {
	dirs, err := config.ProfileDirs() // Get the profile directories
	if err != nil {
		return err
	}
	cached, err := Scan() // Scan cached profiles
	if err != nil {
		return err
	}
	profs, err := profiles.UpdateAll(dirs, cached) // Cache all profiles as needed
	if err != nil {
		return err
	}
	if cached, err = Scan(); err != nil { // Attempt to rescan cached profiles
		return err
	}
	return Clean(profs, cached) // Clean up orphaned cached profiles
}
