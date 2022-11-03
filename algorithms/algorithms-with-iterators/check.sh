#!/bin/bash

[[ $(grep -c -E '#include ["<]algorithm[">]' task.h) = 0 ]]
[[ $(grep -c -E '#define' task.h) = 0 ]]
