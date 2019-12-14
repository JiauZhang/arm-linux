## Kconfig 语言

### 简介
可配置数据库是以树结构组织的配置选项的集合
```shell
+- Code maturity level options
|  +- Prompt for development and/or incomplete code/drivers
+- General setup
|  +- Networking support
|  +- System V IPC
|  +- BSD Process Accounting
|  +- Sysctl support
+- Loadable module support
|  +- Enable loadable module support
|     +- Set version information on all module symbols
|     +- Kernel module loader
+- ...
```
每个配置项都有自己的依赖，而这些依赖则决定这个配置项是否可见。同时，任何配置项
子项只有在父配置项可见时才可见。

### 菜单项
