#find . -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec echo  {} \;
find . -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec clang-format-7 -i {} \;
