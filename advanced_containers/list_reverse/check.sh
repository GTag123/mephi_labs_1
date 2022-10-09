#!/bin/bash

includes=$(grep '#include' task.cpp | grep -c -v 'task.h')

[[ $includes = 0 ]]
