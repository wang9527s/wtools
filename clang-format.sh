#find . -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec echo  {} \;

if [ $(dpkg -l |grep clang-format-7 | wc -l) -eq 0 ];then
	sudo apt install clang-format-7
fi


find . -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec clang-format-7 -i {} \;
