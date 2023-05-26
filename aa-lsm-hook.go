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

package main

import (
	"fmt"
	"github.com/getsolus/aa-lsm-hook/cache"
	"os"
	"os/user"
)

func main() {
	// Get the current user
	u, err := user.Current()
	if err != nil {
		fmt.Println(err.Error())
		os.Exit(1)
	}
	if u.Uid != "0" || u.Gid != "0" { // Not running as root
		fmt.Println("You must be root to run this program")
		os.Exit(1)
	}
	if err = cache.Init(); err != nil {
		fmt.Println(err.Error())
		os.Exit(1)
	}
	if err = cache.Update(); err != nil { // Update cache on disk
		fmt.Println(err.Error())
		os.Exit(1)
	}
	if err = cache.Load(); err != nil { // Reload the cache
		fmt.Println(err.Error())
		os.Exit(1)
	}
}
