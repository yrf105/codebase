#!/bin/bash

# echo -n "输入一些文本 > " # -n 不要换行
# read text
# echo "你的输入：$text"

# echo -n "输入一些文本 > " # -n 不要换行
# read text1 text2
# echo "你的输入：$text1 $text2"

# echo -n "输入一些文本 > " # -n 不要换行
# read 
# echo "你的输入：$REPLY"

filename='/home/wddxrw/workspace/codebase/Linux/tmp.txt' 

while read line
do
    echo "$line"
done < $filename