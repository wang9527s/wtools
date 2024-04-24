TEMPLATE = subdirs

SUBDIRS += src/ScreenShot
SUBDIRS += src/ToolKitEntry
SUBDIRS += src/DynamicDesktop

# 无法设置，不在src中生成debug和release目录
# 只能选择gitignore中忽略