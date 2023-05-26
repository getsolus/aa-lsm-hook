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
	"fmt"
	"os"
	"path/filepath"

	"github.com/getsolus/aa-lsm-hook/profiles"
)

// FindNewest gets the newest entry in the list of entries
func FindNewest(entries []profiles.Entry) profiles.Entry {
	// Newest will always be first for single item
	if len(entries) == 1 {
		return entries[0]
	}
	// Find any entry newer than all the others
	newest := entries[0]
	for _, e := range entries {
		if e.Mod.After(newest.Mod) {
			newest = e
		}
	}
	return newest
}

// DeleteAll removes all of the cache entries for this profile
func DeleteAll(name string, entries []profiles.Entry) error {
	for _, e := range entries {
		if err := os.Remove(filepath.Join(e.Path, name)); err != nil {
			return err
		}
	}
	return nil
}

// DeleteOlder removes all but the most recent cache entries for this profile
func DeleteOlder(name string, newest profiles.Entry, entries []profiles.Entry) error {
	for _, e := range entries {
		if e.Mod.Before(newest.Mod) {
			if err := os.Remove(filepath.Join(e.Path, name)); err != nil {
				return err
			}
		}
	}
	return nil
}

// Clean removes unnecessary cache entries for various profiles
func Clean(profs, cached profiles.ProfMap) error {
	for name, entries := range cached {
		// Remove all if the profiles don't exist anymore
		if len(profs[name]) == 0 {
			if err := DeleteAll(name, entries); err != nil {
				return err
			}
			continue
		}
	}

	return nil
}

// CleanDirs cleans outdated directories
func CleanDirs() error {
	curDir, err := CurrentDir()
	if err != nil {
		return err
	}

	paths, err := ScanDirs()
	if err != nil {
		return err
	}

	for _, path := range paths {
		if path != curDir {
			if err = os.RemoveAll(path); err != nil {
				return fmt.Errorf("failed to remove old cache directory: %w", err)
			}
		}
	}

	return nil
}
