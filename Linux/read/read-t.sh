#!/bin/bash

if read -t 3 text; then
    echo "$text"
else
    echo '超时'
fi