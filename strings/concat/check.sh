#!/bin/bash

[[ $(grep -c -E '#include ["<]c?string[">]' task.cpp) = 0 ]] && [[ $(grep -c "memcpy" task.cpp) = 0 ]]
