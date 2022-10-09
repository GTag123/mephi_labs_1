#!/bin/bash

includes=$(grep '#include' task.h | grep -c -v '<iostream>')

[[ $includes = 0 ]]
